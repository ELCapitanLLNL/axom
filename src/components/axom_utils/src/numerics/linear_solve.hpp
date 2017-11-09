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

#ifndef AXOM_NUMERICS_LINEAR_SOLVE_HPP_
#define AXOM_NUMERICS_LINEAR_SOLVE_HPP_

#include "axom/Types.hpp" // for AXOM_NULLPTR

#include "axom_utils/Determinants.hpp" // for Determinants
#include "axom_utils/LU.hpp"           // for lu_decompose()/lu_solve()
#include "axom_utils/Matrix.hpp"       // for Matrix

// C/C++ includes
#include <cassert> // for assert()

namespace axom
{
namespace numerics
{

/*!
 * \brief Solves a linear system of the form \f$ Ax=b \f$.
 *
 * \param [in] A a square input matrix
 * \param [in] b the right-hand side
 * \param [out] x the solution vector (computed)
 * \return rc return value, 0 if the solve is successful.
 *
 * \pre A.isSquare() == true
 * \pre b != AXOM_NULLPTR
 * \pre x != AXOM_NULLPTR
 *
 * \note The input matrix is destroyed (modified) in the process.
 */
template < typename T >
int linear_solve( Matrix< T >& A, const T * b, T * x );

} /* end namespace numerics */
} /* end namespace axom */

//------------------------------------------------------------------------------
// Implementation
//------------------------------------------------------------------------------
namespace axom
{
namespace numerics
{

template < typename T >
int linear_solve( Matrix< T >& A, const T * b, T * x )
{
  assert( "pre: input matrix must be square" && A.isSquare() );
  assert( "pre: solution vector is null" && (x != AXOM_NULLPTR) );
  assert( "pre: right-hand side vector is null" && (b != AXOM_NULLPTR) );

  if ( !A.isSquare() )
  {
    return LU_NONSQUARE_MATRIX;
  }

  int N = A.getNumColumns();

  if ( N==1 )
  {

    if ( utilities::isNearlyEqual( A(0,0), 0.0 ) )
    {
      return -1;
    }

    x[ 0 ] = b[ 0 ] / A(0,0);

  }
  else if ( N==2 )
  {

    // trivial solve
    T det = numerics::determinant( A );

    if ( utilities::isNearlyEqual( det, 0.0 ) )
    {
      return -1;
    }

    T invdet = 1 / det;
    x[ 0 ]   = ( A( 1,1 )*b[0] - A( 0,1 )*b[1] ) * invdet;
    x[ 1 ]   = ( -A( 1,0)*b[0] + A( 0,0 )*b[1] ) * invdet;

  }
  else
  {

    // non-trivial system, use LU
    int * pivots = new int[ N ];

    int rc = lu_decompose( A, pivots );
    if ( rc == LU_SUCCESS )
    {
      rc = lu_solve( A, pivots, b, x );
    }

    delete [] pivots;
    if ( rc != LU_SUCCESS )
    {
      return -1;
    }

  }

  return 0;
}

} /* end namespace numerics */
} /* end namespace axom */

#endif
