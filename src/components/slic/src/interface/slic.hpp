/*
 * Copyright (c) 2015, Lawrence Livermore National Security, LLC.
 * Produced at the Lawrence Livermore National Laboratory.
 *
 * All rights reserved.
 *
 * This source code cannot be distributed without permission and further
 * review from Lawrence Livermore National Laboratory.
 */

/*!
 *******************************************************************************
 * \file slic.hpp
 *
 * \date May 31, 2015
 * \author George Zagaris (zagaris2@llnl.gov)
 *
 *******************************************************************************
 */

#ifndef SLIC_HPP_
#define SLIC_HPP_

#include "slic/Logger.hpp"
#include "slic/LogStream.hpp"
#include "slic/MessageLevel.h"

#include "common/CommonTypes.hpp" // for ATK_NULLPTR

// C/C++ includes
#include <iostream> // for std::endl, std::ends
#include <sstream>  // for std::ostringstream

/// \name ERROR MACROS
/// @{

/*!
 ******************************************************************************
 * \def SLIC_ERROR( msg )
 * \brief Logs an error and aborts the application.
 * \param [in] msg user-supplied message
 * \note The SLIC_ERROR macro is always active.
 * \warning This macro calls processAbort().
 *
 * Usage:
 * \code
 *   SLIC_ERROR( "my_val should always be positive" );
 * \endcode
 *
 ******************************************************************************
 */
#define SLIC_ERROR( msg )                                                     \
  do {                                                                        \
    std::ostringstream oss;                                                   \
    oss << msg;                                                               \
    axom::slic::logErrorMessage( oss.str(),__FILE__, __LINE__);               \
  } while ( 0 )

/*!
 ******************************************************************************
 * \def SLIC_ERROR_IF( EXP, msg )
 * \brief Logs an error iff EXP is true and aborts the application.
 * \param [in] EXP user-supplied boolean expression.
 * \param [in] msg user-supplied message.
 * \note The SLIC_ERROR_IF macro is always active.
 * \warning This macro calls processAbort() if EXP is true.
 *
 * Usage:
 * \code
 *   SLIC_ERROR_IF( (val < 0), "my_val should always be positive" );
 * \endcode
 *
 ******************************************************************************
 */
#define SLIC_ERROR_IF( EXP, msg )                                             \
  do {                                                                        \
    if ( EXP ) {                                                              \
      std::ostringstream oss;                                                 \
      oss << msg;                                                             \
      axom::slic::logErrorMessage(oss.str(),__FILE__,__LINE__);               \
    }                                                                         \
  } while ( 0 )

/// @}

/// \name WARNING MACROS
/// @{

/*!
 ******************************************************************************
 * \def SLIC_WARNING( msg )
 * \brief Logs a warning message.
 * \param [in] msg user-supplied message
 * \note The SLIC_WARNING macro is always active.
 *
 * Usage:
 * \code
 *   SLIC_WARNING( "my_val should always be positive" );
 * \endcode
 *
 ******************************************************************************
 */
#define SLIC_WARNING( msg )                                                   \
  do {                                                                        \
    std::ostringstream oss;                                                   \
    oss << msg;                                                               \
    axom::slic::logWarningMessage(oss.str(),__FILE__, __LINE__ );             \
  } while ( 0 )

/*!
 ******************************************************************************
 * \def SLIC_WARNING_IF( EXP, msg )
 * \brief Logs an error iff EXP is true and aborts the application.
 * \param [in] EXP user-supplied boolean expression.
 * \param [in] msg user-supplied message.
 * \note The SLIC_WARNING_IF macro is always active.
 *
 * Usage:
 * \code
 *   SLIC_WARNING_IF( (val < 0), "my_val should always be positive" );
 * \endcode
 *
 ******************************************************************************
 */
#define SLIC_WARNING_IF( EXP, msg )                                           \
  do {                                                                        \
    if ( EXP ) {                                                              \
      std::ostringstream oss;                                                 \
      oss << msg;                                                             \
      axom::slic::logWarningMessage(oss.str(),__FILE__,__LINE__ );            \
    }                                                                         \
  } while ( 0 )

/// @}

#ifdef ATK_DEBUG

//-----------------------------------------------------------------------------
/// \name ASSERT MACROS
/// @{

/*!
 ******************************************************************************
 * \def SLIC_ASSERT( EXP )
 * \brief Asserts that a given expression is true. If the expression is not true
 *  an error will be logged and the application will be aborted.
 * \param [in] EXP user-supplied boolean expression.
 * \note This macro is only active when debugging is turned on.
 * \warning This macro calls processAbort() if EXP is false.
 *
 * Usage:
 * \code
 *   SLIC_ASSERT( my_val >= 0 );
 * \endcode
 *
 ******************************************************************************
 */
#define SLIC_ASSERT( EXP )                                                    \
  do {                                                                        \
    if ( !(EXP) ) {                                                           \
      std::ostringstream oss;                                                 \
      oss << "Failed Assert: " << # EXP << std::ends;                         \
      axom::slic::logErrorMessage(oss.str(),__FILE__,__LINE__ );              \
    }                                                                         \
  } while ( 0 )

/*!
 ******************************************************************************
 * \def SLIC_ASSERT_MSG( EXP, msg )
 * \brief Same as SLIC_ASSERT, but with a custom error message.
 * \param [in] EXP user-supplied boolean expression.
 * \param [in] msg user-supplied message
 * \note This macro is only active when debugging is turned on.
 * \warning This macro calls processAbort() if EXP is false.
 * \see SLIC_ASSERT( EXP )
 *
 * Usage:
 * \code
 *   SLIC_ASSERT_MSG( my_val >= 0, "my_val must always be positive" );
 * \endcode
 *
 ******************************************************************************
 */
#define SLIC_ASSERT_MSG( EXP, msg )                                           \
  do {                                                                        \
    if ( !(EXP) ) {                                                           \
      std::ostringstream oss;                                                 \
      oss << "Failed Assert: " << # EXP << std::endl << msg << std::ends;     \
      axom::slic::logErrorMessage(oss.str(),__FILE__,__LINE__ );              \
    }                                                                         \
  } while ( 0 )

/// @}

//-----------------------------------------------------------------------------
/// \name DEBUG MACROS
/// @{

/*!
 ******************************************************************************
 * \def SLIC_CHECK( EXP )
 * \brief Checks that a given expression is true. If the expression is not true
 *  a warning is logged, but, in contrast to the similar SLIC_ASSERT macro the
 *  application is not aborted.
 * \param [in] EXP user-supplied boolean expression.
 * \note This macro is only active when debugging is turned on.
 *
 * Usage:
 * \code
 *   SLIC_CHECK( my_val >= 0 );
 * \endcode
 *
 ******************************************************************************
 */
#define SLIC_CHECK( EXP )                                                     \
  do {                                                                        \
    if ( !(EXP) ) {                                                           \
      std::ostringstream oss;                                                 \
      oss << "Failed Check: " << # EXP << std::ends;                          \
      if (axom::slic::debug::checksAreErrors) {                               \
        axom::slic::logErrorMessage( oss.str(),__FILE__, __LINE__);           \
      }                                                                       \
      else {                                                                  \
        axom::slic::logWarningMessage( oss.str(),__FILE__, __LINE__);         \
      }                                                                       \
    }                                                                         \
  } while ( 0 )

/*!
 ******************************************************************************
 * \def SLIC_CHECK_MSG( EXP, msg )
 * \brief Same as SLIC_CHECK, but with a custom error message.
 * \param [in] EXP user-supplied boolean expression.
 * \param [in] msg user-supplied message
 * \note This macro is only active when debugging is turned on.
 * \see SLIC_DEBUG( EXP )
 *
 * Usage:
 * \code
 *   SLIC_CHECK_MSG( my_val >= 0, "my_val must always be positive" );
 * \endcode
 *
 ******************************************************************************
 */
#define SLIC_CHECK_MSG( EXP, msg )                                            \
  do {                                                                        \
    if ( !(EXP) ) {                                                           \
      std::ostringstream oss;                                                 \
      oss << "Failed Check: " << # EXP << std::endl << msg <<  std::ends;     \
      if (axom::slic::debug::checksAreErrors) {                               \
        axom::slic::logErrorMessage( oss.str(),__FILE__, __LINE__);           \
      }                                                                       \
      else {                                                                  \
        axom::slic::logWarningMessage( oss.str(),__FILE__, __LINE__);         \
      }                                                                       \
    }                                                                         \
  } while ( 0 )

/// @}

#else // turn off debug macros and asserts

#define SLIC_ASSERT( ignore_EXP ) ( (void)0 )
#define SLIC_ASSERT_MSG( ignore_EXP, ignore_msg ) ( (void)0 )
#define SLIC_CHECK( ignore_EXP ) ( (void)0 )
#define SLIC_CHECK_MSG( ignore_EXP, ignore_msg ) ( (void)0 )

#endif /* END ifdef ATK_DEBUG */

/*!
 ******************************************************************************
 * \def SLIC_INFO( msg )
 * \brief Logs an Info message.
 * \param [in] msg user-supplied message
 * \note The SLIC_INFO macro is always active.
 *
 * Usage:
 * \code
 *   SLIC_INFO( "informative text goes here" );
 * \endcode
 *
 ******************************************************************************
 */
#define SLIC_INFO( msg )                                                      \
  do {                                                                        \
    std::ostringstream oss;                                                   \
    oss << msg;                                                               \
    axom::slic::logMessage(axom::slic::message::Info                          \
                           , oss.str()                                        \
                           ,__FILE__                                          \
                           , __LINE__ );                                      \
  } while ( 0 )

#ifdef ATK_DEBUG

/*!
 ******************************************************************************
 * \def SLIC_DEBUG( msg )
 * \brief Logs a Debug message.
 * \param [in] msg user-supplied message
 * \note The SLIC_Debug macro is active in debug mode.
 *
 * Usage:
 * \code
 *   SLIC_DEBUG( "debug message goes here" );
 * \endcode
 *
 ******************************************************************************
 */
#define SLIC_DEBUG( msg )                                                     \
  do {                                                                        \
    std::ostringstream oss;                                                   \
    oss << msg;                                                               \
    axom::slic::logMessage(axom::slic::message::Debug                         \
                           , oss.str()                                        \
                           ,__FILE__                                          \
                           , __LINE__ );                                      \
  } while ( 0 )

#else // turn off debug macros

#define SLIC_DEBUG( ignore_EXP ) ( (void)0 )

#endif

namespace axom {
namespace slic {

struct debug
{
  static bool checksAreErrors;
};

/*!
 *******************************************************************************
 * \brief Initializes the SLIC logging environment.
 *******************************************************************************
 */
void initialize();

/*!
 *******************************************************************************
 * \brief Checks if the SLIC logging environment is initialized.
 * \return status true if initialized, else, false.
 *******************************************************************************
 */
bool isInitialized();

/*!
 *******************************************************************************
 * \brief Creates a new logger associated with the given name.
 * \param [in] name the name to associate with the new logger.
 * \param [in] imask inheritance mask, indicates the log level streams that
 *  will be inherited from the "root" logger. By default, nothing is inherited.
 *******************************************************************************
 */
void createLogger( const std::string& name,
                   char imask=inherit::nothing );

/*!
 *******************************************************************************
 * \brief Activates the logger associated with the given name.
 * \param [in] name the name of the logger to activate.
 * \return True if the named logger was activated, False otherwise
 *******************************************************************************
 */
bool activateLogger( const std::string& name );

/*!
 *******************************************************************************
 * \brief Returns the name of the active logger.
 * \return s a string corresponding to the name of the active logger.
 *******************************************************************************
 */
std::string getActiveLoggerName();

/*!
 *******************************************************************************
 * \brief Sets desired logging level.
 * \param [in] level user-supplied level to log.
 *******************************************************************************
 */
void setLoggingMsgLevel( message::Level level );

/*!
 *******************************************************************************
 * \brief Toggles the abort behavior for errors on the current active logger.
 * \param [in] status user-supplied flag.
 *******************************************************************************
 */
void setAbortOnError( bool status );

/*!
 *******************************************************************************
 * \brief Enables aborts on error messages for the current active logger.
 * \note This is equivalent to calling slic::setAbortOnError( true )
 * \post slic::isAbortOnErrorsEnabled() == true.
 * \pre slic::isInitialized() == true
 *******************************************************************************
 */
void enableAbortOnError();

/*!
 *******************************************************************************
 * \brief Disables aborts on error messages for the current active logger.
 * \note this is equivalent to calling slic::setAbortOnError( false )
 * \post slic::isAbortOnErrorsEnabled() == false.
 * \pre slic::isInitialized() == true
 *******************************************************************************
 */
void disableAbortOnError();

/*!
 *******************************************************************************
 * \brief Checks whether aborts on errors are enabled for the current logger.
 * \return status true if aborts on errors are enabled, otherwise, false.
 * \pre slic::isInitialized() == true.
 *******************************************************************************
 */
bool isAbortOnErrorsEnabled();

/*!
 *******************************************************************************
 * \brief Toggles the abort behavior for warnings on the current active logger.
 * \param [in] status user-supplied flag.
 *******************************************************************************
 */
void setAbortOnWarning( bool status );

/*!
 *******************************************************************************
 * \brief Enables aborts on warnings messages for the current active logger.
 * \note This is equivalent to calling slic::setAbortOnWarning( true )
 * \post slic::isAbortOnWarningsEnabled() == true.
 * \pre slic::isInitialized() == true.
 *******************************************************************************
 */
void enableAbortOnWarning();

/*!
 *******************************************************************************
 * \brief Disables aborts on warnings messages for the curernt active logger.
 * \note This is equivalent to calling slic::setAbortOnWarnings( false )
 * \post slic::isAbortOnWarnigsEnabled() == true.
 * \pre slic::isInitialized() == true.
 *******************************************************************************
 */
void disableAbortOnWarning();

/*!
 *******************************************************************************
 * \brief Checks whether aborts on warnings are enabled for the current logger.
 * \return status true if aborts on warnings are enabled, otherwise, false.
 * \pre slic::isInitialized() == true.
 *******************************************************************************
 */
bool isAbortOnWarningsEnabled();

/*!
 *******************************************************************************
 * \brief Adds the given stream to the the given level.
 * \param [in] ls pointer to the log stream.
 * \param [in] level the level to log.
 * \pre ls != ATK_NULLPTR
 *******************************************************************************
 */
void addStreamToMsgLevel( LogStream* ls, message::Level level);

/*!
 *******************************************************************************
 * \brief Adds the given stream to all levels.
 * \param [in] ls pointer to the log stream.
 * \pre ls != ATK_NULLPTR.
 *******************************************************************************
 */
void addStreamToAllMsgLevels( LogStream* ls );

/*!
 *******************************************************************************
 * \brief Logs the given message to all registered streams.
 * \param [in] level the level of the message being logged.
 * \param [in] message user-supplied message.
 * \param [in] filter_duplicates optional parameter that indicates whether
 * duplicate messages resulting from running in parallel will be filtered out.
 * Default is false.
 *******************************************************************************
 */
void logMessage( message::Level level,
                 const std::string& message,
                 bool filter_duplicates=false );

/*!
 *******************************************************************************
 * \brief Logs the given message to all registered streams.
 * \param [in] level the level of the message being logged.
 * \param [in] message user-supplied message.
 * \param [in] tag user-supplied associated with this message.
 * \param [in] filter_duplicates optional parameter that indicates whether
 * duplicate messages resulting from running in parallel will be filtered out.
 * Default is false.
 *******************************************************************************
 */
void logMessage( message::Level level,
                 const std::string& message,
                 const std::string& tag,
                 bool filter_duplicates=false );

/*!
 *******************************************************************************
 * \brief Logs the given message to all registered streams.
 * \param [in] level the level of the message being logged.
 * \param [in] message user-supplied message.
 * \param [in] fileName the name of the file this message is logged from.
 * \param [in] line the line number within the file this message is logged.
 * \param [in] filter_duplicates optional parameter that indicates whether
 * duplicate messages resulting from running in parallel will be filtered out.
 * Default is false.
 *******************************************************************************
 */
void logMessage( message::Level level,
                 const std::string& message,
                 const std::string& fileName,
                 int line,
                 bool filter_duplicates=false );

/*!
 *******************************************************************************
 * \brief Logs the given message to all registered streams.
 * \param [in] level the level of the message being logged.
 * \param [in] message user-supplied message.
 * \param [in] tag user-supplied tag associated with the message.
 * \param [in] fileName the name of the file this message is logged form.
 * \param [in] line the line number within the file this message is logged.
 * \param [in] filter_duplicates optional parameter that indicates whether
 * duplicate messages resulting from running in parallel will be filtered out.
 * Default is false.
 *******************************************************************************
 */
void logMessage( message::Level level,
                 const std::string& message,
                 const std::string& tag,
                 const std::string& fileName,
                 int line,
                 bool filter_duplicates=false );

/*!
 *******************************************************************************
 * \brief Convenience method to log an error message.
 * \param [in] message user-supplied message.
 * \param [in] fileName the name of the file this message is logged from.
 * \param [in] line the line number within the file that the message is logged.
 *******************************************************************************
 */
void logErrorMessage( const std::string& message,
                      const std::string& fileName,
                      int line);

/*!
 *******************************************************************************
 * \brief Convenience method to log warning messages.
 * \param [in] message user-supplied message.
 * \param [in] fileName the name of the file this message is logged from.
 * \param [in] line the line number within the file that the message is logged.
 *******************************************************************************
 */
void logWarningMessage( const std::string& message,
                        const std::string& fileName,
                        int line );

/*!
 *******************************************************************************
 * \brief Flushes all streams.
 * \see Logger::flushStreams.
 *******************************************************************************
 */
void flushStreams();

/*!
 *******************************************************************************
 * \brief Pushes all streams.
 * \see Logger::pushStreams.
 *******************************************************************************
 */
void pushStreams();

/*!
 *******************************************************************************
 * \brief Finalizes the slic logging environment.
 *******************************************************************************
 */
void finalize();

/*!
 *******************************************************************************
 * \brief Uses glibc's backtrace() functionality to return a stacktrace
 * \return s a string corresponding to the stacktrace.
 *******************************************************************************
 */
std::string stacktrace( );

} /* namespace slic */

} /* namespace axom */

#endif /* SLIC_HPP_ */
