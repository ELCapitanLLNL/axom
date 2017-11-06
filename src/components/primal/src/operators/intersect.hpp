/*
 * Copyright (c) 2015, Lawrence Livermore National Security, LLC.
 * Produced at the Lawrence Livermore National Laboratory.
 *
 * All rights reserved.
 *
 * This source code cannot be distributed without permission and further
 * review from Lawrence Livermore National Laboratory.
 */

/*!
 * \file intersect.hpp
 *
 * \brief Consists of functions to test intersection among geometric primitives.
 */

#ifndef INTERSECTION_HPP_
#define INTERSECTION_HPP_

#include "primal/OrientedBoundingBox.hpp"
#include "primal/BoundingBox.hpp"
#include "primal/Point.hpp"
#include "primal/Ray.hpp"
#include "primal/Segment.hpp"
#include "primal/Triangle.hpp"
#include "axom_utils/Utilities.hpp"

#include "primal/intersect_impl.hpp"

namespace axom {
namespace primal {

/*!
 * \brief Tests if 3D Triangles t1 and t2 intersect.
 * \return status true iff t1 intersects with t2, otherwise, false.
 *
 * If parameter includeBoundary is false (default), this function will
 * return true if the interior of t1 intersects the interior of t2.  To include
 * triangle boundaries in intersections, specify includeBoundary as true.
 */
template < typename T >
bool intersect( const Triangle< T, 3 >& t1,
                const Triangle< T, 3 >& t2,
                const bool includeBoundary = false)
{
  return detail::intersect_tri3D_tri3D< T >(t1, t2, includeBoundary);
}

/*!
 * \brief Tests if 2D Triangles t1 and t2 intersect.
 * \return status true iff t1 intersects with t2, otherwise, false.
 *
 * If parameter includeBoundary is false (default), this function will
 * return true if the interior of t1 intersects the interior of t2.  To include
 * triangle boundaries in intersections, specify includeBoundary as true.
 */
template < typename T >
bool intersect( const Triangle< T, 2 >& t1,
                const Triangle< T, 2 >& t2,
                const bool includeBoundary = false)
{
  return detail::intersect_tri2D_tri2D< T >(t1, t2, includeBoundary);
}

/*!
 * \brief Computes the intersection of the given ray, R, with the segment, S.
 *      ip returns the intersection point on S.
 * \return status true iff R intersects with S, otherwise, false.
 */
template < typename T >
bool intersect( const Ray< T,2 >& R, const Segment< T,2 >& S, Point< T,2 >& ip )
{
  return detail::intersect_ray_seg(R, S, ip);
}

/*!
 * \brief Computes the intersection of the given ray, R, with the Box, bb.
 *      ip the point of intersection on R.
 * \return status true iff bb intersects with R, otherwise, false.
 *
 * Computes Ray Box intersection using the slab method from pg 180 of
 * Real Time Collision Detection by Christer Ericson.
 */
template < typename T, int DIM >
bool intersect( const Ray< T,DIM > & R,
                const BoundingBox< T,DIM > & bb,
                Point< T,DIM > & ip)
{
  return detail::intersect_ray_bbox(R, bb, ip);
}

/*!
 * \brief Computes the intersection of the given segment, S, with the Box, bb.
 *     ip the point of intersection on S.
 * \return status true iff bb intersects with S, otherwise, false.
 *
 * Computes Segment Box intersection using the slab method from pg 180 of
 * Real Time Collision Detection by Christer Ericson.
 * WIP: More test cases for this
 */
template < typename T, int DIM >
bool intersect( const Segment< T,DIM > & S,
                const BoundingBox< T,DIM > & bb,
                Point< T,DIM > & ip)
{
  return detail::intersect_seg_bbox(S, bb, ip);
}

/*!
 * \brief Determines if two axis aligned bounding boxes intersect
 * \param [in] bb1 user-supplied axis aligned bounding box.
 * \param [in] bb2 user-supplied axis aligned bounding box.
 * \return true iff bb1 intersects with bb2, otherwise, false.
 */
template < typename T, int DIM >
bool intersect( const BoundingBox< T, DIM >& bb1,
                const BoundingBox< T, DIM >& bb2)
{
  return bb1.intersectsWith(bb2);
}

/*!
 * \brief Determines if a triangle and a bounding box intersect
 * \param [in] tri user-supplied triangle (with three vertices).
 * \param [in] bb user-supplied axis aligned bounding box.
 * \return true iff tri intersects with bb, otherwise, false.
 */
template < typename T >
bool intersect( const Triangle< T, 3 >& tri, const BoundingBox< T, 3 >& bb)
{
  return detail::intersect_tri_bbox(tri, bb);
}

/*!
 * \brief Determines if a 3D triangle intersects a 3D ray.
 * \param [in] tri A 3D triangle
 * \param [in] ray A 3D ray
 * \return true iff tri intersects with ray, otherwise, false.
 */
template < typename T >
bool intersect(const Triangle< T, 3 >& tri, const Ray< T,3 >& ray)
{
  T t = T();
  Point< double, 3 > p;
  return detail::intersect_tri_ray(tri, ray, t, p);
}

/*!
 * \brief Determines if a 3D triangle intersects a 3D ray.
 * \param [in] tri A 3D triangle
 * \param [in] ray A 3D ray
 * \param [out] t Intersection point of tri and R, w.r.t. parametrization of R
 * \note If there is an intersection, the intersection point is:  R.at(t)
 * \return true iff tri intersects with ray, otherwise, false.
 */
template < typename T >
bool intersect(const Triangle< T, 3 >& tri, const Ray< T,3 >& ray, T& t)
{
  Point< double, 3 > p;
  return detail::intersect_tri_ray(tri, ray, t, p);
}

/*!
 * \brief Determines if a 3D triangle intersects a 3D ray.
 * \param [in] tri A 3D triangle
 * \param [in] ray A 3D ray
 * \param [out] t Intersection point of tri and R, w.r.t. parametrization of R
 * \param [out] p Intersection point of tri and R, in barycentric coordinates
 *   relative to tri.
 * \note If there is an intersection, the intersection point is:  R.at(t)
 * \return true iff tri intersects with ray, otherwise, false.
 */
template < typename T >
bool intersect(const Triangle< T, 3 >& tri, const Ray< T,3 >& ray,
               T& t, Point< double, 3 > & p)
{
  bool retval = detail::intersect_tri_ray(tri, ray, t, p);
  double normalizer = p[0] + p[1] + p[2];
  SLIC_CHECK_MSG( std::abs(normalizer) > 1e-6,
     "Barycentric coordinates sum to less than 1e-6 prior to normalization." );
  p.array() *= 1. / normalizer;
  return retval;
}

/*!
 * \brief Determines if a 3D triangle intersects a 3D segment.
 * \param [in] tri A 3D triangle
 * \param [in] seg A 3D line segment
 * \return true iff tri intersects with seg, otherwise, false.
 */
template < typename T >
bool intersect(const Triangle< T, 3 >& tri, const Segment< T,3 >& seg)
{
  T t = T();
  Point< double, 3 > p;
  return detail::intersect_tri_segment(tri, seg, t, p);
}

/*!
 * \brief Determines if a 3D triangle intersects a 3D segment.
 * \param [in] tri A 3D triangle
 * \param [in] seg A 3D line segment
 * \param [out] t Intersection point of tri and seg, w.r.t. seg's parametrization
 * \return true iff tri intersects with seg, otherwise, false.
 */
template < typename T >
bool intersect(const Triangle< T, 3 >& tri, const Segment< T,3 >& seg, T& t)
{
  Point< double, 3 > p;
  return detail::intersect_tri_segment(tri, seg, t, p);
}

/*!
 * \brief Determines if a 3D triangle intersects a 3D segment.
 * \param [in] tri A 3D triangle
 * \param [in] seg A 3D line segment
 * \param [out] t Intersection point of tri and seg, w.r.t. seg's parametrization
 * \param [out] p Intersection point of tri and R, in barycentric coordinates
 *   relative to tri.
 * \note If there is an intersection, the intersection point pt is:
 *                     pt = seg.source() + t * ( seg.dest() - seg.target() )
 * \return true iff tri intersects with seg, otherwise, false.
 */
template < typename T >
bool intersect(const Triangle< T, 3 >& tri, const Segment< T,3 >& seg,
               T& t, Point< double, 3 > & p)
{
  bool retval = detail::intersect_tri_segment(tri, seg, t, p);
  double normalizer = p[0] + p[1] + p[2];
  SLIC_CHECK_MSG( std::abs(normalizer) > 1e-6,
     "Barycentric coordinates sum to less than 1e-6 prior to normalization." );
  double scale = 1.0 / normalizer;
  p = Point< double, 3 >(scale * p.array());
  return retval;
}

template < typename T >
bool intersect(const OrientedBoundingBox< T, 1 > & b1,
               const OrientedBoundingBox< T, 1 >& b2)
{
  return detail::intersect_obb1D_obb1D(b1, b2);
}

/*!
 * \brief Determines if a 2D OBB intersects a 2D OBB.
 * \param [in] b1 A 2D OrientedBoundingBox
 * \param [in] b2 A 2D OrientedBoundingBox
 * \return true iff b1 intersects with b2, otherwise, false.
 */
template < typename T >
bool intersect(const OrientedBoundingBox< T, 2 >& b1,
               const OrientedBoundingBox< T, 2 >& b2)
{
  return detail::intersect_obb2D_obb2D(b1, b2);
}

/*!
 * \brief Determines if a 3D OBB intersects a 3D OBB.
 * \param [in] b1 A 3D OrientedBoundingBox
 * \param [in] b2 A 3D OrientedBoundingBox
 * \param [in] EPS error tolerance for intersection
 * \return true iff b1 intersects with b2, otherwise, false.
 */
template < typename T >
bool intersect(const OrientedBoundingBox< T, 3 >& b1,
               const OrientedBoundingBox< T, 3 >& b2, double EPS=1E-4)
{
  return detail::intersect_obb3D_obb3D(b1, b2, EPS);
}

} /* namespace primal */
} /* namespace axom */

#endif /* INTERSECTION_HPP_ */
