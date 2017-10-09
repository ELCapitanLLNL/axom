/*
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Copyright (c) 2017, Lawrence Livermore National Security, LLC.
 *
 * Produced at the Lawrence Livermore National Laboratory
 *
 * LLNL-CODE-xxxxxxx
 *
 * All rights reserved.
 *
 * This file is part of Axom.
 *
 * For details about use and distribution, please read axom/LICENSE.
 *
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

#ifndef SPARSE_OCTREE_LEVEL__HXX_
#define SPARSE_OCTREE_LEVEL__HXX_


#include "axom/config.hpp"    // defines AXOM_USE_CXX11
#include "axom/Types.hpp"

#include "primal/MortonIndex.hpp"

#include "quest/Brood.hpp"
#include "quest/OctreeLevel.hpp"

#ifdef AXOM_USE_CXX11
  #include <type_traits>
  #include <unordered_map>
#elif defined(AXOM_USE_BOOST)
  #include <boost/static_assert.hpp>
  #include <boost/type_traits.hpp>
  #include "boost/unordered_map.hpp"
#else
  #error "quest::SparseOctreeLevel requires either C++11 or boost"
#endif

namespace axom {  
namespace quest {

    /**
     * \brief Traits class to manage types for different point representations in a SparseOctreeLevel
     *
     * The general case is meant for Representations types that are unsigned integers
     * and uses a Morton-based index as the hashmap key.
     */
    template<typename CoordType, int DIM, typename BroodDataType, typename RepresentationType>
    struct BroodRepresentationTraits
    {
        typedef Point<CoordType,DIM> GridPt;
        typedef RepresentationType PointRepresenationType;

        // Requires an unsigned int for RepresentationType with 8-,16-,32-, or 64- bits
        #if defined(AXOM_USE_CXX11)
            static_assert( std::is_integral<CoordType>::value, "CoordType must be integral" );
            static_assert( std::is_integral<PointRepresenationType>::value, "RepresentationType must be integral" );
            static_assert( std::is_unsigned<PointRepresenationType>::value, "RepresentationType must be unsigned" );

            typedef std::unordered_map<RepresentationType, BroodDataType> MapType;
        #elif defined(AXOM_USE_BOOST)
            BOOST_STATIC_ASSERT(boost::is_integral<CoordType>::value);
            BOOST_STATIC_ASSERT(boost::is_integral<PointRepresenationType>::value);
            BOOST_STATIC_ASSERT(boost::is_unsigned<PointRepresenationType>::value);

            typedef boost::unordered_map<PointRepresenationType, BroodDataType> MapType;
        #endif

        typedef Brood<GridPt, PointRepresenationType> BroodType;

        /** Simple function to convert a point to its representation type */
        static PointRepresenationType convertPoint(const GridPt& pt)
        {
            return BroodType::MortonizerType::mortonize(pt);
        }
    };

    /**
     * \brief Traits class to manage types for different point representations in a SparseOctreeLevel
     *
     * This is a specialization meant for point representation
     * that use an integer grid point.  The underlying hashmap uses a Morton-based hash function.
     */
    template<typename CoordType, int DIM, typename BroodDataType>
    struct BroodRepresentationTraits<CoordType, DIM, BroodDataType, Point<CoordType,DIM> >
    {
        typedef Point<CoordType,DIM> GridPt;
        typedef GridPt PointRepresenationType;

      #if defined(AXOM_USE_CXX11)
        static_assert( std::is_integral<CoordType>::value, "CoordType must be integral" );
        typedef std::unordered_map<GridPt, BroodDataType, primal::PointHash<int> > MapType;
      #elif defined(AXOM_USE_BOOST)
        BOOST_STATIC_ASSERT(boost::is_integral<CoordType>::value);
        typedef boost::unordered_map<GridPt, BroodDataType, primal::PointHash<int> > MapType;
      #endif

        typedef Brood<GridPt, GridPt> BroodType;

        /** Simple function to convert a point to its representation type
         *  \note This is a pass through function
         *        since the representation and grid point types are the same
         */
        static const PointRepresenationType& convertPoint(const GridPt& pt)
        {
            return pt;  // simple pass through function
        }
    };


    /**
     * \class
     * \brief A representation of a sparse OctreeLevel.
     *
     *  A SparseOctreeLevel is a concrete implementation of an OctreeLevel
     *  that associates data with its Octree block using a hash map
     *  whose key type is of type PointRepresentationType
     *  (either an integer grid point hashed by a Morton index,
     *   or an Morton index (unsigned integer of a specified bitwidth)
     *  and whose value type is a BlockDataType.
     *  For efficiency, the data is associated with an entire brood, a collection of
     *  siblings that are created simultaneously. In dimension DIM, there are 2^DIM siblings in a brood.
     *
     *  \see OctreeLevel
     */
    template<int DIM, typename BlockDataType, typename PointRepresenationType>
    class SparseOctreeLevel : public OctreeLevel<DIM,BlockDataType>
    {
    public:
      typedef OctreeLevel<DIM, BlockDataType> Base;
      typedef typename Base::GridPt                   GridPt;
      typedef typename Base::BroodData                BroodData;
      typedef typename Base::BlockIteratorHelper      BaseBlockIteratorHelper;
      typedef typename Base::ConstBlockIteratorHelper ConstBaseBlockIteratorHelper;

      typedef BroodRepresentationTraits<typename GridPt::CoordType, GridPt::DIMENSION, BroodData, PointRepresenationType> BroodTraits;
      typedef typename BroodTraits::MapType MapType;
      typedef typename BroodTraits::BroodType BroodType;

      typedef typename MapType::iterator       MapIter;
      typedef typename MapType::const_iterator ConstMapIter;

      template<typename OctreeLevelType, typename AdaptedIterType, typename ParentType> class IteratorHelper;

      typedef IteratorHelper<SparseOctreeLevel, MapIter, BaseBlockIteratorHelper> IterHelper;
      typedef IteratorHelper<const SparseOctreeLevel, ConstMapIter, ConstBaseBlockIteratorHelper> ConstIterHelper;



    public:

        /**
         * \brief Concrete instance of the BlockIteratorHelper class defined in the OctreeLevel base class.
         */
        template<typename OctreeLevelType, typename AdaptedIterType, typename ParentType>
        class IteratorHelper : public ParentType
        {
        public:
            typedef IteratorHelper<OctreeLevelType, AdaptedIterType, ParentType> self;
            typedef ParentType     BaseBlockItType;

            IteratorHelper(OctreeLevelType* octLevel, bool begin)
                : m_offset(0),
                  m_isLevelZero( octLevel->level() == 0)
            {
                m_currentIter = begin
                        ? octLevel->m_map.begin()
                        : octLevel->m_map.end();
            }

            /** Increment to next block in the level */
            void increment()
            {
                ++m_offset;

                if(m_offset == Base::BROOD_SIZE || m_isLevelZero)
                {
                    ++m_currentIter;
                    m_offset = 0;
                }
            }

            /** Accessor for point associated with iterator's block  */
            GridPt pt() const
            {
                return BroodType::reconstructGridPt(m_currentIter->first, m_offset);
            }

            /** Accessor for data associated with the iterator's block */
            BlockDataType* data() { return &m_currentIter->second[m_offset]; }
            /** Const accessor for data associated with the iterator's block */
            const BlockDataType* data() const { return &m_currentIter->second[m_offset]; }

            /** \brief Predicate to determine if two block iterators are the same */
            bool equal(const BaseBlockItType* other)
            {
                const self* pother = dynamic_cast<const self*>(other);

                return (pother != AXOM_NULLPTR)
                     && (m_currentIter == pother->m_currentIter)   // iterators are the same
                     && (m_offset == pother->m_offset);            // brood indices are the same
            }
        private:
            AdaptedIterType m_currentIter;
            int m_offset;
            bool m_isLevelZero;
        };

    public:

        /** \brief Default constructor for an octree level */
        SparseOctreeLevel(int level = -1): Base(level){}


        /**
         * \brief Factory function to return a SparseBlockIterHelper for this level
         *
         * \param begin A boolean to determine if this is to be a begin (true) or end (false) iterator
         */
        BaseBlockIteratorHelper* getIteratorHelper(bool begin)
        {
            return new IterHelper(this, begin);
        }

        /**
         * \brief Factory function to return a ConstSparseBlockIterHelper for this level
         *
         * \param begin A boolean to determine if this is to be a begin (true) or end (false) iterator
         */
        ConstBaseBlockIteratorHelper* getIteratorHelper(bool begin) const
        {
            return new ConstIterHelper(this, begin);
        }


        /**
         * \brief Predicate to check whether the block associated with the given GridPt pt is in the current level
         */
        bool hasBlock(const GridPt& pt) const
        {
            const BroodType brood(pt);
            ConstMapIter blockIt = m_map.find(brood.base());
            return blockIt != m_map.end();
        }

        /**
         * \brief Adds all children of the given grid point to the octree level
         *
         * \param [in] pt The gridPoint associated with the parent of the children that are being added
         * \pre pt must be in bounds for the level
         * \sa inBounds()
         */
        void addAllChildren(const GridPt& pt)
        {
            SLIC_ASSERT_MSG(this->inBounds(pt)
                           , "Problem while inserting children of point " << pt
                           << " into octree level " << this->m_level
                           << ". Point was out of bounds -- "
                           << "each coordinate must be between 0 and " << this->maxCoord() << ".");

            BroodData& bd = getBroodData(pt);   // Adds entire brood at once (default constructed)
            if( this->level() == 0)
            {
                for(int j=1; j< Base::BROOD_SIZE; ++j)
                    bd[j].setNonBlock();
            }
        }



        /** \brief Accessor for the data associated with pt */
        BlockDataType& operator[](const GridPt& pt)
        {
            const BroodType brood(pt);
            return m_map[brood.base()][brood.offset()];
        }

        /** \brief Const accessor for the data associated with pt */
        const BlockDataType& operator[](const GridPt& pt) const
        {
            SLIC_ASSERT_MSG(hasBlock(pt)
                            ,"(" << pt <<", "<< this->m_level << ") was not a block in the tree at level.");

            // Note: Using find() method on hashmap since operator[] is non-const
            const BroodType brood(pt);
            ConstMapIter blockIt = m_map.find(brood.base());
            return blockIt->second[brood.offset()];
        }

        /** \brief Access the data associated with the entire brood */
        BroodData& getBroodData(const GridPt& pt) { return m_map[ BroodTraits::convertPoint(pt)]; }

        /** \brief Const access to data associated with the entire brood */
        const BroodData& getBroodData(const GridPt& pt) const {
            SLIC_ASSERT_MSG(hasBlock(pt)
                            ,"(" << pt <<", "<< this->m_level << ") was not a block in the tree at level.");

            // Note: Using find() method on hashmap since operator[] is non-const
            ConstMapIter blockIt = m_map.find( BroodTraits::convertPoint(pt) );
            return blockIt->second;
        }

        /** \brief Predicate to check if there are any blocks in this octree level */
        bool empty() const { return m_map.empty(); }

        /** \brief Returns the number of blocks (internal and leaf) in the level */
        int numBlocks() const
        {
            if(empty())
                return 0;
            return (this->m_level == 0)? 1 : (m_map.size() * Base::BROOD_SIZE);
        }

        /** \brief Returns the number of internal blocks in the level */
        int numInternalBlocks() const { return numBlocks() - numLeafBlocks(); }

        /** \brief Returns the number of leaf blocks in the level */
        int numLeafBlocks() const
        {
            if(empty())
                return 0;

            int count = 0;
            for(ConstMapIter it = m_map.begin(), itEnd = m_map.end(); it != itEnd; ++it)
            {
                const BroodData& bd  = it->second;
                for(int i=0; i< Base::BROOD_SIZE; ++i)
                {
                    if(bd[i].isLeaf())
                         ++count;
                }
            }
            return count;
        }

        /**
         * \brief Helper function to determine the status of an octree block within this octree level
         *
         * \param pt The grid point of the block index that we are testing
         * \return The status of the grid point pt (e.g. LeafBlock, InternalBlock, ...)
         */
        TreeBlockStatus blockStatus(const GridPt & pt) const
        {
            const BroodType brood(pt);
            ConstMapIter blockIt = m_map.find(brood.base());

            return (blockIt == m_map.end())
                    ? BlockNotInTree
                    : (blockIt->second[brood.offset()].isLeaf())
                        ? LeafBlock
                        : InternalBlock;
        }

    private:
        DISABLE_COPY_AND_ASSIGNMENT(SparseOctreeLevel);
        DISABLE_MOVE_AND_ASSIGNMENT(SparseOctreeLevel);

    private:
      MapType m_map;
    };

} // end namespace quest
} // end namespace axom 

#endif  // SPARSE_OCTREE_LEVEL__HXX_
