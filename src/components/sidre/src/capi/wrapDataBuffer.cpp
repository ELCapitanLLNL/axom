//
// Copyright (c) 2015, Lawrence Livermore National Security, LLC.
// Produced at the Lawrence Livermore National Laboratory.
//
// All rights reserved.
//
// This source code cannot be distributed without permission and
// further review from Lawrence Livermore National Laboratory.
//
// wrapDataBuffer.cpp
#define EXAMPLE_WRAPPER_IMPL
#include "wrapDataBuffer.h"
#include "sidre/DataBuffer.hpp"

extern "C" {
namespace asctoolkit {
namespace sidre {

int ATK_databuffer_get_uid(ATK_databuffer * self)
{
DataBuffer *selfobj = static_cast<DataBuffer *>(self);
// splicer begin
int rv = selfobj->getUID();
return rv;
// splicer end
}

ATK_databuffer * ATK_databuffer_declare(ATK_databuffer * self, ATK_TypeEnum type, long len)
{
DataBuffer *selfobj = static_cast<DataBuffer *>(self);
// splicer begin
DataBuffer * rv = selfobj->declare(static_cast<asctoolkit::sidre::TypeEnum>(type), len);
return rv;
// splicer end
}

ATK_databuffer * ATK_databuffer_allocate(ATK_databuffer * self)
{
DataBuffer *selfobj = static_cast<DataBuffer *>(self);
// splicer begin
DataBuffer * rv = selfobj->allocate();
return rv;
// splicer end
}

void * ATK_databuffer_get_data(ATK_databuffer * self)
{
DataBuffer *selfobj = static_cast<DataBuffer *>(self);
// splicer begin
void * rv = selfobj->getData();
return rv;
// splicer end
}

}  // namespace asctoolkit
}  // namespace sidre
}  // extern "C"
