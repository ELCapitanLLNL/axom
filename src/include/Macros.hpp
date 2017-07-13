/*
 * Copyright (c) 2015, Lawrence Livermore National Security, LLC.
 * Produced at the Lawrence Livermore National Laboratory.
 *
 * All rights reserved.
 *
 * This source code cannot be distributed without permission and
 * further review from Lawrence Livermore National Laboratory.
 */

#include "axom/config.hpp"           // defines AXOM_USE_CXX11

/*!
 *******************************************************************************
 * \file AxomMacros.hpp
 *
 * \brief Contains several useful macros for the axom project
 *******************************************************************************
 */

#ifndef AXOM_MACROS_HPP_
#define AXOM_MACROS_HPP_

/*!
 *******************************************************************************
 * \def AXOM_NOT_USED(x)
 * \brief Macro used to silence compiler warnings in methods with unused
 *  arguments.
 * \note The intent is to use this macro in the function signature. For example:
 * \code
 *
 *  void my_function(int x, int AXOM_NOT_USED(y))
 *  {
 *    // my implementation
 *  }
 *
 * \endcode
 *******************************************************************************
 */
#define AXOM_NOT_USED(x)

/*!
 *******************************************************************************
 * \def AXOM_STATIC_ASSERT(cond)
 * \def AXOM_STATIC_ASSERT_MSG(cond, MSG)
 *
 * \brief This macro wraps C++11 compile time static_assert functionality. Used
 *  for backwards compatibility with non C++11 compilers.
 *******************************************************************************
 */
#ifdef AXOM_USE_CXX11
#define AXOM_STATIC_ASSERT( cond ) static_assert( cond, #cond )
#define AXOM_STATIC_ASSERT_MSG( cond, MSG ) static_assert( cond, MSG )
#else
#define AXOM_STATIC_ASSERT( cond )
#define AXOM_STATIC_ASSERT_MSG( cond, MSG )
#endif

/*!
 *******************************************************************************
 * \def AXOM_DEBUG_VAR(x)
 * \brief Macro used to silence compiler warnings about variables
 *        that are defined but not used.
 * \note The intent is to use this macro for variables that are only used
 *       for debugging purposes (e.g. in debug assertions). For example:
 * \code
 *
 *  double myVar = ...
 *  AXOM_DEBUG_VAR(myVar);       // code will emit the following warning in release builds
 *                              // if extra warnings are enabled and macro is not called
 *                              // warning: unused variable 'myVar' [-Wunused-variable]
 *  SLIC_ASSERT(myVar > 0)
 *
 * \endcode
 *******************************************************************************
 */
#define AXOM_DEBUG_VAR(_x)   static_cast<void>(_x)

/*!
 *******************************************************************************
 * \def AXOM_DEBUG_PARAM(x)
 * \brief Macro used to silence compiler warnings about parameters
 *        that are used in debug code but not in release code.
 * \note Default values are ok
 * \code
 *
 *  void my_function(int x, int AXOM_DEBUG_PARAM(y))
 *  {
 *    // my implementation
 *    SLIC_ASSERT(y > 0)
 *  }
 *
 * \endcode
 *******************************************************************************
 */
#ifdef AXOM_DEBUG
 #define AXOM_DEBUG_PARAM(_x)  _x
#else
 #define AXOM_DEBUG_PARAM(_x)
#endif

/*!
 *******************************************************************************
 * \def DISABLE_DEFAULT_CTOR(className)
 * \brief Macro to disable default constructor for the given class.
 * \note This macro should only be used within the private section of a class,
 *  as indicated in the example below.
 *
 * \code
 *
 *   class Foo
 *   {
 *   public:
 *
 *       // Public methods here
 *
 *   private:
 *      DISABLE_DEFAULT_CTOR(Foo);
 *   };
 *
 * \endcode
 *******************************************************************************
 */
#ifdef AXOM_USE_CXX11
#define DISABLE_DEFAULT_CTOR(className)                      \
  className( ) = delete;
#else
#define DISABLE_DEFAULT_CTOR(className)                      \
  className( );
#endif

/*!
 *******************************************************************************
 * \def DISABLE_COPY_AND_ASSIGNMENT(className)
 * \brief Macro to disable copy and assignment operations for the given class.
 * \note This macro should only be used within the private section of a class,
 *  as indicated in the example below.
 *
 * \code
 *
 *   class Foo
 *   {
 *   public:
 *      Foo();
 *      ~Foo();
 *
 *       // Other methods here
 *
 *   private:
 *      DISABLE_COPY_AND_ASSIGNMENT(Foo);
 *   };
 *
 * \endcode
 *******************************************************************************
 */
#ifdef AXOM_USE_CXX11
#define DISABLE_COPY_AND_ASSIGNMENT(className)                                \
  className( const className& ) = delete;                                     \
  className& operator=(const className&) = delete
#else
#define DISABLE_COPY_AND_ASSIGNMENT(className)                                \
  className( const className& );                                              \
  className& operator=( const className& )
#endif

/*!
 *******************************************************************************
 * \def DISABLE_MOVE_AND_ASSIGNMENT(className)
 * \brief Macro to disable move constructor and move assignment operations for
 * the given class.
 * \note This macro should only be used within the private section of a class,
 *  as indicated in the example below.
 *
 * \code
 *
 *   class Foo
 *   {
 *   public:
 *      Foo();
 *      ~Foo();
 *
 *       // Other methods here
 *
 *   private:
 *      DISABLE_MOVE_AND_ASSIGNMENT(Foo);
 *   };
 *
 * \endcode
 *******************************************************************************
 */
#ifdef AXOM_USE_CXX11
#define DISABLE_MOVE_AND_ASSIGNMENT(className)                                \
  className( const className&& ) = delete;                                    \
  className& operator=(const className&&) = delete
#else
#define DISABLE_MOVE_AND_ASSIGNMENT(className)
#endif


#endif /* AXOM_MACROS_HPP_ */
