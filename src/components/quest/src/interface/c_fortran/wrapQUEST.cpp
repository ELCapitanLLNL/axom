// wrapQUEST.cpp
// This is generated code, do not edit
//
// Copyright (c) 2017, Lawrence Livermore National Security, LLC.
//
// Produced at the Lawrence Livermore National Laboratory
//
// LLNL-CODE-xxxxxx
//
// All rights reserved.
//
// This file is part of Axom.
//
// For details about use and distribution, please read axom/LICENSE.
//
// wrapQUEST.cpp
#include "wrapQUEST.h"
#include <string>
#include "quest/quest.hpp"

namespace axom {
namespace quest {

// splicer begin CXX_definitions
// splicer end CXX_definitions

extern "C" {

// splicer begin C_definitions
// splicer end C_definitions

void QUEST_initialize(MPI_Fint comm, const char * fileName, bool requiresDistance, int ndims, int maxElements, int maxLevels)
{
// splicer begin function.initialize
    const std::string SH_fileName(fileName);
    initialize(MPI_Comm_f2c(comm), SH_fileName, requiresDistance, ndims, maxElements, maxLevels);
    return;
// splicer end function.initialize
}

void QUEST_initialize_bufferify(MPI_Fint comm, const char * fileName, int LfileName, bool requiresDistance, int ndims, int maxElements, int maxLevels)
{
// splicer begin function.initialize_bufferify
    const std::string SH_fileName(fileName, LfileName);
    initialize(MPI_Comm_f2c(comm), SH_fileName, requiresDistance, ndims, maxElements, maxLevels);
    return;
// splicer end function.initialize_bufferify
}

void QUEST_finalize()
{
// splicer begin function.finalize
    finalize();
    return;
// splicer end function.finalize
}

double QUEST_distance(double x, double y, double z)
{
// splicer begin function.distance
    double SH_rv = distance(x, y, z);
    return SH_rv;
// splicer end function.distance
}

int QUEST_inside(double x, double y, double z)
{
// splicer begin function.inside
    int SH_rv = inside(x, y, z);
    return SH_rv;
// splicer end function.inside
}

void QUEST_mesh_min_bounds(double * coords)
{
// splicer begin function.mesh_min_bounds
    mesh_min_bounds(coords);
    return;
// splicer end function.mesh_min_bounds
}

void QUEST_mesh_max_bounds(double * coords)
{
// splicer begin function.mesh_max_bounds
    mesh_max_bounds(coords);
    return;
// splicer end function.mesh_max_bounds
}

void QUEST_mesh_center_of_mass(double * coords)
{
// splicer begin function.mesh_center_of_mass
    mesh_center_of_mass(coords);
    return;
// splicer end function.mesh_center_of_mass
}

}  // extern "C"

}  // namespace quest
}  // namespace axom
