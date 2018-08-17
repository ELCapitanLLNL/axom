/*
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Copyright (c) 2017-2018, Lawrence Livermore National Security, LLC.
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

/*!
 *
 * \file Utilities.hpp
 *
 * \brief Header file containing utility functions.
 *
 */

#ifndef AXOM_UTILITIES_HPP_
#define AXOM_UTILITIES_HPP_

#include "axom/core/Types.hpp"
#include "axom/core/Macros.hpp" // for AXOM_STATIC_ASSERT

#include <cassert>         // for assert()
#include <cmath>           // for log2()
#include <cstdlib>         // for std::malloc, std::realloc, std::free

#include <random>        // for random  number generator
#include <type_traits>   // for std::is_floating_point()


namespace axom
{
namespace utilities
{

/*!
 * \brief Allocates a chunk of memory of type T.
 * \param [in] n the number of elements to allocate.
 * \tparam T the type of pointer returned.
 * \return A pointer to the new allocation or a null pointer if allocation
 *  failed.
 */
template < typename T >
inline T* alloc( std::size_t n )
{
  return static_cast< T* >( std::malloc( n * sizeof( T ) ) );
}

/*!
 * \brief Reallocates the chunk of memory pointed to by pointer.
 * \param [in] pointer pointer to memory previously allocated with
 *  alloc or realloc, or a null pointer.
 * \param [in] n the number of elements to allocate.
 * \tparam T the type pointer points to.
 * \return A pointer to the new allocation or a null pointer if allocation
 *  failed.
 */
template < typename T >
inline T* realloc( T* pointer, std::size_t n )
{
  if ( n == 0 )
  {
    std::free( pointer );
    return nullptr;
  }

  return static_cast< T* >( std::realloc( pointer,  n * sizeof( T ) ) );
}

/*!
 * \brief Frees the chunk of memory pointed to by pointer.
 *
 * \param [in] pointer pointer to memory previously allocated with
 *  alloc or realloc or a null pointer.
 *
 *  \post pointer == nullptr
 */
inline void free( void* pointer )
{
  std::free( pointer );
  pointer = nullptr;
}

/*!
 * \brief Gracefully aborts the application
 */
void processAbort();

/*!
 * \brief Returns the absolute value of x.
 * \param [in] x value whose absolute value is computed.
 * \return abs(x) the absolute value of x.
 */
template < typename T >
inline T abs( const T& x )
{
  return ( x < T(0) ) ? -x : x;
}

/*!
 * \brief Returns the max value of x and y.
 * \param [in] x the first value to check.
 * \param [in] y the second value to check.
 * \return max(x, y) the max value of x and y.
 */
template < typename T >
inline const T& max( const T& x, const T& y )
{
  return ( y < x ) ? x : y;
}

/*!
 * \brief Returns the min value of x and y.
 * \param [in] x the first value to check.
 * \param [in] y the second value to check.
 * \return min(x, y) the min value of x and y.
 */
template < typename T >
inline const T& min( const T& x, const T& y )
{
  return ( y < x ) ? y : x;
}

/*!
 * \brief Swaps the values of a, b.
 * \param [in,out] a 1st object to swap.
 * \param [in,out] b 2nd object to swap.
 */
template < typename T >
inline void swap( T& a, T& b )
{
  T tmp = a;
  a = b; b = tmp;
}

/*!
 * \brief Returns the base 2 logarithm of the input.
 * \param [in] val The input value
 */
template < typename T >
inline T log2( T& val)
{
  return std::log2(val);
}

/*!
 * \brief Clamps an input value to a given range.
 * \param [in] val  The value to clamp.
 * \param [in] lower The lower range.
 * \param [in] upper The upper range.
 * \return The clamped value.
 * \pre lower <= upper
 * \post lower <= returned value <= upper.
 */
template < typename T >
inline T clampVal( T val, T lower, T upper )
{
  return (val < lower) ? lower
         : (val > upper) ? upper : val;
}


/*!
 * \brief Clamps the upper range on an input value
 *
 * \param [in] val The value to clamp
 * \param [in] upper The upper range
 * \return upper if val > upper, else val
 * \post returned value is less than or equal to upper
 */
template < typename T >
inline T clampUpper(T val, T upper)
{
  return val > upper ? upper : val;
}

/*!
 * \brief Clamps the lower range on an input value
 *
 * \param [in] val The value to clamp
 * \param [in] lower The lower range
 * \return lower if val < lower, else val
 * \post returned value is greater than or equal to lower
 */
template < typename T >
inline T clampLower(T val, T lower)
{
  return val < lower ? lower : val;
}

/*!
 * \brief Returns a random real number within the specified interval
 *
 * \param [in] a the interval's lower bound
 * \param [in] b the interval's upper bound
 * \return r the random real number within the specified interval.
 *
 * \tparam T a built-in floating point type, e.g., double, float, long double.
 *
 * \note Consecutive calls to this method will generate a non-deterministic
 *  sequence of numbers.
 *
 * \pre a < b
 * \post a <= r < b
 */
template < typename T >
inline T random_real( const T& a, const T& b )
{
  AXOM_STATIC_ASSERT( std::is_floating_point< T >::value );
  assert( (a < b) && "invalid bounds, a < b" );

  static std::random_device rd;
  static std::mt19937_64 mt( rd() );
  static std::uniform_real_distribution< T > dist(0.0, 1.0);

  double temp = dist(mt);
  return temp * ( b - a ) + a;
}

/*!
 * \brief Returns a random real number within the specified interval given
 *  the bounds of the interval and a seed value for the underlying random
 *  number generator.
 *
 * \param [in] a the interval's lower bound
 * \param [in] b the interval's upper bound
 * \param [in] seed user-supplied seed for the random number generator
 * \return r the random real number within the specified interval.
 *
 * \tparam T a built-in floating type, e.g., double, float, long double.
 *
 * \note Consecutive calls to this method will generate a deterministic
 *  sequence of numbers.
 *
 * \pre a < b
 * \post a <= r < b
 */
template < typename T >
inline T random_real( const T& a, const T& b, unsigned int seed )
{
  AXOM_STATIC_ASSERT( std::is_floating_point< T >::value );
  assert( (a < b) && "invalid bounds, a < b" );

  static std::mt19937_64 mt( seed );
  static std::uniform_real_distribution< T > dist(0.0, 1.0);

  double temp = dist(mt);
  return temp * ( b - a ) + a;
}

/*!
 * \brief Tests the endianness of the system.
 * \return True, if the system is little endian, false otherwise.
 */
inline bool isLittleEndian()
{
  // Note: Endianness test adapted from: https://stackoverflow.com/a/2103095

  enum
  {
    O32_LITTLE_ENDIAN = 0x03020100ul,
    O32_BIG_ENDIAN    = 0x00010203ul,
    O32_PDP_ENDIAN    = 0x01000302ul
  };

  const union
  {
    axom::common::uint8 raw[4];
    axom::common::uint32 value;
  }  host_order = {
    { 0, 1, 2, 3 }
  };

  return host_order.value == O32_LITTLE_ENDIAN;
}

/*!
 * \brief Swaps the endianness of the input value.
 * \param [in] val The input value.
 * \return The value with endianness swapped.
 * \note Assumes endianness is either little or big (not PDP).
 * \pre T is a native arithmetic type (i.e. integral or floating point).
 * \pre sizeof(T) must be 2, 4, or 8 bytes.
 */
template < typename T >
T swapEndian(T val)
{
  const int NBYTES = sizeof(T);

  AXOM_STATIC_ASSERT_MSG(
    NBYTES == 2 || NBYTES == 4 || NBYTES == 8,
    "swapEndian only valid for types of size 2, 4 or 8 bytes.");

  AXOM_STATIC_ASSERT_MSG( std::is_arithmetic<T>::value,
                          "swapEndian only valid for native arithmetic types");

  union
  {
    axom::common::uint8 raw[ NBYTES ];
    T val;
  } swp;

  axom::common::uint8* src = reinterpret_cast<axom::common::uint8*>(&val);

  // Reverse the bytes
  for(int i=0 ; i < NBYTES ; ++i)
  {
    swp.raw[i] = src[(NBYTES-1)-i];
  }

  return swp.val;
}

/*!
 * \brief Fuzzy comparison of two real valued quantities.
 * \param [in] a The first real valued quantities we are comparing.
 * \param [in] b The second real valued quantities we are comparing.
 * \param [in] thresh The threshold of the fuzzy comparison.  Default is 1.0e-8.
 * \return True if the absolute value of the difference is less than thresh and
 *  false otherwise.
 */
template<typename RealType>
bool isNearlyEqual(RealType a, RealType b, RealType thresh = 1.0e-8)
{
  return abs(a-b) <= thresh;
}

/*!
 * \brief Fuzzy comparison of two real valued quantities.
 * \param [in] a The first real valued quantities we are comparing.
 * \param [in] b The second real valued quantities we are comparing.
 * \param [in] relThresh The relative threshold of the fuzzy comparison.
 *  Default is 1.0e-6.
 * \param [in] absThresh The absolute threshold of the fuzzy comparison.
 *  Default is 1.0e-8.
 * \return True if the absolute value of the difference is less than the sum of
 *  absThresh and the relative difference (relThresh times the absolute max of
 *  a and b).
 */
template<typename RealType>
bool isNearlyEqualRelative(RealType a, RealType b, RealType relThresh = 1.0e-6,
                           RealType absThresh = 1.0e-8)
{
  RealType maxFabs = max(abs(a), abs(b) );
  return abs(a-b) <= ( maxFabs * relThresh + absThresh);

  // Equation from Real-Time Collsion Detection book --
  // http://realtimecollisiondetection.net/pubs/Tolerances/
  // Note: If we use this, we must update the doxygen
  // return abs(a-b) <= max(absThresh, relThresh * maxFabs );
}

}  // namespace utilities
}  // namespace axom

#endif  // AXOM_UTILITIES_HPP_
