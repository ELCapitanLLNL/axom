# Copyright (c) 2017-2022, Lawrence Livermore National Security, LLC and
# other Axom Project Developers. See the top-level LICENSE file for details.
#
# SPDX-License-Identifier: (BSD-3-Clause)
#------------------------------------------------------------------------------
# Quest component
#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
# Check necessary dependencies
#
# Note: Quest also optionally depends on Klee, when Klee is enabled
#------------------------------------------------------------------------------
axom_component_requires(NAME       QUEST
                        COMPONENTS MINT PRIMAL SLAM SLIC SPIN)

#------------------------------------------------------------------------------
# Specify headers, sources and dependencies
#------------------------------------------------------------------------------
set( quest_headers

    Delaunay.hpp
    SignedDistance.hpp

    ## All-nearest-neighbors query
    AllNearestNeighbors.hpp
    detail/AllNearestNeighbors_detail.hpp

    ## Shaping
    Discretize.hpp
    detail/Discretize_detail.hpp

    ## In/out query
    InOutOctree.hpp
    detail/inout/BlockData.hpp
    detail/inout/MeshWrapper.hpp
    detail/inout/InOutOctreeMeshDumper.hpp
    detail/inout/InOutOctreeStats.hpp
    detail/inout/InOutOctreeValidator.hpp

    # Mesh tester
    MeshTester.hpp
    detail/MeshTester_detail.hpp

    # PointInCell
    PointInCell.hpp
    detail/PointFinder.hpp
    detail/PointInCellMeshWrapper_mfem.hpp

    ## File readers
    readers/STLReader.hpp

    ## quest interface
    interface/internal/mpicomm_wrapper.hpp
    interface/internal/QuestHelpers.hpp
    interface/inout.hpp
    interface/signed_distance.hpp

    )

set( quest_sources

    ## Signed-distance query
    SignedDistance.cpp

    ## All-nearest-neighbors query
    AllNearestNeighbors.cpp

    ## Discretize shapes
    Discretize.cpp

    ## Mesh tester
    MeshTester.cpp

    ## File readers
    readers/STLReader.cpp

    ## quest interface
    interface/internal/QuestHelpers.cpp
    interface/inout.cpp
    interface/signed_distance.cpp

   )


set( quest_depends_on
    core
    slic
    mint
    slam
    spin
    primal
    fmt
    )

blt_list_append(TO quest_depends_on IF AXOM_ENABLE_SIDRE ELEMENTS sidre)
blt_list_append(TO quest_depends_on IF ENABLE_CUDA ELEMENTS cuda)
blt_list_append(TO quest_depends_on IF ENABLE_HIP ELEMENTS blt::hip)
blt_list_append(TO quest_depends_on IF ENABLE_OPENMP ELEMENTS openmp)
blt_list_append(TO quest_depends_on IF SPARSEHASH_FOUND ELEMENTS sparsehash)
blt_list_append(TO quest_depends_on IF MFEM_FOUND ELEMENTS mfem)

if(AXOM_ENABLE_SIDRE)
    list(APPEND quest_headers    ScatteredInterpolation.hpp )
endif()

if(CONDUIT_FOUND AND ENABLE_MPI)
    list(APPEND quest_headers    DistributedClosestPoint.hpp )
    list(APPEND quest_depends_on conduit::conduit
                                 conduit::conduit_mpi)
endif()

if(MFEM_FOUND AND AXOM_ENABLE_KLEE AND AXOM_ENABLE_SIDRE AND AXOM_ENABLE_MFEM_SIDRE_DATACOLLECTION)
    list(APPEND quest_headers Shaper.hpp
                              SamplingShaper.hpp
                              IntersectionShaper.hpp
                              detail/shaping/shaping_helpers.hpp)
    list(APPEND quest_sources Shaper.cpp
                              detail/shaping/shaping_helpers.cpp)
endif()

if(C2C_FOUND)
    list(APPEND quest_headers readers/C2CReader.hpp)
    list(APPEND quest_sources readers/C2CReader.cpp)
    list(APPEND quest_depends_on c2c)
endif()

if (ENABLE_MPI)
    list(APPEND quest_headers readers/PSTLReader.hpp)
    blt_list_append(TO quest_headers IF C2C_FOUND ELEMENTS readers/PC2CReader.hpp)

    list(APPEND quest_sources readers/PSTLReader.cpp)
    blt_list_append(TO quest_sources IF C2C_FOUND ELEMENTS readers/PC2CReader.cpp)
    
    list(APPEND quest_depends_on mpi)
endif()

if (SHROUD_FOUND)
    add_subdirectory(interface)
endif()

if (ENABLE_FORTRAN)
    ## append shroud generated headers/sources to corresponding lists
    list(APPEND quest_headers interface/c_fortran/wrapQUEST.h)
    list(APPEND quest_headers interface/c_fortran/typesQUEST.h)
    list(APPEND quest_sources interface/c_fortran/wrapQUEST.cpp)
    list(APPEND quest_sources interface/c_fortran/wrapfquest.F)
endif()

if (BUILD_SHARED_LIBS)
    # Install setup.py file used to build quest Python module.
    configure_file(interface/python/setup.py.in
                   ${CMAKE_CURRENT_BINARY_DIR}/interface/python/setup.py)
    configure_file(interface/python/quest_test.py.in
                   ${CMAKE_CURRENT_BINARY_DIR}/interface/python/quest_test.py)
endif()

#------------------------------------------------------------------------------
# Make and install the library
#------------------------------------------------------------------------------
blt_add_library(
    NAME        quest
    SOURCES     ${quest_sources}
    HEADERS     ${quest_headers}
    DEPENDS_ON  ${quest_depends_on}
    FOLDER      axom/quest
    OBJECT      TRUE
    )

axom_write_unified_header(NAME    quest
                          HEADERS ${quest_headers}
                          )

axom_install_component(NAME    quest
                       HEADERS ${quest_headers}
                       )

#------------------------------------------------------------------------------
# Add tests and examples
#------------------------------------------------------------------------------
if (AXOM_ENABLE_EXAMPLES)
    add_subdirectory(examples)
endif()

if (AXOM_ENABLE_TESTS)
    add_subdirectory(tests)
endif()

axom_add_code_checks(PREFIX quest)
