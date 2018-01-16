/*
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Copyright (c) 2017-2018, Lawrence Livermore National Security, LLC.
 *
 * Produced at the Lawrence Livermore National Laboratory
 *
 * LLNL-CODE-741217
 *
 * All rights reserved.
 *
 * This file is part of Axom.
 *
 * For details about use and distribution, please read axom/LICENSE.
 *
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

#include "SynchronizedStream.hpp"

#include <vector>

#include "axom/Macros.hpp"
#include "axom_utils/StringUtilities.hpp"

namespace axom
{
namespace slic
{

struct SynchronizedStream::MessageCache
{

  std::vector< std::string > messages;

  void printMessages( std::ostream* stream )
  {
    if ( stream == AXOM_NULLPTR )
    {
      std::cerr << "ERROR: cannot write to NULL stream!\n";
      return;
    }

    const unsigned N = messages.size();

    if ( N==0 )
    {
      /* short-circuit */
      return;
    }

    for ( unsigned i=0 ; i < N ; ++i )
    {
      (*stream) << messages[ i ];
    } // END for all messages

    messages.clear();
  }

};

//------------------------------------------------------------------------------
SynchronizedStream::SynchronizedStream(std::ostream* stream, MPI_Comm comm) :
  m_comm( comm ),
  m_cache( new MessageCache() ),
  m_stream( stream )
{}

//------------------------------------------------------------------------------
SynchronizedStream::SynchronizedStream( std::ostream* stream,
                                        MPI_Comm comm,
                                        const std::string& format ) :
  m_comm( comm ),
  m_cache( new MessageCache ),
  m_stream( stream )
{
  this->setFormatString( format );
}

//------------------------------------------------------------------------------
SynchronizedStream::~SynchronizedStream()
{
  delete m_cache;
  m_cache = static_cast< MessageCache* >( AXOM_NULLPTR );
}

//------------------------------------------------------------------------------
void SynchronizedStream::append( message::Level msgLevel,
                                 const std::string& message,
                                 const std::string& tagName,
                                 const std::string& fileName,
                                 int line,
                                 bool AXOM_NOT_USED(filter_duplicates) )
{

  if ( m_cache == AXOM_NULLPTR )
  {
    std::cerr << "ERROR: NULL cache!\n";
    return;
  }

  int rank   = -1;
  MPI_Comm_rank( m_comm, &rank );

  // STEP 1: cache formatted message
  m_cache->messages.push_back(
    this->getFormatedMessage(message::getLevelAsString( msgLevel ),
                             message, tagName,
                             axom::utilities::string::intToString(rank),
                             fileName, line) );
}

//------------------------------------------------------------------------------
void SynchronizedStream::flush()
{
  if ( m_cache == AXOM_NULLPTR )
  {
    std::cerr << "ERROR: NULL cache!\n";
    return;
  }

  if ( m_comm == MPI_COMM_NULL )
  {
    std::cerr << "ERROR: NULL communicator!\n";
    return;
  }

  int rank   = -1;
  int nranks =  0;
  MPI_Comm_rank( m_comm, &rank   );
  MPI_Comm_size( m_comm, &nranks );

  const int prevrank = rank-1;
  const int nextrank = rank+1;

  MPI_Request null_request = MPI_REQUEST_NULL;

  if ( rank > 0 )
  {
    // wait for signal from previous rank
    MPI_Recv(AXOM_NULLPTR,0,MPI_INT,prevrank,MPI_ANY_TAG,m_comm,
             MPI_STATUSES_IGNORE);
  }

  // print messages for this rank
  m_cache->printMessages( m_stream );

  // signal next rank
  if( nranks > 1 && nextrank < nranks )
  {
    MPI_Isend(AXOM_NULLPTR,0,MPI_INT,nextrank,0,m_comm,&null_request);
    MPI_Request_free(&null_request);
  }
}

} /* namespace slic */
} /* namespace axom */
