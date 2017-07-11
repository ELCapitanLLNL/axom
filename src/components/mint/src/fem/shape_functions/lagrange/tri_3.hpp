/*
 * Copyright (c) 2015, Lawrence Livermore National Security, LLC.
 * Produced at the Lawrence Livermore National Laboratory.
 *
 * All rights reserved.
 *
 * This source code cannot be distributed without permission and further
 * review from Lawrence Livermore National Laboratory.
 */

#ifndef MINT_TRI3_HPP_
#define MINT_TRI3_HPP_

// Mint includes
#include "mint/CellType.hpp"
#include "mint/FEBasisTypes.hpp"
#include "mint/ShapeFunction.hpp"

// Slic includes
#include "slic/slic.hpp"

namespace axom {
namespace mint {

static const double TRI_ONE_THIRD =  1.0/3.0;

/*!
 *******************************************************************************
 * \brief Lagrange Finite Element definition for the Linear Triangle.
 *
 * \verbatim
 *
 * tri_3:
 *
 *         2
 *         X
 *       /   \
 *      /     \
 *     /       \
 *    X ------- X
 *   0           1
 *
 * \endverbatim
 *
 * \see ShapeFunction
 *******************************************************************************
 */
template < >
class Lagrange< MINT_TRIANGLE > :
      public ShapeFunction< Lagrange< MINT_TRIANGLE > >
{
public:

  static int getCellType() { return MINT_TRIANGLE; }

  static int getType() { return MINT_LAGRANGE_BASIS; }

  static int getNumDofs() { return 3; }

  static int getMaxNewtonIters() { return 16; }

  static int getDimension() { return 2; }

  static double getMin() { return 0; }

  static double getMax() { return 1; }

  static void getCenter( double* center )
  {
    SLIC_ASSERT( center != AXOM_NULLPTR );
    center[ 0 ] = center[ 1 ] = TRI_ONE_THIRD;
  }

  static void getCoords( double* coords )
  {
    SLIC_ASSERT( coords != AXOM_NULLPTR );

    // node 0
    coords[ 0 ] = 0.0;
    coords[ 1 ] = 0.0;

    // node 1
    coords[ 2 ] = 1.0;
    coords[ 3 ] = 0.0;

    // node 2
    coords[ 4 ] = 0.0;
    coords[ 5 ] = 1.0;
  }

  static void computeShape( const double* xr, double* phi )
  {
    SLIC_ASSERT( xr != AXOM_NULLPTR );
    SLIC_ASSERT( phi != AXOM_NULLPTR );

    const double r  = xr[0];
    const double s  = xr[1];

    phi[ 0 ] = 1 - r - s;
    phi[ 1 ] = r;
    phi[ 2 ] = s;
  }

  static void computeDerivatives( const double* AXOM_NOT_USED(xr),
                                  double* phidot )
  {
    SLIC_ASSERT( phidot != AXOM_NULLPTR );

    // r derivatives
    phidot[ 0 ] = -1;
    phidot[ 1 ] = 1;
    phidot[ 2 ] = 0;

    // s derivatives
    phidot[ 3 ] = -1;
    phidot[ 4 ] = 0;
    phidot[ 5 ] = 1;
  }

};

} /* namespace mint */
} /* namespace axom */

#endif /* MINT_TRI3_HPP_ */
