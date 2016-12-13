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
 * \brief   Header file containing definition of IOManager class.
 *
 ******************************************************************************
 */

#ifndef IOPARALLEL_HPP_
#define IOPARALLEL_HPP_

#include "mpi.h"

#include "hdf5.h"

// Other CS Toolkit headers
#include "common/ATKMacros.hpp"
#include "common/CommonTypes.hpp"
#include "sidre/DataGroup.hpp"
#include "spio/IOBaton.hpp"


namespace asctoolkit
{
namespace spio
{


/*!
 * \class IOManager
 *
 * \brief IOManager manages and organizes the I/O operations.
 *
 * This class handles the bookkeeping and organizing tasks that must be done
 * before calling DataGroup's I/O methods.  It uses IOBaton to control the
 * parallel I/O operations, such that one rank at a time interacts with any
 * particular output file.
 */
class IOManager
{
public:

  /*!
   * \brief Constructor
   *
   * \param com               MPI communicator
   */
  IOManager(MPI_Comm com);

  /*!
   * \brief Destructor
   */
  ~IOManager();

  /*!
   * \brief write a DataGroup to output files
   *
   * The DataGroup, including all of its child groups and views, is written
   * to files according to the given protocol.
   *
   * valid protocols:
   *
   *    sidre_hdf5
   *    sidre_conduit_json
   *    sidre_json
   *    conduit_hdf5
   *    conduit_bin
   *    conduit_json
   *    json
   *
   * \param group         DataGroup to write to output
   * \param num_files     number of output data files
   * \param file_string   base name for output files
   * \param protocol      identifies I/O protocol
   */
  void write(sidre::DataGroup * group,
             int num_files,
             const std::string& file_string,
             const std::string& protocol);

  /*!
   * \brief write additional group to existing root file
   *
   * Should be called after write().  The native layout of the group will
   * be added to the root file.
   *
   * This may be called more than once to write multiple groups to the file.
   *
   * This is not an MPI collective call.  It writes one group from one rank to
   * one root file.
   *
   * \param group         DataGroup to add to root file
   * \param file_name     name of existing root file
   */
  void writeGroupToRootFile(sidre::DataGroup * group,
                            const std::string& file_name);

  /*!
   * \brief write additional group to a path inside an existing root file
   *
   * This should be called on an existing root file that already contains a
   * DataGroup, most likely from a previous call to writeGroupToRootFile.
   *
   * group_path is a path that specifies a location somewhere in the native
   * layout of a DataGroup already in the file.  The group added in this
   * method will be stored as a child group of the group specified by the
   * path.
   *
   * This is not an MPI collective call.  It writes one group from one rank to
   * one root file.
   *
   * \param group         DataGroup to add to root file
   * \param file_name     name of existing root file
   * \param group_path    path to a location within the root file
   */
  void writeGroupToRootFileAtPath(sidre::DataGroup * group,
                                  const std::string& file_name,
                                  const std::string& group_path);

  /*!
   * \brief write additional group to a path inside an existing root file
   *
   * This should be called on an existing root file that already contains a
   * DataGroup, most likely from a previous call to writeGroupToRootFile.
   *
   * group_path is a path that specifies a location somewhere in the native
   * layout of a DataGroup already in the file.  The view added in this
   * method will be stored as a child view of the group specified by the
   * path.
   *
   * This is not an MPI collective call.  It writes one view from one rank to
   * one root file.
   *
   * \param view          DataView to add to root file
   * \param file_name     name of existing root file
   * \param group_path    path to a location within the root file
   */
  void writeViewToRootFileAtPath(sidre::DataView * view,
                                 const std::string& file_name,
                                 const std::string& group_path);

  /*!
   * \brief read from input files
   *
   * \param group         DataGroup to fill with input data
   * \param file_string   base name of input files
   * \param protocol      identifies I/O protocol
   */
  void read(sidre::DataGroup * group,
            const std::string& file_string,
            const std::string& protocol);

  /*!
   * \brief read from a root file
   *
   * \param group         DataGroup to fill with input data
   * \param root_file     root file containing input data
   */
  void read(sidre::DataGroup * group, const std::string& root_file);

  /*!
   * \brief load external data into a group
   *
   * \param group         DataGroup to fill with external data from input
   * \param root_file     root file containing input data
   */
  void loadExternalData(sidre::DataGroup * group,
                        const std::string& root_file);


  /*!
   * \brief gets the number of files in the dataset from the specified root file
   */ 
  int getNumFilesFromRoot(const std::string& root_file);

private:

  DISABLE_COPY_AND_ASSIGNMENT( IOManager );

  void createRootFile(const std::string& root_name,
                      const std::string& file_base,
                      int num_files);

  std::string getHDF5FilePattern(const std::string& root_name);

  std::string getHDF5FileName(const std::string& file_pattern,
                              const std::string& root_name,
                              int rankgroup_id);


  int m_comm_size;  // num procs in the MPI communicator
  int m_my_rank;    // rank of this proc

  IOBaton * m_baton;

  MPI_Comm m_mpi_comm;
};


} /* end namespace spio */
} /* end namespace asctoolkit */

#endif /* IOPARALLEL_HPP_ */
