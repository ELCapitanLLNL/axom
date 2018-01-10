!
! Copyright (c) 2017-2018, Lawrence Livermore National Security, LLC.
! 
! Produced at the Lawrence Livermore National Laboratory
!
! LLNL-CODE-741217
!
! All rights reserved.
!
! This file is part of Axom.
!
! For details about use and distribution, please read axom/LICENSE.
!

program spio_basic_write_read
  use iso_c_binding
  use axom_sidre
  use axom_spio
  implicit none

  include 'mpif.h'

  integer mpierr
  integer num_files
  integer testvalue1, testvalue2
  integer return_val

  type(SidreDataStore) ds1, ds2
  type(SidreGroup) root1, root2
  type(SidreGroup) flds1, flds2
  type(SidreGroup) ga, gb
  type(SidreView)  view1, view2

  type(iomanager) writer, reader

  call mpi_init(mpierr)

  ds1 = datastore_new()
  root1 = ds1%get_root()

  flds1 = root1%create_group("fields")
  flds2 = root1%create_group("fields2")

  ga = flds1%create_group("a")
  gb = flds2%create_group("b")

  view1 = ga%create_view_scalar_int("i0", 101)
  view2 = gb%create_view_scalar_int("i1", 404)

  num_files = 1
  writer = iomanager_new(MPI_COMM_WORLD)

  call writer%write(root1, num_files, "F_out_spio_basic_write_read", "sidre_hdf5")

  ds2 = datastore_new()

  reader = iomanager_new(MPI_COMM_WORLD)

  root2 = ds2%get_root()
  call reader%read(root2, "F_out_spio_basic_write_read.root")

  return_val = 0
  if (.not. root2%is_equivalent_to(root1)) then
     return_val = 1 
  endif

  view1 = root1%get_view("fields/a/i0")
  testvalue1 = view1%get_data_int()
  view2 = root2%get_view("fields/a/i0")
  testvalue2 = view1%get_data_int()

  if (testvalue1 .ne. testvalue2) then
     return_val = 1
  endif

  view1 = root1%get_view("fields2/b/i1")
  testvalue1 = view1%get_data_int()
  view2 = root2%get_view("fields2/b/i1")
  testvalue2 = view1%get_data_int()

  if (testvalue1 .ne. testvalue2) then
     return_val = 1
  endif

  call ds1%delete()
  call ds2%delete()

  call mpi_finalize(mpierr)

  call exit(return_val)
end program spio_basic_write_read
