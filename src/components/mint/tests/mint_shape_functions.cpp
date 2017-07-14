/*
 * Copyright (c) 2015, Lawrence Livermore National Security, LLC.
 * Produced at the Lawrence Livermore National Laboratory.
 *
 * All rights reserved.
 *
 * This source code cannot be distributed without permission and further
 * review from Lawrence Livermore National Laboratory.
 */

#include "gtest/gtest.h"

#include "mint/Lagrange.hpp"
#include "mint/ShapeFunction.hpp"
#include "mint/FEBasis.hpp"
#include "mint/FEBasisTypes.hpp"
#include "mint/CellType.hpp"

#include "slic/slic.hpp"

// C/C++ includes
#include <limits>

using namespace axom;
using mint::ShapeFunction;
using mint::Lagrange;

//------------------------------------------------------------------------------
//  INTERNAL HELPER METHODS
//------------------------------------------------------------------------------
namespace {

/*!
 *******************************************************************************
 * \brief Tests basic attributes of the shape function
 *
 * \tparam BasisType basis bound to the FiniteElemen, e.g., MINT_LAGRANGE_BASIS
 * \tparam CellType the corresponding cell type, e.g., MINT_QUAD
 *******************************************************************************
 */
template < int BasisType, int CellType >
void reference_element( double TOL=std::numeric_limits<double>::epsilon() )
{
  typedef typename mint::FEBasis< BasisType, CellType > FEMType;
  typedef typename FEMType::ShapeFunctionType ShapeFunctionType;
  ShapeFunctionType sf;

  SLIC_INFO( "checking " << mint::basis_name[ BasisType ] << " / "
                         << mint::cell::name[ CellType ] );

  const int ctype = sf.cellType();
  EXPECT_TRUE( (ctype >= 0) && (ctype < MINT_NUM_CELL_TYPES) );

  const int type = sf.type();
  EXPECT_TRUE( (type >= 0) && ( type < MINT_NUM_BASIS_TYPES ) );

  const int ndofs = sf.numDofs();
  EXPECT_TRUE( ndofs > 0 );
  EXPECT_TRUE( sf.maxNewtonIters() >= 16 );

  const int ndims = sf.dimension();
  EXPECT_TRUE( ndims > 0 && ndims < 4 );

  const double LO = sf.min()-TOL;
  const double HI = sf.max()+TOL;

  double* xi = new double[ ndims ];
  sf.center( xi );
  for ( int i=0; i < ndims; ++i ) {
    EXPECT_TRUE( xi[ i ] > LO );
    EXPECT_TRUE( xi[ i ] < HI );
  }
  delete [ ] xi;

  const int N = ndofs*ndims;
  double* coords = new double[ N ];
  sf.coords( coords );
  for ( int i=0; i < N; ++i ) {
     EXPECT_TRUE( coords[ i ] > LO );
     EXPECT_TRUE( coords[ i ] < HI );
  }
  delete [ ] coords;

}

/*!
 *******************************************************************************
 * \brief Ensures shape functions satisfy the kronecker delta property.
 *******************************************************************************
 */
template < int BasisType, int CellType >
void kronecker_delta( )
{
  typedef typename mint::FEBasis< BasisType, CellType > FEMType;
  typedef typename FEMType::ShapeFunctionType ShapeFunctionType;
  ShapeFunctionType sf;

  SLIC_INFO( "checking " << mint::basis_name[ BasisType ] << " / "
                         << mint::cell::name[ CellType ] );

  int ndims = sf.dimension();
  int ndofs = sf.numDofs();

  const int N    = ndofs*ndims;
  double* phi    = new double[ ndofs ];
  double* coords = new double[ N ];
  sf.coords( coords );

  for ( int i=0; i < ndofs; ++i ) {

     double* nc = &coords[ i*ndims ];
     sf.evaluate( nc, phi );

     for ( int j=0; j < ndofs; ++j ) {
        double expected = ( i==j )? 1.0 : 0.0;
        EXPECT_DOUBLE_EQ( expected, phi[j] );
     }

  } // END for all dofs

  delete [ ] phi;
  delete [ ] coords;
}

/*!
 *******************************************************************************
 * \brief Ensures shape functions satisfy the partition of unity property.
 *******************************************************************************
 */
template < int BasisType, int CellType >
void partition_of_unity()
{
  typedef typename mint::FEBasis< BasisType, CellType > FEMType;
  typedef typename FEMType::ShapeFunctionType ShapeFunctionType;
  ShapeFunctionType sf;

  SLIC_INFO( "checking " << mint::basis_name[ BasisType ] << " / "
                         << mint::cell::name[ CellType ] );

  int ndims      = sf.dimension();
  int ndofs      = sf.numDofs();
  const int N    = ndofs*ndims;
  double* coords = new double[ N ];
  double* phi    = new double [ ndofs ];
  double* center = new double[ ndims ];

  sf.coords( coords );
  sf.center( center );

  for ( int i=0; i < ndofs; ++i ) {

     double* nc = &coords[ i*ndims ];
     sf.evaluate( nc, phi );

     double sum=0.0;
     for ( int j=0; j < ndofs; ++j ) {
        sum += phi[ j ];
     } // END

     // the sum of the weights should be unity
     EXPECT_DOUBLE_EQ( 1.0, sum );

  } // END for all dofs

  // test center
  sf.evaluate( center, phi );
  double sum=0.0;
  for ( int i=0; i < ndofs; ++i ) {
     sum += phi[ i ];
  }
  EXPECT_DOUBLE_EQ( 1.0, sum );

  // test other interior nodes
  double* nc = new double[ ndims ];
  for ( int i=0; i < ndofs; ++i ) {

     double* xi = &coords[i*ndims];
     for ( int j=0; j < ndims; ++j ) {
       nc[ j ] = ( xi[ j ] + center[ j ] )*0.5;
     }

     sf.evaluate( nc, phi );

     double sum=0.0;
     for ( int j=0; j < ndofs; ++j ) {
        sum += phi[ j ];
     }
     EXPECT_DOUBLE_EQ( 1.0, sum );
  }

  delete [] phi;
  delete [] coords;
  delete [] center;
  delete [] nc;
}

} /* end anonymous namespace */

//------------------------------------------------------------------------------
// UNIT TESTS
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
TEST( mint_shape_functions, check_reference_element )
{
  reference_element< MINT_LAGRANGE_BASIS, MINT_QUAD >( );
  reference_element< MINT_LAGRANGE_BASIS, MINT_TRIANGLE >( );
  reference_element< MINT_LAGRANGE_BASIS, MINT_TET >( );
  reference_element< MINT_LAGRANGE_BASIS, MINT_HEX >( );
  reference_element< MINT_LAGRANGE_BASIS, MINT_PRISM >( );
  reference_element< MINT_LAGRANGE_BASIS, MINT_PYRAMID >( );

  reference_element< MINT_LAGRANGE_BASIS, MINT_QUAD9 >( );
}

//------------------------------------------------------------------------------
TEST( mint_shape_functions, check_kronecker_delta )
{
  kronecker_delta< MINT_LAGRANGE_BASIS, MINT_QUAD >( );
  kronecker_delta< MINT_LAGRANGE_BASIS, MINT_TRIANGLE >( );
  kronecker_delta< MINT_LAGRANGE_BASIS, MINT_TET >( );
  kronecker_delta< MINT_LAGRANGE_BASIS, MINT_HEX >( );
  kronecker_delta< MINT_LAGRANGE_BASIS, MINT_PRISM >( );
  kronecker_delta< MINT_LAGRANGE_BASIS, MINT_PYRAMID >( );

  kronecker_delta< MINT_LAGRANGE_BASIS, MINT_QUAD9 >( );
}

//------------------------------------------------------------------------------
TEST( mint_shape_functions, check_partition_of_unity )
{
  partition_of_unity< MINT_LAGRANGE_BASIS, MINT_QUAD >( );
  partition_of_unity< MINT_LAGRANGE_BASIS, MINT_TRIANGLE >( );
  partition_of_unity< MINT_LAGRANGE_BASIS, MINT_TET >( );
  partition_of_unity< MINT_LAGRANGE_BASIS, MINT_HEX >( );
  partition_of_unity< MINT_LAGRANGE_BASIS, MINT_PRISM >( );
  partition_of_unity< MINT_LAGRANGE_BASIS, MINT_PYRAMID >( );

  partition_of_unity< MINT_LAGRANGE_BASIS, MINT_QUAD9 >( );
}

//------------------------------------------------------------------------------
#include "slic/UnitTestLogger.hpp"
using axom::slic::UnitTestLogger;

int main(int argc, char * argv[])
{
  int result = 0;

  ::testing::InitGoogleTest(&argc, argv);

  UnitTestLogger logger;  // create & initialize test logger,

  // finalized when exiting main scope

  result = RUN_ALL_TESTS();

  return result;
}


