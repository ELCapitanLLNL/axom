# Copyright (c) 2017-2022, Lawrence Livermore National Security, LLC and
# other Axom Project Developers. See the top-level LICENSE file for details.
#
# SPDX-License-Identifier: (BSD-3-Clause)
#------------------------------------------------------------------------------
# Unit tests for Spin component
#------------------------------------------------------------------------------

set(spin_tests
    spin_implicit_grid.cpp
    spin_morton.cpp
    spin_octree.cpp
    spin_rectangular_lattice.cpp
    spin_spatial_octree.cpp
    spin_uniform_grid.cpp
   )

set(spin_tests_depends
    axom
    gtest
    fmt
    )

blt_list_append( TO spin_tests_depends ELEMENTS cuda IF ${ENABLE_CUDA} )
blt_list_append( TO spin_tests_depends ELEMENTS blt::hip IF ${ENABLE_HIP} )


foreach ( test ${spin_tests} )

    get_filename_component( test_name ${test} NAME_WE )

    blt_add_executable(
      NAME       ${test_name}_test
      SOURCES    ${test}
      OUTPUT_DIR ${TEST_OUTPUT_DIRECTORY}
      DEPENDS_ON ${spin_tests_depends}
      FOLDER     axom/spin/tests
      )

    axom_add_test(
      NAME    ${test_name}
      COMMAND ${test_name}_test
      )

endforeach()

if (AXOM_ENABLE_MINT)

    blt_add_executable(
      NAME       spin_bvh_test
      SOURCES    spin_bvh.cpp
      OUTPUT_DIR ${TEST_OUTPUT_DIRECTORY}
      DEPENDS_ON ${spin_tests_depends}
      FOLDER     axom/spin/tests
      )

    axom_add_test(
      NAME            spin_bvh
      COMMAND         spin_bvh_test
      NUM_OMP_THREADS 4
      )

endif()
