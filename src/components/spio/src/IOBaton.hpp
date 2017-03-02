/*
 * Copyright (c) 2015, Lawrence Livermore National Security, LLC.
 * Produced at the Lawrence Livermore National Laboratory.
 *
 * All rights reserved.
 *
 * This source code cannot be distributed without permission and
 * further review from Lawrence Livermore National Laboratory.
 */

/*!
 ******************************************************************************
 *
 * \file
 *
 * \brief   Header file containing definition of IOBaton class.
 *
 ******************************************************************************
 */

#ifndef IOBATON_HPP_
#define IOBATON_HPP_

#include "mpi.h"

// Other CS Toolkit headers
#include "common/ATKMacros.hpp"
#include "common/CommonTypes.hpp"


namespace axom
{
namespace spio
{

/*!
 * \class IOBaton
 *
 * \brief IOBaton ensures that during I/O operations, only one rank will
 * interact with a particular file at one time.
 *
 * Each rank is placed into a group of ranks, with the number of groups being
 * equal to the number of I/O files, and then the ranks use the wait and
 * pass methods to pass control of I/O operations from one rank to the next
 * within the group.
 */
class IOBaton
{
public:

  /*!
   * \brief Constructor
   *
   * \param com        MPI communicator
   * \param num_files  Number of files involved in an I/O operation
   */
  IOBaton(MPI_Comm com,
          int num_files);

  /*!
   * \brief Destructor
   */
  ~IOBaton();

  /*!
   * \brief Wait for previous rank to pass control to the local rank.
   *
   * \return An integer id for the group of which this rank is a member.
   */
  int wait();

  /*!
   * \brief Pass control to the next rank.
   *
   * \return  0 if sucessful, -1 if not.
   */
  int pass(); 

  /*!
   * \brief Size of local rank's group.
   *
   * \return Number of ranks in the group.
   */
  int groupSize() const
  {
    return m_my_rank < m_first_regular_group_rank ? m_group_size + 1 : m_group_size; 
  }

  /*!
   * \brief Tells if the local rank is the first (lowest) in its group.
   */
  bool isFirstInGroup() const
  {
     return (m_rank_within_group == 0); 
  }

  /*!
   * \brief Tells if the local rank is the last (highest) in its group.
   */
  bool isLastInGroup() const
  {
     return (m_rank_after_me == s_invalid_rank_id); 
  }

  /*!
   * \brief Get the number of files involved in the I/O operation.
   */
  int getNumFiles() const
  {
     return m_num_files;
  }

private:

  DISABLE_COPY_AND_ASSIGNMENT( IOBaton );

  static const int s_invalid_rank_id;

  MPI_Comm m_mpi_comm;

  int m_comm_size;  // num procs in the MPI communicator
  int m_my_rank;    // rank of this proc
  int m_num_files; // number of groups (files)
  int m_num_larger_groups;  // some group have one extra
  int m_group_size; // regular group size (m_comm_size / m_num_files) w/o remainder
  int m_group_id;
  int m_first_regular_group_rank;
  int m_rank_within_group;
  int m_rank_before_me;
  int m_rank_after_me;

  int m_mpi_tag;
};


} /* end namespace spio */
} /* end namespace axom */

#endif /* IOBATON_HPP_ */
