/*
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Copyright (c) 2017-2018, Lawrence Livermore National Security, LLC.
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

#ifndef FIELDVARIABLE_HPP_
#define FIELDVARIABLE_HPP_

#include "mint/Field.hpp"               // Base class

// axom includes
#include "axom/Macros.hpp"
#include "axom/Types.hpp"
#include "mint/FieldTypes.hpp"
#include "mint/Vector.hpp"
#include "mint/DataTypes.hpp"
#include "slic/slic.hpp"

// C/C++ includes
#include <cstddef>                     // for NULL
#include <string>                      // for C++ string


namespace axom
{
namespace mint
{

template < typename FieldType >
class FieldVariable : public Field
{

public:


  /*!
   * \brief Creates a FieldVariable instance associated by the given name,
   *  size and number of components per tuple.
   * \param [in] name the name of this FieldVariable.
   * \param [in] size the number of tuples.
   * \param [in] capacity the number of tuples to allocate.
   * \param [in] num_components the number of components per tuple.
   */
  FieldVariable( const std::string& name, localIndex size, localIndex capacity,
                 int num_components, double resize_ratio):
    Field( name ),
    m_data( capacity, num_components, resize_ratio )
  {
    SLIC_ASSERT( size >= 0 );
    SLIC_ASSERT( capacity >= 0 );
    SLIC_ASSERT( num_components >= 0 );

    m_data.setSize( size );
    this->m_type = field_of< FieldType >::type;
  }


  /*!
   * \brief Destructor.
   */
  virtual ~FieldVariable()
  {}


  virtual localIndex getNumTuples() const
  { return m_data.getSize(); }


  virtual int getNumComponents() const
  { return m_data.getNumComponents(); }


  virtual localIndex getCapacity() const
  { return m_data.getCapacity(); }


  virtual double getResizeRatio() const
  { return m_data.getResizeRatio(); }

  // /*!
  //  * \brief Returns a double pointer to the field data.
  //  * \return ptr pointer to the field data of type double.
  //  * \post ptr==AXOM_NULLPTR iff the data is not of type double.
  //  */
  // virtual double* getDoublePtr();

  // /*!
  //  * \brief Returns a constant double pointer to the field data.
  //  * \return ptr constant pointer to the field data of type double.
  //  * \post ptr==AXOM_NULLPTR iff the data is not of type double.
  //  */
  // virtual const double* getDoublePtr() const;

  // /*!
  //  * \brief Returns an integer pointer to the field data.
  //  * \return ptr pointer to the field data of type int.
  //  * \post ptr==AXOM_NULLPTR iff the data is not of type int.
  //  */
  // virtual int* getIntPtr();

  // /*!
  //  * \brief Returns a constant integer pointer to the field data.
  //  * \return ptr constant pointer to the field data of type int.
  //  * \post ptr==AXOM_NULLPTR iff the data is not of type int.
  //  */
  // virtual const int* getIntPtr() const;

  virtual void setNumTuples( localIndex size )
  { m_data.setSize( size ); }
  

  virtual void setTuplesCapacity( localIndex capacity )
  { m_data.setCapacity( capacity ); }
  

  virtual void setResizeRatio( double ratio )
  { m_data.setResizeRatio( ratio ); }

private:

  /*!
   * \brief FieldVariable constructor. Does nothing. Made private to prevent
   *  its use in application code.
   */
  FieldVariable() 
  {}

  Vector< FieldType > m_data;

  DISABLE_COPY_AND_ASSIGNMENT(FieldVariable);
  DISABLE_MOVE_AND_ASSIGNMENT(FieldVariable);
};


//------------------------------------------------------------------------------
//                  FIELD VARIABLE IMPLEMENTATION
//------------------------------------------------------------------------------

// //------------------------------------------------------------------------------
// template < typename FieldType, typename IndexType >
// double* FieldVariable< FieldType, IndexType >::getDoublePtr()
// { return AXOM_NULLPTR; }

// //------------------------------------------------------------------------------
// template < typename IndexType >
// double* FieldVariable< double, IndexType >::getDoublePtr()
// { return m_data.getData(); }

// //------------------------------------------------------------------------------
// template < typename FieldType, typename IndexType >
// const double* FieldVariable< FieldType, IndexType >::getDoublePtr() const
// {
//   return const_cast< const int* >(
//     const_cast< FieldVariable* >( this )->getIntPtr() );
// }

// //------------------------------------------------------------------------------
// template < typename FieldType, typename IndexType >
// int* FieldVariable< FieldType, IndexType >::getIntPtr()
// { return AXOM_NULLPTR; }

// //------------------------------------------------------------------------------
// template < typename IndexType >
// int* FieldVariable< int, IndexType >::getIntPtr()
// { return m_data.getData(); }

// //------------------------------------------------------------------------------
// template < typename FieldType, typename IndexType >
// const int* FieldVariable< FieldType, Indextype >::getIntPtr() const
// {
//   return const_cast< const int* >(
//     const_cast< FieldVariable* >( this )->getIntPtr() );
// }

} /* namespace mint */
} /* namespace axom */

#endif /* FIELDVARIABLE_HPP_ */
