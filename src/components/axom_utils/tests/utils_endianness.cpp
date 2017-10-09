/*
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Copyright (c) 2017, Lawrence Livermore National Security, LLC.
 *
 * Produced at the Lawrence Livermore National Laboratory
 *
 * LLNL-CODE-xxxxxxx
 *
 * All rights reserved.
 *
 * This file is part of Axom.
 *
 * For details about use and distribution, please read axom/LICENSE.
 *
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

#include "gtest/gtest.h"

#include "axom/config.hpp"
#include "axom_utils/Utilities.hpp"

#include <limits>
#include <iostream>

TEST(axom_utils_endianness,print_endianness)
{
  std::cout << "Checking system endianness..." << std::endl;

  const bool isLittleEndian = axom::utilities::isLittleEndian();

  std::cout << "System is "
      << (isLittleEndian ? "little" : "big")
      << " endian." << std::endl;
}

TEST(axom_utils_endianness,endianness_16)
{
    union SixteenBit
    {
      axom::common::uint8 raw[ 2 ];
      axom::common::int16 i_val;
      axom::common::uint16 ui_val;
      short short_val;
      unsigned short ushort_val;
    };

    SixteenBit valOrig = {{ 0, 1}};
    SixteenBit valSwap = {{ 1, 0}};

    // Test short
    {
      short v1 = valOrig.short_val;
      short v2 = axom::utilities::swapEndian( v1 );
      short v3 = axom::utilities::swapEndian( v2 );

      EXPECT_EQ( valOrig.short_val, v1);
      EXPECT_EQ( valSwap.short_val, v2);
      EXPECT_EQ( valOrig.short_val, v3);
    }

    // Test unsigned short
    {
      unsigned short v1 = valOrig.ushort_val;
      unsigned short v2 = axom::utilities::swapEndian( v1 );
      unsigned short v3 = axom::utilities::swapEndian( v2 );

      EXPECT_EQ( valOrig.ushort_val, v1);
      EXPECT_EQ( valSwap.ushort_val, v2);
      EXPECT_EQ( valOrig.ushort_val, v3);
    }

    // Test int16
    {
      axom::common::int16 v1 = valOrig.i_val;
      axom::common::int16 v2 = axom::utilities::swapEndian( v1 );
      axom::common::int16 v3 = axom::utilities::swapEndian( v2 );

      EXPECT_EQ( valOrig.i_val, v1);
      EXPECT_EQ( valSwap.i_val, v2);
      EXPECT_EQ( valOrig.i_val, v3);
    }

    // Test uint16
    {
      axom::common::uint16 v1 = valOrig.i_val;
      axom::common::uint16 v2 = axom::utilities::swapEndian( v1 );
      axom::common::uint16 v3 = axom::utilities::swapEndian( v2 );

      EXPECT_EQ( valOrig.ui_val, v1);
      EXPECT_EQ( valSwap.ui_val, v2);
      EXPECT_EQ( valOrig.ui_val, v3);
    }
}



TEST(axom_utils_endianness,endianness_32)
{
    union ThirtyTwoBit
    {
      axom::common::uint8 raw[ 4 ];
      axom::common::int32 i_val;
      axom::common::uint32 ui_val;
      int int_val;
      unsigned int uint_val;
      float f_val;
    };

    ThirtyTwoBit valOrig = {{ 0, 1, 2, 3}};
    ThirtyTwoBit valSwap = {{ 3, 2, 1, 0}};

    // Test int
    {
      int v1 = valOrig.int_val;
      int v2 = axom::utilities::swapEndian( v1 );
      int v3 = axom::utilities::swapEndian( v2 );

      EXPECT_EQ( valOrig.int_val, v1);
      EXPECT_EQ( valSwap.int_val, v2);
      EXPECT_EQ( valOrig.int_val, v3);
    }

    // Test unsigned int
    {
      unsigned int v1 = valOrig.uint_val;
      unsigned int v2 = axom::utilities::swapEndian( v1 );
      unsigned int v3 = axom::utilities::swapEndian( v2 );

      EXPECT_EQ( valOrig.uint_val, v1);
      EXPECT_EQ( valSwap.uint_val, v2);
      EXPECT_EQ( valOrig.uint_val, v3);
    }

    // Test int32
    {
      axom::common::int32 v1 = valOrig.i_val;
      axom::common::int32 v2 = axom::utilities::swapEndian( v1 );
      axom::common::int32 v3 = axom::utilities::swapEndian( v2 );

      EXPECT_EQ( valOrig.i_val, v1);
      EXPECT_EQ( valSwap.i_val, v2);
      EXPECT_EQ( valOrig.i_val, v3);
    }

    // Test uint32
    {
      axom::common::uint32 v1 = valOrig.i_val;
      axom::common::uint32 v2 = axom::utilities::swapEndian( v1 );
      axom::common::uint32 v3 = axom::utilities::swapEndian( v2 );

      EXPECT_EQ( valOrig.ui_val, v1);
      EXPECT_EQ( valSwap.ui_val, v2);
      EXPECT_EQ( valOrig.ui_val, v3);
    }

    // Test float
    {
      float v1 = valOrig.f_val;
      float v2 = axom::utilities::swapEndian( v1 );
      float v3 = axom::utilities::swapEndian( v2 );

      EXPECT_EQ( valOrig.f_val, v1);
      EXPECT_EQ( valSwap.f_val, v2);
      EXPECT_EQ( valOrig.f_val, v3);
    }
}



TEST(axom_utils_endianness,endianness_64)
{
    union SixtyFourBit
    {
      axom::common::uint8 raw[ 8 ];
#ifndef AXOM_NO_INT64_T
      axom::common::int64 i_val;
      axom::common::uint64 ui_val;
#endif
      double d_val;
    };

    SixtyFourBit valOrig = {{ 0, 1, 2, 3, 4, 5, 6, 7}};
    SixtyFourBit valSwap = {{ 7, 6, 5, 4, 3, 2, 1, 0}};

#ifndef AXOM_NO_INT64_T
    // Test int64
    {
      axom::common::int64 v1 = valOrig.i_val;
      axom::common::int64 v2 = axom::utilities::swapEndian( v1 );
      axom::common::int64 v3 = axom::utilities::swapEndian( v2 );

      EXPECT_EQ( valOrig.i_val, v1);
      EXPECT_EQ( valSwap.i_val, v2);
      EXPECT_EQ( valOrig.i_val, v3);
    }

    // Test uint64
    {
      axom::common::uint64 v1 = valOrig.i_val;
      axom::common::uint64 v2 = axom::utilities::swapEndian( v1 );
      axom::common::uint64 v3 = axom::utilities::swapEndian( v2 );

      EXPECT_EQ( valOrig.ui_val, v1);
      EXPECT_EQ( valSwap.ui_val, v2);
      EXPECT_EQ( valOrig.ui_val, v3);
    }
#endif

    // Test double
    {
      double v1 = valOrig.d_val;
      double v2 = axom::utilities::swapEndian( v1 );
      double v3 = axom::utilities::swapEndian( v2 );

      EXPECT_EQ( valOrig.d_val, v1);
      EXPECT_EQ( valSwap.d_val, v2);
      EXPECT_EQ( valOrig.d_val, v3);
    }
}

#if 0
// Note: Checks that swapEndian static_asserts for types within invalid sizes.
//       Commented out since it should lead to a compile error
TEST(axom_utils_endianness,invalid_byte_width)
{
  {
    axom::common::int8 v1 = 5;
    axom::common::int8 v2 = axom::utilities::swapEndian(v1);

    EXPECT_EQ(v1, v2);
  }

  {
    axom::common::uint8 v1 = 5;
    axom::common::uint8 v2 = axom::utilities::swapEndian(v1);

    EXPECT_EQ(v1, v2);
  }
}
#endif


#if 0
// Note: Checks that swapEndian static_asserts for non-native types.
//       Commented out since it should lead to a compile error.
TEST(axom_utils_endianness,invalid_non_native_types)
{
  struct AxomUtilsTestsNonNative {
    axom::common::uint16 a;
    axom::common::uint16 b;
  };

  AxomUtilsTestsNonNative v1;
  v1.a = 5;
  v1.b = 12;

  AxomUtilsTestsNonNative v2 = axom::utilities::swapEndian(v1);

  EXPECT_NE(v1.a, v2.a);
  EXPECT_NE(v1.b, v2.b);
}
#endif
