// wrapBuffer.h
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
 * \file wrapBuffer.h
 * \brief Shroud generated wrapper for Buffer class
 */
// For C users and C++ implementation

#ifndef WRAPBUFFER_H
#define WRAPBUFFER_H

#include "sidre/SidreTypes.h"
#include "stdlib.h"

// splicer begin class.Buffer.CXX_declarations
// splicer end class.Buffer.CXX_declarations

#ifdef __cplusplus
extern "C" {
#endif

// declaration of wrapped types
struct s_SIDRE_buffer;
typedef struct s_SIDRE_buffer SIDRE_buffer;

// splicer begin class.Buffer.C_declarations
// splicer end class.Buffer.C_declarations

SIDRE_IndexType SIDRE_buffer_get_index(const SIDRE_buffer * self);

size_t SIDRE_buffer_get_num_views(const SIDRE_buffer * self);

void SIDRE_buffer_describe(SIDRE_buffer * self, int type,
                           SIDRE_SidreLength num_elems);

void SIDRE_buffer_allocate_existing(SIDRE_buffer * self);

void SIDRE_buffer_allocate_from_type(SIDRE_buffer * self, int type,
                                     SIDRE_SidreLength num_elems);

void SIDRE_buffer_reallocate(SIDRE_buffer * self, SIDRE_SidreLength num_elems);

void * SIDRE_buffer_get_void_ptr(SIDRE_buffer * self);

int SIDRE_buffer_get_type_id(const SIDRE_buffer * self);

size_t SIDRE_buffer_get_num_elements(const SIDRE_buffer * self);

size_t SIDRE_buffer_get_total_bytes(const SIDRE_buffer * self);

size_t SIDRE_buffer_get_bytes_per_element(const SIDRE_buffer * self);

void SIDRE_buffer_print(const SIDRE_buffer * self);

#ifdef __cplusplus
}
#endif

#endif  // WRAPBUFFER_H
