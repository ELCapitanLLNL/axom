/*
 * Copyright (c) 2015, Lawrence Livermore National Security, LLC.
 * Produced at the Lawrence Livermore National Laboratory.
 *
 * All rights reserved.
 *
 * This source code cannot be distributed without permission and further
 * review from Lawrence Livermore National Laboratory.
 */

#ifndef MINT_TETRA_4_HPP_
#define MINT_TETRA_4_HPP_

// Mint includes
#include "mint/CellType.hpp"
#include "mint/FEBasisTypes.hpp"
#include "mint/ShapeFunction.hpp"

// Slic includes
#include "slic/slic.hpp"

namespace axom {
namespace mint {

/*!
 * \brief Lagrange Finite Element definition for the Linear Tetrahedron
 *
 * \verbatim
 *
 * tetra_4:
 *
 *        3
 *        x
 *       /|\
 *      / | \
 *     /  |  \
 *    /   |   \
 * 0 x----|----x 2
 *    \   |   /
 *     \  |  /
 *      \ | /
 *        x
 *        1
 *
 * \endverbatim
 *
 * \see ShapeFunction
 */
template < >
class Lagrange< MINT_TET > : public ShapeFunction< Lagrange< MINT_TET > >
{
public:

  static int getCellType() { return MINT_TET; }

  static int getType() { return MINT_LAGRANGE_BASIS; }

  static int getNumDofs() { return 4; }

  static int getMaxNewtonIters() { return 16; }

  static int getDimension() { return 3; }

  static double getMin() { return 0; }

  static double getMax() { return 1; }

  static void getCenter( double* center )
  {
    SLIC_ASSERT( center != AXOM_NULLPTR );
    center[ 0 ] = center[ 1 ] = center[ 2 ] = 0.25;
  }

  static void getCoords( double* coords )
  {
    SLIC_ASSERT( coords != AXOM_NULLPTR );

    // node 0
    coords[ 0 ] = 0.0;
    coords[ 1 ] = 0.0;
    coords[ 2 ] = 0.0;

    // node 1
    coords[ 3 ] = 1.0;
    coords[ 4 ] = 0.0;
    coords[ 5 ] = 0.0;

    // node 2
    coords[ 6 ] = 0.0;
    coords[ 7 ] = 1.0;
    coords[ 8 ] = 0.0;

    // node 3
    coords[ 9 ]  = 0.0;
    coords[ 10 ] = 0.0;
    coords[ 11 ] = 1.0;
  }

  static void computeShape( const double* xr, double* phi )
  {
    SLIC_ASSERT( xr != AXOM_NULLPTR );
    SLIC_ASSERT( phi != AXOM_NULLPTR );

    const double r = xr[0];
    const double s = xr[1];
    const double t = xr[2];

    phi[ 0 ] = 1 - r - s - t;
    phi[ 1 ] = r;
    phi[ 2 ] = s;
    phi[ 3 ] = t;
  }

  static void computeDerivatives( const double* AXOM_NOT_USED( xr ),
                                  double* phidot )
  {
    SLIC_ASSERT( phidot != AXOM_NULLPTR );

    // r derivatives
    phidot[ 0 ] = -1;
    phidot[ 1 ] = 1;
    phidot[ 2 ] = 0;
    phidot[ 3 ] = 0;

    // s derivatives
    phidot[ 4 ] = -1;
    phidot[ 5 ] = 0;
    phidot[ 6 ] = 1;
    phidot[ 7 ] = 0;

    // t derivatives
    phidot[ 8 ]  = -1;
    phidot[ 9 ]  = 0;
    phidot[ 10 ] = 0;
    phidot[ 11 ] = 1;
  }
};

} /* namespace mint */
} /* namespace axom */
#endif /* MINT_TETRA_4_HPP_ */
