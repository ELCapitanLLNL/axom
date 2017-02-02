####################################
# 3rd Party Dependencies
####################################

################################
# Conduit
################################
if (CONDUIT_DIR)
  include(cmake/thirdparty/FindConduit.cmake)
  blt_register_library( NAME conduit
                        INCLUDES ${CONDUIT_INCLUDE_DIRS} 
                        LIBRARIES  conduit)
  blt_register_library( NAME conduit_relay
                        INCLUDES ${CONDUIT_INCLUDE_DIRS}
                        LIBRARIES  conduit_relay)
endif()

################################
# HDF5
################################
if (HDF5_DIR)
  include(cmake/thirdparty/FindHDF5.cmake)
  blt_register_library(NAME hdf5
                       INCLUDES ${HDF5_INCLUDE_DIRS}
                       LIBRARIES ${HDF5_LIBRARY} )
endif()


################################
# Sparsehash
################################
if (SPARSEHASH_DIR)
  include(cmake/thirdparty/FindSparsehash.cmake)
  blt_register_library(NAME sparsehash
                       INCLUDES ${SPARSEHASH_INCLUDE_DIRS})
endif()


################################
# Find boost headers
################################
if (BOOST_DIR)
    include(cmake/thirdparty/SetupBoost.cmake)
    blt_register_library(NAME boost
                         INCLUDES ${Boost_INCLUDE_DIR})
endif()

################################
# Python
################################

if(ENABLE_PYTHON AND PYTHON_EXECUTABLE)
    ################################
    # Setup includes for Python
    ################################
    include(cmake/thirdparty/FindPython.cmake)
    message(STATUS "Using Python Include: ${PYTHON_INCLUDE_DIRS}")
    # if we don't find python, throw a fatal error
    if(NOT PYTHON_FOUND)
        message(FATAL_ERROR "ENABLE_PYTHON is set, but Python wasn't found.")
    endif()

    ## Set the Python module directory
    # relative path (used with install)
    set(BLT_Python_SITE_PACKAGES
        "lib/python${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}/site-packages"
        CACHE PATH
        "Relative path where all Python modules will go in the install tree"
    )
    # build site-packages
    set(BLT_Python_MODULE_DIRECTORY
        "${PROJECT_BINARY_DIR}/${BLT_Python_SITE_PACKAGES}"
        CACHE PATH
        "Directory where all Python modules will go in the build tree"
    )

    file(MAKE_DIRECTORY ${BLT_Python_MODULE_DIRECTORY})
    set(ENV{PYTHONPATH} ${BLT_Python_MODULE_DIRECTORY})

    INSTALL(DIRECTORY DESTINATION ${BLT_Python_SITE_PACKAGES})
    INSTALL(CODE " set(ENV\{PYTHONPATH\} ${CMAKE_INSTALL_PREFIX}/${BLT_Python_SITE_PACKAGES}) ")

    blt_register_library(
        NAME python
        INCLUDES ${PYTHON_INCLUDE_DIRS}
        LIBRARIES ${PYTHON_LIBRARIES}
    )
endif(ENABLE_PYTHON AND PYTHON_EXECUTABLE)

################################
# Lua
################################

if (LUA_DIR)
    # Set the hint for FindLua
    set (ENV{LUA_DIR}  ${LUA_DIR})
    find_package(Lua)
    if(NOT LUA_FOUND)
        message( FATAL_ERROR "Requested Lua was not found: ${LUA_DIR}")
    endif()

    set(LUA_EXECUTABLE ${LUA_DIR}/bin/lua)

    blt_register_library(
        NAME lua
        INCLUDES ${LUA_INCLUDE_DIR}
        LIBRARIES ${LUA_LIBRARIES}
    )
endif()

