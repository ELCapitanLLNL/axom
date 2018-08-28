Operators
---------

Primal implements geometric operators with unbound functions.  Currently, these include
the following: 

- ``clip`` finds the polygon resulting from a bounding box clipping a triangle.
- ``closest_point`` takes a primitive P and a query point Q and returns the point
  on P closest to Q.
- ``compute_bounding_box`` finds the bounding box for a given primitive.
- ``squared_distance`` computes the squared distance from a point to another primitive.
- ``orientation`` finds the side of a line segment or triangle where a query point lies.
- ``intersect`` predicate tests if two primitives intersect.  Some of the combinations
  also indicate the point of intersection of a 1D primitive with another primitive.

.. note::
   Most use cases have low dimension, usually 2 or 3.  Dimensionality has been
   generalized to support other values where it does not interfere with the
   common case, but some operators such as triangle intersection do not support
   other dimensionality than 2 or 3.

.. toctree::
   :maxdepth: 2

   opclip
   opclosestpoint
   opbbox
   opintersect
   oporientation
   opsqdist

