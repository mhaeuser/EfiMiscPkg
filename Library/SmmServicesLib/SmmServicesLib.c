/** @file
  Copyright (C) 2016, CupertinoNet.  All rights reserved.<BR>

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
**/

#include <PiSmm.h>

#include <Library/DebugLib.h>
#include <Library/SmmServicesLib.h>
#include <Library/SmmServicesTableLib.h>

// SmmInstallConfigurationTable
/** Adds, updates, or removes a configuration table entry from the System
    Management System Table.

  The SmmInstallConfigurationTable() function is used to maintain the list of
  configuration tables that are stored in the System Management System Table.
  The list is stored as an array of (GUID, Pointer) pairs.  The list must be
  allocated from pool memory with PoolType set to EfiRuntimeServicesData.

  @param[in] SystemTable  A pointer to the SMM System Table (SMST).
  @param[in] Guid         A pointer to the GUID for the entry to add, update,
                          or remove.
  @param[in] Table        A pointer to the buffer of the table to add.
  @param[in] TableSize    The size of the table to install.

  @retval EFI_SUCCESS            The (Guid, Table) pair was added, updated, or
                                 removed.
  @retval EFI_INVALID_PARAMETER  Guid is not valid.
  @retval EFI_NOT_FOUND          An attempt was made to delete a non-existent
                                 entry.
  @retval EFI_OUT_OF_RESOURCES   There is not enough memory available to
                                 complete the operation.
**/
EFI_STATUS
SmmInstallConfigurationTable (
  IN CONST EFI_SMM_SYSTEM_TABLE2  *SystemTable,
  IN CONST EFI_GUID               *Guid,
  IN VOID                         *Table,
  IN UINTN                        TableSize
  )
{
  EFI_STATUS Status;

  ASSERT (SystemTable != NULL);
  ASSERT (Guid != NULL);
  ASSERT (InSmm ());

  Status = gSmst->SmmInstallConfigurationTable (
                    SystemTable,
                    Guid,
                    Table,
                    TableSize
                    );

  if (Status != EFI_NOT_FOUND) {
    ASSERT_EFI_ERROR (Status);
  }

  return Status;
}

// SmmAllocatePool
/** Allocates pool memory.

  @param[in]  PoolType  The type of pool to allocate.
                        MemoryType values in the range 0x70000000..0x7FFFFFFF
                        are reserved for OEM use.  MemoryType values in the
                        range 0x80000000..0xFFFFFFFF are reserved for use by
                        UEFI OS loaders that are provided by operating system
                        vendors.  The only illegal memory type values are those
                        in the range EfiMaxMemoryType..0x6FFFFFFF.
  @param[in]  Size      The number of bytes to allocate from the pool.
  @param[out] Buffer    A pointer to a pointer to the allocated buffer if the
                        call succeeds; undefined otherwise.

  @retval EFI_SUCCESS            The requested number of bytes was allocated.
  @retval EFI_OUT_OF_RESOURCES   The pool requested could not be allocated.
  @retval EFI_INVALID_PARAMETER  PoolType was invalid or Buffer is NULL.
                                 PoolType was EfiPersistentMemory.
**/
EFI_STATUS
SmmAllocatePool (
  IN  EFI_MEMORY_TYPE  PoolType,
  IN  UINTN            Size,
  OUT VOID             **Buffer
  )
{
  EFI_STATUS Status;

  ASSERT ((PoolType < EfiMaxMemoryType) || (PoolType > 0x6FFFFFFF));
  ASSERT (PoolType != EfiPersistentMemory);
  ASSERT (Size > 0);
  ASSERT (Buffer != NULL);
  ASSERT (InSmm ());

  Status = gSmst->SmmAllocatePool (PoolType, Size, Buffer);

  ASSERT_EFI_ERROR (Status);

  return Status;
}

// SmmFreePool
/** Returns pool memory to the system.

  @param[in] Buffer  The pointer to the buffer to free.

  @retval EFI_SUCCESS            The memory was returned to the system.
  @retval EFI_INVALID_PARAMETER  Buffer was invalid.
**/
EFI_STATUS
SmmFreePool (
  IN VOID  *Buffer
  )
{
  EFI_STATUS Status;

  ASSERT (Buffer != NULL);
  ASSERT (InSmm ());

  Status = gSmst->SmmFreePool (Buffer);

  ASSERT_EFI_ERROR (Status);

  return Status;
}

// SmmAllocatePages
/** Allocates memory pages from the system.

  @param[in]      Type        The type of allocation to perform.
  @param[in]      MemoryType  The type of memory to allocate.
                              MemoryType values in the range
                              0x70000000..0x7FFFFFFF are reserved for OEM use.
                              MemoryType values in the range
                              0x80000000..0xFFFFFFFF are reserved for use by
                              UEFI OS loaders that are provided by operating
                              system vendors.  The only illegal  memory type
                              values are those in the range
                              EfiMaxMemoryType..0x6FFFFFFF.
  @param[in]      Pages       The number of contiguous 4 KB pages to allocate.
  @param[in, out] Memory      The pointer to a physical address. On input, the
                              way in which the address is used depends on the
                              value of Type.

  @retval EFI_SUCCESS            The requested pages were allocated.
  @retval EFI_INVALID_PARAMETER  1) Type is not AllocateAnyPages or
                                 AllocateMaxAddress or AllocateAddress.
                                 2) MemoryType is in the range
                                 EfiMaxMemoryType..0x6FFFFFFF.
                                 3) Memory is NULL.
                                 4) MemoryType was EfiPersistentMemory.
  @retval EFI_OUT_OF_RESOURCES   The pages could not be allocated.
  @retval EFI_NOT_FOUND          The requested pages could not be found.
**/
EFI_STATUS
SmmAllocatePages (
  IN     EFI_ALLOCATE_TYPE     Type,
  IN     EFI_MEMORY_TYPE       MemoryType,
  IN     UINTN                 Pages,
  IN OUT EFI_PHYSICAL_ADDRESS  *Memory
  )
{
  EFI_STATUS Status;

  ASSERT ((Type >= AllocateAnyPages) && (Type < MaxAllocateType));
  ASSERT ((MemoryType < EfiMaxMemoryType) || (MemoryType > 0x6FFFFFFF));
  ASSERT (MemoryType != EfiPersistentMemory);
  ASSERT (Pages > 0);
  ASSERT (Memory != NULL);
  ASSERT ((*Memory != 0)
      || ((Type != AllocateMaxAddress) && (Type != AllocateAddress)));

  ASSERT ((Type != AllocateMaxAddress)
      || ((EFI_PAGES_TO_SIZE (Pages) - 1) <= (UINTN)*Memory));

  ASSERT ((Type != AllocateAddress)
      || (EFI_PAGES_TO_SIZE (Pages) - 1) <= (MAX_ADDRESS - (UINTN)*Memory));

  ASSERT (InSmm ());

  Status = gSmst->SmmAllocatePages (Type, MemoryType, Pages, Memory);

  if (Status != EFI_NOT_FOUND) {
    ASSERT_EFI_ERROR (Status);
  }

  return Status;
}

// SmmFreePages
/** Frees memory pages.

  @param[in]  Memory  The base physical address of the pages to be freed.
  @param[in]  Pages   The number of contiguous 4 KB pages to free.

  @retval EFI_SUCCESS            The requested pages were freed.
  @retval EFI_INVALID_PARAMETER  Memory is not a page-aligned address or Pages
                                 is invalid.
  @retval EFI_NOT_FOUND          The requested memory pages were not allocated
                                 with AllocatePages().
**/
EFI_STATUS
SmmFreePages (
  IN EFI_PHYSICAL_ADDRESS  Memory,
  IN UINTN                 Pages
  )
{
  EFI_STATUS Status;

  ASSERT (Memory != 0);
  ASSERT (ALIGN_VALUE (Memory, EFI_PAGE_SIZE) == Memory);
  ASSERT (Pages > 0);
  ASSERT (InSmm ());

  Status = gSmst->SmmFreePages (Memory, Pages);

  ASSERT_EFI_ERROR (Status);

  return Status;
}

// SmmStartupThisAp
/** The SmmStartupThisAp() lets the caller to get one distinct application
    processor (AP) in the enabled processor pool to execute a caller-provided
    code stream while in SMM. It runs the given code on this processor and
    reports the status.  It must be noted that the supplied code stream will
    be run only on an enabled processor which has also entered SMM.

  @param[in]     Procedure      A pointer to the code stream to be run on the
                                designated AP of the system.
  @param[in]     CpuNumber      The zero-based index of the processor number of
                                the AP on which the code stream is supposed to
                                run.
  @param[in,out] ProcArguments  Allow the caller to pass a list of parameters
                                to the code that is run by the AP.

  @retval EFI_SUCCESS            The call was successful and the return
                                 parameters are valid.
  @retval EFI_INVALID_PARAMETER  The input arguments are out of range.
  @retval EFI_INVALID_PARAMETER  The CPU requested is not available on this SMI
                                 invocation.
  @retval EFI_INVALID_PARAMETER  The CPU cannot support an additional service
                                 invocation.
**/
EFI_STATUS
SmmStartupThisAp (
  IN     EFI_AP_PROCEDURE  Procedure,
  IN     UINTN             CpuNumber,
  IN OUT VOID              *ProcArguments OPTIONAL
  )
{
  EFI_STATUS Status;

  ASSERT (Procedure != NULL);
  ASSERT ((CpuNumber >= 0) && (CpuNumber < gSmst->NumberOfCpus));
  ASSERT (InSmm ());

  Status = gSmst->SmmStartupThisAp (Procedure, CpuNumber, ProcArguments);

  ASSERT_EFI_ERROR (Status);

  return Status;
}

// SmmInstallProtocolInterface
/** Installs a protocol interface on a device handle.  If the handle does not
    exist, it is created and added to the list of handles in the system.
    InstallMultipleProtocolInterfaces() performs more error checking than
    InstallProtocolInterface(), so it is recommended that
    InstallMultipleProtocolInterfaces() be used in place of
    InstallProtocolInterface()

  @param[in, out] Handle         A pointer to the EFI_HANDLE on which the
                                 interface is to be installed.
  @param[in]      Protocol       The numeric ID of the protocol interface.
  @param[in]      InterfaceType  Indicates whether Interface is supplied in
                                 native form.
  @param[in]      Interface      A pointer to the protocol interface.

  @retval EFI_SUCCESS            The protocol interface was installed.
  @retval EFI_OUT_OF_RESOURCES   Space for a new handle could not be allocated.
  @retval EFI_INVALID_PARAMETER  Handle is NULL.
  @retval EFI_INVALID_PARAMETER  Protocol is NULL.
  @retval EFI_INVALID_PARAMETER  InterfaceType is not EFI_NATIVE_INTERFACE.
  @retval EFI_INVALID_PARAMETER  Protocol is already installed on the handle
                                 specified by Handle.
**/
EFI_STATUS
SmmInstallProtocolInterface (
  IN OUT EFI_HANDLE          *Handle,
  IN     EFI_GUID            *Protocol,
  IN     EFI_INTERFACE_TYPE  InterfaceType,
  IN     VOID                *Interface
  )
{
  EFI_STATUS Status;

  ASSERT (Handle != NULL);
  ASSERT (Protocol != NULL);
  ASSERT (InterfaceType == EFI_NATIVE_INTERFACE);
  ASSERT (InSmm ());

  if (*Handle != NULL) {
    ASSERT_SMM_PROTOCOL_ALREADY_INSTALLED (*Handle, Protocol);
  }

  Status = gSmst->SmmInstallProtocolInterface (
                    Handle,
                    Protocol,
                    InterfaceType,
                    Interface
                    );

  ASSERT_EFI_ERROR (Status);

  return Status;
}

// SmmUninstallProtocolInterface
/** Removes a protocol interface from a device handle. It is recommended that
    UninstallMultipleProtocolInterfaces() be used in place of
    UninstallProtocolInterface().

  @param[in] Handle     The handle on which the interface was installed.
  @param[in] Protocol   The numeric ID of the interface.
  @param[in] Interface  A pointer to the interface.

  @retval EFI_SUCCESS            The interface was removed.
  @retval EFI_NOT_FOUND          The interface was not found.
  @retval EFI_ACCESS_DENIED      The interface was not removed because the
                                 interface is still being used by a driver.
  @retval EFI_INVALID_PARAMETER  Handle is NULL.
  @retval EFI_INVALID_PARAMETER  Protocol is NULL.
**/
EFI_STATUS
SmmUninstallProtocolInterface (
  IN EFI_HANDLE  Handle,
  IN EFI_GUID    *Protocol,
  IN VOID        *Interface
  )
{
  EFI_STATUS Status;

  ASSERT (Handle != NULL);
  ASSERT (Protocol != NULL);
  ASSERT (Interface != NULL);
  ASSERT (InSmm ());

  Status = gSmst->SmmUninstallProtocolInterface (Handle, Protocol, Interface);

  if ((Status != EFI_NOT_FOUND) || (Status != EFI_ACCESS_DENIED)) {
    ASSERT_EFI_ERROR (Status);
  }

  return Status;
}

// SmmHandleProtocol
/** Queries a handle to determine if it supports a specified protocol.

  @param[in]  Handle     The handle being queried.
  @param[in]  Protocol   The published unique identifier of the protocol.
  @param[out] Interface  Supplies the address where a pointer to the
                         corresponding Protocol Interface is returned.

  @retval EFI_SUCCESS            The interface information for the specified
                                 protocol was returned.
  @retval EFI_UNSUPPORTED        The device does not support the specified
                                 protocol.
  @retval EFI_INVALID_PARAMETER  Handle is NULL.
  @retval EFI_INVALID_PARAMETER  Protocol is NULL.
  @retval EFI_INVALID_PARAMETER  Interface is NULL.
**/
EFI_STATUS
SmmHandleProtocol (
  IN  EFI_HANDLE  Handle,
  IN  EFI_GUID    *Protocol,
  OUT VOID        **Interface
  )
{
  EFI_STATUS Status;

  ASSERT (Handle != NULL);
  ASSERT (Protocol != NULL);
  ASSERT (Interface != NULL);
  ASSERT (InSmm ());

  Status = gSmst->SmmHandleProtocol (Handle, Protocol, Interface);

  if (Status != EFI_UNSUPPORTED) {
    ASSERT_EFI_ERROR (Status);
  }

  return Status;
}

// SmmRegisterProtocolNotify
/** Register a callback function be called when a particular protocol interface
    is installed.

  The SmmRegisterProtocolNotify() function creates a registration Function that
  is to be called whenever a protocol interface is installed for Protocol by
  SmmInstallProtocolInterface().  If Function == NULL and Registration is an
  existing registration, then the callback is unhooked.

  @param[in]  Protocol      The unique ID of the protocol for which the event
                            is to be registered.
  @param[in]  Function      Points to the notification function.
  @param[out] Registration  A pointer to a memory location to receive the
                            registration value.

  @retval EFI_SUCCESS            Successfully returned the registration record
                                 that has been added or unhooked.
  @retval EFI_INVALID_PARAMETER  Protocol is NULL or Registration is NULL.
  @retval EFI_OUT_OF_RESOURCES   Not enough memory resource to finish the
                                 request.
  @retval EFI_NOT_FOUND          If the registration is not found when
                                 Function == NULL.
**/
EFI_STATUS
SmmRegisterProtocolNotify (
  IN  CONST EFI_GUID     *Protocol,
  IN  EFI_SMM_NOTIFY_FN  Function,
  OUT VOID               **Registration
  )
{
  EFI_STATUS Status;

  ASSERT (Protocol != NULL);
  ASSERT (Registration != NULL);
  ASSERT ((((Function != NULL) ? 1 : 0)
             ^ ((*Registration != NULL) ? 1 : 0)) != 0);

  ASSERT (InSmm ());

  Status = gSmst->SmmRegisterProtocolNotify (Protocol, Function, Registration);

  ASSERT_EFI_ERROR (Status);

  return Status;
}

// SmmLocateHandle
/** Returns an array of handles that support a specified protocol.

  @param[in]      SearchType  Specifies which handle(s) are to be returned.
  @param[in]      Protocol    Specifies the protocol to search by.
  @param[in]      SearchKey   Specifies the search key.
  @param[in, out] BufferSize  On input, the size in bytes of Buffer.
                              On output, the size in bytes of the array
                              returned in Buffer (if the buffer was large
                              enough) or the size, in bytes, of the buffer
                              needed to obtain the array (if the buffer was not
                              large enough).
  @param[out]     Buffer      The buffer in which the array is returned.

  @retval EFI_SUCCESS            The array of handles was returned.
  @retval EFI_NOT_FOUND          No handles match the search.
  @retval EFI_BUFFER_TOO_SMALL   The BufferSize is too small for the result.
  @retval EFI_INVALID_PARAMETER  SearchType is not a member of
                                 EFI_LOCATE_SEARCH_TYPE.
  @retval EFI_INVALID_PARAMETER  SearchType is ByRegisterNotify and SearchKey
                                 is NULL.
  @retval EFI_INVALID_PARAMETER  SearchType is ByProtocol and Protocol is NULL.
  @retval EFI_INVALID_PARAMETER  One or more matches are found and BufferSize
                                 is NULL.
  @retval EFI_INVALID_PARAMETER  BufferSize is large enough for the result and
                                 Buffer is NULL.
**/
EFI_STATUS
SmmLocateHandle (
  IN     EFI_LOCATE_SEARCH_TYPE  SearchType,
  IN     EFI_GUID                *Protocol, OPTIONAL
  IN     VOID                    *SearchKey, OPTIONAL
  IN OUT UINTN                   *BufferSize,
  OUT    EFI_HANDLE              *Buffer
  )
{
  EFI_STATUS Status;

  ASSERT ((SearchType >= AllHandles) && (SearchType <= ByProtocol));
  ASSERT ((SearchType != ByRegisterNotify) || (SearchKey != NULL));
  ASSERT ((SearchType != ByProtocol) || (Protocol != NULL));
  ASSERT (BufferSize != NULL);
  ASSERT ((((*BufferSize > 0) ? 1 : 0) ^ ((Buffer == NULL) ? 1 : 0)) != 0);
  ASSERT (InSmm ());

  Status = gSmst->SmmLocateHandle (
                    SearchType,
                    Protocol,
                    SearchKey,
                    BufferSize,
                    Buffer
                    );

  return Status;
}

// SmmLocateProtocol
/** Returns the first protocol instance that matches the given protocol.

  @param[in]  Protocol      Provides the protocol to search for.
  @param[in]  Registration  Optional registration key returned from
                            RegisterProtocolNotify().
  @param[out] Interface     On return, a pointer to the first interface that
                            matches Protocol and Registration.

  @retval EFI_SUCCESS            A protocol instance matching Protocol was
                                 found and returned in Interface.
  @retval EFI_NOT_FOUND          No protocol instances were found that match
                                 Protocol and Registration.
  @retval EFI_INVALID_PARAMETER  Interface is NULL.
**/
EFI_STATUS
SmmLocateProtocol (
  IN  EFI_GUID  *Protocol,
  IN  VOID      *Registration, OPTIONAL
  OUT VOID      **Interface
  )
{
  EFI_STATUS Status;

  ASSERT (Protocol != NULL);
  ASSERT (Interface != NULL);
  ASSERT (InSmm ());

  Status = gSmst->SmmLocateProtocol (Protocol, Registration, Interface);

  if (Status != EFI_NOT_FOUND) {
    ASSERT_EFI_ERROR (Status);
  }

  return Status;
}

// SmiManage
/** Manage SMI of a particular type.

  @param[in]     HandlerType     Points to the handler type or NULL for root
                                 SMI handlers.
  @param[in]     Context         Points to an optional context buffer.
  @param[in,out] CommBuffer      Points to the optional communication buffer.
  @param[in,out] CommBufferSize  Points to the size of the optional
                                 communication buffer.

  @retval EFI_WARN_INTERRUPT_SOURCE_PENDING  Interrupt source was processed
                                             successfully but not quiesced.
  @retval EFI_INTERRUPT_PENDING              One or more SMI sources could not
                                             be quiesced.
  @retval EFI_NOT_FOUND                      Interrupt source was not handled
                                             or quiesced.
  @retval EFI_SUCCESS                        Interrupt source was handled and
                                             quiesced.
**/
EFI_STATUS
SmiManage (
  IN     CONST EFI_GUID  *HandlerType, OPTIONAL
  IN     CONST VOID      *Context, OPTIONAL
  IN OUT VOID            *CommBuffer, OPTIONAL
  IN OUT UINTN           *CommBufferSize OPTIONAL
  )
{
  EFI_STATUS Status;

  ASSERT (InSmm ());

  Status = gSmst->SmiManage (HandlerType, Context, CommBuffer, CommBufferSize);

  return Status;
}

// SmiHandlerRegister
/** Registers a handler to execute within SMM.

  @param[in]  Handler         Handler service funtion pointer.
  @param[in]  HandlerType     Points to the handler type or NULL for root SMI
                              handlers.
  @param[out] DispatchHandle  On return, contains a unique handle which can be
                              used to later unregister the handler function.

  @retval EFI_SUCCESS            SMI handler added successfully.
  @retval EFI_INVALID_PARAMETER  Handler is NULL or DispatchHandle is NULL.
**/
EFI_STATUS
SmiHandlerRegister (
  IN  EFI_SMM_HANDLER_ENTRY_POINT2  Handler,
  IN  CONST EFI_GUID                *HandlerType, OPTIONAL
  OUT EFI_HANDLE                    *DispatchHandle
  )
{
  EFI_STATUS Status;

  ASSERT (Handler != NULL);
  ASSERT (DispatchHandle != NULL);
  ASSERT (InSmm ());

  Status = gSmst->SmiHandlerRegister (Handler, HandlerType, DispatchHandle);

  ASSERT_EFI_ERROR (Status);

  return Status;
}

// SmiHandlerUnRegister
/** Unregister a handler in SMM.

  @param[in] DispatchHandle  The handle that was specified when the handler was
                             registered.

  @retval EFI_SUCCESS            Handler function was successfully
                                 unregistered.
  @retval EFI_INVALID_PARAMETER  DispatchHandle does not refer to a valid
                                 handle.
**/
EFI_STATUS
SmiHandlerUnRegister (
  IN EFI_HANDLE  DispatchHandle
  )
{
  EFI_STATUS Status;
  ASSERT (DispatchHandle != NULL);
  ASSERT (InSmm ());

  Status = gSmst->SmiHandlerUnRegister (DispatchHandle);
  
  ASSERT_EFI_ERROR (Status);

  return Status;
}
