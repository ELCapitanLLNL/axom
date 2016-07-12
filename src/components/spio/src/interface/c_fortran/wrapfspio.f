! wrapfspio.f
! This is generated code, do not edit
!
! Copyright (c) 2015, Lawrence Livermore National Security, LLC.
! Produced at the Lawrence Livermore National Laboratory.
!
! All rights reserved.
!
! This source code cannot be distributed without permission and
! further review from Lawrence Livermore National Laboratory.
!
!>
!! \file wrapfspio.f
!! \brief Shroud generated wrapper for SPIO library
!<
module spio_mod
    use, intrinsic :: iso_c_binding, only : C_PTR
    ! splicer begin module_use
    ! splicer end module_use
    ! splicer begin class.IOManager.module_use
    ! splicer end class.IOManager.module_use
    implicit none
    
    ! splicer begin module_top
    ! splicer end module_top
    
    ! splicer begin class.IOManager.module_top
    ! splicer end class.IOManager.module_top
    
    type iomanager
        type(C_PTR), private :: voidptr
        ! splicer begin class.IOManager.component_part
        ! splicer end class.IOManager.component_part
    contains
        procedure :: delete => iomanager_delete
        procedure :: write => iomanager_write
        procedure :: write_group_to_root_file => iomanager_write_group_to_root_file
        procedure :: read_0 => iomanager_read_0
        procedure :: read_1 => iomanager_read_1
        procedure :: load_external_data => iomanager_load_external_data
        procedure :: get_instance => iomanager_get_instance
        procedure :: set_instance => iomanager_set_instance
        procedure :: associated => iomanager_associated
        generic :: read => &
            ! splicer begin class.IOManager.generic.read
            ! splicer end class.IOManager.generic.read
            read_0,  &
            read_1
        ! splicer begin class.IOManager.type_bound_procedure_part
        ! splicer end class.IOManager.type_bound_procedure_part
    end type iomanager
    
    
    interface operator (.eq.)
        module procedure iomanager_eq
    end interface
    
    interface operator (.ne.)
        module procedure iomanager_ne
    end interface
    
    interface
        
        function c_iomanager_new(com) &
                result(rv) &
                bind(C, name="SPIO_iomanager_new")
            use iso_c_binding
            implicit none
            integer(C_INT), value, intent(IN) :: com
            type(C_PTR) :: rv
        end function c_iomanager_new
        
        subroutine c_iomanager_delete(self) &
                bind(C, name="SPIO_iomanager_delete")
            use iso_c_binding
            implicit none
            type(C_PTR), value, intent(IN) :: self
        end subroutine c_iomanager_delete
        
        subroutine c_iomanager_write(self, group, num_files, file_string, protocol) &
                bind(C, name="SPIO_iomanager_write")
            use iso_c_binding
            implicit none
            type(C_PTR), value, intent(IN) :: self
            type(C_PTR), value, intent(IN) :: group
            integer(C_INT), value, intent(IN) :: num_files
            character(kind=C_CHAR), intent(IN) :: file_string(*)
            character(kind=C_CHAR), intent(IN) :: protocol(*)
        end subroutine c_iomanager_write
        
        subroutine c_iomanager_write_bufferify(self, group, num_files, file_string, Lfile_string, protocol, Lprotocol) &
                bind(C, name="SPIO_iomanager_write_bufferify")
            use iso_c_binding
            implicit none
            type(C_PTR), value, intent(IN) :: self
            type(C_PTR), value, intent(IN) :: group
            integer(C_INT), value, intent(IN) :: num_files
            character(kind=C_CHAR), intent(IN) :: file_string(*)
            integer(C_INT), value, intent(IN) :: Lfile_string
            character(kind=C_CHAR), intent(IN) :: protocol(*)
            integer(C_INT), value, intent(IN) :: Lprotocol
        end subroutine c_iomanager_write_bufferify
        
        subroutine c_iomanager_write_group_to_root_file(self, group, file_name) &
                bind(C, name="SPIO_iomanager_write_group_to_root_file")
            use iso_c_binding
            implicit none
            type(C_PTR), value, intent(IN) :: self
            type(C_PTR), value, intent(IN) :: group
            character(kind=C_CHAR), intent(IN) :: file_name(*)
        end subroutine c_iomanager_write_group_to_root_file
        
        subroutine c_iomanager_write_group_to_root_file_bufferify(self, group, file_name, Lfile_name) &
                bind(C, name="SPIO_iomanager_write_group_to_root_file_bufferify")
            use iso_c_binding
            implicit none
            type(C_PTR), value, intent(IN) :: self
            type(C_PTR), value, intent(IN) :: group
            character(kind=C_CHAR), intent(IN) :: file_name(*)
            integer(C_INT), value, intent(IN) :: Lfile_name
        end subroutine c_iomanager_write_group_to_root_file_bufferify
        
        subroutine c_iomanager_read_0(self, group, file_string, protocol) &
                bind(C, name="SPIO_iomanager_read_0")
            use iso_c_binding
            implicit none
            type(C_PTR), value, intent(IN) :: self
            type(C_PTR), value, intent(IN) :: group
            character(kind=C_CHAR), intent(IN) :: file_string(*)
            character(kind=C_CHAR), intent(IN) :: protocol(*)
        end subroutine c_iomanager_read_0
        
        subroutine c_iomanager_read_0_bufferify(self, group, file_string, Lfile_string, protocol, Lprotocol) &
                bind(C, name="SPIO_iomanager_read_0_bufferify")
            use iso_c_binding
            implicit none
            type(C_PTR), value, intent(IN) :: self
            type(C_PTR), value, intent(IN) :: group
            character(kind=C_CHAR), intent(IN) :: file_string(*)
            integer(C_INT), value, intent(IN) :: Lfile_string
            character(kind=C_CHAR), intent(IN) :: protocol(*)
            integer(C_INT), value, intent(IN) :: Lprotocol
        end subroutine c_iomanager_read_0_bufferify
        
        subroutine c_iomanager_read_1(self, group, root_file) &
                bind(C, name="SPIO_iomanager_read_1")
            use iso_c_binding
            implicit none
            type(C_PTR), value, intent(IN) :: self
            type(C_PTR), value, intent(IN) :: group
            character(kind=C_CHAR), intent(IN) :: root_file(*)
        end subroutine c_iomanager_read_1
        
        subroutine c_iomanager_read_1_bufferify(self, group, root_file, Lroot_file) &
                bind(C, name="SPIO_iomanager_read_1_bufferify")
            use iso_c_binding
            implicit none
            type(C_PTR), value, intent(IN) :: self
            type(C_PTR), value, intent(IN) :: group
            character(kind=C_CHAR), intent(IN) :: root_file(*)
            integer(C_INT), value, intent(IN) :: Lroot_file
        end subroutine c_iomanager_read_1_bufferify
        
        subroutine c_iomanager_load_external_data(self, group, root_file) &
                bind(C, name="SPIO_iomanager_load_external_data")
            use iso_c_binding
            implicit none
            type(C_PTR), value, intent(IN) :: self
            type(C_PTR), value, intent(IN) :: group
            character(kind=C_CHAR), intent(IN) :: root_file(*)
        end subroutine c_iomanager_load_external_data
        
        subroutine c_iomanager_load_external_data_bufferify(self, group, root_file, Lroot_file) &
                bind(C, name="SPIO_iomanager_load_external_data_bufferify")
            use iso_c_binding
            implicit none
            type(C_PTR), value, intent(IN) :: self
            type(C_PTR), value, intent(IN) :: group
            character(kind=C_CHAR), intent(IN) :: root_file(*)
            integer(C_INT), value, intent(IN) :: Lroot_file
        end subroutine c_iomanager_load_external_data_bufferify
        
        ! splicer begin class.IOManager.additional_interfaces
        ! splicer end class.IOManager.additional_interfaces
    end interface

contains
    
    function iomanager_new(com) result(rv)
        implicit none
        integer, value, intent(IN) :: com
        type(iomanager) :: rv
        ! splicer begin class.IOManager.method.new
        rv%voidptr = c_iomanager_new(com)
        ! splicer end class.IOManager.method.new
    end function iomanager_new
    
    subroutine iomanager_delete(obj)
        use iso_c_binding, only : C_NULL_PTR
        implicit none
        class(iomanager) :: obj
        ! splicer begin class.IOManager.method.delete
        call c_iomanager_delete(obj%voidptr)
        obj%voidptr = C_NULL_PTR
        ! splicer end class.IOManager.method.delete
    end subroutine iomanager_delete
    
    subroutine iomanager_write(obj, group, num_files, file_string, protocol)
        use iso_c_binding, only : C_INT
        use sidre_mod, only : datagroup
        implicit none
        class(iomanager) :: obj
        type(datagroup), value, intent(IN) :: group
        integer(C_INT), value, intent(IN) :: num_files
        character(*), intent(IN) :: file_string
        character(*), intent(IN) :: protocol
        ! splicer begin class.IOManager.method.write
        call c_iomanager_write_bufferify(  &
            obj%voidptr,  &
            group%get_instance(),  &
            num_files,  &
            file_string,  &
            len_trim(file_string, kind=C_INT),  &
            protocol,  &
            len_trim(protocol, kind=C_INT))
        ! splicer end class.IOManager.method.write
    end subroutine iomanager_write
    
    subroutine iomanager_write_group_to_root_file(obj, group, file_name)
        use iso_c_binding, only : C_INT
        use sidre_mod, only : datagroup
        implicit none
        class(iomanager) :: obj
        type(datagroup), value, intent(IN) :: group
        character(*), intent(IN) :: file_name
        ! splicer begin class.IOManager.method.write_group_to_root_file
        call c_iomanager_write_group_to_root_file_bufferify(  &
            obj%voidptr,  &
            group%get_instance(),  &
            file_name,  &
            len_trim(file_name, kind=C_INT))
        ! splicer end class.IOManager.method.write_group_to_root_file
    end subroutine iomanager_write_group_to_root_file
    
    subroutine iomanager_read_0(obj, group, file_string, protocol)
        use iso_c_binding, only : C_INT
        use sidre_mod, only : datagroup
        implicit none
        class(iomanager) :: obj
        type(datagroup), value, intent(IN) :: group
        character(*), intent(IN) :: file_string
        character(*), intent(IN) :: protocol
        ! splicer begin class.IOManager.method.read_0
        call c_iomanager_read_0_bufferify(  &
            obj%voidptr,  &
            group%get_instance(),  &
            file_string,  &
            len_trim(file_string, kind=C_INT),  &
            protocol,  &
            len_trim(protocol, kind=C_INT))
        ! splicer end class.IOManager.method.read_0
    end subroutine iomanager_read_0
    
    subroutine iomanager_read_1(obj, group, root_file)
        use iso_c_binding, only : C_INT
        use sidre_mod, only : datagroup
        implicit none
        class(iomanager) :: obj
        type(datagroup), value, intent(IN) :: group
        character(*), intent(IN) :: root_file
        ! splicer begin class.IOManager.method.read_1
        call c_iomanager_read_1_bufferify(  &
            obj%voidptr,  &
            group%get_instance(),  &
            root_file,  &
            len_trim(root_file, kind=C_INT))
        ! splicer end class.IOManager.method.read_1
    end subroutine iomanager_read_1
    
    subroutine iomanager_load_external_data(obj, group, root_file)
        use iso_c_binding, only : C_INT
        use sidre_mod, only : datagroup
        implicit none
        class(iomanager) :: obj
        type(datagroup), value, intent(IN) :: group
        character(*), intent(IN) :: root_file
        ! splicer begin class.IOManager.method.load_external_data
        call c_iomanager_load_external_data_bufferify(  &
            obj%voidptr,  &
            group%get_instance(),  &
            root_file,  &
            len_trim(root_file, kind=C_INT))
        ! splicer end class.IOManager.method.load_external_data
    end subroutine iomanager_load_external_data
    
    function iomanager_get_instance(obj) result (voidptr)
        use iso_c_binding, only: C_PTR
        implicit none
        class(iomanager), intent(IN) :: obj
        type(C_PTR) :: voidptr
        voidptr = obj%voidptr
    end function iomanager_get_instance
    
    subroutine iomanager_set_instance(obj, voidptr)
        use iso_c_binding, only: C_PTR
        implicit none
        class(iomanager), intent(INOUT) :: obj
        type(C_PTR), intent(IN) :: voidptr
        obj%voidptr = voidptr
    end subroutine iomanager_set_instance
    
    function iomanager_associated(obj) result (rv)
        use iso_c_binding, only: c_associated
        implicit none
        class(iomanager), intent(IN) :: obj
        logical rv
        rv = c_associated(obj%voidptr)
    end function iomanager_associated
    
    ! splicer begin class.IOManager.additional_functions
    ! splicer end class.IOManager.additional_functions
    
    function iomanager_eq(a,b) result (rv)
        use iso_c_binding, only: c_associated
        implicit none
        type(iomanager), intent(IN) ::a,b
        logical :: rv
        if (c_associated(a%voidptr, b%voidptr)) then
            rv = .true.
        else
            rv = .false.
        endif
    end function iomanager_eq
    
    function iomanager_ne(a,b) result (rv)
        use iso_c_binding, only: c_associated
        implicit none
        type(iomanager), intent(IN) ::a,b
        logical :: rv
        if (.not. c_associated(a%voidptr, b%voidptr)) then
            rv = .true.
        else
            rv = .false.
        endif
    end function iomanager_ne

end module spio_mod
