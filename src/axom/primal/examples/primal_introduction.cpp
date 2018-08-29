/*
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Copyright (c) 2018, Lawrence Livermore National Security, LLC.
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

/*! \file primal_introduction.cpp
 *  \brief This example code is a demonstration of Axom's primal component.
 *
 *  This file shows how to use Primal to represent geometric primitives, perform
 *  geometric operations, and use a spatial index.  Running the executable from
 *  this file will produce a collection of Asymptote source files.  When
 *  compiled, the Asymptote files produce the figures that accompany primal's
 *  Sphinx documentation.
 */

/* This example code contains snippets used in the Primal Sphinx documentation.
 * They begin and end with comments such as
 *
 * prims_header_start
 * prims_header_end
 * clip_header_start
 * clip_header_end
 * closest_point_header_start
 * closest_point_header_end
 *
 * each prepended with an underscore.
 */

// _prims_header_start
// Axom primitives
#include "axom/primal/geometry/BoundingBox.hpp"
#include "axom/primal/geometry/OrientedBoundingBox.hpp"
#include "axom/primal/geometry/Point.hpp"
#include "axom/primal/geometry/Polygon.hpp"
#include "axom/primal/geometry/Ray.hpp"
#include "axom/primal/geometry/Segment.hpp"
#include "axom/primal/geometry/Triangle.hpp"
#include "axom/primal/geometry/Vector.hpp"
// _prims_header_end

// Axom operations
// Each header is used in its own example, so each one is bracketed for separate
// inclusion.
// _clip_header_start
#include "axom/primal/operators/clip.hpp"
// _clip_header_end
// _closest_point_header_start
#include "axom/primal/operators/closest_point.hpp"
// _closest_point_header_end
// _bbox_header_start
#include "axom/primal/operators/compute_bounding_box.hpp"
// _bbox_header_end
// _intersect_header_start
#include "axom/primal/operators/intersect.hpp"
// _intersect_header_end
// _orient_header_start
#include "axom/primal/operators/orientation.hpp"
// _orient_header_end
// _sqdist_header_start
#include "axom/primal/operators/squared_distance.hpp"
// _sqdist_header_end

// C++ headers
#include <cmath> // do we need this?
#include <iostream>
#include <fstream>

#include "fmt/fmt.hpp"

// _using_start
// "using" directives to simplify code
using namespace axom;
using namespace primal;

// almost all our examples are in 3D
const int in3D = 3;
const int in2D = 2;

// primitives represented by doubles in 3D
typedef Point<double, in3D> PointType;
typedef Triangle<double, in3D> TriangleType;
typedef BoundingBox<double, in3D> BoundingBoxType;
typedef OrientedBoundingBox<double, in3D> OrientedBoundingBoxType;
typedef Polygon<double, in3D> PolygonType;
typedef Ray<double, in3D> RayType;
typedef Segment<double, in3D> SegmentType;
typedef Vector<double, in3D> VectorType;
// _using_end

// Axom spatial index
// _ugrid_triintersect_header_start
#include "axom/primal/spatial_acceleration/UniformGrid.hpp"
// the UniformGrid will store ints ("thing" indexes) in 3D
typedef UniformGrid<int, in3D> UniformGridType;
// _ugrid_triintersect_header_end
// _bvhtree_header_start
#include "axom/primal/spatial_acceleration/BVHTree.hpp"
// the BVHTree is in 2D, storing an index to 2D triangles
typedef BVHTree<int, in2D> BVHTree2DType;
// supporting classes
typedef BoundingBox<double, in2D> BoundingBox2DType;
typedef Point<double, in2D> Point2DType;
typedef Triangle<double, in2D> Triangle2DType;
// _bvhtree_header_end

std::string printPoint(PointType pt)
{
  return fmt::format("({},{},{})", pt[0], pt[1], pt[2]);
}

std::string printPoint(Point2DType pt)
{
  return fmt::format("({},{})", pt[0], pt[1]);
}

std::string printPoint(double* pt)
{
  return fmt::format("({},{},{})", pt[0], pt[1], pt[2]);
}

void writeToFile(std::string fname, std::string contents)
{
  std::ofstream outfile(fname);
  if (!outfile.good())
  {
    std::cout << "Could not write to " << fname << std::endl;
  }
  else
  {
    outfile << contents;
  }
}

PolygonType showClip()
{
  // _clip_start
  TriangleType tri(PointType::make_point(1.2,   0,   0),
                   PointType::make_point(  0, 1.8,   0),
                   PointType::make_point(  0,   0, 1.4));

  BoundingBoxType bbox(PointType::make_point(0, -0.5, 0),
                       PointType::make_point(1,    1, 1));

  PolygonType poly = clip(tri, bbox);
  // _clip_end

  std::cout << "----- showClip -----" << std::endl;
  std::cout << "clipping triangle " << tri << " with bounding box " << bbox <<
    " gives polygon " << poly << std::endl << std::endl;

  // Now write out an Asymptote file showing what we did.
  std::string basefname = "showClip";
  std::string fname = basefname + ".asy";
  std::string ifname = basefname + ".png";

  std::string polygon;
  for (int i = 0 ; i < poly.numVertices() ; ++i)
  {
    polygon += printPoint(poly[i]);
    polygon += "--";
  }
  polygon += "cycle";

  std::string contents = fmt::format(
    "// To turn this Asymptote source file into an image for inclusion in\n"
    "// Axom's documentation,\n"
    "// 1. run Asymptote:\n"
    "//    asy -f png {0}\n"
    "// 2. Optionally, use ImageMagick to convert the white background to transparent:\n"
    "//    convert {1} -transparent white {1}\n\n"

    "// preamble\n"
    "settings.render = 6;\n"
    "import three;\n"
    "size(6cm, 0);\n\n"

    "// axes\n"
    "draw(O -- 1.7X, arrow=Arrow3(DefaultHead2), "
      "L=Label(\"$x$\", position=EndPoint, align=W));\n"
    "draw(O -- 2.4Y, arrow=Arrow3(), "
      "L=Label(\"$y$\", position=EndPoint));\n"
    "draw(O -- 2Z, arrow=Arrow3(), "
      "L=Label(\"$z$\", position=EndPoint));\n\n"

    "// polygon\n"
    "path3 pgon = {2};\n\n"

    "// triangle\n"
    "path3 tri = {3}--{4}--{5}--cycle;\n\n"

    "// draw triangle then polygon\n"
    "draw(surface(tri), surfacepen=blue+opacity(0.4));\n"
    "draw(tri);\n\n"
    "draw(surface(pgon), surfacepen=yellow+opacity(0.4));\n"
    "draw(pgon, yellow);\n\n"

    "// bounding box\n"
    "draw(box({6}, {7}));\n",
    fname, ifname, polygon,
    printPoint(tri[0]), printPoint(tri[1]), printPoint(tri[2]),
    printPoint(bbox.getMin()), printPoint(bbox.getMax()));

  writeToFile(fname, contents);

  return poly;
}

void showClosestPoint()
{
  // _closest_point_start
  TriangleType tri(PointType::make_point(1, 0, 0),
                   PointType::make_point(0, 1, 0),
                   PointType::make_point(0, 0, 1));

  PointType pto = PointType::make_point( 0, 0, 0);
  PointType pta = PointType::make_point(-1, 2, 1);

  // Query point o lies at the origin.  Its closest point lies in the
  // interior of tri.
  PointType cpto = closest_point(pto, tri);

  // Query point a lies farther from the triangle.  Its closest point
  // is on tri's edge.
  int lcpta = 0;
  PointType cpta = closest_point(pta, tri, &lcpta);
  // _closest_point_end

  std::cout << "----- showClosestPoint -----" << std::endl;
  std::cout << "For triangle " << tri << "," << std::endl <<
    "point closest to " << pto << " is " << cpto << std::endl <<
    "point closest to " << pta << " is " << cpta << std::endl << std::endl;

  // Now write out an Asymptote file showing what we did.
  // Projected points
  PointType ppta = PointType::make_point(pta[0], pta[1], 0.);
  PointType pcpta = PointType::make_point(cpta[0], cpta[1], 0.);
  PointType pcpto = PointType::make_point(cpto[0], cpto[1], 0.);
  std::string basefname = "showClosestPoint";
  std::string fname = basefname + ".asy";
  std::string ifname = basefname + ".png";

  std::string contents = fmt::format(
    "// To turn this Asymptote source file into an image for inclusion in\n"
    "// Axom's documentation,\n"
    "// 1. run Asymptote:\n"
    "//    asy -f png {0}\n"
    "// 2. Optionally, use ImageMagick to convert the white background to transparent:\n"
    "//    convert {1} -transparent white {1}\n\n"

    "// preamble\n"
    "settings.render = 6;\n"
    "import three;\n"
    "size(6cm, 0);\n\n"

    "// axes\n"
    "draw(-4.5X -- 1.7X, arrow=Arrow3(DefaultHead2), "
      "L=Label(\"$x$\", position=EndPoint, align=W));\n"
    "draw(O -- 2.4Y, arrow=Arrow3(), "
      "L=Label(\"$y$\", position=EndPoint));\n"
    "draw(O -- 2Z, arrow=Arrow3(), "
      "L=Label(\"$z$\", position=EndPoint));\n\n"

    "// triangle\n"
    "path3 tri = {2}--{3}--{4}--cycle;\n\n"

    "// triangle\n"
    "triple pto = {5};\n"
    "triple pta = {6};\n"
    "triple cpto = {7};\n"
    "triple cpta = {8};\n"
    "triple ppta = {9};\n"
    "triple pcpto = {10};\n"
    "triple pcpta = {11};\n\n"

    "// draw triangle then points and projections\n"
    "draw(tri);\n"
    "dot(pto, blue);\n"
    "label(\"$o$\", pto, align=W);\n"
    "dot(cpto, mediumblue);\n"
    "label(\"$o'$\", cpto, align=N);\n"
    "draw(cpto--pcpto, dotted);\n"
    "dot(pta, lightolive);\n"
    "label(\"$a$\", pta, align=W);\n"
    "draw(pta--ppta, dotted);\n"
    "dot(cpta, yellow);\n"
    "label(\"$a'$\", cpta, align=NE);\n"
    "draw(cpta--pcpta, dotted);\n",
    fname, ifname,
    printPoint(tri[0]), printPoint(tri[1]), printPoint(tri[2]),
    printPoint(pto), printPoint(pta), printPoint(cpto),
    printPoint(cpta), printPoint(ppta), printPoint(pcpto),
    printPoint(pcpta));

  writeToFile(fname, contents);
}


void showBoundingBoxes()
{
  // _bbox_start
  // An array of Points to include in the bounding boxes
  const int nbr_points = 6;
  PointType data[nbr_points];
  data[0] = PointType::make_point(0.6, 1.2, 1.0);
  data[1] = PointType::make_point(1.3, 1.6, 1.8);
  data[2] = PointType::make_point(2.9, 2.4, 2.3);
  data[3] = PointType::make_point(3.2, 3.5, 3.0);
  data[4] = PointType::make_point(3.6, 3.2, 4.0);
  data[5] = PointType::make_point(4.3, 4.3, 4.5);

  // A BoundingBox constructor takes an array of Point objects
  BoundingBoxType bbox(data, nbr_points);
  // Make an OrientedBoundingBox
  OrientedBoundingBoxType obbox =
    compute_oriented_bounding_box(data, nbr_points);
  // _bbox_end

  std::cout << "----- showBoundingBoxes -----" << std::endl;
  std::cout << "For the " << nbr_points << " points:" << std::endl;
  for (int i = 0 ; i < nbr_points ; ++i)
  {
    std::cout << data[i] << std::endl;
  }
  std::cout << "(Axis-aligned) bounding box is " << bbox << std::endl <<
    "oriented bounding box is " << obbox << std::endl;

  // Now write out an Asymptote file showing what we did.
  std::string pointses, dotses;
  for (int i = 0 ; i < nbr_points ; ++i)
  {
    pointses += fmt::format("points[{0}] = {1};\n", i, printPoint(data[i]));
    dotses += fmt::format("dot(points[{0}], blue);\n", i);
  }

  std::vector<PointType> obpts = obbox.vertices();
  std::string obboxpts;
  for (int i = 0 ; i < 8 ; ++i)
  {
    obboxpts += fmt::format("obpts[{0}] = {1};\n", i, printPoint(obpts[i]));
  }

  std::string basefname = "showBoundingBoxes";
  std::string fname = basefname + ".asy";
  std::string ifname = basefname + ".png";
  std::string contents = fmt::format(
    "// To turn this Asymptote source file into an image for inclusion in\n"
    "// Axom's documentation,\n"
    "// 1. run Asymptote:\n"
    "//    asy -f png {0}\n"
    "// 2. Optionally, use ImageMagick to convert the white background to transparent:\n"
    "//    convert {1} -transparent white {1}\n\n"

    "// preamble\n"
    "settings.render = 6;\n"
    "import three;\n"
    "size(6cm, 0);\n\n"

    "// projection\n"
    "currentprojection = perspective((4, -1.8, 3), (0.07, 0.07, 1));\n\n"

    "// axes\n"
    "draw(O -- 4X, arrow=Arrow3(DefaultHead2), "
      "L=Label(\"$x$\", position=EndPoint));\n"
    "draw(O -- 7Y, arrow=Arrow3(), "
      "L=Label(\"$y$\", position=EndPoint));\n"
    "draw(O -- 5Z, arrow=Arrow3(), "
      "L=Label(\"$z$\", position=EndPoint));\n\n"

    "// points\n"
    "triple[] points = new triple[6];\n"
    "{2}\n"

    "// bbox\n"
    "triple bboxmin = {3};\n"
    "triple bboxmax = {4};\n\n"

    "// oriented bounding box\n"
    "triple[] obpts = new triple[8];\n"
    "{5}\n"

    "// draw points\n"
    "{6}\n"

    "// draw bbox\n"
    "draw(box(bboxmin, bboxmax));\n\n"

    "// draw oriented bounding box\n"
    "path3[] obboxpath = obpts[0]--obpts[1]--obpts[3]--obpts[2]--cycle\n"
    "     ^^ obpts[4]--obpts[5]--obpts[7]--obpts[6]--cycle\n"
    "     ^^ obpts[0]--obpts[4] ^^ obpts[1]--obpts[5]\n"
    "     ^^ obpts[2]--obpts[6] ^^ obpts[3]--obpts[7];\n"
    "draw(obboxpath, orange);\n\n",
    fname, ifname, pointses,
    printPoint(bbox.getMin()), printPoint(bbox.getMax()),
    obboxpts, dotses);

  writeToFile(fname, contents);
}

void showIntersect()
{
  std::cout << "----- showIntersect -----" << std::endl;

  // _intersect_start
  // Two triangles
  TriangleType tri1(PointType::make_point(1.2,   0,   0),
                    PointType::make_point(  0, 1.8,   0),
                    PointType::make_point(  0,   0, 1.4));

  TriangleType tri2(PointType::make_point(  0,   0, 0.5),
                    PointType::make_point(0.8, 0.1, 1.2),
                    PointType::make_point(0.8, 1.4, 1.2));

  // tri1 and tri2 should intersect
  if (intersect(tri1, tri2))
  {
    std::cout << "Triangles intersect as expected." << std::endl;
  }
  else
  {
    std::cout << "There's an error somewhere..." << std::endl;
  }

  // A vertical ray constructed from origin and point
  RayType ray(SegmentType(PointType::make_point(0.4, 0.4, 0),
                          PointType::make_point(0.4, 0.4, 1)));

  // t will hold the intersection point between ray and tri1,
  // as parameterized along ray.
  double rt1t = 0;
  // rt1b will hold the intersection point barycentric coordinates,
  // and rt1p will hold the physical coordinates.
  PointType rt1b, rt1p;

  // The ray should intersect tri1 and tri2.
  if (intersect(tri1, ray, rt1t, rt1b) && intersect(tri2, ray))
  {
    rt1p = tri1.baryToPhysical(rt1b);
    std::cout << "Ray intersects tri1 as expected.  Parameter t: " <<
      rt1t << std::endl << "  Intersect barycentric coordinates: " << rt1b <<
      std::endl << "  Intersect physical coordinates: " << rt1p << std::endl <<
      "Ray also intersects tri2 as expected." << std::endl;
  }
  else
  {
    std::cout << "There's an error somewhere..." << std::endl;
  }

  // A bounding box
  BoundingBoxType bbox(PointType::make_point(0.1, -0.23, 0.1),
                       PointType::make_point(0.8,  0.5,  0.4));

  // The bounding box should intersect tri1 and ray but not tr2.
  PointType bbtr1;
  if (intersect(ray, bbox, bbtr1) && intersect(tri1, bbox) &&
      !intersect(tri2, bbox))
  {
    std::cout << "As hoped, bounding box intersects tri1 at " << bbtr1 <<
      " and ray, but not tri2." << std::endl;
  }
  else
  {
    std::cout << "There is at least one error somewhere..." << std::endl;
  }
  // _intersect_end

  // helper variables
  PolygonType poly = clip(tri1, bbox);
  // These are parametric coordinates ray-tri2-t, tri1-tri2 leg A-t,
  // tri1-tri2 leg C-t
  // and corresponding physical points.
  double rt2t, t1t2at, t1t2ct;
  (void)intersect(tri2, ray, rt2t);
  PointType rt2p = ray.at(rt2t);
  SegmentType t2lega(tri2[0], tri2[1]);
  (void)intersect(tri1, t2lega, t1t2at);
  PointType t1t2ap = t2lega.at(t1t2at);
  SegmentType t2legc(tri2[2], tri2[0]);
  (void)intersect(tri1, t2legc, t1t2ct);
  PointType t1t2cp = t2legc.at(t1t2ct);
  // Project point C of tri2 onto the XY plane
  PointType tr2c = tri2[2];
  PointType pp = PointType::make_point(tr2c[0], tr2c[1], 0.);

  // Now write out an Asymptote file showing what we did.
  std::string basefname = "showIntersect";
  std::string fname = basefname + ".asy";
  std::string ifname = basefname + ".png";

  std::string polygon;
  for (int i = 0 ; i < poly.numVertices() ; ++i)
  {
    polygon += printPoint(poly[i]);
    polygon += "--";
  }
  polygon += "cycle";

  std::string contents = fmt::format(
    "// To turn this Asymptote source file into an image for inclusion in\n"
    "// Axom's documentation,\n"
    "// 1. run Asymptote:\n"
    "//    asy -f png {0}\n"
    "// 2. Optionally, use ImageMagick to convert the white background to transparent:\n"
    "//    convert {1} -transparent white {1}\n\n"

    "// preamble\n"
    "settings.render = 6;\n"
    "import three;\n"
    "size(6cm, 0);\n\n"

    "// axes\n"
    "draw(O -- 1.7X, arrow=Arrow3(DefaultHead2), "
      "L=Label(\"$x$\", position=EndPoint));\n"
    "draw(O -- 2.4Y, arrow=Arrow3(), "
      "L=Label(\"$y$\", position=EndPoint));\n"
    "draw(O -- 2Z, arrow=Arrow3(), "
      "L=Label(\"$z$\", position=EndPoint, align=W));\n\n"

    "// triangle 1\n"
    "path3 tri1 = {2}--{3}--{4}--cycle;\n\n"

    "// triangle 2\n"
    "path3 tri2 = {5}--{6}--{7}--cycle;\n\n"

    "// ray\n"
    "path3 ray = {8}--{9};\n\n"

    "// polygon of intersection between bbox and triangle\n"
    "path3 pgon = {10};\n\n"

    "// draw bounding box and other geometry\n"
    "draw(box({11}, {12}), blue);\n"
    "draw(pgon, deepblue);\n\n"
    "draw(ray, arrow=Arrow3(DefaultHead2), red);\n"
    "  dot({13}, red);\n  dot({14}, red);\n  dot({15}, red);\n"
    "  draw(tri1);\n  draw(tri2, blue);\n"
    "draw({16}--{17}, deepblue);\n"
    "draw({18}--{19}, dotted);\n",
    fname, ifname,
    printPoint(tri1[0]), printPoint(tri1[1]), printPoint(tri1[2]),
    printPoint(tri2[0]), printPoint(tri2[1]), printPoint(tri2[2]),
    printPoint(ray.origin()), printPoint(ray.at(1.8)),
    polygon,
    printPoint(bbox.getMin()), printPoint(bbox.getMax()),
    printPoint(bbtr1), printPoint(rt1p), printPoint(rt2p),
    printPoint(t1t2ap), printPoint(t1t2cp),
    printPoint(tr2c), printPoint(pp));

  writeToFile(fname, contents);
}

void showOrientation()
{
  std::cout << "----- showOrientation -----" << std::endl;

  // _orient_start
  // A triangle
  TriangleType tri(PointType::make_point(1.2,   0,   0),
                   PointType::make_point(  0, 1.8,   0),
                   PointType::make_point(  0,   0, 1.4));

  // Three points:
  //    one on the triangle's positive side,
  PointType pos = PointType::make_point(0.45, 1.5, 1);
  //    one coplanar to the triangle, the centroid,
  PointType cpl = PointType::lerp(PointType::lerp(tri[0], tri[1], 0.5),
                                  tri[2], 1./3.);
  //    and one on the negative side
  PointType neg = PointType::make_point(0, 0, 0.7);

  // Test orientation
  if (orientation(pos, tri)  == ON_POSITIVE_SIDE &&
      orientation(cpl, tri) == ON_BOUNDARY &&
      orientation(neg, tri)  == ON_NEGATIVE_SIDE)
  {
    std::cout << "As expected, point pos is on the positive side," <<
      std::endl << "    point cpl is on the boundary (on the triangle)," <<
      std::endl << "    and point neg is on the negative side." << std::endl;
  }
  else
  {
    std::cout << "Someone wrote this wrong." << std::endl;
  }
  // _orient_end

  // Helper variables
  // Project onto the XY plane
  PointType ppos = PointType::make_point(pos[0], pos[1], 0.);
  PointType pcpl = PointType::make_point(cpl[0], cpl[1], 0.);

  // Now write out an Asymptote file showing what we did.
  std::string basefname = "showOrientation";
  std::string fname = basefname + ".asy";
  std::string ifname = basefname + ".png";

  std::string contents = fmt::format(
    "// To turn this Asymptote source file into an image for inclusion in\n"
    "// Axom's documentation,\n"
    "// 1. run Asymptote:\n"
    "//    asy -f png {0}\n"
    "// 2. Optionally, use ImageMagick to convert the white background to transparent:\n"
    "//    convert {1} -transparent white {1}\n\n"

    "// preamble\n"
    "settings.render = 6;\n"
    "import three;\n"
    "size(6cm, 0);\n\n"

    "// axes\n"
    "draw(O -- 1.7X, arrow=Arrow3(DefaultHead2), "
      "L=Label(\"$x$\", position=EndPoint));\n"
    "draw(O -- 2.4Y, arrow=Arrow3(), "
      "L=Label(\"$y$\", position=EndPoint));\n"
    "draw(O -- 2Z, arrow=Arrow3(), "
      "L=Label(\"$z$\", position=EndPoint, align=W));\n\n"

    "// triangle\n"
    "path3 tri = {2}--{3}--{4}--cycle;\n\n"
    "triple centroid = {5};\n"

    "draw(tri);\n"
    "dot({6}, blue);\n"
    "label(\"$N$\", {6}, align=W);\n"
    "dot({5}, blue);\n"
    "draw(centroid--1.6centroid, arrow=Arrow3(DefaultHead2));\n"
    "dot({7}, blue);\n"
    "label(\"$P$\", {7}, align=E);\n"
    "draw({7}--{8}, dotted);\n",
    fname, ifname,
    printPoint(tri[0]), printPoint(tri[1]), printPoint(tri[2]),
    printPoint(cpl), printPoint(neg),
    printPoint(pos), printPoint(ppos));

  writeToFile(fname, contents);
}

void showDistance()
{
  // _sqdist_start
  // The point from which we'll query
  PointType q = PointType::make_point(0.75, 1.2, 0.4);

  // Find distance to:
  PointType p = PointType::make_point(0.2, 1.4, 1.1);
  SegmentType seg(PointType::make_point(1.1, 0.0, 0.2),
                  PointType::make_point(1.1, 0.5, 0.2));
  TriangleType tri(PointType::make_point(0.2,  -0.3, 0.4),
                   PointType::make_point(0.25, -0.1, 0.3),
                   PointType::make_point(0.3,  -0.3, 0.35));
  BoundingBoxType bbox(PointType::make_point(-0.3, -0.2, 0.7),
                       PointType::make_point( 0.4,  0.3, 0.9));

  double dp = squared_distance(q, p);
  double dseg = squared_distance(q, seg);
  double dtri = squared_distance(q, tri);
  double dbox = squared_distance(q, bbox);
  // _sqdist_end

  std::cout << "----- showDistance -----" << std::endl;
  std::cout << "Squared distance from query point q " << q << std::endl <<
    dp   << " to point " << p << std::endl <<
    dseg << " to segment " << seg << std::endl <<
    dtri << " to triangle " << tri << std::endl <<
    dbox << " to bounding box " << bbox << std::endl << std::endl;

  // Helper variables
  // Project q and p onto onto XY plane
  PointType pq = PointType::make_point(q[0], q[1], 0.);
  PointType pp = PointType::make_point(p[0], p[1], 0.);
  PointType boxpt = bbox.getMax();
  boxpt[2] = bbox.getMin()[2];
  PointType pboxpt = boxpt;
  pboxpt[2] = 0;
  PointType pseg = seg.target();
  pseg[2] = 0;
  PointType ptri = tri[1];
  ptri[2] = 0;

  // Now write out an Asymptote file showing what we did.
  std::string basefname = "showDistance";
  std::string fname = basefname + ".asy";
  std::string ifname = basefname + ".png";
  std::string contents = fmt::format(
    "// To turn this Asymptote source file into an image for inclusion in\n"
    "// Axom's documentation,\n"
    "// 1. run Asymptote:\n"
    "//    asy -f png {0}\n"
    "// 2. Optionally, use ImageMagick to convert the white background to transparent:\n"
    "//    convert {1} -transparent white {1}\n\n"

    "// preamble\n"
    "settings.render = 6;\n"
    "import three;\n"
    "size(6cm, 0);\n\n"

    "// axes\n"
    "draw(O -- 1.3X, arrow=Arrow3(DefaultHead2), "
      "L=Label(\"$x$\", position=EndPoint, align=W));\n"
    "draw(O -- 1.8Y, arrow=Arrow3(), "
      "L=Label(\"$y$\", position=EndPoint));\n"
    "draw(O -- 1.2Z, arrow=Arrow3(), "
      "L=Label(\"$z$\", position=EndPoint, align=W));\n\n"

    "// query point\n"
    "triple q = {2};\n"
    "// other primitives\n"
    "triple boxpt = {3};\n"
    "triple p = {4};\n"
    "dot(q);    dot(p, blue);\n"
    "draw({5}--{6}, blue);\n"
    "draw({7}--{8}--{9}--cycle, blue);\n"
    "draw(box({10}, {11}), blue);\n\n"

    "// distances and drop-points\n"
    "draw(q--p, L=Label(\"{12}\"));\n"
    "draw(q--{6}, L=Label(\"{13}\"));\n"
    "draw(q--{8}, L=Label(\"{14}\"));\n"
    "draw(q--boxpt, L=Label(\"{15}\"));\n"
    "draw(q--{16}, dotted);\n"
    "draw(p--{17}, dotted);\n"
    "draw({6}--{18}, dotted);\n"
    "draw({8}--{19}, dotted);\n"
    "draw(boxpt--{20}, dotted);\n",
    fname, ifname,
    printPoint(q), printPoint(boxpt), printPoint(p),
    printPoint(seg.source()), printPoint(seg.target()),
    printPoint(tri[0]), printPoint(tri[1]), printPoint(tri[2]),
    printPoint(bbox.getMin()), printPoint(bbox.getMax()),
    dp, dseg, dtri, dbox,
    printPoint(pq), printPoint(pp), printPoint(pseg),
    printPoint(ptri), printPoint(pboxpt));

  writeToFile(fname, contents);
}

// _naive_triintersect_start
void findTriIntersectionsNaively(
  std::vector<TriangleType> & tris,
  std::vector< std::pair<int, int> > & clashes
  )
{
  int tcount = tris.size();

  for (int i = 0 ; i < tcount ; ++i)
  {
    TriangleType & t1 = tris[i];
    for (int j = i + 1 ; j < tcount ; ++j)
    {
      TriangleType & t2 = tris[j];
      if (intersect(t1, t2))
      {
        clashes.push_back(std::make_pair(i, j));
      }
    }
  }
}
// _naive_triintersect_end

BoundingBoxType findBbox(std::vector<TriangleType> & tris)
{
  BoundingBoxType bbox;

  for (size_t i = 0 ; i < tris.size() ; ++i)
  {
    bbox.addPoint(tris[i][0]);
    bbox.addPoint(tris[i][1]);
    bbox.addPoint(tris[i][2]);
  }

  return bbox;
}

BoundingBoxType findBbox(TriangleType & tri)
{
  BoundingBoxType bbox;

  bbox.addPoint(tri[0]);
  bbox.addPoint(tri[1]);
  bbox.addPoint(tri[2]);

  return bbox;
}

BoundingBox2DType findBbox(Triangle2DType & tri)
{
  BoundingBox2DType bbox;

  bbox.addPoint(tri[0]);
  bbox.addPoint(tri[1]);
  bbox.addPoint(tri[2]);

  return bbox;
}

// _ugrid_triintersect_start
BoundingBoxType findBbox(std::vector<TriangleType> & tris);
BoundingBoxType findBbox(TriangleType & tri);

void findTriIntersectionsAccel(
  std::vector<TriangleType> & tris,
  std::vector< std::pair<int, int> > & clashes
  )
{
  // Prepare to construct the UniformGrid.
  BoundingBoxType allbbox = findBbox(tris);
  const PointType & minBBPt = allbbox.getMin();
  const PointType & maxBBPt = allbbox.getMax();

  int tcount = tris.size();

  // The number of bins along one side of the UniformGrid.
  // This is a heuristic.
  int res = (int)(1 + std::pow(tcount, 1/3.));
  int ress[3] = {res, res, res};

  // Construct the UniformGrid with minimum point, maximum point,
  // and number of bins along each side.  Then insert the triangles.
  UniformGridType ugrid(minBBPt.data(), maxBBPt.data(), ress);
  for (int i = 0 ; i < tcount ; ++i)
  {
    TriangleType & t1 = tris[i];
    BoundingBoxType bbox = findBbox(t1);
    ugrid.insert(bbox, i);
  }

  // Now check for intersections.
  // For each triangle t1,
  for (int i = 0 ; i < tcount ; ++i)
  {
    TriangleType & t1 = tris[i];
    BoundingBoxType bbox = findBbox(t1);

    // Get all the bins t1 occupies
    std::vector<int> neighborTriangles;
    const std::vector<int> bToCheck = ugrid.getBinsForBbox(bbox);
    size_t checkcount = bToCheck.size();

    // Load all the triangles in these bins whose indices are
    // greater than i into a vector.
    for (size_t curb = 0 ; curb < checkcount ; ++curb)
    {
      std::vector<int> ntlist = ugrid.getBinContents(bToCheck[curb]);
      for (size_t j = 0 ; j < ntlist.size() ; ++j)
      {
        if (ntlist[j] > i)
        {
          neighborTriangles.push_back(ntlist[j]);
        }
      }
    }

    // Sort the neighboring triangles, and throw out duplicates.
    // This is not strictly necessary but saves some calls to intersect().
    std::sort(neighborTriangles.begin(), neighborTriangles.end());
    std::vector<int>::iterator jend =
      std::unique(neighborTriangles.begin(), neighborTriangles.end());
    std::vector<int>::iterator j = neighborTriangles.begin();

    // Test for intersection between t1 and each of its neighbors.
    while (j != jend)
    {
      TriangleType & t2 = tris[*j];
      if (primal::intersect(t1, t2))
      {
        clashes.push_back(std::make_pair(i, *j));
      }
      ++j;
    }
  }
}
// _ugrid_triintersect_end

void makeUniformGridTriangles(std::vector<TriangleType> & tris)
{
  PointType p[8];
  p[0] = PointType::make_point(0.3,  0.93, 0.03);
  p[1] = PointType::make_point(0.1,  0.85, 0.01);
  p[2] = PointType::make_point(0.3,  0.78, 0.03);
  p[3] = PointType::make_point(0.18, 0.36, 0.018);
  p[4] = PointType::make_point(0.8,  0.58, 0.08);
  p[5] = PointType::make_point(0.6,  0.5, 0.06);
  p[6] = PointType::make_point(0.55, 0.42, 0.055);
  p[7] = PointType::make_point(0.61, 0.1, 0.061);

  TriangleType t0(p[0], p[1], p[2]);    tris.push_back(t0);
  TriangleType t1(p[2], p[1], p[3]);    tris.push_back(t1);
  TriangleType t2(p[2], p[3], p[6]);    tris.push_back(t2);
  TriangleType t3(p[6], p[3], p[7]);    tris.push_back(t3);
  TriangleType t4(p[4], p[2], p[6]);    tris.push_back(t4);
  TriangleType t5(p[4], p[5], p[7]);    tris.push_back(t5);
}

void printPairs(std::string title,
                std::vector< std::pair<int, int> > & clashes)
{
  int ccount = clashes.size();
  std::cout << ccount << title << std::endl;
  for (int i = 0 ; i < ccount ; ++i)
  {
    std::cout << clashes[i].first << "   " << clashes[i].second << std::endl;
  }
}

void driveUniformGrid()
{
  std::vector<TriangleType> tris;
  makeUniformGridTriangles(tris);

  std::vector< std::pair<int, int> > naiveclashes;
  findTriIntersectionsNaively(tris, naiveclashes);

  std::vector< std::pair<int, int> > accelclashes;
  findTriIntersectionsAccel(tris, accelclashes);

  std::cout << "----- driveUniformGrid -----" << std::endl;
  printPairs(" clashes found by naive algorithm:", naiveclashes);
  printPairs(" clashes found by accelerated algorithm:", accelclashes);
  std::cout << std::endl;
}

// _bvhtree_start
BoundingBox2DType findBbox(Triangle2DType & tri);

BVHTree2DType* buildBVHTree(std::vector<Triangle2DType> & tris,
                            Point2DType ppoint,
                            std::vector<int> & intersections)
{
  // Initialize BVHTree with the triangles
  const int MaxBinFill = 1;
  const int MaxLevels = 4;
  int tricount = tris.size();
  BVHTree2DType* tree  = new BVHTree2DType( tricount, MaxLevels );

  for ( int i=0 ; i < tricount ; ++i )
  {
    tree->insert( findBbox( tris[i] ), i );
  }

  // Build bounding volume hierarchy
  tree->build( MaxBinFill );

  // Which triangles does the probe point intersect?
  // Get the candidate bins
  std::vector<int> bins;
  tree->find(ppoint, bins);
  size_t nbins = bins.size();
  std::vector< int > candidates;

  // for each candidate bin,
  for (size_t curb = 0 ; curb < nbins ; ++curb)
  {
    // get its size and object array
    int bcount = tree->getBucketNumObjects(bins[curb]);
    const int* ary = tree->getBucketObjectArray(bins[curb]);

    // For each object in the current bin,
    for (int j = 0 ; j < bcount ; ++j)
    {
      // find the tree's internal object ID
      int treeObjID = ary[j];
      // and use it to retrieve the triangle's ID.
      int triID = tree->getObjectData(treeObjID);

      // Then store the ID in the candidates list.
      candidates.push_back(triID);
    }
  }

  // Sort the candidate triangles, and throw out duplicates.
  // This is not strictly necessary but saves some calls to checkInTriangle().
  std::sort(candidates.begin(), candidates.end());
  std::vector<int>::iterator jend =
    std::unique(candidates.begin(), candidates.end());
  std::vector<int>::iterator j = candidates.begin();

  // Test if ppoint lands in any of its neighbor triangles.
  while (j != jend)
  {
    Triangle2DType & t = tris[*j];
    if (t.checkInTriangle(ppoint))
    {
      intersections.push_back(*j);
    }
    ++j;
  }

  return tree;
}
// _bvhtree_end

void makeBVHTreeTriangles(std::vector<Triangle2DType> & tris)
{
  Point2DType p[19];
  p[0] = Point2DType::make_point(.13, .88);
  p[1] = Point2DType::make_point(.26, .87);
  p[2] = Point2DType::make_point(.11, .77);
  p[3] = Point2DType::make_point(.18, .78);
  p[4] = Point2DType::make_point(.13, .74);
  p[5] = Point2DType::make_point(.37, .75);
  p[6] = Point2DType::make_point(.12, .61);
  p[7] = Point2DType::make_point(.25, .51);
  p[8] = Point2DType::make_point(.11, .44);
  p[9] = Point2DType::make_point(.26, .40);
  p[10] = Point2DType::make_point(.12, .25);
  p[11] = Point2DType::make_point(.85, .38);
  p[12] = Point2DType::make_point(.94, .37);
  p[13] = Point2DType::make_point(.84, .26);
  p[14] = Point2DType::make_point(.92, .27);
  p[15] = Point2DType::make_point(.96, .28);
  p[16] = Point2DType::make_point(.84, .16);
  p[17] = Point2DType::make_point(.92, .16);
  p[18] = Point2DType::make_point(.93, .09);

  Triangle2DType t0(p[1], p[0], p[3]);      tris.push_back(t0);
  Triangle2DType t1(p[0], p[2], p[3]);      tris.push_back(t1);
  Triangle2DType t2(p[1], p[3], p[5]);      tris.push_back(t2);
  Triangle2DType t3(p[3], p[2], p[4]);      tris.push_back(t3);
  Triangle2DType t4(p[3], p[4], p[5]);      tris.push_back(t4);
  Triangle2DType t5(p[2], p[6], p[4]);      tris.push_back(t5);
  Triangle2DType t6(p[5], p[4], p[6]);      tris.push_back(t6);
  Triangle2DType t7(p[5], p[6], p[7]);      tris.push_back(t7);
  Triangle2DType t8(p[7], p[6], p[8]);      tris.push_back(t8);
  Triangle2DType t9(p[7], p[8], p[9]);      tris.push_back(t9);
  Triangle2DType t10(p[9], p[8], p[10]);    tris.push_back(t10);
  Triangle2DType t11(p[11], p[13], p[14]);  tris.push_back(t11);
  Triangle2DType t12(p[12], p[11], p[14]);  tris.push_back(t12);
  Triangle2DType t13(p[12], p[14], p[15]);  tris.push_back(t13);
  Triangle2DType t14(p[14], p[13], p[16]);  tris.push_back(t14);
  Triangle2DType t15(p[14], p[16], p[17]);  tris.push_back(t15);
  Triangle2DType t16(p[16], p[18], p[17]);  tris.push_back(t16);
}

void driveBVHTree()
{
  std::vector<Triangle2DType> tris;
  makeBVHTreeTriangles(tris);
  Point2DType ppoint = Point2DType::make_point(0.45, 0.25);
  std::vector<int> intersections;

  BVHTree2DType* tree = buildBVHTree(tris, ppoint, intersections);
  tree->writeVtkFile("BVHTree.out.vtk");

  std::cout << "----- driveBVHTree -----" << std::endl;
  std::cout << "Point " << ppoint << " hit the following " <<
    intersections.size() << " triangles (0 expected):" << std::endl;
  for (size_t i = 0 ; i < intersections.size() ; ++i)
  {
    std::cout << intersections[i] << std::endl;
  }

  delete tree;
}

int main(int argc, char** argv)
{

  // Deal with unused variables
  AXOM_DEBUG_VAR(argc);
  AXOM_DEBUG_VAR(argv);

  showClip();
  showClosestPoint();
  showBoundingBoxes();
  showIntersect();
  showOrientation();
  showDistance();
  driveUniformGrid();
  driveBVHTree();

  return 0;
}
