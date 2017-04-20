// wrapSLIC.h
// This is generated code, do not edit
//
// Copyright (c) 2015, Lawrence Livermore National Security, LLC.
// Produced at the Lawrence Livermore National Laboratory.
//
// All rights reserved.
//
// This source code cannot be distributed without permission and
// further review from Lawrence Livermore National Laboratory.
//
/**
 * \file wrapSLIC.h
 * \brief Shroud generated wrapper for SLIC library
 */
// For C users and C++ implementation

#ifndef WRAPSLIC_H
#define WRAPSLIC_H

#ifdef __cplusplus
extern "C" {
#endif

// declaration of wrapped types

// splicer begin C_definition
// splicer end C_definition

void SLIC_initialize();

bool SLIC_is_initialized();

void SLIC_finalize();

void SLIC_create_logger(const char * name, char imask);

void SLIC_create_logger_bufferify(const char * name, int Lname, char imask);

bool SLIC_activate_logger(const char * name);

bool SLIC_activate_logger_bufferify(const char * name, int Lname);

void SLIC_get_active_logger_name_bufferify(char * name, int Nname);

void SLIC_set_logging_msg_level(int level);

void SLIC_set_abort_on_error(bool status);

void SLIC_enable_abort_on_error();

void SLIC_disable_abort_on_error();

bool SLIC_is_abort_on_errors_enabled();

void SLIC_set_abort_on_warning(bool status);

void SLIC_enable_abort_on_warning();

void SLIC_disable_abort_on_warning();

bool SLIC_is_abort_on_warnings_enabled();

void SLIC_log_message(int level, const char * message, const char * fileName,
                      int line, bool filter);

void SLIC_log_message_bufferify(int level, const char * message, int Lmessage,
                                const char * fileName, int LfileName, int line,
                                bool filter);

#ifdef __cplusplus
}
#endif

#endif  // WRAPSLIC_H
