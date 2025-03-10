#------------------------------------------------------------------------------
# !!!! This is a generated file, edit at own risk !!!!
#------------------------------------------------------------------------------
# CMake executable path: /usr/bin/cmake
#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
# Compilers
#------------------------------------------------------------------------------
# Compiler Spec: clang@10.0.0
#------------------------------------------------------------------------------
if(DEFINED ENV{SPACK_CC})

  set(CMAKE_C_COMPILER "/home/axom/axom_tpls/spack/lib/spack/env/clang/clang" CACHE PATH "")

  set(CMAKE_CXX_COMPILER "/home/axom/axom_tpls/spack/lib/spack/env/clang/clang++" CACHE PATH "")

  set(CMAKE_Fortran_COMPILER "/home/axom/axom_tpls/spack/lib/spack/env/clang/gfortran" CACHE PATH "")

else()

  set(CMAKE_C_COMPILER "/usr/bin/clang" CACHE PATH "")

  set(CMAKE_CXX_COMPILER "/usr/bin/clang++" CACHE PATH "")

  set(CMAKE_Fortran_COMPILER "/usr/bin/gfortran" CACHE PATH "")

endif()

set(CMAKE_C_FLAGS "-pthread" CACHE STRING "")

set(CMAKE_CXX_FLAGS "-pthread" CACHE STRING "")

set(ENABLE_FORTRAN ON CACHE BOOL "")

set(BLT_EXE_LINKER_FLAGS " -Wl,-rpath,/usr/lib -Wl,-rpath,/usr/lib64" CACHE STRING "Adds a missing libstdc++ rpath")

#------------------------------------------------------------------------------
# MPI
#------------------------------------------------------------------------------

set(MPI_C_COMPILER "/usr/bin/mpicc" CACHE PATH "")

set(MPI_CXX_COMPILER "/usr/bin/mpic++" CACHE PATH "")

set(MPI_Fortran_COMPILER "/usr/bin/mpif90" CACHE PATH "")

set(MPIEXEC_EXECUTABLE "/usr/bin/mpirun" CACHE PATH "")

set(MPIEXEC_NUMPROC_FLAG "-np" CACHE STRING "")

set(ENABLE_MPI ON CACHE BOOL "")

#------------------------------------------------------------------------------
# Hardware
#------------------------------------------------------------------------------

#------------------------------------------------
# Hardware Specifics
#------------------------------------------------

set(ENABLE_OPENMP ON CACHE BOOL "")

set(ENABLE_GTEST_DEATH_TESTS ON CACHE BOOL "")

#------------------------------------------------------------------------------
# TPLs
#------------------------------------------------------------------------------

# Root directory for generated TPLs

set(TPL_ROOT "/home/axom/axom_tpls/clang-10.0.0" CACHE PATH "")

set(CONDUIT_DIR "${TPL_ROOT}/conduit-0.8.3" CACHE PATH "")

# C2C not built

set(MFEM_DIR "${TPL_ROOT}/mfem-4.4.0" CACHE PATH "")

set(HDF5_DIR "${TPL_ROOT}/hdf5-1.8.22" CACHE PATH "")

set(LUA_DIR "${TPL_ROOT}/lua-5.3.5" CACHE PATH "")

set(RAJA_DIR "${TPL_ROOT}/raja-2022.03.0" CACHE PATH "")

set(UMPIRE_DIR "${TPL_ROOT}/umpire-2022.03.1" CACHE PATH "")

set(CAMP_DIR "${TPL_ROOT}/camp-2022.03.0" CACHE PATH "")

# scr not built

#------------------------------------------------------------------------------
# Devtools
#------------------------------------------------------------------------------

# ClangFormat disabled due to disabled devtools

set(ENABLE_CLANGFORMAT OFF CACHE BOOL "")

set(ENABLE_DOCS OFF CACHE BOOL "")


