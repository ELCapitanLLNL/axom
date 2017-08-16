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
 * \file parallel_logging_example.cc
 *
 */

// C/C++ includes
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

// axom includes
#include "axom/Types.hpp"
#include "slic/slic.hpp"
#include "slic/LumberjackStream.hpp"

// MPI
#include <mpi.h>

using namespace axom;

#define CYCLELIMIT 5
#define RANKSLIMIT 5

#define N 20

slic::message::Level getRandomEvent( const int start, const int end )
{
  return( static_cast<slic::message::Level>(std::rand() % (end-start) + start));
}

//------------------------------------------------------------------------------
int main( int argc, char** argv )
{
  // Initialize MPI
  MPI_Init( &argc, &argv );
  int rank=-1;
  MPI_Comm_rank( MPI_COMM_WORLD, &rank );

  // Initialize SLIC
  std::string format = std::string( "<MESSAGE>\n") +
                       std::string( "\t<TIMESTAMP>\n" ) +
                       std::string( "\tLEVEL=<LEVEL>\n") +
                       std::string( "\tRANKS=<RANK>\n") +
                       std::string( "\tFILE=<FILE>\n") +
                       std::string( "\tLINE=<LINE>\n");
  slic::initialize();

  // Set SLIC logging level and Lumberjack Logging stream
  slic::setLoggingMsgLevel( slic::message::Debug );
  slic::disableAbortOnError();
  slic::LumberjackStream* ljStream =
        new slic::LumberjackStream( &std::cout, MPI_COMM_WORLD, RANKSLIMIT, format );
  slic::addStreamToAllMsgLevels( ljStream );

  // Queue messages
  int cycleCount = 0;
  for (int i = 0; i < N; ++i){
    std::ostringstream oss;
    oss << "message " << i << "/" << N-1;

    slic::logMessage( getRandomEvent(0,slic::message::Num_Levels),
                      oss.str(),
                      __FILE__,
                      __LINE__
                      );

    ++cycleCount;
    if (cycleCount > CYCLELIMIT) {
      // Incrementally push messages through the log stream
      slic::pushStreams();
      cycleCount = 0;
    }
  }

  // Fully flush system of messages
  slic::flushStreams();

  // Shutdown SLIC which in turn shutsdown Lumberjack
  slic::finalize();

  // Finalize MPI
  MPI_Finalize();

  return 0;
}
