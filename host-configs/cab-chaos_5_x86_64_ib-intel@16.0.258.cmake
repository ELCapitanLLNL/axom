##################################
# uberenv host-config
#
# This is a generated file, edit at own risk.
##################################
# chaos_5_x86_64_ib-intel@16.0.258
##################################

# cmake from uberenv
# cmake executable path: /usr/workspace/wsa/axom/thirdparty_libs/builds/2017_11_15_10_37_37/spack/opt/spack/chaos_5_x86_64_ib/intel-16.0.258/cmake-3.8.2-2esy2nq3rdz3fpan6scsvrxibu3sfag4/bin/cmake

#######
# using intel@16.0.258 compiler spec
#######

# c compiler used by spack
set(CMAKE_C_COMPILER "/usr/local/tools/ic-16.0.258/bin/icc" CACHE PATH "")

# cpp compiler used by spack
set(CMAKE_CXX_COMPILER "/usr/local/tools/ic-16.0.258/bin/icpc" CACHE PATH "")

# fortran compiler used by spack
set(ENABLE_FORTRAN ON CACHE BOOL "")

set(CMAKE_Fortran_COMPILER "/usr/local/tools/ic-16.0.258/bin/ifort" CACHE PATH "")

# Root directory for generated TPLs
set(TPL_ROOT "/usr/workspace/wsa/axom/thirdparty_libs/builds/2017_11_15_10_37_37/spack/opt/spack/chaos_5_x86_64_ib/intel-16.0.258" CACHE PATH "")

# hdf5 from uberenv
set(HDF5_DIR "${TPL_ROOT}/hdf5-1.8.16-zvfopdsqd7w4vjwfswxp5utura6sr3cv" CACHE PATH "")

# scr not built by uberenv

# conduit from uberenv
set(CONDUIT_DIR "${TPL_ROOT}/conduit-0.2.1-klgff7xygiguj7kbild575hwf3icqqta" CACHE PATH "")

# mfem from uberenv
set(MFEM_DIR "${TPL_ROOT}/mfem-3.3.2-xbfzoiwzyh5kxekrxd2r4mmcimhg245t" CACHE PATH "")

# boost headers from uberenv
set(BOOST_DIR "${TPL_ROOT}/boost-headers-1.58.0-yiarrlaedsp4aidnxesdu2iqougxt365" CACHE PATH "")

# python from uberenv
set(PYTHON_EXECUTABLE "${TPL_ROOT}/python-2.7.11-t4izxxjuwjf2ryyv65vddraz6guolocp/bin/python" CACHE PATH "")

# lua from uberenv
set(LUA_DIR "${TPL_ROOT}/lua-5.1.5-slj6twcch6tvfdomhkqdknjjxiwswmzf" CACHE PATH "")

# doxygen from uberenv
set(DOXYGEN_EXECUTABLE "${TPL_ROOT}/doxygen-1.8.11-jqhrxckf2kswxfogk73qrgc2cru3dulc/bin/doxygen" CACHE PATH "")

# sphinx from uberenv
set(SPHINX_EXECUTABLE "${TPL_ROOT}/python-2.7.11-t4izxxjuwjf2ryyv65vddraz6guolocp/bin/sphinx-build" CACHE PATH "")

# shroud from uberenv
set(SHROUD_EXECUTABLE "${TPL_ROOT}/python-2.7.11-t4izxxjuwjf2ryyv65vddraz6guolocp/bin/shroud" CACHE PATH "")

# uncrustify from uberenv
set(UNCRUSTIFY_EXECUTABLE "${TPL_ROOT}/uncrustify-0.61-53naixuibvacrffbdkwexmef7wgj5fyf/bin/uncrustify" CACHE PATH "")

# lcov and genhtml from uberenv
set(LCOV_PATH "${TPL_ROOT}/lcov-1.11-hwb6f57wogxsdgial7urrqaijbh6ufjz/usr/bin/lcov" CACHE PATH "")

set(GENHTML_PATH "${TPL_ROOT}/lcov-1.11-hwb6f57wogxsdgial7urrqaijbh6ufjz/usr/bin/genhtml" CACHE PATH "")

# Disable CXX11 on chaos5 intel/clang builds
set(BLT_CXX_STD "c++98" CACHE PATH "")

##################################
# end uberenv host-config
##################################

##
## Copyright (c) 2017, Lawrence Livermore National Security, LLC.
##
## Produced at the Lawrence Livermore National Laboratory.
##
## LLNL-CODE-741217
##
## All rights reserved.
##
## This file is part of Axom.
##
## For details about use and distribution, please read axom/LICENSE.
##

##############################################################################
# !---------------------------------------------------------------------------
##############################################################################
# Options added manually to 
# lc chaos5 intel@16.0.258 host configs
##############################################################################

set(ENABLE_GTEST_DEATH_TESTS ON CACHE BOOL "")

##############################################################################
# MPI - manually added for now
##############################################################################
set(ENABLE_MPI ON CACHE BOOL "")

set(MPI_HOME             "/usr/local/tools/mvapich2-intel-2.0" CACHE PATH "")
set(MPI_C_COMPILER       "${MPI_HOME}/bin/mpicc" CACHE PATH "")
set(MPI_CXX_COMPILER     "${MPI_HOME}/bin/mpicxx" CACHE PATH "")
set(MPI_Fortran_COMPILER "${MPI_HOME}/bin/mpif90" CACHE PATH "")

set(MPIEXEC              "/usr/bin/srun" CACHE PATH "")
set(MPIEXEC_NUMPROC_FLAG "-n" CACHE PATH "")

##############################################################################
# !---------------------------------------------------------------------------
##############################################################################

