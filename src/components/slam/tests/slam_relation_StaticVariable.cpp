/*
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Copyright (c) 2017, Lawrence Livermore National Security, LLC.
 *
 * Produced at the Lawrence Livermore National Laboratory
 *
 * LLNL-CODE-741217
 *
 * All rights reserved.
 *
 * This file is part of Axom.
 *
 * For details about use and distribution, please read axom/LICENSE.
 *
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */


/**
 * \file slam_relation_StaticVariable.cpp
 *
 * \brief Unit tests for Slam's StaticRelation class configured with variable per-element cardinality
 */


#include <iostream>
#include <iterator>
#include <sstream>

#include "gtest/gtest.h"

#include "axom/config.hpp"  // for AXOM_USE_BOOST

#include "slic/slic.hpp"

#include "slam/ModularInt.hpp"
#include "slam/RangeSet.hpp"
#include "slam/Relation.hpp"

#include "slam/IndirectionPolicies.hpp"
#include "slam/StaticRelation.hpp"

namespace {

namespace slam = axom::slam;
namespace policies = axom::slam::policies;

  using slam::RangeSet;
  using slam::Relation;

  typedef RangeSet::ElementType     ElementType;
  typedef RangeSet::PositionType    PositionType;

  typedef PositionType              SetPosition;
  typedef std::vector<SetPosition>  IndexVec;


  const PositionType FROMSET_SIZE = 7;
  const PositionType TOSET_SIZE = 8;

  typedef policies::STLVectorIndirection<PositionType, PositionType>  STLIndirection;
  typedef policies::ArrayIndirection<PositionType, PositionType>      ArrayIndirection;

  typedef policies::VariableCardinality<PositionType, STLIndirection> VariableCardinality;

  typedef slam::StaticRelation<VariableCardinality, STLIndirection,
      slam::RangeSet, slam::RangeSet>                                 StaticVariableRelationType;

  // Use a slam::ModularInt type for more interesting test data
  typedef policies::CompileTimeSize<int, TOSET_SIZE > CTSize;
  typedef slam::ModularInt< CTSize >                  FixedModularInt;


  PositionType elementCardinality(PositionType fromPos)
  {
    return fromPos;
  }

  PositionType relationData(PositionType fromPos, PositionType toPos)
  {
    return FixedModularInt(fromPos + toPos);
  }


  template<typename StrType, typename VecType>
  void printVector(StrType const& msg, VecType const& vec)
  {
    std::stringstream sstr;

    sstr << "\n** " << msg << "\n\t";
    sstr << "Array of size " << vec.size() << ": ";
    std::copy(vec.begin(), vec.end(), std::ostream_iterator<PositionType>(sstr, " "));

    SLIC_INFO( sstr.str() );
  }


  template<typename VecType>
  void generateIncrementingRelations(VecType* begins, VecType* indices)
  {
    VecType& beginsVec = *begins;
    VecType& indicesVec = *indices;

    PositionType curIdx = PositionType();

    for(PositionType i = 0; i < FROMSET_SIZE; ++i)
    {
      beginsVec.push_back( curIdx );
      for(PositionType j = 0; j < elementCardinality(i); ++j)
      {
        indicesVec.push_back( relationData(i,j) );
        ++curIdx;
      }
    }
    beginsVec.push_back ( curIdx );
  }



  /**
   * \brief Traverses the relation's entities using the double subscript access API
   *
   * \note Assumes that the relation data has been set using the generateIncrementingRelations() function
   */
  template<typename RelationType>
  void traverseRelation_doubleSubscript(RelationType& rel)
  {
    SLIC_INFO("Traversing relation data using double subscript: " );
    for(PositionType fromPos = 0; fromPos < rel.fromSet()->size(); ++fromPos)
    {
      const PositionType fromSize = rel.size(fromPos);
      EXPECT_EQ( elementCardinality(fromPos), fromSize );

      for(int toPos = 0; toPos < fromSize; ++toPos)
      {
        PositionType actualVal = rel[fromPos][toPos];

        EXPECT_EQ(relationData(fromPos,toPos), actualVal);
      }
    }
  }

  /**
   * \brief Traverses relation using separated subscript operators
   *
   * The first subscript operator gets the set of entities in the ToSet that are mapped to the
   * given element of the relation's FromSet
   *
   * \note Assumes that the relation data has been set using the generateIncrementingRelations() function
   */
  template<typename RelationType>
  void traverseRelation_delayedSubscript(RelationType& rel)
  {
    SLIC_INFO("Traversing relation data using delayed second subscript: " );
    for(PositionType fromPos = 0; fromPos < rel.fromSet()->size(); ++fromPos)
    {
      const PositionType fromSize = rel.size(fromPos);
      EXPECT_EQ( elementCardinality(fromPos), fromSize );

      typename RelationType::RelationSet set = rel[fromPos];
      for(int toPos = 0; toPos < set.size(); ++toPos)
      {
        PositionType actualVal = rel[fromPos][toPos];
        EXPECT_EQ(relationData(fromPos,toPos), actualVal);
      }
    }
  }

  /**
   * \brief Traverses relation using the iterator API (begin()/end() )
   *
   * \note The iterator API depends on boost
   * \note Assumes that the relation data has been set using the generateIncrementingRelations() function
   */
  template<typename RelationType>
  void iterateRelation_begin_end(RelationType& rel)
  {
#ifdef AXOM_USE_BOOST
    typedef typename RelationType::FromSetType      FromSet;
    typedef typename FromSet::iterator              FromSetIter;

    typedef typename RelationType::RelationIterator RelIter;

    SLIC_INFO("Traversing relation data using iterator begin()/end() functions");
    for(FromSetIter sIt = rel.fromSet()->begin(), sItEnd = rel.fromSet()->end(); sIt != sItEnd; ++sIt)
    {
      PositionType actualSize = rel.size( *sIt);

      PositionType fromSetEltNum = std::distance(rel.fromSet()->begin(), sIt);
      EXPECT_EQ( elementCardinality(fromSetEltNum), actualSize );

      RelIter toSetBegin = rel.begin(*sIt);
      RelIter toSetEnd   = rel.end(*sIt);
      for(RelIter relIt = toSetBegin; relIt != toSetEnd; ++relIt)
      {
        PositionType toSetEltNum = std::distance(toSetBegin, relIt);
        ASSERT_EQ( relationData(fromSetEltNum,toSetEltNum), *relIt);
      }
    }
#else
    AXOM_DEBUG_VAR(rel);
    SLIC_INFO("Skipping iterator tests when boost unavailable");
#endif
  }

  /**
   * \brief Traverses relation using the iterator range API
   *
   * \note The iterator API depends on boost
   * \note Assumes that the relation data has been set using the generateIncrementingRelations() function
   */
  template<typename RelationType>
  void iterateRelation_range(RelationType& rel)
  {
#ifdef AXOM_USE_BOOST
    typedef typename RelationType::FromSetType          FromSet;
    typedef typename FromSet::iterator                  FromSetIter;
    typedef typename FromSet::iterator_pair             FromSetIterPair;

    typedef typename RelationType::RelationIterator     RelIter;
    typedef typename RelationType::RelationIteratorPair RelIterPair;

    SLIC_INFO("Traversing relation data using iterator range() functions");
    FromSetIterPair itPair = rel.fromSet()->range();
    for(FromSetIter sIt = itPair.first; sIt != itPair.second; ++sIt)
    {
      PositionType fromSetEltNum = std::distance(itPair.first, sIt);

      RelIterPair toSetItPair = rel.range(*sIt);
      for(RelIter relIt = toSetItPair.first; relIt != toSetItPair.second; ++relIt)
      {
        PositionType toSetEltNum = std::distance(toSetItPair.first, relIt);
        ASSERT_EQ( relationData(fromSetEltNum, toSetEltNum), *relIt);
      }
    }
#else
    AXOM_DEBUG_VAR(rel);
    SLIC_INFO("Skipping iterator tests when boost unavailable");
#endif
  }

} // end anonymous namespace
TEST(slam_static_variable_relation,construct_empty)
{
  SLIC_INFO("Testing empty relation.  isValid() should be false.");

  StaticVariableRelationType emptyRel;

  EXPECT_FALSE(emptyRel.isValid(true));
}

TEST(slam_static_variable_relation,construct_uninitialized)
{
  SLIC_INFO("Testing uninitialized relation.  isValid() should be false.");

  RangeSet fromSet(FROMSET_SIZE);
  RangeSet toSet(TOSET_SIZE);

  StaticVariableRelationType emptyRel(&fromSet, &toSet);

  EXPECT_FALSE(emptyRel.isValid(true));
}


TEST(slam_static_variable_relation,construct_relation)
{
  SLIC_INFO("Testing simple incrementing relation.  isValid() should be true.");

  RangeSet fromSet(FROMSET_SIZE);
  RangeSet toSet(TOSET_SIZE);

  IndexVec relOffsets;
  IndexVec relIndices;

  SLIC_INFO("Uninitialized relation data");
  printVector("begins vector",  relOffsets);
  printVector("indices vector", relIndices);
  generateIncrementingRelations(&relOffsets, &relIndices);

  SLIC_INFO("Initialized relation data");
  printVector("begins vector",  relOffsets);
  printVector("indices vector", relIndices);

  StaticVariableRelationType incrementingRel(&fromSet, &toSet);
  incrementingRel.bindBeginOffsets(fromSet.size(), &relOffsets);
  incrementingRel.bindIndices(relIndices.size(), &relIndices);

  EXPECT_TRUE(incrementingRel.isValid(true));

  // Test traversal of the relation data
  SCOPED_TRACE("Vector_simple_construct");
  traverseRelation_doubleSubscript(incrementingRel);
  traverseRelation_delayedSubscript(incrementingRel);
  iterateRelation_begin_end(incrementingRel);
  iterateRelation_range(incrementingRel);

}


TEST(slam_static_variable_relation,construct_builder)
{
  SLIC_INFO("Testing construction using builder interface.");

  RangeSet fromSet(FROMSET_SIZE);
  RangeSet toSet(TOSET_SIZE);

  IndexVec offsets;
  IndexVec relIndices;
  generateIncrementingRelations(&offsets, &relIndices);

  typedef StaticVariableRelationType::RelationBuilder RelationBuilder;
  StaticVariableRelationType relation =  RelationBuilder()
      .fromSet( &fromSet)
      .toSet( &toSet)
      .begins( RelationBuilder::BeginsSetBuilder()
          .size(offsets.size())
          .data(&offsets) )
      .indices( RelationBuilder::IndicesSetBuilder()
          .size(relIndices.size())
          .data(&relIndices))
  ;
  EXPECT_TRUE(relation.isValid(true));

  // Test traversal of the relation data
  SCOPED_TRACE("Vector_builder");
  traverseRelation_doubleSubscript(relation);
  traverseRelation_delayedSubscript(relation);
  iterateRelation_begin_end(relation);
  iterateRelation_range(relation);

}




TEST(slam_static_variable_relation,empty_relation_out_of_bounds)
{
  StaticVariableRelationType emptyRel;

#ifdef AXOM_DEBUG
  // NOTE: AXOM_DEBUG is disabled in release mode, so this test will only fail in debug mode
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  EXPECT_DEATH_IF_SUPPORTED( emptyRel[FROMSET_SIZE], "");
#else
  SLIC_INFO("Skipped assertion failure check in release mode.");
#endif
}


/// Tests for out-of-bounds access

TEST(slam_static_variable_relation,initialized_rel_out_of_bounds)
{
  SLIC_INFO("Testing simple incrementing relation.  isValid() should be true.");


  IndexVec relOffsets, relIndices;
  generateIncrementingRelations(&relOffsets, &relIndices);

  RangeSet fromSet(FROMSET_SIZE), toSet(TOSET_SIZE);
  StaticVariableRelationType incrementingRel(&fromSet, &toSet);
  incrementingRel.bindBeginOffsets(fromSet.size(), &relOffsets);
  incrementingRel.bindIndices(relIndices.size(), &relIndices);

#ifdef AXOM_DEBUG
  // NOTE: AXOM_DEBUG is disabled in release mode, so this test will only fail in debug mode
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  EXPECT_DEATH_IF_SUPPORTED( incrementingRel[FROMSET_SIZE], "");
#else
  SLIC_INFO("Skipped assertion failure check in release mode.");
#endif
}


//----------------------------------------------------------------------
//----------------------------------------------------------------------
#include "slic/UnitTestLogger.hpp"
using axom::slic::UnitTestLogger;

int main(int argc, char * argv[])
{
  int result = 0;

  ::testing::InitGoogleTest(&argc, argv);

  // create & initialize test logger. finalized when exiting main scope
  UnitTestLogger logger;

  result = RUN_ALL_TESTS();

  return result;
}
