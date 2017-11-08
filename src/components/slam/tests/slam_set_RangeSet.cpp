/*
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Copyright (c) 2017, Lawrence Livermore National Security, LLC.
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

/*
 * \file slam_set_rangeset.cpp
 *
 * \brief Unit tests for Slam's RangeSet
 *
 * A RangeSet is a contiguous range of integers from low to high.
 * It has a configurable offset and size but no indirection and a stride of +1
 *
 * Also tests GenericRangeSets, which provide a runtime size and offset,
 * and the user supplies templates for strides, indirection and subsetting.
 */

#include <iterator>
#include "gtest/gtest.h"

#include "axom/config.hpp"  // for AXOM_USE_BOOST

#include "slic/slic.hpp"

#include "slam/Set.hpp"
#include "slam/RangeSet.hpp"

namespace
{
typedef axom::slam::RangeSet SetType;
typedef SetType::PositionType SetPosition;
typedef SetType::ElementType SetElement;

static const SetPosition MAX_SET_SIZE = 20;
static const SetElement lowerIndex =
  static_cast<SetElement>( .3 * MAX_SET_SIZE);
static const SetElement upperIndex =
  static_cast<SetElement>( .7 * MAX_SET_SIZE);
static const SetElement range = upperIndex - lowerIndex;
}


TEST(slam_range_set,construct)
{
  SLIC_INFO("Testing construction RangeSets using constructors");

  // Empty set
  SetType s0;
  EXPECT_TRUE(s0.isValid() );
  EXPECT_TRUE(s0.empty() );
  EXPECT_EQ(  0,  s0.size() );
  EXPECT_EQ(  0,  s0.offset() );

  // Construct using only a size
  SetType s1(range);
  EXPECT_TRUE(s1.isValid() );
  EXPECT_EQ(  range,  s1.size() );
  EXPECT_EQ(  0,      s1.offset() );


  // Construct a sized range starting at 0
  SetType s1_b(0, range);
  EXPECT_TRUE(s1_b.isValid() );
  EXPECT_EQ(  range,  s1_b.size() );
  EXPECT_EQ(  0,      s1_b.offset() );

  EXPECT_EQ(  s1,     s1_b);


  // Construct using a lower and upper bound
  SetType s2(lowerIndex, upperIndex);
  EXPECT_TRUE(s2.isValid(true));
  EXPECT_EQ(  range,      s2.size() );
  EXPECT_EQ(  lowerIndex, s2.offset() );

  EXPECT_EQ(  s1.size(),  s2.size());


  // Check that negative ranges are fine
  SetType s3(-upperIndex, -lowerIndex);
  EXPECT_TRUE(s3.isValid(true));
  EXPECT_EQ(  range,        s3.size() );
  EXPECT_EQ(  -upperIndex,  s3.offset() );

}

TEST(slam_range_set,set_builder)
{
  SLIC_INFO("Testing construction RangeSets using SetBuilders");

  typedef SetType::SetBuilder SetBuilder;


  // Empty set, with and without offset
  SetType s0( SetBuilder()
              .size(0) );
  EXPECT_TRUE(s0.isValid() );
  EXPECT_TRUE(s0.empty() );
  EXPECT_EQ(  0,  s0.size() );
  EXPECT_EQ(  0,  s0.offset() );

  SetType s0_b( SetBuilder()
                .offset(10) );
  EXPECT_TRUE(s0_b.isValid() );
  EXPECT_TRUE(s0_b.empty() );
  EXPECT_EQ(  0,  s0_b.size() );
  EXPECT_EQ(  10, s0_b.offset() );


  // Construct using only a size
  SetType s1( SetBuilder()
              .size(range));
  EXPECT_TRUE(s1.isValid() );
  EXPECT_EQ(  range,  s1.size() );
  EXPECT_EQ(  0,      s1.offset() );


  // Construct a sized range starting at 0
  SetType s1_b(SetBuilder()
               .range(0, range) );
  EXPECT_TRUE(s1_b.isValid() );
  EXPECT_EQ(  range,  s1_b.size() );
  EXPECT_EQ(  0,      s1_b.offset() );

  EXPECT_EQ(  s1,     s1_b);


  // Construct using a lower and upper bound
  SetType s2(SetBuilder()
             .range(lowerIndex, upperIndex));
  EXPECT_TRUE(s2.isValid(true));
  EXPECT_EQ(  range,      s2.size() );
  EXPECT_EQ(  lowerIndex, s2.offset() );

  SetType s2_b(SetBuilder()
               .size(range)
               .offset(lowerIndex));
  EXPECT_TRUE(s2_b.isValid(true));
  EXPECT_EQ(  range,      s2_b.size() );
  EXPECT_EQ(  lowerIndex, s2_b.offset() );

  EXPECT_EQ(  s2,         s2_b);


  // Check that negative ranges are fine
  SetType s3(SetBuilder().range(-upperIndex, -lowerIndex));
  EXPECT_TRUE(s3.isValid(true));
  EXPECT_EQ(  range,        s3.size() );
  EXPECT_EQ(  -upperIndex,  s3.offset() );

  SetType s3_b(SetBuilder()
               .size(range)
               .offset(-upperIndex));
  EXPECT_TRUE(s3_b.isValid(true));
  EXPECT_EQ(  range,        s3_b.size() );
  EXPECT_EQ(  -upperIndex,  s3_b.offset() );

  EXPECT_EQ(  s3,           s3_b);

}

TEST(slam_range_set,iterate)
{
  SetType s(lowerIndex, upperIndex);

  SLIC_INFO( "Iterating through range set of size "
             << s.size()
             << "\n\twith lower element " << lowerIndex << " (included in set)"
             << "\n\twith upper element " << upperIndex
             << " (not included in set)");
  const SetPosition expectedSize = upperIndex - lowerIndex;
  EXPECT_EQ(expectedSize, s.size() );

  SLIC_INFO("Testing random access -- operator[] and at() function");
  {
    std::stringstream sstr;
    for(SetPosition pos = SetPosition() ; pos < s.size() ; ++pos)
    {
      SetElement expected = pos + lowerIndex;
      EXPECT_EQ(  expected, s[pos] );
      EXPECT_EQ(  expected, s.at(pos) );
      EXPECT_EQ(  s[pos],   s.at(pos) );

      sstr << s[pos] << "\t";
    }
    SLIC_INFO(sstr.str());
  }

#ifdef AXOM_USE_BOOST
  SLIC_INFO("Testing iterator access");
  {
    std::stringstream sstr;

    typedef SetType::iterator SetIterator;
    for(SetIterator it = s.begin(), itEnd = s.end() ; it != itEnd ; ++it)
    {
      SetPosition position = std::distance(s.begin(), it);
      SetElement expected = position + lowerIndex;
      EXPECT_EQ( expected, *it )
        << "Iterator dereference should be equal "
        << "to its translated position in the windowed range set";
      sstr << *it << "\t";
    }
    SLIC_INFO(sstr.str());
  }
#endif
}

TEST(slam_range_set,out_of_range)
{
  SetType s(lowerIndex, upperIndex);

  SLIC_INFO("Using random access on invalid address "
            << "-- Note: We are testing for the expected failures.");
#ifdef AXOM_DEBUG
  // NOTE: AXOM_DEBUG is disabled in release mode,
  // so this test will only fail in debug mode

  // add this line to avoid a warning in the output about thread safety
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  EXPECT_DEATH_IF_SUPPORTED(  s.at(upperIndex),   "");
  EXPECT_DEATH_IF_SUPPORTED(  s.at(MAX_SET_SIZE), "");
#else
  SLIC_INFO("Skipped assertion failure check in release mode.");
#endif

  SLIC_INFO("done.");
}

TEST(slam_generic_range_set,virtual_parent_set)
{
  namespace policies = axom::slam::policies;

  typedef axom::slam::GenericRangeSet<
      policies::StrideOne<SetPosition>,
      policies::NoIndirection<SetPosition,SetElement>,
      policies::VirtualParentSubset>        GenericRangeSet;

  typedef GenericRangeSet::SetBuilder SetBuilder;

  SLIC_INFO("Generating a parent set, and a subset and checking validity");
  GenericRangeSet parentSet(SetBuilder().size(MAX_SET_SIZE));
  GenericRangeSet childSet( SetBuilder()
                            .range(lowerIndex, upperIndex)
                            .parent(&parentSet));

  SetType nonChildSet( SetType::SetBuilder()
                       .range(lowerIndex, upperIndex));

  EXPECT_TRUE(parentSet.isValid(true));
  EXPECT_TRUE(childSet.isValid(true));
  EXPECT_TRUE(nonChildSet.isValid(true));


  SLIC_INFO("Checking that the child is a subset, "
            << " but not the parent or the non-child windowed set.");
  EXPECT_FALSE(parentSet.isSubset());
  EXPECT_TRUE(childSet.isSubset());
  EXPECT_FALSE(nonChildSet.isSubset());

  SLIC_INFO("Checking that the child set's parent is equal to "
            << " the parent set (according to the equality operator==).");
  EXPECT_EQ(parentSet, *childSet.parentSet());

  // Since the parent is using the virtual subsetting policy,
  // it does not have an operator[]
  for(SetPosition pos = 0 ; pos > childSet.parentSet()->size() ; ++pos)
  {
    EXPECT_EQ(parentSet[pos], childSet.parentSet()->at(pos));
  }


  // Note: Equality is based on Base class Set
  //-- it does not differentiate based on whether a set is a subset of another set
  EXPECT_EQ(childSet, nonChildSet);
}


TEST(slam_generic_range_set,concrete_parent_set)
{
  namespace policies = axom::slam::policies;

  typedef SetType ParentType;

  typedef axom::slam::GenericRangeSet<
      policies::StrideOne<SetPosition>,
      policies::NoIndirection<SetPosition,SetElement>,
      policies::ConcreteParentSubset<ParentType> >        GenericRangeSet;

  typedef GenericRangeSet::SetBuilder SetBuilder;

  SLIC_INFO("Generating a parent set, and a subset and checking validity");
  ParentType parentSet(ParentType::SetBuilder().size(MAX_SET_SIZE));
  GenericRangeSet childSet( SetBuilder()
                            .range(lowerIndex, upperIndex)
                            .parent(&parentSet));

  SetType nonChildSet( SetType::SetBuilder()
                       .range(lowerIndex, upperIndex));

  EXPECT_TRUE(parentSet.isValid(true));
  EXPECT_TRUE(childSet.isValid(true));
  EXPECT_TRUE(nonChildSet.isValid(true));


  SLIC_INFO("Checking that the child is a subset, "
            << "but not the parent or the non-child windowed set.");
  EXPECT_FALSE(parentSet.isSubset());
  EXPECT_TRUE(childSet.isSubset());
  EXPECT_FALSE(nonChildSet.isSubset());

  SLIC_INFO("Checking that the child set's parent is equal to "
            << "the parent set (according to the equality operator==).");
  EXPECT_EQ(parentSet, *childSet.parentSet());


  // Since the parent is a concrete OrderedSet, it should have an operator[]
  const ParentType& childParSet = *(childSet.parentSet());
  for(SetPosition pos = 0 ; pos > childParSet.size() ; ++pos)
  {
    EXPECT_EQ(childParSet.at(pos),  childParSet[pos]);
    EXPECT_EQ(parentSet[pos],       childParSet[pos]);
  }


  // Note: Equality is based on Base class Set
  //-- it does not differentiate based on whether a set is a subset of another set
  EXPECT_EQ(childSet, nonChildSet);

}

//----------------------------------------------------------------------
//----------------------------------------------------------------------
#include "slic/UnitTestLogger.hpp"
using axom::slic::UnitTestLogger;

int main(int argc, char * argv[])
{
  int result = 0;

  ::testing::InitGoogleTest(&argc, argv);

  UnitTestLogger logger;  // create & initialize test logger,

  // finalized when exiting main scope

  //axom::slic::debug::checksAreErrors = true;

  result = RUN_ALL_TESTS();

  return result;
}
