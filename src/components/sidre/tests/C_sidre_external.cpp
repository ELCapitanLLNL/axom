/*
 * Copyright (c) 2015, Lawrence Livermore National Security, LLC.
 * Produced at the Lawrence Livermore National Laboratory.
 *
 * All rights reserved.
 *
 * This source code cannot be distributed without permission and
 * further review from Lawrence Livermore National Laboratory.
 */

#include "gtest/gtest.h"

#include "sidre/sidre.h"

#include <stdlib.h>

//------------------------------------------------------------------------------
// Test Group::create_external_view()
//------------------------------------------------------------------------------
TEST(C_sidre_external, create_external_view)
{
  SIDRE_datastore * ds = SIDRE_datastore_new();
  SIDRE_group * root = SIDRE_datastore_get_root(ds);

  const int len = 11;

  int * idata = (int *) malloc(sizeof(int) * len);
  double * ddata = (double *) malloc(sizeof(double) * len);

  for (int ii = 0 ; ii < len ; ++ii)
  {
    idata[ii] = ii;
    ddata[ii] = idata[ii] * 2.0;
  }

  SIDRE_dataview * iview =
    SIDRE_group_create_view_external(root, "idata", idata);
  SIDRE_dataview_apply_type_nelems(iview, SIDRE_INT_ID, len);
  SIDRE_dataview * dview =
    SIDRE_group_create_view_external(root, "ddata", ddata);
  SIDRE_dataview_apply_type_nelems(dview, SIDRE_DOUBLE_ID, len);
  EXPECT_EQ(SIDRE_group_get_num_views(root), 2u);

  SIDRE_dataview_print(iview);
  SIDRE_dataview_print(dview);

  int * idata_chk = (int *) SIDRE_dataview_get_void_ptr(iview);
  for (int ii = 0 ; ii < len ; ++ii)
  {
    EXPECT_EQ(idata_chk[ii], idata[ii]);
  }

  double * ddata_chk = (double *) SIDRE_dataview_get_void_ptr(dview);
  for (int ii = 0 ; ii < len ; ++ii)
  {
    EXPECT_EQ(ddata_chk[ii], ddata[ii]);
  }

  SIDRE_datastore_delete(ds);
  free(idata);
  free(ddata);
}

#if 0
//------------------------------------------------------------------------------
// Test Group::save(), Group::load() with external buffers
//------------------------------------------------------------------------------
TEST(C_sidre_external, save_load_external_view)
{
  SIDRE_datastore * ds = SIDRE_datastore_new();
  SIDRE_group * root = SIDRE_datastore_get_root(ds);

  const int len = 11;

  int * idata = (int *) malloc(sizeof(int) * len);
  double * ddata = (double *) malloc(sizeof(double) * len);

  for (int ii = 0 ; ii < len ; ++ii)
  {
    idata[ii] = ii;
    ddata[ii] = idata[ii] * 2.0;
  }

  SIDRE_dataview * iview =
    SIDRE_group_create_view_external(root, "idata", idata);
  SIDRE_dataview_apply_type_nelems(iview, SIDRE_INT_ID, len);
  SIDRE_dataview * dview =
    SIDRE_group_create_view_external(root, "ddata", ddata);
  SIDRE_dataview_apply_type_nelems(dview, SIDRE_DOUBLE_ID, len);

  EXPECT_EQ(SIDRE_group_get_num_views(root), 2u);

  SIDRE_dataview_print(iview);
  SIDRE_dataview_print(dview);

  SIDRE_group_save(root, "out_sidre_external_save_restore_external_view",
                       "conduit");

  SIDRE_datastore_print(ds);


  SIDRE_datastore * ds2 = SIDRE_datastore_new();
  SIDRE_group * root2 = SIDRE_datastore_get_root(ds);

  SIDRE_group_load(root, "out_sidre_external_save_restore_external_view",
                       "conduit");

  SIDRE_datastore_print(ds2);

  SIDRE_dataview * iview2 = SIDRE_group_get_view_from_name(root2, "idata");
  SIDRE_dataview * dview2 = SIDRE_group_get_view_from_name(root2, "ddata");

  EXPECT_EQ(SIDRE_group_get_num_views(root2), 2u);

  int * idata_chk = (int *) SIDRE_dataview_get_void_ptr(iview2);
  for (int ii = 0 ; ii < len ; ++ii)
  {
    EXPECT_EQ(idata_chk[ii], idata[ii]);
  }

  double * ddata_chk = (double *) SIDRE_dataview_get_void_ptr(dview2);
  for (int ii = 0 ; ii < len ; ++ii)
  {
    EXPECT_EQ(ddata_chk[ii], ddata[ii]);
  }

  SIDRE_datastore_delete(ds);
  SIDRE_datastore_delete(ds2);
  free(idata);
  free(ddata);
}

#endif
