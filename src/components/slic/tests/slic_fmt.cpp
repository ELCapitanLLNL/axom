/*
 * Copyright (c) 2015, Lawrence Livermore National Security, LLC.
 * Produced at the Lawrence Livermore National Laboratory.
 *
 * All rights reserved.
 *
 * This source code cannot be distributed without permission and
 * further review from Lawrence Livermore National Laboratory.
 */

/*!
 *******************************************************************************
 * \file
 *
 * \brief A simple test to see if we can use the fmt library within slic macros.
 *******************************************************************************
 */

#include "fmt/fmt.hpp"

#include "slic/slic.hpp"
#include "slic/UnitTestLogger.hpp"

using axom::slic::UnitTestLogger;

#include "gtest/gtest.h"

//-----------------------------------------------------------------------------
TEST(slic_fmt, basic_use)
{
    SLIC_INFO( "Formatting with C++ streams and with "
            << fmt::format("the '{}' string formatting library", "fmt")
            << " can be used together in slic macros." );

    // Style similar to Python format
    SLIC_INFO( fmt::format("{1} {0}", "world", "Hello") );
    EXPECT_EQ("Hello world", fmt::format("{1} {0}", "world", "Hello") );

    // Python style w/ 'dictionary'
    SLIC_INFO( fmt::format("Hello, {name}! Goodbye, {name}.",
               fmt::arg("name", "ASC Toolkit") ));
    EXPECT_EQ( "Hello, ASC Toolkit! Goodbye, ASC Toolkit.",
                fmt::format("Hello, {name}! Goodbye, {name}.",
                fmt::arg("name", "ASC Toolkit") ) );

    // Python style with additional formatting
    SLIC_INFO(fmt::format("int:{0:d};  hex:{0:x};  oct:{0:o}; bin:{0:b}", 42));
    EXPECT_EQ( "int:42;  hex:2a;  oct:52; bin:101010",
               fmt::format("int:{0:d};  hex:{0:x};  oct:{0:o}; bin:{0:b}", 42));

    // sprintf style
    SLIC_INFO( fmt::sprintf("Two significant digits: %.2f", 1.234567) );
    EXPECT_EQ( "1.23", fmt::sprintf("%.2f", 1.234567));

}


int main(int argc, char * argv[])
{
  int result = 0;

  ::testing::InitGoogleTest(&argc, argv);

  UnitTestLogger logger;  // create & initialize test logger,
                          // finalized when exiting main scope

  result = RUN_ALL_TESTS();

  return result;
}
