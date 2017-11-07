#-------------------------------------------------------------------------------
# Copyright (c) 2017, Lawrence Livermore National Security, LLC.
#
# Produced at the Lawrence Livermore National Laboratory
#
# LLNL-CODE-741217
#
# All rights reserved.
#
# This file is part of Axom.
#
# For details about use and distribution, please read axom/LICENSE.
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
# Setup HDF5
#
# Wrapper around standard CMake' HDF5 Find Logic.
#-------------------------------------------------------------------------------

if(NOT HDF5_DIR)
    MESSAGE(FATAL_ERROR "Could not find HDF5. HDF5 support needs explicit HDF5_DIR")
endif()

# CMake's FindHDF5 module uses the HDF5_ROOT env var
set(HDF5_ROOT ${HDF5_DIR})
set(ENV{HDF5_ROOT} ${HDF5_ROOT}/bin)

# Use CMake's FindHDF5 module, which uses hdf5's compiler wrappers to extract
# all the info about the hdf5 install
include(FindHDF5)

message(STATUS "HDF5_INCLUDE_DIRS: ${HDF5_INCLUDE_DIRS}")
message(STATUS "HDF5_LIBRARIES: ${HDF5_LIBRARIES}")

# FindHDF5 sets HDF5_DIR to it's installed CMake info if it exists
# we want to keep HDF5_DIR as the root dir of the install to be 
# consistent with other packages

set(HDF5_DIR ${HDF5_ROOT} CACHE PATH "" FORCE)

