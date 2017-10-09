/*
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Copyright (c) 2017, Lawrence Livermore National Security, LLC.
 *
 * Produced at the Lawrence Livermore National Laboratory
 *
 * LLNL-CODE-xxxxxxx
 *
 * All rights reserved.
 *
 * This file is part of Axom.
 *
 * For details about use and distribution, please read axom/LICENSE.
 *
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

/*!
 *******************************************************************************
 * \file Communicator.hpp
 * \author Chris White (white238@llnl.gov)
 *
 * \brief This file contains the pure virtual base class definition of the 
 * Communicator.
 *******************************************************************************
 */

#ifndef COMMUNICATOR_HPP
#define COMMUNICATOR_HPP

#include <vector>

#include "lumberjack/Combiner.hpp"
#include "lumberjack/Message.hpp"

namespace axom {
namespace lumberjack {

/*!
 *******************************************************************************
 * \class Communicator
 *
 * \brief Abstract base class defining the interface of all Communicator classes.
 *
 *  Concrete instances need to inherit from this class and implement these functions.
 *  You will need to add your Communicator using Lumberjack::initialize
 *
 * \see BinaryTreeCommunicator RootCommunicator Lumberjack
 *******************************************************************************
 */
class Communicator {
    public:
        /*!
         *****************************************************************************
         * \brief Virtual destructor.
         *****************************************************************************
         */
        virtual ~Communicator(){};
        
        /*!
         *****************************************************************************
         * \brief Called to initialize the Communicator.
         *
         * This performs any setup work the Communicator needs before doing any work.
         * It is required that this is called before using the Communicator.
         *
         * \param [in] comm The MPI communicator
         * \param [in] ranksLimit Limit on how many ranks are individually tracked per Message.
         *****************************************************************************
         */
        virtual void initialize(MPI_Comm comm, int ranksLimit) = 0;

        /*!
         *****************************************************************************
         * \brief Called to finalize the Communicator.
         *
         * This performs any cleanup work the Communicator needs to do before going away.
         * It is required that this is the last function called by the Communicator.
         *****************************************************************************
         */
        virtual void finalize() = 0;

        /*!
         *****************************************************************************
         * \brief Returns the MPI rank of this node
         *****************************************************************************
         */
        virtual int rank() = 0;

        /*!
         *****************************************************************************
         * \brief Sets the rank limit.
         *
         * This is the limit on how many ranks generated a given message are individually tracked
         * per Message.  After the limit has been reached, only the Message::rankCount is 
         * incremented.
         *
         * \param [in] value Limit on how many ranks are individually tracked per Message.
         *****************************************************************************
         */
        virtual void ranksLimit(int value) = 0;

        /*!
         *****************************************************************************
         * \brief Returns the rank limit.
         *
         * This is the limit on how many ranks generated a given message are individually tracked
         * per Message.  After the limit has been reached, only the Message::rankCount is 
         * incremented.
         *****************************************************************************
         */
        virtual int ranksLimit() = 0;

        /*!
         *****************************************************************************
         * \brief Function used by the Lumberjack class to indicate how many individual pushes
         *  fully flush all currently held Message classes to the root node. The Communicator
         *  class's tree structure dictates this.
         *****************************************************************************
         */
        virtual int numPushesToFlush() = 0;

        /*!
         *****************************************************************************
         * \brief This pushes all messages once up the Communicator class's tree structure.
         *
         * All of the children push their Message classes to their parent node. This is
         * is helpful if you want to spread the work load of Lumberjack over a large
         * set of work.
         *
         * \param [in] packedMessagesToBeSent All of this rank's Message classes packed into a single buffer.
         * \param [in,out] receivedPackedMessages Recieved packed message buffers from this nodes children.
         *****************************************************************************
         */
        virtual void push(const char* packedMessagesToBeSent,
                          std::vector<const char*>& receivedPackedMessages) = 0;

        /*!
         *****************************************************************************
         * \brief Function indicates whether this node should be outputting messages.
         * The Communicator class's communication structure dictates this.
         *
         * \return Boolean indicates whether you should output messages
         *****************************************************************************
         */
        virtual bool isOutputNode() = 0;
};

} // end namespace lumberjack
} // end namespace axom

#endif
