/*
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Copyright (c) 2017, Lawrence Livermore National Security, LLC.
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

#include "gtest/gtest.h"

#include "conduit_relay.hpp"
#include "conduit_relay_hdf5.hpp"

#include "sidre/sidre.hpp"
#include "spio/IOManager.hpp"

using axom::sidre::Group;
using axom::sidre::DataStore;
using axom::sidre::DataType;
using axom::sidre::View;
using axom::spio::IOManager;

//------------------------------------------------------------------------------

TEST(spio_parallel, parallel_writeread)
{
  int my_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  int num_ranks;
  MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);

  int num_output = num_ranks / 2;
  if (num_output == 0) {
    num_output = 1;
  }

  /*
   * Create a DataStore and give it a small hierarchy of groups and views.
   *
   * The views are filled with repeatable nonsense data that will vary based
   * on rank.
   */
  DataStore * ds = new DataStore();

  Group * root = ds->getRoot();

  Group * flds = root->createGroup("fields");
  Group * flds2 = root->createGroup("fields2");

  Group * ga = flds->createGroup("a");
  Group * gb = flds2->createGroup("b");
  ga->createViewScalar<int>("i0", 101*my_rank);
  gb->createView("i1")->allocate(DataType::c_int(10));
  int * i1_vals = gb->getView("i1")->getData();

  for(int i=0 ; i<10 ; i++)
  {
    i1_vals[i] = (i+10) * (404-my_rank-i);
  }

  /*
   * Contents of the DataStore written to files with IOManager.
   */
  int num_files = num_output;
  IOManager writer(MPI_COMM_WORLD);

  writer.write(root, num_files, "out_spio_parallel_write_read", "sidre_hdf5");

  std::string root_name = "out_spio_parallel_write_read.root";

  /*
   * Extra stuff to exercise writeGroupToRootFile
   */
  MPI_Barrier(MPI_COMM_WORLD);
  if (my_rank == 0) {
    DataStore * dsextra = new DataStore();
    Group * extra = dsextra->getRoot()->createGroup("extra");
    extra->createViewScalar<double>("dval", 1.1);
    Group * child = extra->createGroup("child");
    child->createViewScalar<int>("ival", 7);
    child->createViewString("word0", "hello");
    child->createViewString("word1", "world");

    writer.writeGroupToRootFile(extra, root_name);

    Group * path_test = dsextra->getRoot()->createGroup("path_test");

    path_test->createViewScalar<int>("path_val", 9);
    path_test->createViewString("word2", "again");

    writer.writeGroupToRootFileAtPath(path_test, root_name, "extra/child");

    View * view_test = dsextra->getRoot()->createViewString("word3", "new_view");

    writer.writeViewToRootFileAtPath(view_test,
                                     root_name,
                                     "extra/child/path_test");

    delete dsextra;
  }
  MPI_Barrier(MPI_COMM_WORLD);

  /*
   * Read the root file on rank 1, unless this is a serial run.
   */
  if (my_rank == 1 || num_ranks == 1) {

    conduit::Node n;
    conduit::relay::io::load(root_name + ":extra", "hdf5", n);

    double dval = n["dval"].to_double();

    EXPECT_TRUE(dval > 1.0000009 && dval < 1.1000001);

    EXPECT_EQ(n["child"]["ival"].to_int(), 7);
    EXPECT_EQ(n["child"]["word0"].as_string(), "hello");
    EXPECT_EQ(n["child"]["word1"].as_string(), "world");
    EXPECT_EQ(n["child"]["path_test"]["path_val"].to_int(), 9);
    EXPECT_EQ(n["child"]["path_test"]["word2"].as_string(), "again");
    EXPECT_EQ(n["child"]["path_test"]["word3"].as_string(), "new_view");

  }

  /*
   * Create another DataStore that holds nothing but the root group.
   */
  DataStore * ds2 = new DataStore();

  /*
   * Read from the files that were written above.
   */
  IOManager reader(MPI_COMM_WORLD);


  reader.read(ds2->getRoot(), root_name);


  /*
   * Verify that the contents of ds2 match those written from ds.
   */
  EXPECT_TRUE(ds2->getRoot()->isEquivalentTo(root));

  int testvalue =
    ds->getRoot()->getGroup("fields")->getGroup("a")->getView("i0")->getData();
  int testvalue2 =
    ds2->getRoot()->getGroup("fields")->getGroup("a")->getView("i0")->getData();

  EXPECT_EQ(testvalue, testvalue2);

  View * view_i1_orig =
    ds->getRoot()->getGroup("fields2")->getGroup("b")->getView("i1");
  View * view_i1_restored =
    ds2->getRoot()->getGroup("fields2")->getGroup("b")->getView("i1");

  int num_elems = view_i1_orig->getNumElements();
  EXPECT_EQ(view_i1_restored->getNumElements(), num_elems);
  if (view_i1_restored->getNumElements() == num_elems)
  {
    int * i1_orig = view_i1_orig->getData();
    int * i1_restored = view_i1_restored->getData();

    for (int i = 0; i < num_elems; ++i) {
      EXPECT_EQ(i1_orig[i], i1_restored[i]);
    }
  }

  delete ds;
  delete ds2;

}

const std::string PROTOCOL = "sidre_hdf5";
const std::string ROOT_EXT = ".root";

//----------------------------------------------------------------------
TEST(spio_parallel, write_read_write)
{
  int my_rank, num_ranks;
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);

  int num_files = std::max( num_ranks / 2, 1);
  std::stringstream sstr;
  sstr << "out_spio_WRW_" << num_ranks;
  std::string filename = sstr.str();

  // Initialize a datastore and dump to disk
  DataStore * ds = new DataStore();
  ds->getRoot()->createViewScalar("grp/i",2);
  ds->getRoot()->createViewScalar("grp/f",3.0);
  IOManager writer_a(MPI_COMM_WORLD);
  writer_a.write(ds->getRoot(), num_files, filename, PROTOCOL);

  // Create another DataStore to read into.
  DataStore ds_r;
  IOManager reader(MPI_COMM_WORLD);
  reader.read(ds_r.getRoot(), filename + ROOT_EXT);

  // Dump this datastore to disk.
  // Regression: This used to produce the following HDF5 error:
  //  HDF5-DIAG: Error detected in HDF5 (1.8.16) thread 0:
  //    #000: H5F.c line 522 in H5Fcreate(): unable to create file
  //      major: File accessibility
  //      minor: Unable to open file
  //    #001: H5Fint.c line 1024 in H5F_open(): unable to truncate a file which is already open
  //      major: File accessibility
  //      minor: Unable to open file
  IOManager writer_b(MPI_COMM_WORLD);
  writer_b.write(ds_r.getRoot(), num_files, filename, PROTOCOL);
}

//------------------------------------------------------------------------------
TEST(spio_parallel, external_writeread)
{
  int my_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  int num_ranks;
  MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);

  int num_output = num_ranks / 2;
  if (num_output == 0) {
    num_output = 1;
  }

  const int nvals = 10;
  int orig_vals1[nvals], orig_vals2[nvals];
  for(int i=0 ; i<10 ; i++)
  {
    orig_vals1[i] = (i+10) * (404-my_rank-i);
    orig_vals2[i] = (i+10) * (404-my_rank-i) + 20;
  }

  /*
   * Create a DataStore and give it a small hierarchy of groups and views.
   *
   * The views are filled with repeatable nonsense data that will vary based
   * on rank.
   */
  DataStore * ds1 = new DataStore();

  Group * root1 = ds1->getRoot();

  Group * flds = root1->createGroup("fields");
  Group * flds2 = root1->createGroup("fields2");

  Group * ga = flds->createGroup("a");
  Group * gb = flds2->createGroup("b");
  ga->createView("external_array", axom::sidre::INT_ID, nvals, orig_vals1);
  gb->createView("external_undescribed")->setExternalDataPtr(orig_vals2);

  /*
   * Contents of the DataStore written to files with IOManager.
   */
  int num_files = num_output;
  IOManager writer(MPI_COMM_WORLD);

  writer.write(root1, num_files, "out_spio_external_write_read", "sidre_hdf5");

  /*
   * Create another DataStore than holds nothing but the root group.
   */
  DataStore * ds2 = new DataStore();
  Group * root2 = ds2->getRoot();

  /*
   * Read from the files that were written above.
   */
  IOManager reader(MPI_COMM_WORLD);

  reader.read(root2, "out_spio_external_write_read.root");

  int restored_vals1[nvals], restored_vals2[nvals];
  for (int i = 0; i < nvals; ++i) {
    restored_vals1[i] = -1;
    restored_vals2[i] = -1;
  }

  View * view1 = root2->getView("fields/a/external_array");
  view1->setExternalDataPtr(restored_vals1);

  View * view2 = root2->getView("fields2/b/external_undescribed");
  view2->setExternalDataPtr(restored_vals2);

  reader.loadExternalData(root2, "out_spio_external_write_read.root");

  enum SpioTestResult
  {
    SPIO_TEST_SUCCESS = 0,
    HIERARCHY_ERROR   = 1<<0,
    EXT_ARRAY_ERROR   = 1<<1,
    EXT_UNDESC_ERROR  = 1<<2
  };
  int result = SPIO_TEST_SUCCESS;

  /*
   * Verify that the contents of ds2 match those written from ds.
   */
  EXPECT_TRUE(ds2->getRoot()->isEquivalentTo(root1));
  if (!ds2->getRoot()->isEquivalentTo(root1)) {
    result |= HIERARCHY_ERROR;
  }
  SLIC_WARNING_IF( result & HIERARCHY_ERROR, "Tree layouts don't match");

  EXPECT_EQ(view1->getNumElements(), nvals);
  if (view1->getNumElements() != nvals) {
    result |= EXT_ARRAY_ERROR;
  }
  else {
    for (int i = 0; i < nvals; ++i) {
      EXPECT_EQ(orig_vals1[i], restored_vals1[i]);
      if (orig_vals1[i] != restored_vals1[i]) {
        result |= EXT_ARRAY_ERROR;
        break;
      }
    }
  }
  SLIC_WARNING_IF( result & EXT_ARRAY_ERROR, "External_array was not correctly loaded");

  /*
   * external_undescribed was not written to disk (since it is undescribed)
   * make sure it was not read in.
   */
  for (int i = 0; i < nvals; ++i) {
    EXPECT_EQ(-1, restored_vals2[i]);
    if (-1 != restored_vals2[i]) {
      result |= EXT_UNDESC_ERROR;
      break;
    }
  }
  SLIC_WARNING_IF( result & EXT_UNDESC_ERROR, "External_undescribed data was modified.");

  delete ds1;
  delete ds2;

}

#include "slic/UnitTestLogger.hpp"
using axom::slic::UnitTestLogger;

//----------------------------------------------------------------------
TEST(spio_paralle, irregular_writeread)
{
  int my_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  int num_ranks;
  MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);

  int num_output = num_ranks / 2; 
  if (num_output == 0) {
    num_output = 1;
  }

  /*
   * Create a DataStore and give it a small hierarchy of groups and views.
   *
   * The views are filled with repeatable nonsense data that will vary based
   * on rank.
   */
  DataStore * ds1 = new DataStore();

  Group * root1 = ds1->getRoot();

  int num_fields = my_rank + 2;

  for (int f = 0; f < num_fields; ++f) {
    std::ostringstream ostream;
    ostream << "fields" << f;
    Group * flds = root1->createGroup(ostream.str());

    int num_subgroups = ((f+my_rank)%3) + 1;
    for (int g = 0; g < num_subgroups; ++g) {
      std::ostringstream gstream;
      gstream << "subgroup" << g;
      Group * sg = flds->createGroup(gstream.str());

      std::ostringstream vstream;
      vstream << "view" << g;
      if (g % 2) {
        sg->createView(vstream.str())->allocate(DataType::c_int(10+my_rank));
        int * vals = sg->getView(vstream.str())->getData();

        for(int i=0 ; i<10+my_rank ; i++)
        {
          vals[i] = (i+10) * (404-my_rank-i-g-f);
        }

      } else {
        sg->createViewScalar<int>(vstream.str(), 101*my_rank*(f+g+1));
      } 
    } 
  } 


  /*
   * Contents of the DataStore written to files with IOManager.
   */
  int num_files = num_output;
  IOManager writer(MPI_COMM_WORLD);

  writer.write(root1, num_files, "out_spio_irregular_write_read", "sidre_hdf5");

  /*
   * Create another DataStore that holds nothing but the root group.
   */
  DataStore * ds2 = new DataStore();

  /*
   * Read from the files that were written above.
   */
  IOManager reader(MPI_COMM_WORLD);

  reader.read(ds2->getRoot(), "out_spio_irregular_write_read.root");


  /*
   * Verify that the contents of ds2 match those written from ds.
   */
  EXPECT_TRUE(ds2->getRoot()->isEquivalentTo(root1));

  for (int f = 0; f < num_fields; ++f) {
    std::ostringstream ostream;
    ostream << "fields" << f;
    Group * flds1 = ds1->getRoot()->getGroup(ostream.str());
    Group * flds2 = ds2->getRoot()->getGroup(ostream.str());

    int num_subgroups = ((f+my_rank)%3) + 1;
    for (int g = 0; g < num_subgroups; ++g) {
      std::ostringstream gstream;
      gstream << "subgroup" << g;
      Group * sg1 = flds1->getGroup(gstream.str());
      Group * sg2 = flds2->getGroup(gstream.str());

      std::ostringstream vstream;
      vstream << "view" << g;
      if (g % 2) {

        View * view_orig = sg1->getView(vstream.str());
        View * view_restored = sg2->getView(vstream.str());

        int num_elems = view_orig->getNumElements();
        EXPECT_EQ(view_restored->getNumElements(), num_elems);
        int * vals_orig = view_orig->getData();
        int * vals_restored = view_restored->getData();

        for (int i = 0; i < num_elems; ++i) {
          EXPECT_EQ(vals_orig[i], vals_restored[i]);
        }
      } else {
        int testvalue1 = sg1->getView(vstream.str())->getData();
        int testvalue2 = sg2->getView(vstream.str())->getData();

        EXPECT_EQ(testvalue1, testvalue2);
      }
    }
  } 

  delete ds1;
  delete ds2;
}

//------------------------------------------------------------------------------
TEST(spio_parallel, preserve_writeread)
{
  int my_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  int num_ranks;
  MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);

  int num_output = num_ranks / 2; 
  if (num_output == 0) {
    num_output = 1;
  }

  /*
   * Create a DataStore and give it a small hierarchy of groups and views.
   *
   * The views are filled with repeatable nonsense data that will vary based
   * on rank.
   */
  DataStore * ds = new DataStore();

  Group * root = ds->getRoot();

  Group * flds = root->createGroup("fields");
  Group * flds2 = root->createGroup("fields2");

  Group * ga = flds->createGroup("a");
  Group * gb = flds2->createGroup("b");
  ga->createViewScalar<int>("i0", 101*my_rank);
  gb->createView("i1")->allocate(DataType::c_int(10));
  int * i1_vals = gb->getView("i1")->getData();

  for(int i=0 ; i<10 ; i++)
  {
    i1_vals[i] = (i+10) * (404-my_rank-i);
  }

  /*
   * Contents of the DataStore written to files with IOManager.
   */
  int num_files = num_output;
  IOManager writer(MPI_COMM_WORLD);

  writer.write(root, num_files, "out_spio_preserve_write_read", "sidre_hdf5");

  std::string root_name = "out_spio_preserve_write_read.root";

  /*
   * Extra stuff to exercise preserve_contents option
   */
  MPI_Barrier(MPI_COMM_WORLD);
  DataStore * dsextra = new DataStore();
  Group * extra = dsextra->getRoot()->createGroup("extra");
  extra->createViewScalar<double>("dval", 1.1);
  Group * child = extra->createGroup("child");
  child->createViewScalar<int>("ival", 7);
  child->createViewString("word0", "hello");
  child->createViewString("word1", "world");

  writer.write(extra, num_files, "out_spio_extra", "sidre_hdf5");
  std::string extra_root = "out_spio_extra.root";
 
  MPI_Barrier(MPI_COMM_WORLD);

  /*
   * Create another DataStore that holds nothing but the root group.
   */
  DataStore * ds2 = new DataStore();

  /*
   * Read from the files that were written above.
   */
  IOManager reader(MPI_COMM_WORLD);

  reader.read(ds2->getRoot(), root_name);

  /*
   * Verify that the contents of ds2 match those written from ds.
   */
  EXPECT_TRUE(ds2->getRoot()->isEquivalentTo(root));

  int testvalue =
    ds->getRoot()->getGroup("fields")->getGroup("a")->getView("i0")->getData();
  int testvalue2 =
    ds2->getRoot()->getGroup("fields")->getGroup("a")->getView("i0")->getData();

  EXPECT_EQ(testvalue, testvalue2);

  View * view_i1_orig =
    ds->getRoot()->getGroup("fields2")->getGroup("b")->getView("i1");
  View * view_i1_restored =
    ds2->getRoot()->getGroup("fields2")->getGroup("b")->getView("i1");

  int num_elems = view_i1_orig->getNumElements();
  EXPECT_EQ(view_i1_restored->getNumElements(), num_elems);

  int * i1_orig = view_i1_orig->getData();
  int * i1_restored = view_i1_restored->getData();

  for (int i = 0; i < num_elems; ++i) {
    EXPECT_EQ(i1_orig[i], i1_restored[i]);
  }

  /*
   * Read in extra file while preserving contents
   */
  Group * extra_fields = ds2->getRoot()->getGroup("fields");
  reader.read(extra_fields, extra_root, true);

  /*
   * Test one of the pre-existing Views to show it's unchanged.
   */
  int testvalue_extra = extra_fields->getGroup("a")->getView("i0")->getData();
  EXPECT_EQ(testvalue_extra, testvalue2);

  /*
   * Test the data from the extra file.
   */
  double dval = extra_fields->getView("dval")->getData();

  EXPECT_TRUE(dval > 1.0000009 && dval < 1.1000001);

  int ival = extra_fields->getView("child/ival")->getData();
  EXPECT_EQ(ival, 7);

  EXPECT_EQ(std::string(extra_fields->getView("child/word0")->getString()), "hello");

  EXPECT_EQ(std::string(extra_fields->getView("child/word1")->getString()), "world");

  delete ds;
  delete ds2;
  delete dsextra;
}

//----------------------------------------------------------------------
int main(int argc, char * argv[])
{
  int result = 0;

  ::testing::InitGoogleTest(&argc, argv);

  UnitTestLogger logger;  // create & initialize test logger,

  MPI_Init(&argc, &argv);
  result = RUN_ALL_TESTS();
  MPI_Finalize();

  return result;
}

