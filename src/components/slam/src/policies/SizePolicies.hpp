/*
 * Copyright (c) 2015, Lawrence Livermore National Security, LLC.
 * Produced at the Lawrence Livermore National Laboratory.
 *
 * All rights reserved.
 *
 * This source code cannot be distributed without permission and
 * further review from Lawrence Livermore National Laboratory.
 */

/**
 * \file SizePolicies.hpp
 *
 * \brief Size policies for SLAM
 *
 * Size policies are meant to represent the size of a SLAM entity (e.g. the size of a set).
 * A valid size policy must support the following interface:
 *   * [required]
 *     * DEFAULT_VALUE is a public static constant of type IntType
 *     * size() : IntType  -- returns the underlying integer size
 *     * empty() : bool -- returns whether the size is zero
 *     * isValid() : bool -- indicates whether the Size policy of the set is valid
 *   * [optional]
 *     * operator(): IntType -- alternate accessor for the size value
 */

#ifndef SLAM_POLICIES_SIZE_H_
#define SLAM_POLICIES_SIZE_H_

#include "axom/Macros.hpp"
#include "slic/slic.hpp"

namespace axom {
namespace slam {
namespace policies {

  /**
   * \name OrderedSet_Size_Policies
   * \brief A few default policies for the size of an OrderedSet
   */

  /// \{

  /**
   * \brief A policy class for the size of a set whose value can be set at runtime.
   */
  template<typename IntType>
  struct RuntimeSize
  {
  public:
    static const IntType DEFAULT_VALUE = IntType();

    RuntimeSize(IntType sz = DEFAULT_VALUE) : m_sz(sz) {}

    inline IntType          size() const { return m_sz; }
    inline IntType&         size() { return m_sz; }

    inline IntType operator ()() const { return size(); }
    inline IntType& operator()() { return size(); }

    inline bool             empty() const { return m_sz == IntType(); }
    inline bool             isValid(bool) const { return m_sz >= IntType(); } // We do not (currently) allow negatively sized sets
  private:
    IntType m_sz;
  };

  /**
   * \brief A policy class for a compile-time known set size
   */
  template<typename IntType, IntType INT_VAL>
  struct CompileTimeSize
  {
    static const IntType DEFAULT_VALUE = INT_VAL;

    CompileTimeSize(IntType val = DEFAULT_VALUE)
    {
      AXOM_DEBUG_VAR(val);
      SLIC_ASSERT_MSG( val == INT_VAL,
          "slam::CompileTimeSize -- tried to initialize a compile time size policy with value ("
          << val << " ) that differs from the template parameter of " << INT_VAL << ".");
    }

    inline IntType          size() const { return INT_VAL; }

    inline IntType operator ()() const { return size(); }

    inline IntType          empty() const { return INT_VAL == IntType(); }
    inline bool             isValid(bool) const { return INT_VAL >= IntType(); } // We do not (currently) allow negatively sized sets
  };

  /**
   * \brief A policy class for an empty set (no size)
   */
  template<typename IntType>
  struct ZeroSize
  {
    static const IntType DEFAULT_VALUE = IntType();

    ZeroSize(IntType val = DEFAULT_VALUE)
    {
      AXOM_DEBUG_VAR(val);
      SLIC_ASSERT_MSG( val == DEFAULT_VALUE,
          "slam::ZeroSize policy-- tried to initialize a NoSize set with value with value ("
          << val << " ) but should always be zero.");
    }

    inline IntType          size() const { return DEFAULT_VALUE; }
    inline IntType operator ()()  const { return size(); }
    inline IntType          empty() const { return true; }
    inline bool             isValid(bool) const { return true; }
  };

  /// \}

} // end namespace policies
} // end namespace slam
} // end namespace axom

#endif // SLAM_POLICIES_SIZE_H_
