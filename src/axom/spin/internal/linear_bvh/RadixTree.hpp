// Copyright (c) 2017-2022, Lawrence Livermore National Security, LLC and
// other Axom Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)

#ifndef AXOM_SPIN_RADIXTREE_HPP_
#define AXOM_SPIN_RADIXTREE_HPP_

#include "axom/core/Array.hpp"

#include "axom/core/utilities/AnnotationMacros.hpp"  // for annotations

#include "axom/primal/geometry/BoundingBox.hpp"

namespace axom
{
namespace spin
{
namespace internal
{
namespace linear_bvh
{
/*!
 * \brief RadixTree provides a binary radix tree representation that stores a
 *  list of axis-aligned bounding boxes sorted according to their Morton code.
 *
 * \note This data-structure provides an intermediate representation that serves
 *  as the building-block to construct a BVH in parallel.
 */
template <typename FloatType, int NDIMS>
struct RadixTree
{
  using BoxType = primal::BoundingBox<FloatType, NDIMS>;

  int32 m_size;
  int32 m_inner_size;

  axom::Array<int32> m_left_children;
  axom::Array<int32> m_right_children;
  axom::Array<int32> m_parents;
  axom::Array<BoxType> m_inner_aabbs;

  axom::Array<int32> m_leafs;
  axom::Array<uint32> m_mcodes;
  axom::Array<BoxType> m_leaf_aabbs;

  void allocate(int32 size, int allocID)
  {
    AXOM_PERF_MARK_FUNCTION("RadixTree::allocate");

    m_size = size;
    m_inner_size = m_size - 1;
    int32 parent_size = m_size + m_inner_size;

    m_left_children = axom::Array<int32>(m_inner_size, m_inner_size, allocID);
    m_right_children = axom::Array<int32>(m_inner_size, m_inner_size, allocID);
    m_parents = axom::Array<int32>(parent_size, parent_size, allocID);
    m_inner_aabbs = axom::Array<BoxType>(ArrayOptions::Uninitialized {},
                                         m_inner_size,
                                         m_inner_size,
                                         allocID);

    m_leafs = axom::Array<int32>(m_size, m_size, allocID);
    m_mcodes = axom::Array<uint32>(m_size, m_size, allocID);
    m_leaf_aabbs =
      axom::Array<BoxType>(ArrayOptions::Uninitialized {}, m_size, m_size, allocID);
  }
};

} /* namespace linear_bvh */
} /* namespace internal */
} /* namespace spin */
} /* namespace axom */

#endif /* AXOM_RADIXTREE_HPP_ */
