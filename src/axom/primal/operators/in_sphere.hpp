// Copyright (c) 2017-2022, Lawrence Livermore National Security, LLC and
// other Axom Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)

/*!
 * \file in_sphere.hpp
 *
 * \brief Consists of methods that test whether a given query point is
 * inside the unique sphere circumscribing a 2D triangle or a 3D tetrahedron.
 *
 * This is a well known computational geometry primitive.  For reference,
 * see Section 3.1.6.4 in "Real-time collision detection" by C. Ericson.
 */

#ifndef AXOM_PRIMAL_IN_SPHERE_H_
#define AXOM_PRIMAL_IN_SPHERE_H_

#include "axom/core.hpp"
#include "axom/primal/geometry/Point.hpp"
#include "axom/primal/geometry/Triangle.hpp"
#include "axom/primal/geometry/Tetrahedron.hpp"

namespace axom
{
namespace primal
{
/*!
 * \brief Tests whether a query point lies inside a 2D triangle's circumcircle
 *
 * A triangle's circumcircle is the unique circle (i.e. a 2-sphere) that
 * passes through each of its three vertices.
 *
 * \param [in] q the query point
 * \param [in] p0 the first vertex of the triangle
 * \param [in] p1 the second vertex of the triangle
 * \param [in] p2 the third vertex of the triangle
 * \param [in] EPS tolerance for determining if \a q is on the boundary. Default: 1e-8.
 * \return true if the point is inside the circumcircle, false if it is on
 * the circle's boundary or outside the circle
 */
template <typename T>
inline bool in_sphere(const Point<T, 2>& q,
                      const Point<T, 2>& p0,
                      const Point<T, 2>& p1,
                      const Point<T, 2>& p2,
                      double EPS = 1e-8)
{
  const Point<T, 2> a(p0.array() - q.array());
  const Point<T, 2> b(p1.array() - q.array());
  const Point<T, 2> c(p2.array() - q.array());

  // clang-format off
  const double det = axom::numerics::determinant(
    a[0], a[1], (a[0]*a[0] + a[1]*a[1]),
    b[0], b[1], (b[0]*b[0] + b[1]*b[1]),
    c[0], c[1], (c[0]*c[0] + c[1]*c[1]));
  // clang-format on

  return axom::utilities::isNearlyEqual(det, 0., EPS) ? false : (det > 0);
}

/*!
 * \brief Tests whether a query point lies inside a 2D triangle's circumcircle
 *
 * \param [in] q the query point
 * \param [in] tri the triangle
 * \param [in] EPS tolerance for determining if \a q is on the boundary. Default: 1e-8.
 * \see in_sphere
 */
template <typename T>
inline bool in_sphere(const Point<T, 2>& q,
                      const Triangle<T, 2>& tri,
                      double EPS = 1e-8)
{
  return in_sphere(q, tri[0], tri[1], tri[2], EPS);
}

/*!
 * \brief Tests whether a query point lies inside a 3D tetrahedron's
 * circumsphere
 *
 * A tetrahedron's circumsphere is the unique sphere that passes through each
 * of its four vertices.
 *
 * \param [in] q the query point
 * \param [in] p0 the first vertex of the tetrahedron
 * \param [in] p1 the second vertex of the tetrahedron
 * \param [in] p2 the third vertex of the tetrahedron
 * \param [in] p3 the fourth vertex of the tetrahedron
 * \param [in] EPS tolerance for determining if \a q is on the boundary. Default: 1e-8.
 * \return true if the point is inside the circumsphere, false if it is on
 * the sphere's boundary or outside the sphere
 */
template <typename T>
inline bool in_sphere(const Point<T, 3>& q,
                      const Point<T, 3>& p0,
                      const Point<T, 3>& p1,
                      const Point<T, 3>& p2,
                      const Point<T, 3>& p3,
                      double EPS = 1e-8)
{
  const Point<T, 3> a(p0.array() - q.array());
  const Point<T, 3> b(p1.array() - q.array());
  const Point<T, 3> c(p2.array() - q.array());
  const Point<T, 3> d(p3.array() - q.array());

  // clang-format off
  const double det = axom::numerics::determinant(
    a[0], a[1], a[2], (a[0]*a[0] + a[1]*a[1] + a[2]*a[2]),
    b[0], b[1], b[2], (b[0]*b[0] + b[1]*b[1] + b[2]*b[2]),
    c[0], c[1], c[2], (c[0]*c[0] + c[1]*c[1] + c[2]*c[2]),
    d[0], d[1], d[2], (d[0]*d[0] + d[1]*d[1] + d[2]*d[2]));
  // clang-format on

  return axom::utilities::isNearlyEqual(det, 0., EPS) ? false : (det < 0);
}

/*!
 * \brief Tests whether a query point lies inside a 3D tetrahedron's circumsphere
 *
 * \param [in] q the query point
 * \param [in] tet the tetrahedron
 * \param [in] EPS tolerance for determining if \a q is on the boundary. Default: 1e-8.
 * \see in_sphere
 */
template <typename T>
inline bool in_sphere(const Point<T, 3>& q,
                      const Tetrahedron<T, 3>& tet,
                      double EPS = 1e-8)
{
  return in_sphere(q, tet[0], tet[1], tet[2], tet[3], EPS);
}

}  // namespace primal
}  // namespace axom

#endif  // AXOM_PRIMAL_IN_SPHERE_H_
