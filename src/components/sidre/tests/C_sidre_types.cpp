/*
 * Copyright (c) 2015, Lawrence Livermore National Security, LLC.
 * Produced at the Lawrence Livermore National Laboratory.
 *
 * All rights reserved.
 *
 * This source code cannot be distributed without permission and
 * further review from Lawrence Livermore National Laboratory.
 */

#include "gtest/gtest.h"

#include <limits>

#include "axom/CommonTypes.hpp"

#include "sidre/sidre.hpp"
#include "sidre/SidreTypes.h"
#include "sidre/SidreTypes.hpp"

using axom::sidre::DataType;
using axom::sidre::TypeID;
using axom::sidre::getTypeID;

namespace
{

/**
 * \brief Tests the equivalence of two integral types
 * \see These test complement the ones in common/tests/common_test.cpp
 */
template<typename CommonType, typename SidreType>
void testTypesForEquality()
{
  // check if both are integral types
  bool com_is_int = std::numeric_limits<CommonType>::is_integer;
  bool sid_is_int = std::numeric_limits<SidreType>::is_integer;
  EXPECT_EQ(com_is_int, sid_is_int);

  // check if both are signed (or both are unsigned)
  bool com_is_signed = std::numeric_limits<CommonType>::is_signed;
  bool sid_is_signed = std::numeric_limits<SidreType>::is_signed;
  EXPECT_EQ( com_is_signed, sid_is_signed);

  // check that both have same number of bytes
  EXPECT_EQ(sizeof(CommonType), sizeof(SidreType) );

  // check that both have same number of digits
  EXPECT_EQ(std::numeric_limits<CommonType>::digits
            , std::numeric_limits<SidreType>::digits);

}

}

//------------------------------------------------------------------------------
// This test verifies the equivalence of the fixed-width types
// defined in the common and sidre toolkit components
//------------------------------------------------------------------------------
TEST(sidre_types,compare_common_types)
{
  namespace com = axom::common;
  namespace sid = axom::sidre::detail;

  testTypesForEquality<com::int8, sid::sidre_int8>();
  testTypesForEquality<com::uint8, sid::sidre_uint8>();

  testTypesForEquality<com::int16,sid::sidre_int16>();
  testTypesForEquality<com::uint16,sid::sidre_uint16>();

  testTypesForEquality<com::int32,sid::sidre_int32>();
  testTypesForEquality<com::uint32,sid::sidre_uint32>();

  #ifndef AXOM_NO_INT64_T
  testTypesForEquality<com::int64,sid::sidre_int64>();
  testTypesForEquality<com::uint64,sid::sidre_uint64>();
  #endif
}

//------------------------------------------------------------------------------
// This test verifies that the Conduit, Sidre C++ and C use the same types.
//------------------------------------------------------------------------------
TEST(sidre_types,get_sidre_type)
{
  EXPECT_EQ(SIDRE_INT8_ID,      CONDUIT_INT8_ID);
  EXPECT_EQ(SIDRE_INT16_ID,     CONDUIT_INT16_ID);
  EXPECT_EQ(SIDRE_INT32_ID,     CONDUIT_INT32_ID);
  EXPECT_EQ(SIDRE_INT64_ID,     CONDUIT_INT64_ID);
  EXPECT_EQ(SIDRE_UINT8_ID,     CONDUIT_UINT8_ID);
  EXPECT_EQ(SIDRE_UINT16_ID,    CONDUIT_UINT16_ID);
  EXPECT_EQ(SIDRE_UINT32_ID,    CONDUIT_UINT32_ID);
  EXPECT_EQ(SIDRE_UINT64_ID,    CONDUIT_UINT64_ID);
  EXPECT_EQ(SIDRE_FLOAT32_ID,   CONDUIT_FLOAT32_ID);
  EXPECT_EQ(SIDRE_FLOAT64_ID,   CONDUIT_FLOAT64_ID);
  EXPECT_EQ(SIDRE_CHAR8_STR_ID, CONDUIT_CHAR8_STR_ID);

  EXPECT_EQ(SIDRE_INT_ID,    CONDUIT_NATIVE_INT_ID);
  EXPECT_EQ(SIDRE_UINT_ID,   CONDUIT_NATIVE_UNSIGNED_INT_ID);
  EXPECT_EQ(SIDRE_LONG_ID,   CONDUIT_NATIVE_LONG_ID);
  EXPECT_EQ(SIDRE_ULONG_ID,  CONDUIT_NATIVE_UNSIGNED_LONG_ID);
  EXPECT_EQ(SIDRE_FLOAT_ID,  CONDUIT_NATIVE_FLOAT_ID);
  EXPECT_EQ(SIDRE_DOUBLE_ID, CONDUIT_NATIVE_DOUBLE_ID);

  EXPECT_EQ(axom::sidre::INT8_ID,   getTypeID(SIDRE_INT8_ID));
  EXPECT_EQ(axom::sidre::INT16_ID,  getTypeID(SIDRE_INT16_ID));
  EXPECT_EQ(axom::sidre::INT32_ID,  getTypeID(SIDRE_INT32_ID));
  EXPECT_EQ(axom::sidre::INT64_ID,  getTypeID(SIDRE_INT64_ID));

  EXPECT_EQ(axom::sidre::UINT8_ID,   getTypeID(SIDRE_UINT8_ID));
  EXPECT_EQ(axom::sidre::UINT16_ID,  getTypeID(SIDRE_UINT16_ID));
  EXPECT_EQ(axom::sidre::UINT32_ID,  getTypeID(SIDRE_UINT32_ID));
  EXPECT_EQ(axom::sidre::UINT64_ID,  getTypeID(SIDRE_UINT64_ID));

  EXPECT_EQ(axom::sidre::FLOAT32_ID,    getTypeID(SIDRE_FLOAT32_ID));
  EXPECT_EQ(axom::sidre::FLOAT64_ID,    getTypeID(SIDRE_FLOAT64_ID));
  EXPECT_EQ(axom::sidre::CHAR8_STR_ID,  getTypeID(SIDRE_CHAR8_STR_ID));

  EXPECT_EQ(axom::sidre::INT_ID,  getTypeID(SIDRE_INT_ID));
  EXPECT_EQ(axom::sidre::UINT_ID,  getTypeID(SIDRE_UINT_ID));
  EXPECT_EQ(axom::sidre::LONG_ID,  getTypeID(SIDRE_LONG_ID));
  EXPECT_EQ(axom::sidre::ULONG_ID,  getTypeID(SIDRE_ULONG_ID));
  EXPECT_EQ(axom::sidre::FLOAT_ID,  getTypeID(SIDRE_FLOAT_ID));
  EXPECT_EQ(axom::sidre::DOUBLE_ID,  getTypeID(SIDRE_DOUBLE_ID));

}
