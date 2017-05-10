##################################
# uberenv host-config
#
# This is a generated file, edit at own risk.
##################################
# toss_3_x86_64_ib-clang@3.9.0
##################################

# cmake from uberenv
# cmake executable path: /usr/workspace/wsrzc/axom/thirdparty_libs/builds/2017_05_01_16_48_17/spack/opt/spack/toss_3_x86_64_ib/clang-3.9.0/cmake-3.3.1-usuntzhfbmwz5ci5fryejg52swgb5ruo/bin/cmake

#######
# using clang@3.9.0 compiler spec
#######

# c compiler used by spack
set(CMAKE_C_COMPILER "/usr/tce/packages/clang/clang-3.9.0/bin/clang" CACHE PATH "")

# cpp compiler used by spack
set(CMAKE_CXX_COMPILER "/usr/tce/packages/clang/clang-3.9.0/bin/clang++" CACHE PATH "")

# fortran compiler used by spack
# no fortran compiler

set(ENABLE_FORTRAN "OFF" CACHE PATH "")

# Root directory for generated TPLs
set(TPL_ROOT "/usr/workspace/wsrzc/axom/thirdparty_libs/builds/2017_05_01_16_48_17/spack/opt/spack/toss_3_x86_64_ib/clang-3.9.0" CACHE PATH "")

# hdf5 from uberenv
set(HDF5_DIR "${TPL_ROOT}/hdf5-1.8.16-qy646zjgaj4crjrgwrlqj2slnvjel5re" CACHE PATH "")

# conduit from uberenv
set(CONDUIT_DIR "${TPL_ROOT}/conduit-0.2.1-spqnui5wsqcmq46b3uyaimo5gxz3bttj" CACHE PATH "")

# boost headers from uberenv
set(BOOST_DIR "${TPL_ROOT}/boost-headers-1.58.0-cjtvgl2bbxo727ky5bfaiihg4uublmbp" CACHE PATH "")

# python from uberenv
set(PYTHON_EXECUTABLE "${TPL_ROOT}/python-2.7.11-dozo3tjzgctvcgazuvlp4gcnduc43ery/bin/python" CACHE PATH "")

# lua from uberenv
set(LUA_DIR "${TPL_ROOT}/lua-5.1.5-izxbfxhlava24dron2y7i2pruwx3cuh3" CACHE PATH "")

# doxygen from uberenv
set(DOXYGEN_EXECUTABLE "${TPL_ROOT}/doxygen-1.8.11-bprlm2fmh7jvshvtty2a2c6utnyw6u7s/bin/doxygen" CACHE PATH "")

# sphinx from uberenv
set(SPHINX_EXECUTABLE "${TPL_ROOT}/python-2.7.11-dozo3tjzgctvcgazuvlp4gcnduc43ery/bin/sphinx-build" CACHE PATH "")

# uncrustify from uberenv
set(UNCRUSTIFY_EXECUTABLE "${TPL_ROOT}/uncrustify-0.61-vgt2p36odqlrkexkus4xzzy53uyiopug/bin/uncrustify" CACHE PATH "")

# lcov and genhtml from uberenv
set(LCOV_PATH "${TPL_ROOT}/lcov-1.11-hucmk5dnudjpfjy2vhnid46g3fmi3r7n/usr/bin/lcov" CACHE PATH "")

set(GENHTML_PATH "${TPL_ROOT}/lcov-1.11-hucmk5dnudjpfjy2vhnid46g3fmi3r7n/usr/bin/genhtml" CACHE PATH "")

##################################
# end uberenv host-config
##################################

##############################################################################
# !---------------------------------------------------------------------------
##############################################################################
# Options added manually to 
# lc toss3 clang@3.9.0  host configs
##############################################################################

##############################################################################
# MPI - manually added for now
##############################################################################

# Temporarily disabling MPI on toss3 clang. See JIRA issue ATK-1017.
set(ENABLE_MPI OFF CACHE BOOL "")

set(MPI_HOME             "/usr/tce/packages/mvapich2/mvapich2-2.2-clang-3.9.0" CACHE PATH "")
set(MPI_C_COMPILER       "${MPI_HOME}/bin/mpicc"   CACHE PATH "")
set(MPI_CXX_COMPILER     "${MPI_HOME}/bin/mpicxx"  CACHE PATH "")
set(MPI_Fortran_COMPILER "${MPI_HOME}/bin/mpifort" CACHE PATH "")

set(MPIEXEC              "/usr/bin/srun" CACHE PATH "")
set(MPIEXEC_NUMPROC_FLAG "-n" CACHE PATH "")

##############################################################################
# SHROUD - manually added for now. Use a public build add to TPL later
##############################################################################
set(SHROUD_EXECUTABLE "/usr/apps/shroud/bin/shroud" CACHE PATH "")

##############################################################################
# !---------------------------------------------------------------------------
##############################################################################

