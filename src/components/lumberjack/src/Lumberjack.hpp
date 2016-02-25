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
 *******************************************************************************
 * \file Lumberjack.hpp
 * \author Chris White (white238@llnl.gov)
 *
 * \brief This file contains the class definition of the Lumberjack. This class
 * is the main class users will interact with.
 *******************************************************************************
 */

#ifndef LUMBERJACK_HPP
#define LUMBERJACK_HPP

#include <string>

#include "mpi.h"

#include "lumberjack/Combiner.hpp"
#include "lumberjack/Communicator.hpp"
#include "lumberjack/Message.hpp"
#include "lumberjack/TextEqualityCombiner.hpp"

namespace asctoolkit {
namespace lumberjack {

/*!
 *******************************************************************************
 * \class Lumberjack
 *
 * \brief Class that all user interactions with Lumberjack are through.
 *
 *  This class performs all the high level functionality of Lumberjack, such as,
 *  holding and combining Message classes and telling the given Communicator
 *  to push Message classes.
 *
 * \see BinaryTreeCommunicator RootCommunicator Message Combiner
 *******************************************************************************
 */
class Lumberjack {
    public:
        /*!
         *****************************************************************************
         * \brief Called to initialize the Lumberjack.
         *
         * This performs any setup work the Lumberjack needs before doing any work.
         * It is required that this is called before using the Lumberjack.
         *
         * \param [in] communicator The Lumberjack Communicator that will send/recieve messages
         * \param [in] ranksLimit Limit on how many ranks are individually tracker per Message.
         *****************************************************************************
         */
        void initialize(Communicator* communicator, int ranksLimit);

        /*!
         *****************************************************************************
         * \brief Called to finalize the Lumberjack.
         *
         * This performs any cleanup work the Lumberjack needs to do before going away.
         * It is required that this is the last function called by the Lumberjack.
         *****************************************************************************
         */
        void finalize();

        /*!
         *****************************************************************************
         * \brief Adds a Combiner to the Lumberjack.
         *
         * The Lumberjack can have multiple Combiner classes.  This is helpful when you have different
         * combining criteria for different Message classes. If you try to add the same Combiner
         * more than once, the second Combiner will not be added.  This is determined solely
         * on Combiner::id. Combiner classes that are added first have precedence.  The Lumberjack
         * will call delete on the Combiner when finalize is called.
         *
         * \param [in] combiner The Combiner that will be added.
         *****************************************************************************
         */
        void addCombiner(Combiner* combiner);

        /*!
         *****************************************************************************
         * \brief Removes a Combiner from the Lumberjack.
         *
         * This removes and calls delete on a Combiner held by the Lumberjack. If no 
         * Combiner::id matches the given identifier than nothing is removed. 
         *
         * \param [in] combinerIdentifier The Combiner identifier that will be removed.
         *****************************************************************************
         */
        void removeCombiner(const std::string& combinerIdentifier);

        /*!
         *****************************************************************************
         * \brief Clears all Combiner classes from the Lumberjack.
         *
         * This removes and calls delete on all Combiner classes held by the Lumberjack. 
         *****************************************************************************
         */
        void clearCombiners();

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
        void ranksLimit(int value);

        /*!
         *****************************************************************************
         * \brief Returns the limit on tracked ranks.
         *
         * This is the limit on how many ranks generated a given message are individually tracked
         * per Message.  After the limit has been reached, only the Message::rankCount is 
         * incremented.
         *
         * \return The limit on tracked ranks
         *****************************************************************************
         */
        int ranksLimit();

        /*!
         *****************************************************************************
         * \brief Clears all Message classes from the Lumberjack.
         *
         * This removes and calls delete on all Messages classes held by the Lumberjack. 
         *****************************************************************************
         */
        void clearMessages();

        /*!
         *****************************************************************************
         * \brief Returns a const reference vector with all currently held Message classes.
         *
         * This returns a const reference to the vector that holds the Message classes held by
         * this node. You should check isOutputNode() to indicate if you should
         * output messages depending on your communication scheme.
         *
         * \param [in,out] filledVector An empty vector that will be filled with Message classes.
         *****************************************************************************
         */
        const std::vector<Message*>& getMessages() const;

        /*!
         *****************************************************************************
         * \brief Queues a message to be sent and combined
         *
         * This creates a Message and queues it to be sent through the Communicator
         * to the root node.  This message may be combined with others depending on the
         * given criteria by the already defined Combiner classes. Depending on the behavior
         * of the Communicator, the message will not be outputted immediately.
         *
         * \param [in] text Text of the Message.
         *****************************************************************************
         */
        void queueMessage(const std::string& text);

        /*!
         *****************************************************************************
         * \brief Queues a message to be sent and combined
         *
         * This creates a Message and queues it to be sent through the Communicator
         * to the root node.  This message may be combined with others depending on the
         * given criteria by the already defined Combiner classes. Depending on the behavior
         * of the Communicator, the message will not be outputted immediately.
         *
         * \param [in] text Text of the Message
         * \param [in] fileName File name of Message
         * \param [in] lineNumber Line number of Message
         * \param [in] level The level of the severity of the Message.
         * \param [in] tag The tag of where the Message originated.
         *****************************************************************************
         */
        void queueMessage(const std::string& text, const std::string& fileName, const int lineNumber,
                          int level, const std::string& tag);

        /*!
         *****************************************************************************
         * \brief This pushes all messages once up the Communicator class's tree structure.
         *
         * All of the children push their Message classes to their parent node. This is
         * is helpful if you want to spread the work load of Lumberjack over a large
         * set of work. Before and after Message classes are pushed, they are combined.
         *****************************************************************************
         */
        void pushMessagesOnce();

        /*!
         *****************************************************************************
         * \brief This pushes all messages fully up the Communicator class's tree structure.
         *
         * All messages are continually pushed until all messages are pushed to the root node.
         * After Message classes are pushed once they are combined before being pushed again.
         *****************************************************************************
         */
        void pushMessagesFully();

        /*!
         *****************************************************************************
         * \brief Function indicates whether this node should be outputting messages.
         * The Communicator class's communication structure dictates this.
         *
         * \return Boolean indicates whether you should output messages
         *****************************************************************************
         */
        bool isOutputNode();
    private:
        /*!
         *****************************************************************************
         * \brief This packages all currently held Message classes into one const char buffer.
         *
         * The messages are packed into the following format:
         *  \<message count>*\<largest message size>*\<packed message size>*\<packed message>\<packed message size>...
         * This function does not delete messages.
         *
         * \return Packed version of all currently held messages
         *****************************************************************************
         */
        const char* packMessages();

        /*!
         *****************************************************************************
         * \brief This unpackages the given const char buffer and creates Messages classes out of them.
         *
         * The messages are packed into the following format:
         *  \<message count>*\<largest message size>*\<packed message size>*\<packed message>\<packed message size>...
         * After creating the Message classes it adds them into Lumberjack.
         *
         * \param [in] packedMessages Packed messages to be unpacked
         *****************************************************************************
         */
        void unpackMessages(const char* packedMessages);

        /*!
         *****************************************************************************
         * \brief All Message classes are combined by the currently held Combiner classes.
         *****************************************************************************
         */
        void combineMessages();

        Communicator* m_communicator;
        int m_ranksLimit;
        std::vector<Combiner*> m_combiners;
        std::vector<Message*> m_messages;
};

} // end namespace lumberjack
} // end namespace asctoolkit

#endif
