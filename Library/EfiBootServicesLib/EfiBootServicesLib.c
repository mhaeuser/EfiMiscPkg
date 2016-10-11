/** @file
  Copyright (C) 2015 - 2016, CupertinoNet.  All rights reserved.<BR>

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

#include <Uefi.h>

#include <Library/DebugLib.h>
#include <Library/EfiBootServicesLib.h>
#include <Library/MiscRuntimeLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>

#ifndef MDE_NDEBUG

// mPreviousTpl
STATIC EFI_TPL mPreviousTpl = MAX_UINTN;

#endif // !MDE_NDEBUG

// Task Priority Services

// EfiRaiseTpl
/** Raises a task's priority level and returns its previous level.

  @param[in] NewTpl  The new task priority level.

  @return  Previous task priority level
**/
EFI_TPL
EfiRaiseTpl (
  IN EFI_TPL  NewTpl
  )
{
  EFI_TPL OldTpl;

  ASSERT (!EfiAtRuntime ());

  OldTpl = gBS->RaiseTPL (NewTpl);

  DEBUG_CODE (
    mPreviousTpl = OldTpl;
    );

  return OldTpl;
}

// EfiRestoreTpl
/** Restores a task's priority level to its previous value.

  @param[in] OldTpl  The previous task priority level to restore.
**/
VOID
EfiRestoreTpl (
  IN EFI_TPL  OldTpl
  )
{
  ASSERT (mPreviousTpl != MAX_UINTN);
  ASSERT (OldTpl == mPreviousTpl);
  ASSERT (!EfiAtRuntime ());

  gBS->RestoreTPL (OldTpl);
}

// Memory Services

// EfiAllocatePages
/** Allocates memory pages from the system.

  @param[in]      Type        The type of allocation to perform.
  @param[in]      MemoryType  The type of memory to allocate.
                              MemoryType values in the range
                              0x70000000..0x7FFFFFFF are reserved for OEM use.
                              MemoryType values in the range
                              0x80000000..0xFFFFFFFF are reserved for use by
                              UEFI OS loaders that are provided by operating
                              system vendors. The only illegal  memory type
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
EfiAllocatePages (
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
      || ((Type != AllocateMaxAddress)
       && (Type != AllocateAddress)));

  ASSERT ((Type != AllocateMaxAddress)
      || ((EFI_PAGES_TO_SIZE (Pages) - 1) <= (UINTN)*Memory));

  ASSERT ((Type != AllocateAddress)
      || (EFI_PAGES_TO_SIZE (Pages) - 1) <= (MAX_ADDRESS - (UINTN)*Memory));

  ASSERT (!EfiAtRuntime ());
  ASSERT (EfiGetCurrentTpl () <= TPL_NOTIFY);

  Status = gBS->AllocatePages (Type, MemoryType, Pages, Memory);

  if (Status != EFI_NOT_FOUND) {
    ASSERT_EFI_ERROR (Status);
  }

  return Status;
}

// EfiFreePages
/** Frees memory pages.

  @param[in] Memory  The base physical address of the pages to be freed.
  @param[in] Pages   The number of contiguous 4 KB pages to free.

  @retval EFI_SUCCESS            The requested pages were freed.
  @retval EFI_INVALID_PARAMETER  Memory is not a page-aligned address or Pages
                                 is invalid.
  @retval EFI_NOT_FOUND          The requested memory pages were not allocated
                                 with AllocatePages().
**/
EFI_STATUS
EfiFreePages (
  IN EFI_PHYSICAL_ADDRESS  Memory,
  IN UINTN                 Pages
  )
{
  EFI_STATUS Status;

  ASSERT (Memory != 0);
  ASSERT (ALIGN_VALUE (Memory, EFI_PAGE_SIZE) == Memory);
  ASSERT (Pages > 0);
  ASSERT (!EfiAtRuntime ());
  ASSERT (EfiGetCurrentTpl () <= TPL_NOTIFY);

  Status = gBS->FreePages (Memory, Pages);

  ASSERT_EFI_ERROR (Status);

  return Status;
}

// EfiGetMemoryMap
/** Returns the current memory map.

  @param[in, out] MemoryMapSize      A pointer to the size, in bytes, of the
                                     MemoryMap Buffer.
                                     On input, this is the size of the Buffer
                                     allocated by the caller.
                                     On output, it is the size of the Buffer
                                     returned by the firmware if the Buffer was
                                     large enough, or the size of the Buffer
                                     needed to contain the map if the Buffer
                                     was too small.
  @param[in, out] MemoryMap          A pointer to the Buffer in which firmware
                                     places the current memory map.
  @param[out]     MapKey             A pointer to the location in which
                                     firmware returns the key for the current
                                     memory map.
  @param[out]     DescriptorSize     A pointer to the location in which
                                     firmware returns the size, in bytes, of an
                                     individual EFI_MEMORY_DESCRIPTOR.
  @param[out]     DescriptorVersion  A pointer to the location in which
                                     firmware returns the version number
                                     associated with the EFI_MEMORY_DESCRIPTOR.

  @retval EFI_SUCCESS            The memory map was returned in the MemoryMap
                                 Buffer.
  @retval EFI_BUFFER_TOO_SMALL   The MemoryMap Buffer was too small.  The
                                 current Buffer size needed to hold the memory
                                 map is returned in MemoryMapSize.
  @retval EFI_INVALID_PARAMETER  1) MemoryMapSize is NULL.
                                 2) The MemoryMap Buffer is not too small and
                                 MemoryMap is NULL.
**/
EFI_STATUS
EfiGetMemoryMap (
  IN OUT UINTN                  *MemoryMapSize,
  IN OUT EFI_MEMORY_DESCRIPTOR  *MemoryMap,
  OUT    UINTN                  *MapKey,
  OUT    UINTN                  *DescriptorSize,
  OUT    UINT32                 *DescriptorVersion
  )
{
  EFI_STATUS Status;

  ASSERT (MemoryMapSize != NULL);
  ASSERT ((((*MemoryMapSize > 0) ? 1 : 0)
             ^ ((MemoryMap == NULL) ? 1 : 0)) != 0);

  ASSERT (!EfiAtRuntime ());
  ASSERT (EfiGetCurrentTpl () <= TPL_NOTIFY);

  Status = gBS->GetMemoryMap (
                  MemoryMapSize,
                  MemoryMap,
                  MapKey,
                  DescriptorSize,
                  DescriptorVersion
                  );

  if (Status != EFI_BUFFER_TOO_SMALL) {
    ASSERT_EFI_ERROR (Status);
  }

  return Status;
}

// EfiAllocatePool
/** Allocates pool memory.

  @param[in]  PoolType  The type of pool to allocate.
                        MemoryType values in the range
                        0x70000000..0x7FFFFFFF are reserved for OEM use.
                        MemoryType values in the range 0x80000000..0xFFFFFFFF
                        are reserved for use by UEFI OS loaders that are
                        provided by operating system vendors.  The only illegal
                        memory type values are those in the range
                        EfiMaxMemoryType..0x6FFFFFFF.
  @param[in]  Size      The number of bytes to allocate from the pool.
  @param[out] Buffer    A pointer to a pointer to the allocated buffer if the
                        call succeeds; undefined otherwise.

  @retval EFI_SUCCESS            The requested number of bytes was allocated.
  @retval EFI_OUT_OF_RESOURCES   The pool requested could not be allocated.
  @retval EFI_INVALID_PARAMETER  PoolType was invalid or Buffer is NULL.
                                 PoolType was EfiPersistentMemory.

**/
EFI_STATUS
EfiAllocatePool (
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
  ASSERT (!EfiAtRuntime ());
  ASSERT (EfiGetCurrentTpl () <= TPL_NOTIFY);

  Status = gBS->AllocatePool (PoolType, Size, Buffer);

  ASSERT_EFI_ERROR (Status);

  return Status;
}

// EfiFreePool
/** Returns pool memory to the system.

  @param[in] Buffer  The pointer to the buffer to free.

  @retval EFI_SUCCESS            The memory was returned to the system.
  @retval EFI_INVALID_PARAMETER  Buffer was invalid.
**/
EFI_STATUS
EfiFreePool (
  IN VOID  *Buffer
  )
{
  EFI_STATUS Status;

  ASSERT (Buffer != NULL);
  ASSERT (!EfiAtRuntime ());
  ASSERT (EfiGetCurrentTpl () <= TPL_NOTIFY);

  Status = gBS->FreePool (Buffer);

  ASSERT_EFI_ERROR (Status);

  return Status;
}

// Event & Timer Services

// EfiCreateEvent
/** Creates an event.
 
  @param[in]  Type            The type of event to create and its mode and
                              attributes.
  @param[in]  NotifyTpl       The task priority level of event notifications, if
                              needed.
  @param[in]  NotifyFunction  The pointer to the event's notification function,
                              if any.
  @param[in]  NotifyContext   The pointer to the notification function's
                              context; corresponds to parameter Context in the
                              notification function.
  @param[out] Event           The pointer to the newly created event if the
                              call succeeds; undefined otherwise.

  @retval EFI_SUCCESS            The event structure was created.
  @retval EFI_INVALID_PARAMETER  One or more parameters are invalid.
  @retval EFI_OUT_OF_RESOURCES   The event could not be allocated.
**/
EFI_STATUS
EfiCreateEvent (
  IN  UINT32            Type,
  IN  EFI_TPL           NotifyTpl,
  IN  EFI_EVENT_NOTIFY  NotifyFunction,
  IN  VOID              *NotifyContext,
  OUT EFI_EVENT         *Event
  )
{
  EFI_STATUS Status;

  ASSERT (Type != 0);
  ASSERT ((Type & ~(EVT_TIMER
                  | EVT_RUNTIME
                  | EVT_NOTIFY_WAIT
                  | EVT_NOTIFY_SIGNAL
                  | EVT_SIGNAL_EXIT_BOOT_SERVICES
                  | EVT_SIGNAL_VIRTUAL_ADDRESS_CHANGE)) == 0);

  ASSERT ((((Type & (EVT_NOTIFY_SIGNAL | EVT_NOTIFY_WAIT)) != 0) ? 1 : 0)
         ^ ((NotifyFunction == NULL) ? 1 : 0) != 0);

  ASSERT (Event != NULL);
  ASSERT (!EfiAtRuntime ());
  ASSERT (EfiGetCurrentTpl () < TPL_HIGH_LEVEL);

  Status = gBS->CreateEvent (
                  Type,
                  NotifyTpl,
                  NotifyFunction,
                  NotifyContext,
                  Event
                  );

  ASSERT_EFI_ERROR (Status);

  return Status;
}

// EfiSetTimer
/** Sets the type of timer and the trigger time for a timer event.

  @param[in] Event        The timer event that is to be signaled at the
                          specified time.
  @param[in] Type         The type of time that is specified in TriggerTime.
  @param[in] TriggerTime  The number of 100ns units until the timer expires.
                          A TriggerTime of 0 is legal.
                          If Type is TimerRelative and TriggerTime is 0, then
                          the timer event will be signaled on the next timer
                          tick.
                          If Type is TimerPeriodic and TriggerTime is 0, then
                          the timer event will be signaled on every timer tick.

  @retval EFI_SUCCESS            The event has been set to be signaled at the
                                 requested time.
  @retval EFI_INVALID_PARAMETER  Event or Type is not valid.
**/
EFI_STATUS
EfiSetTimer (
  IN EFI_EVENT        Event,
  IN EFI_TIMER_DELAY  Type,
  IN UINT64           TriggerTime
  )
{
  EFI_STATUS Status;

  ASSERT (Event != NULL);
  ASSERT ((Type >= TimerCancel) && (Type <= TimerRelative));
  ASSERT (!EfiAtRuntime ());
  ASSERT (EfiGetCurrentTpl () < TPL_HIGH_LEVEL);

  Status = gBS->SetTimer (Event, Type, TriggerTime);

  ASSERT_EFI_ERROR (Status);

  return Status;
}

// EfiWaitForEvent
/** Stops execution until an event is signaled.

  @param[in]  NumberOfEvents  The number of events in the Event array.
  @param[in]  Event           An array of EFI_EVENT.
  @param[out] Index           The pointer to the index of the event which
                              satisfied the wait condition.

  @retval EFI_SUCCESS            The event indicated by Index was signaled.
  @retval EFI_INVALID_PARAMETER  1) NumberOfEvents is 0.
                                 2) The event indicated by Index is of type
                                 EVT_NOTIFY_SIGNAL.
  @retval EFI_UNSUPPORTED        The current TPL is not TPL_APPLICATION.
**/
EFI_STATUS
EfiWaitForEvent (
  IN  UINTN      NumberOfEvents,
  IN  EFI_EVENT  *Event,
  OUT UINTN      *Index
  )
{
  EFI_STATUS Status;

  ASSERT (NumberOfEvents > 0);
  ASSERT (Event != NULL);
  ASSERT (Index != NULL);
  ASSERT (!EfiAtRuntime ());
  ASSERT (EfiGetCurrentTpl () == TPL_APPLICATION);

  Status = gBS->WaitForEvent (NumberOfEvents, Event, Index);

  ASSERT_EFI_ERROR (Status);

  return Status;
}

// EfiSignalEvent
/** Signals an event.

  @param[in] Event  The event to signal.

  @retval EFI_SUCCESS  The event has been signaled.
**/
EFI_STATUS
EfiSignalEvent (
  IN EFI_EVENT  Event
  )
{
  EFI_STATUS Status;

  ASSERT (Event != NULL);
  ASSERT (!EfiAtRuntime ());
  ASSERT (EfiGetCurrentTpl () <= TPL_HIGH_LEVEL);

  Status = gBS->SignalEvent (Event);

  ASSERT_EFI_ERROR (Status);

  return Status;
}

// EfiCloseEvent
/** Closes an event.

  @param[in] Event  The event to close.

  @retval EFI_SUCCESS  The event has been closed.
**/
EFI_STATUS
EfiCloseEvent (
  IN EFI_EVENT  Event
  )
{
  EFI_STATUS Status;

  ASSERT (Event != NULL);
  ASSERT (!EfiAtRuntime ());
  ASSERT (EfiGetCurrentTpl () < TPL_HIGH_LEVEL);

  Status = gBS->CloseEvent (Event);

  ASSERT_EFI_ERROR (Status);

  return Status;
}

// EfiCheckEvent
/** Checks whether an event is in the signaled state.

  @param[in] Event  The event to check.

  @retval EFI_SUCCESS            The event is in the signaled state.
  @retval EFI_NOT_READY          The event is not in the signaled state.
  @retval EFI_INVALID_PARAMETER  Event is of type EVT_NOTIFY_SIGNAL.
**/
EFI_STATUS
EfiCheckEvent (
  IN EFI_EVENT  Event
  )
{
  EFI_STATUS Status;

  ASSERT (Event != NULL);
  ASSERT (!EfiAtRuntime ());
  ASSERT (EfiGetCurrentTpl () < TPL_HIGH_LEVEL);

  Status = gBS->CheckEvent (Event);

  if (Status != EFI_NOT_READY) {
    ASSERT_EFI_ERROR (Status);
  }

  return Status;
}

// Protocol Handler Services

// EfiInstallProtocolInterface
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
EfiInstallProtocolInterface (
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
  ASSERT (!EfiAtRuntime ());
  ASSERT (EfiGetCurrentTpl () <= TPL_NOTIFY);

  if (*Handle != NULL) {
    ASSERT_PROTOCOL_ALREADY_INSTALLED (*Handle, Protocol);
  }

  Status = gBS->InstallProtocolInterface (
                  Handle,
                  Protocol,
                  InterfaceType,
                  Interface
                  );

  ASSERT_EFI_ERROR (Status);

  return Status;
}

// EfiReinstallProtocolInterface
/** Reinstalls a protocol interface on a device handle.

  @param[in] Handle        Handle on which the interface is to be reinstalled.
  @param[in] Protocol      The numeric ID of the interface.
  @param[in] OldInterface  A pointer to the old interface.  NULL can be used if
                           a structure is not associated with Protocol.
  @param[in] NewInterface  A pointer to the new interface.

  @retval EFI_SUCCESS            The protocol interface was reinstalled.
  @retval EFI_NOT_FOUND          The OldInterface on the handle was not found.
  @retval EFI_ACCESS_DENIED      The protocol interface could not be
                                 reinstalled, because OldInterface is still
                                 being used by a driver that will not release
                                 it.
  @retval EFI_INVALID_PARAMETER  Handle is NULL.
  @retval EFI_INVALID_PARAMETER  Protocol is NULL.
**/
EFI_STATUS
EfiReinstallProtocolInterface (
  IN EFI_HANDLE  Handle,
  IN EFI_GUID    *Protocol,
  IN VOID        *OldInterface,
  IN VOID        *NewInterface
  )
{
  EFI_STATUS Status;

  VOID *Interface;

  ASSERT (Handle != NULL);
  ASSERT (Protocol != NULL);
  ASSERT (!EfiAtRuntime ());
  ASSERT (EfiGetCurrentTpl () <= TPL_NOTIFY);
  ASSERT (EfiHandleProtocol (Handle, Protocol, &Interface) != EFI_UNSUPPORTED);

  Status = gBS->ReinstallProtocolInterface (
                  Handle,
                  Protocol,
                  OldInterface,
                  NewInterface
                  );

  if (Status != EFI_ACCESS_DENIED) {
    ASSERT_EFI_ERROR (Status);
  }

  return Status;
}

// EfiUninstallProtocolInterface
/** Removes a protocol interface from a device handle.  It is recommended that
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
EfiUninstallProtocolInterface (
  IN EFI_HANDLE  Handle,
  IN EFI_GUID    *Protocol,
  IN VOID        *Interface
  )
{
  EFI_STATUS Status;

  ASSERT (Handle != NULL);
  ASSERT (Protocol != NULL);
  ASSERT (!EfiAtRuntime ());
  ASSERT (EfiGetCurrentTpl () <= TPL_NOTIFY);

  Status = gBS->UninstallProtocolInterface (Handle, Protocol, Interface);

  if (Status != EFI_ACCESS_DENIED) {
    ASSERT_EFI_ERROR (Status);
  }

  return Status;
}

// EfiHandleProtocol
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
EfiHandleProtocol (
  IN  EFI_HANDLE  Handle,
  IN  EFI_GUID    *Protocol,
  OUT VOID        **Interface
  )
{
  EFI_STATUS Status;

  ASSERT (Handle != NULL);
  ASSERT (Protocol != NULL);
  ASSERT (Interface != NULL);
  ASSERT (!EfiAtRuntime ());
  ASSERT (EfiGetCurrentTpl () <= TPL_NOTIFY);

  Status = gBS->HandleProtocol (Handle, Protocol, Interface);

  if (Status != EFI_UNSUPPORTED) {
    ASSERT_EFI_ERROR (Status);
  }

  return Status;
}

// EfiRegisterProtocolNotify
/** Creates an event that is to be signaled whenever an interface is installed
    for a specified protocol.

  @param[in]  Protocol      The numeric ID of the protocol for which the event
                            is to be registered.
  @param[in]  Event         Event that is to be signaled whenever a protocol
                            interface is registered for Protocol.
  @param[out] Registration  A pointer to a memory location to receive the
                            registration value.

  @retval EFI_SUCCESS            The notification event has been registered.
  @retval EFI_OUT_OF_RESOURCES   Space for the notification event could not be
                                 allocated.
  @retval EFI_INVALID_PARAMETER  Protocol is NULL.
  @retval EFI_INVALID_PARAMETER  Event is NULL.
  @retval EFI_INVALID_PARAMETER  Registration is NULL.
**/
EFI_STATUS
EfiRegisterProtocolNotify (
  IN  EFI_GUID   *Protocol,
  IN  EFI_EVENT  Event,
  OUT VOID       **Registration
  )
{
  EFI_STATUS Status;

  ASSERT (Protocol != NULL);
  ASSERT (Event != NULL);
  ASSERT (Registration != NULL);
  ASSERT (!EfiAtRuntime ());
  ASSERT (EfiGetCurrentTpl () <= TPL_NOTIFY);

  Status = gBS->RegisterProtocolNotify (Protocol, Event, Registration);

  ASSERT_EFI_ERROR (Status);

  return Status;
}

// EfiLocateHandle
/** Returns an array of handles that support a specified protocol.

  @param[in]      SearchType  Specifies which handle(s) are to be returned.
  @param[in]      Protocol    Specifies the protocol to search by.
  @param[in]      SearchKey   Specifies the search key.
  @param[in, out] BufferSize  On input, the size in bytes of Buffer.
                              On output, the size in bytes of the array
                              returned in Buffer (if the Buffer was large
                              enough) or the size, in bytes, of the Buffer
                              needed to obtain the array (if the Buffer was not
                              large enough).
  @param[out]     Buffer      The Buffer in which the array is returned.

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
EfiLocateHandle (
  IN     EFI_LOCATE_SEARCH_TYPE  SearchType,
  IN     EFI_GUID                *Protocol, OPTIONAL
  IN     VOID                    *SearchKey, OPTIONAL
  IN OUT UINTN                   *BufferSize,
  OUT    EFI_HANDLE              *Buffer
  )
{
  EFI_STATUS Status;

  ASSERT ((SearchType >= AllHandles) && (SearchType <= ByProtocol));
  ASSERT ((((SearchType == ByRegisterNotify) ? 1 : 0)
         ^ ((SearchKey == NULL) ? 1 : 0)) != 0);

  ASSERT ((((SearchType == ByProtocol) ? 1 : 0)
         ^ ((Protocol == NULL) ? 1 : 0)) != 0);

  ASSERT (BufferSize != NULL);
  ASSERT ((((*BufferSize > 0) ? 1 : 0) ^ ((Buffer == NULL) ? 1 : 0)) != 0);
  ASSERT (!EfiAtRuntime ());
  ASSERT (EfiGetCurrentTpl () <= TPL_NOTIFY);

  Status = gBS->LocateHandle (
                  SearchType,
                  Protocol,
                  SearchKey,
                  BufferSize,
                  Buffer
                  );

  if ((Status != EFI_NOT_FOUND) && (Status != EFI_BUFFER_TOO_SMALL)) {
    ASSERT_EFI_ERROR (Status);
  }

  return Status;
}

// EfiLocateDevicePath
/** Locates the handle to a device on the device path that supports the
    specified protocol.

  @param[in]      Protocol    Specifies the protocol to search for.
  @param[in, out] DevicePath  On input, a pointer to a pointer to the device
                              path.
                              On output, the device path pointer is modified to
                              point to the remaining part of the device path.
  @param[out]     Device      A pointer to the returned device handle.

  @retval EFI_SUCCESS            The resulting handle was returned.
  @retval EFI_NOT_FOUND          No handles match the search.
  @retval EFI_INVALID_PARAMETER  Protocol is NULL.
  @retval EFI_INVALID_PARAMETER  DevicePath is NULL.
  @retval EFI_INVALID_PARAMETER  A handle matched the search and Device is
                                 NULL.
**/
EFI_STATUS
EfiLocateDevicePath (
  IN     EFI_GUID                  *Protocol,
  IN OUT EFI_DEVICE_PATH_PROTOCOL  **DevicePath,
  OUT    EFI_HANDLE                *Device
  )
{
  EFI_STATUS Status;

  ASSERT (Protocol != NULL);
  ASSERT (DevicePath != NULL);
  ASSERT (Device != NULL);
  ASSERT (!EfiAtRuntime ());
  ASSERT (EfiGetCurrentTpl () <= TPL_NOTIFY);

  Status = gBS->LocateDevicePath (Protocol, DevicePath, Device);

  if (Status != EFI_NOT_FOUND) {
    ASSERT_EFI_ERROR (Status);
  }

  return Status;
}

// EfiInstallConfigurationTable
/** Adds, updates, or removes a configuration table entry from the EFI System
    Table.

  @param[in] Guid   A pointer to the GUID for the entry to add, update, or
                    remove.
  @param[in] Table  A pointer to the configuration table for the entry to add,
                    update, or remove.  May be NULL.

  @retval EFI_SUCCESS            The (Guid, Table) pair was added, updated, or
                                 removed.
  @retval EFI_NOT_FOUND          An attempt was made to delete a nonexistent
                                 entry.
  @retval EFI_INVALID_PARAMETER  Guid is NULL.
  @retval EFI_OUT_OF_RESOURCES   There is not enough memory available to
                                 complete the operation.
**/
EFI_STATUS
EfiInstallConfigurationTable (
  IN EFI_GUID  *Guid,
  IN VOID      *Table OPTIONAL
  )
{
  EFI_STATUS Status;

  ASSERT (Guid != NULL);
  ASSERT (!EfiAtRuntime ());
  ASSERT (EfiGetCurrentTpl () <= TPL_NOTIFY);

  Status = gBS->InstallConfigurationTable (Guid, Table);

  if (Status != EFI_NOT_FOUND) {
    ASSERT_EFI_ERROR (Status);
  }

  return Status;
}

// Image Services

// EfiLoadImage
/** Loads an EFI image into memory.

  @param[in]  BootPolicy         If TRUE, indicates that the request originates
                                 from the boot manager, and that the boot
                                 manager is attempting to load FilePath as a
                                 boot selection.  Ignored if SourceBuffer is
                                 not NULL.
  @param[in]  ParentImageHandle  The caller's image handle.
  @param[in]  DevicePath         The DeviceHandle specific file path from which
                                 the image is loaded.
  @param[in]  SourceBuffer       If not NULL, a pointer to the memory location
                                 containing a copy of the image to be loaded.
  @param[in]  SourceSize         The size in bytes of SourceBuffer.  Ignored if
                                 SourceBuffer is NULL.
  @param[out] ImageHandle        The pointer to the returned image handle that
                                 is created when the image is successfully
                                 loaded.

  @retval EFI_SUCCESS             Image was loaded into memory correctly.
  @retval EFI_NOT_FOUND           Both SourceBuffer and DevicePath are NULL.
  @retval EFI_INVALID_PARAMETER   One or more parametes are invalid.
  @retval EFI_UNSUPPORTED         The image type is not supported.
  @retval EFI_OUT_OF_RESOURCES    Image was not loaded due to insufficient
                                  resources.
  @retval EFI_LOAD_ERROR          Image was not loaded because the image format
                                  was corrupt or not understood.
  @retval EFI_DEVICE_ERROR        Image was not loaded because the device
                                  returned a read error.
  @retval EFI_ACCESS_DENIED       Image was not loaded because the platform
                                  policy prohibits the image from being loaded.
                                  NULL is returned in *ImageHandle.
  @retval EFI_SECURITY_VIOLATION  Image was loaded and an ImageHandle was
                                  created with a valid
                                  EFI_LOADED_IMAGE_PROTOCOL.  However, the
                                  current platform policy specifies that the
                                  image should not be started.
**/
EFI_STATUS
EfiLoadImage (
  IN  BOOLEAN                   BootPolicy,
  IN  EFI_HANDLE                ParentImageHandle,
  IN  EFI_DEVICE_PATH_PROTOCOL  *DevicePath,
  IN  VOID                      *SourceBuffer, OPTIONAL
  IN  UINTN                     SourceSize,
  OUT EFI_HANDLE                *ImageHandle
  )
{
  EFI_STATUS Status;

  ASSERT (ParentImageHandle != NULL);
  ASSERT ((DevicePath != NULL) || (SourceBuffer != NULL));
  ASSERT (SourceSize > 0);
  ASSERT (ImageHandle != NULL);
  ASSERT (!EfiAtRuntime ());
  ASSERT (EfiGetCurrentTpl () < TPL_CALLBACK);

  Status = gBS->LoadImage (
                  BootPolicy,
                  ParentImageHandle,
                  DevicePath,
                  SourceBuffer,
                  SourceSize,
                  ImageHandle
                  );

  if ((Status != EFI_UNSUPPORTED)
   && (Status != EFI_LOAD_ERROR)
   && (Status != EFI_ACCESS_DENIED)) {
    ASSERT_EFI_ERROR (Status);
  }

  return Status;
}

// EfiStartImage
/** Transfers control to a loaded image's entry point.

  @param[in]  ImageHandle   Handle of image to be started.
  @param[out] ExitDataSize  The pointer to the size, in bytes, of ExitData.
  @param[out] ExitData      The pointer to a pointer to a data Buffer that
                            includes a Null-terminated string, optionally
                            followed by additional binary data.

  @return                         Exit code from image.
  @retval EFI_INVALID_PARAMETER   ImageHandle is either an invalid image handle
                                  or the image
                                  has already been initialized with StartImage.
  @retval EFI_SECURITY_VIOLATION  The current platform policy specifies that
                                  the image should not be started.
**/
EFI_STATUS
EfiStartImage (
  IN  EFI_HANDLE  ImageHandle,
  OUT UINTN       *ExitDataSize,
  OUT CHAR16      **ExitData OPTIONAL
  )
{
  EFI_STATUS Status;

  ASSERT (ImageHandle != NULL);
  ASSERT (!EfiAtRuntime ());
  ASSERT (EfiGetCurrentTpl () <= TPL_CALLBACK);

  Status = gBS->StartImage (ImageHandle, ExitDataSize, ExitData);

  if (Status != EFI_SECURITY_VIOLATION) {
    ASSERT_EFI_ERROR (Status);
  }

  return Status;
}

// EfiExit
/** Terminates a loaded EFI image and returns control to boot services.

  @param[in] ImageHandle   Handle that identifies the image.  This parameter is
                           passed to the image on entry.
  @param[in] ExitStatus    The image's exit code.
  @param[in] ExitDataSize  The size, in bytes, of ExitData.  Ignored if
                           ExitStatus is EFI_SUCCESS.
  @param[in] ExitData      The pointer to a data Buffer that includes a
                           Null-terminated string, optionally followed by
                           additional binary data.  The string is a description
                           that the caller may use to further indicate the
                           reason for the image's exit.  ExitData is only valid
                           if ExitStatus is something other than EFI_SUCCESS.
                           The ExitData Buffer must be allocated by calling
                           AllocatePool().

  @retval EFI_SUCCESS            The image specified by ImageHandle was
                                 unloaded.
  @retval EFI_INVALID_PARAMETER  The image specified by ImageHandle has been
                                 loaded and started with LoadImage() and
                                 StartImage(), but the image is not the
                                 currently executing image.
**/
EFI_STATUS
EfiExit (
  IN EFI_HANDLE  ImageHandle,
  IN EFI_STATUS  ExitStatus,
  IN UINTN       ExitDataSize,
  IN CHAR16      *ExitData OPTIONAL
  )
{
  EFI_STATUS Status;

  ASSERT (ImageHandle != NULL);
  ASSERT ((ExitDataSize == 0) || (ExitData != NULL));
  ASSERT (!EfiAtRuntime ());
  ASSERT (EfiGetCurrentTpl () <= TPL_CALLBACK);

  Status = gBS->Exit (ImageHandle, ExitStatus, ExitDataSize, ExitData);

  ASSERT_EFI_ERROR (Status);

  return Status;
}

// EfiUnloadImage
/** Unloads an image.

  @param[in] ImageHandle  Handle that identifies the image to be unloaded.

  @retval EFI_SUCCESS            The image has been unloaded.
  @retval EFI_INVALID_PARAMETER  ImageHandle is not a valid image handle.
**/
EFI_STATUS
EfiUnloadImage (
  IN EFI_HANDLE  ImageHandle
  )
{
  EFI_STATUS Status;

  ASSERT (ImageHandle != NULL);
  ASSERT (!EfiAtRuntime ());
  ASSERT (EfiGetCurrentTpl () <= TPL_CALLBACK);

  Status = gBS->UnloadImage (ImageHandle);

  ASSERT_EFI_ERROR (Status);

  return Status;
}

// EfiExitBootServices
/** Terminates all boot services.

  @param[in] ImageHandle  Handle that identifies the exiting image.
  @param[in] MapKey       Key to the latest memory map.

  @retval EFI_SUCCESS            Boot services have been terminated.
  @retval EFI_INVALID_PARAMETER  MapKey is incorrect.
**/
EFI_STATUS
EfiExitBootServices (
  IN EFI_HANDLE  ImageHandle,
  IN UINTN       MapKey
  )
{
  EFI_STATUS Status;

  ASSERT (ImageHandle != NULL);
  ASSERT (MapKey != 0);
  ASSERT (!EfiAtRuntime ());
  ASSERT (EfiGetCurrentTpl () == TPL_APPLICATION);

  Status = gBS->ExitBootServices (ImageHandle, MapKey);

  return Status;
}

// Miscellaneous Services

// EfiGetNextMonotonicCount
/** Returns a monotonically increasing count for the platform.

  @param[out] Count  The pointer to returned value.

  @retval EFI_SUCCESS            The next monotonic count was returned.
  @retval EFI_INVALID_PARAMETER  Count is NULL.
  @retval EFI_DEVICE_ERROR       The device is not functioning properly.
**/
EFI_STATUS
EfiGetNextMonotonicCount (
  OUT UINT64  *Count
  )
{
  EFI_STATUS Status;

  ASSERT (Count != NULL);
  ASSERT (!EfiAtRuntime ());
  ASSERT (EfiGetCurrentTpl () <= TPL_NOTIFY);

  Status = gBS->GetNextMonotonicCount (Count);

  ASSERT_EFI_ERROR (Status);

  return Status;
}

// EfiStall
/** Induces a fine-grained stall.

  @param[in] Microseconds  The number of microseconds to stall execution.

  @retval EFI_SUCCESS  Execution was stalled at least the requested number of
                       Microseconds.
**/
EFI_STATUS
EfiStall (
  IN UINTN  Microseconds
  )
{
  EFI_STATUS Status;

  ASSERT (Microseconds > 0);
  ASSERT (EfiGetCurrentTpl () <= TPL_NOTIFY);

  Status = gBS->Stall (Microseconds);

  ASSERT_EFI_ERROR (Status);

  return Status;
}

// EfiSetWatchdogTimer
/** Sets the system's watchdog timer.
 
  @param[in] Timeout       The number of seconds to set the watchdog timer to.
  @param[in] WatchdogCode  The numeric code to log on a watchdog timer timeout
                           event.
  @param[in] DataSize      The size, in bytes, of WatchdogData.
  @param[in] WatchdogData  A data Buffer that includes a Null-terminated
                           string, optionally followed by additional binary
                           data.

  @retval EFI_SUCCESS            The timeout has been set.
  @retval EFI_INVALID_PARAMETER  The supplied WatchdogCode is invalid.
  @retval EFI_UNSUPPORTED        The system does not have a watchdog timer.
  @retval EFI_DEVICE_ERROR       The watchdog timer could not be programmed due
                                 to a hardware error.
**/
EFI_STATUS
EfiSetWatchdogTimer (
  IN UINTN   Timeout,
  IN UINT64  WatchdogCode,
  IN UINTN   DataSize,
  IN CHAR16  *WatchdogData OPTIONAL
  )
{
  EFI_STATUS Status;

  ASSERT ((((DataSize > 0) ? 1 : 0) ^ ((WatchdogData == NULL) ? 1 : 0)) != 0);
  ASSERT (!EfiAtRuntime ());
  ASSERT (EfiGetCurrentTpl () <= TPL_NOTIFY);

  Status = gBS->SetWatchdogTimer (
                  Timeout,
                  WatchdogCode,
                  DataSize,
                  WatchdogData
                  );

  if ((Status != EFI_UNSUPPORTED) && (Status != EFI_DEVICE_ERROR)) {
    ASSERT_EFI_ERROR (Status);
  }

  return Status;
}

// DriverSupport Services

// EfiConnectController
/** Connects one or more drivers to a controller.

  @param[in] ControllerHandle     The handle of the controller to which
                                  driver(s) are to be connected.
  @param[in] DriverImageHandle    A pointer to an ordered list handles that
                                  support the EFI_DRIVER_BINDING_PROTOCOL.
  @param[in] RemainingDevicePath  A pointer to the device path that specifies a
                                  child of the controller specified by
                                  ControllerHandle.
  @param[in] Recursive            If TRUE, then ConnectController() is called
                                  recursively until the entire tree of
                                  controllers below the controller specified by
                                  ControllerHandle have been created.
                                  If FALSE, then the tree of controllers is
                                  only expanded one level.

  @retval EFI_SUCCESS             1) One or more drivers were connected to
                                  ControllerHandle.
                                  2) No drivers were connected to
                                  ControllerHandle, but RemainingDevicePath is
                                  not NULL, and it is an End Device Path Node.
  @retval EFI_INVALID_PARAMETER   ControllerHandle is NULL.
  @retval EFI_NOT_FOUND           1) There are no EFI_DRIVER_BINDING_PROTOCOL
                                  instances present in the system.
                                  2) No drivers were connected to
                                  ControllerHandle.
  @retval EFI_SECURITY_VIOLATION  The user has no permission to start UEFI
                                  device drivers on the device path associated
                                  with the ControllerHandle or specified by the
                                  RemainingDevicePath.
**/
EFI_STATUS
EfiConnectController (
  IN EFI_HANDLE                ControllerHandle,
  IN EFI_HANDLE                *DriverImageHandle, OPTIONAL
  IN EFI_DEVICE_PATH_PROTOCOL  *RemainingDevicePath, OPTIONAL
  IN BOOLEAN                   Recursive
  )
{
  EFI_STATUS Status;

  ASSERT (ControllerHandle != NULL);
  ASSERT (!EfiAtRuntime ());
  ASSERT (EfiGetCurrentTpl () <= TPL_NOTIFY);

  Status = gBS->ConnectController (
                  ControllerHandle,
                  DriverImageHandle,
                  RemainingDevicePath,
                  Recursive
                  );

  if (Status != EFI_NOT_FOUND) {
    ASSERT_EFI_ERROR (Status);
  }

  return Status;
}

// EfiDisconnectController
/** Disconnects one or more drivers from a controller.

  @param[in] ControllerHandle   The handle of the controller from which
                                driver(s) are to be disconnected.
  @param[in] DriverImageHandle  The driver to disconnect from ControllerHandle.
                                If DriverImageHandle is NULL, then all the
                                drivers currently managing ControllerHandle are
                                disconnected from ControllerHandle.
  @param[in] ChildHandle        The handle of the child to destroy.
                                If ChildHandle is NULL, then all the children
                                of ControllerHandle are destroyed before the
                                drivers are disconnected from ControllerHandle.

  @retval EFI_SUCCESS            1) One or more drivers were disconnected from
                                 the controller.
                                 2) On entry, no drivers are managing
                                 ControllerHandle.
                                 3) DriverImageHandle is not NULL, and on entry
                                 DriverImageHandle is not managing
                                 ControllerHandle.
  @retval EFI_INVALID_PARAMETER  1) ControllerHandle is NULL.
                                 2) DriverImageHandle is not NULL, and it is
                                 not a valid EFI_HANDLE.
                                 3) ChildHandle is not NULL, and it is not a
                                 valid EFI_HANDLE.
                                 4) DriverImageHandle does not support the
                                 EFI_DRIVER_BINDING_PROTOCOL.
  @retval EFI_OUT_OF_RESOURCES   There are not enough resources available to
                                 disconnect any drivers from ControllerHandle.
  @retval EFI_DEVICE_ERROR       The controller could not be disconnected
                                 because of a device error.
**/
EFI_STATUS
EfiDisconnectController (
  IN EFI_HANDLE  ControllerHandle,
  IN EFI_HANDLE  DriverImageHandle, OPTIONAL
  IN EFI_HANDLE  ChildHandle OPTIONAL
  )
{
  EFI_STATUS Status;

  ASSERT (ControllerHandle != NULL);
  ASSERT (!EfiAtRuntime ());
  ASSERT (EfiGetCurrentTpl () <= TPL_NOTIFY);

  Status = gBS->DisconnectController (
                  ControllerHandle,
                  DriverImageHandle,
                  ChildHandle
                  );

  if ((Status != EFI_OUT_OF_RESOURCES) && (Status != EFI_DEVICE_ERROR)) {
    ASSERT_EFI_ERROR (Status);
  }

  return Status;
}

// Open and Close Protocol Services

// EfiOpenProtocol
/** Queries a handle to determine if it supports a specified protocol.  If the
    protocol is supported by the handle, it opens the protocol on behalf of the
    calling agent.

  @param[in]  Handle            The handle for the protocol interface that is
                                being opened.
  @param[in]  Protocol          The published unique identifier of the
                                protocol.
  @param[out] Interface         Supplies the address where a pointer to the
                                corresponding Protocol Interface is returned.
  @param[in]  AgentHandle       The handle of the agent that is opening the
                                protocol interface specified by Protocol and
                                Interface.
  @param[in]  ControllerHandle  If the agent that is opening a protocol is a
                                driver that follows the UEFI Driver Model, then
                                this parameter is the controller handle that
                                requires the protocol interface.  If the agent
                                does not follow the UEFI Driver Model, then
                                this parameter is optional and may be NULL.
  @param[in]  Attributes        The open mode of the protocol interface
                                specified by Handle and Protocol.

  @retval EFI_SUCCESS            An item was added to the open list for the
                                 protocol interface, and the protocol interface
                                 was returned in Interface.
  @retval EFI_UNSUPPORTED        Handle does not support Protocol.
  @retval EFI_INVALID_PARAMETER  One or more parameters are invalid.
  @retval EFI_ACCESS_DENIED      Required attributes can't be supported in
                                 current environment.
  @retval EFI_ALREADY_STARTED    Item on the open list already has requierd
                                 attributes whose agent handle is the same as
                                 AgentHandle.
**/
EFI_STATUS
EfiOpenProtocol (
  IN  EFI_HANDLE  Handle,
  IN  EFI_GUID    *Protocol,
  OUT VOID        **Interface, OPTIONAL
  IN  EFI_HANDLE  AgentHandle,
  IN  EFI_HANDLE  ControllerHandle,
  IN  UINT32      Attributes
  )
{
  EFI_STATUS Status;

  ASSERT (Handle != NULL);
  ASSERT (Protocol != NULL);
  ASSERT ((Attributes != 0) && ((Attributes & ~0x3F) == 0));
  ASSERT (((((Attributes & EFI_OPEN_PROTOCOL_TEST_PROTOCOL) != 0) ? 1 : 0)
             ^ ((Interface == NULL) ? 1 : 0)) != 0);

  ASSERT (!((Attributes == EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER)
        || (Attributes == EFI_OPEN_PROTOCOL_BY_DRIVER)
        || (Attributes == EFI_OPEN_PROTOCOL_EXCLUSIVE)
        || (Attributes ==
             (EFI_OPEN_PROTOCOL_BY_DRIVER | EFI_OPEN_PROTOCOL_EXCLUSIVE)))
       || (AgentHandle != NULL));

  ASSERT (!((Attributes == EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER)
        || (Attributes == EFI_OPEN_PROTOCOL_BY_DRIVER)
        || (Attributes ==
             (EFI_OPEN_PROTOCOL_BY_DRIVER | EFI_OPEN_PROTOCOL_EXCLUSIVE)))
       || (ControllerHandle != NULL));

  ASSERT ((Attributes != EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER)
      || (ControllerHandle != Handle));

  ASSERT (!EfiAtRuntime ());
  ASSERT (EfiGetCurrentTpl () <= TPL_NOTIFY);

  Status = gBS->OpenProtocol (
                  Handle,
                  Protocol,
                  Interface,
                  AgentHandle,
                  ControllerHandle,
                  Attributes
                  );

  if (Status != EFI_UNSUPPORTED) {
    ASSERT_EFI_ERROR (Status);
  }

  return Status;
}

// EfiCloseProtocol
/** Closes a protocol on a handle that was opened using OpenProtocol().

  @param[in] Handle            The handle for the protocol interface that was
                               previously opened with OpenProtocol(), and is
                               now being closed.
  @param[in] Protocol          The published unique identifier of the protocol.
  @param[in] AgentHandle       The handle of the agent that is closing the
                               protocol interface.
  @param[in] ControllerHandle  If the agent that opened a protocol is a driver
                               that follows the UEFI Driver Model, then this
                               parameter is the controller handle that required
                               the protocol interface.

  @retval EFI_SUCCESS            The protocol instance was closed.
  @retval EFI_INVALID_PARAMETER  1) Handle is NULL.
                                 2) AgentHandle is NULL.
                                 3) ControllerHandle is not NULL and
                                 ControllerHandle is not a valid EFI_HANDLE.
                                 4) Protocol is NULL.
  @retval EFI_NOT_FOUND          1) Handle does not support the protocol
                                 specified by Protocol.
                                 2) The protocol interface specified by Handle
                                 and Protocol is not currently open by
                                 AgentHandle and ControllerHandle.
**/
EFI_STATUS
EfiCloseProtocol (
  IN EFI_HANDLE  Handle,
  IN EFI_GUID    *Protocol,
  IN EFI_HANDLE  AgentHandle,
  IN EFI_HANDLE  ControllerHandle
  )
{
  EFI_STATUS Status;

  ASSERT (Handle != NULL);
  ASSERT (Protocol != NULL);
  ASSERT (AgentHandle != NULL);
  ASSERT (!EfiAtRuntime ());
  ASSERT (EfiGetCurrentTpl () <= TPL_NOTIFY);

  Status = gBS->CloseProtocol (
                  Handle,
                  Protocol,
                  AgentHandle,
                  ControllerHandle
                  );

  ASSERT_EFI_ERROR (Status);

  return Status;
}

// EfiOpenProtocolInformation
/** Retrieves the list of agents that currently have a protocol interface
    opened.

  @param[in]  Handle       The handle for the protocol interface that is being
                           queried.
  @param[in]  Protocol     The published unique identifier of the protocol.
  @param[out] EntryBuffer  A pointer to a Buffer of open protocol information
                           in the form of EFI_OPEN_PROTOCOL_INFORMATION_ENTRY
                           structures.
  @param[out] EntryCount   A pointer to the number of entries in EntryBuffer.

  @retval EFI_SUCCESS           The open protocol information was returned in
                                EntryBuffer, and the number of entries was
                                returned EntryCount.
  @retval EFI_OUT_OF_RESOURCES  There are not enough resources available to
                                allocate EntryBuffer.
  @retval EFI_NOT_FOUND         Handle does not support the protocol specified
                                by Protocol.
**/
EFI_STATUS
EfiOpenProtocolInformation (
  IN  EFI_HANDLE                           Handle,
  IN  EFI_GUID                             *Protocol,
  OUT EFI_OPEN_PROTOCOL_INFORMATION_ENTRY  **EntryBuffer,
  OUT UINTN                                *EntryCount
  )
{
  EFI_STATUS Status;

  ASSERT (Handle != NULL);
  ASSERT (Protocol != NULL);
  ASSERT (EntryBuffer != NULL);
  ASSERT (EntryCount != NULL);
  ASSERT (!EfiAtRuntime ());
  ASSERT (EfiGetCurrentTpl () <= TPL_NOTIFY);

  Status = gBS->OpenProtocolInformation (
                  Handle,
                  Protocol,
                  EntryBuffer,
                  EntryCount
                  );

  if (Status != EFI_NOT_FOUND) {
    ASSERT_EFI_ERROR (Status);
  }

  return Status;
}

// Library Services

// EfiProtocolsPerHandle
/** Retrieves the list of protocol interface GUIDs that are installed on a
    handle in a Buffer allocated from pool.

  @param[in]  Handle               The handle from which to retrieve the list
                                   of protocol interface GUIDs.
  @param[out] ProtocolBuffer       A pointer to the list of protocol interface
                                   GUID pointers that are installed on Handle.
  @param[out] ProtocolBufferCount  A pointer to the number of GUID pointers
                                   present in ProtocolBuffer.

  @retval EFI_SUCCESS            The list of protocol interface GUIDs installed
                                 on Handle was returned in ProtocolBuffer.
                                 The number of protocol interface GUIDs was
                                 returned in ProtocolBufferCount.
  @retval EFI_OUT_OF_RESOURCES   There is not enough pool memory to store the
                                 results.
  @retval EFI_INVALID_PARAMETER  Handle is NULL.
  @retval EFI_INVALID_PARAMETER  Handle is not a valid EFI_HANDLE.
  @retval EFI_INVALID_PARAMETER  ProtocolBuffer is NULL.
  @retval EFI_INVALID_PARAMETER  ProtocolBufferCount is NULL.
**/
EFI_STATUS
EfiProtocolsPerHandle (
  IN  EFI_HANDLE  Handle,
  OUT EFI_GUID    ***ProtocolBuffer,
  OUT UINTN       *ProtocolBufferCount
  )
{
  EFI_STATUS Status;

  ASSERT (Handle != NULL);
  ASSERT (ProtocolBuffer != NULL);
  ASSERT (ProtocolBufferCount != NULL);
  ASSERT (!EfiAtRuntime ());
  ASSERT (EfiGetCurrentTpl () <= TPL_NOTIFY);

  Status = gBS->ProtocolsPerHandle (
                  Handle,
                  ProtocolBuffer,
                  ProtocolBufferCount
                  );

  ASSERT_EFI_ERROR (Status);

  return Status;
}

// EfiLocateHandleBuffer
/** Returns an array of handles that support the requested protocol in a Buffer
    allocated from pool.

  @param[in]      SearchType  Specifies which handle(s) are to be returned.
  @param[in]      Protocol    Provides the protocol to search by.
                              This parameter is only valid for a SearchType of
                              ByProtocol.
  @param[in]      SearchKey   Supplies the search key depending on the
                              SearchType.
  @param[in, out] NoHandles   The number of handles returned in Buffer.
  @param[out]     Buffer      A pointer to the Buffer to return the requested
                              array of handles that support Protocol.

  @retval EFI_SUCCESS            The array of handles was returned in Buffer,
                                 and the number of handles in Buffer was
                                 returned in NoHandles.
  @retval EFI_NOT_FOUND          No handles match the search.
  @retval EFI_OUT_OF_RESOURCES   There is not enough pool memory to store the
                                 matching results.
  @retval EFI_INVALID_PARAMETER  NoHandles is NULL.
  @retval EFI_INVALID_PARAMETER  Buffer is NULL.
**/
EFI_STATUS
EfiLocateHandleBuffer (
  IN     EFI_LOCATE_SEARCH_TYPE  SearchType,
  IN     EFI_GUID                *Protocol, OPTIONAL
  IN     VOID                    *SearchKey, OPTIONAL
  IN OUT UINTN                   *NoHandles,
  OUT    EFI_HANDLE              **Buffer
  )
{
  EFI_STATUS Status;

  ASSERT ((((SearchType == ByProtocol) ? 1 : 0)
             ^ ((Protocol == NULL) ? 1 : 0)) != 0);

  ASSERT (NoHandles != NULL);
  ASSERT (Buffer != NULL);
  ASSERT (!EfiAtRuntime ());
  ASSERT (EfiGetCurrentTpl () <= TPL_NOTIFY);

  Status = gBS->LocateHandleBuffer (
                  SearchType,
                  Protocol,
                  SearchKey,
                  NoHandles,
                  Buffer
                  );

  if (Status != EFI_NOT_FOUND) {
    ASSERT_EFI_ERROR (Status);
  }

  return Status;
}

// EfiLocateProtocol
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
EfiLocateProtocol (
  IN  EFI_GUID  *Protocol,
  IN  VOID      *Registration, OPTIONAL
  OUT VOID      **Interface
  )
{
  EFI_STATUS Status;

  ASSERT (Protocol != NULL);
  ASSERT (Interface != NULL);
  ASSERT (!EfiAtRuntime ());
  ASSERT (EfiGetCurrentTpl () <= TPL_NOTIFY);

  Status = gBS->LocateProtocol (Protocol, Registration, Interface);

  if (Status != EFI_NOT_FOUND) {
    ASSERT_EFI_ERROR (Status);
  }

  return Status;
}

// 32-bit CRC Services

// CalculateCrc32
/** Computes and returns a 32-bit CRC for a data Buffer.

  @param[in]  Data      A pointer to the Buffer on which the 32-bit CRC is to
                        be computed.
  @param[in]  DataSize  The number of bytes in the Buffer Data.
  @param[out] Crc32     The 32-bit CRC that was computed for the data Buffer
                        specified by Data and DataSize.

  @retval EFI_SUCCESS            The 32-bit CRC was computed for the data
                                 Buffer and returned in Crc32.
  @retval EFI_INVALID_PARAMETER  Data is NULL.
  @retval EFI_INVALID_PARAMETER  Crc32 is NULL.
  @retval EFI_INVALID_PARAMETER  DataSize is 0.
**/
EFI_STATUS
CalculateCrc32 (
  IN  VOID    *Data,
  IN  UINTN   DataSize,
  OUT UINT32  *Crc32
  )
{
  EFI_STATUS Status;

  ASSERT (Data != NULL);
  ASSERT (DataSize > 0);
  ASSERT (Crc32 != NULL);
  ASSERT (!EfiAtRuntime ());
  ASSERT (EfiGetCurrentTpl () <= TPL_NOTIFY);

  *Crc32 = 0;
  Status = gBS->CalculateCrc32 (Data, DataSize, Crc32);

  ASSERT_EFI_ERROR (Status);

  return Status;
}

// Miscellaneous Services

// EfiCreateEventEx
/** Creates an event in a group.

  @param[in]  Type            The type of event to create and its mode and
                              attributes.
  @param[in]  NotifyTpl       The task priority level of event notifications,
                              if needed.
  @param[in]  NotifyFunction  The pointer to the event's notification function,
                              if any.
  @param[in]  NotifyContext   The pointer to the notification function's
                              context; corresponds to parameter Context in the
                              notification function.
  @param[in]  EventGroup      The pointer to the unique identifier of the group
                              to which this event belongs.
                              If this is NULL, then the function behaves as if
                              the parameters were passed to CreateEvent.
  @param[out] Event           The pointer to the newly created event if the
                              call succeeds; undefined otherwise.

  @retval EFI_SUCCESS            The event structure was created.
  @retval EFI_INVALID_PARAMETER  One or more parameters are invalid.
  @retval EFI_OUT_OF_RESOURCES   The event could not be allocated.
**/
EFI_STATUS
EfiCreateEventEx (
  IN  UINT32            Type,
  IN  EFI_TPL           NotifyTpl,
  IN  EFI_EVENT_NOTIFY  NotifyFunction, OPTIONAL
  IN  CONST VOID        *NotifyContext, OPTIONAL
  IN  CONST EFI_GUID    *EventGroup, OPTIONAL
  OUT EFI_EVENT         *Event
  )
{
  EFI_STATUS Status;

  ASSERT ((((Type & (EVT_NOTIFY_SIGNAL | EVT_NOTIFY_WAIT)) != 0) ? 1 : 0)
         ^ ((NotifyFunction == NULL) ? 1 : 0) != 0);

  ASSERT (Event != NULL);
  ASSERT (!EfiAtRuntime ());
  ASSERT (EfiGetCurrentTpl () < TPL_HIGH_LEVEL);

  Status = gBS->CreateEventEx (
                  Type,
                  NotifyTpl,
                  NotifyFunction,
                  NotifyContext,
                  EventGroup,
                  Event
                  );

  ASSERT_EFI_ERROR (Status);

  return Status;
}

// EfiCopyMem
/** Copies the contents of one buffer to another buffer.

  @param[in] Destination  The pointer to the destination buffer of the memory
                          copy.
  @param[in] Source       The pointer to the source buffer of the memory copy.
  @param[in] Length       Number of bytes to copy from Source to Destination.
**/
VOID
EfiCopyMem (
  IN VOID   *Destination,
  IN VOID   *Source,
  IN UINTN  Length
  )
{
  ASSERT (Destination != NULL);
  ASSERT (Source != NULL);
  ASSERT (Length > 0);
  ASSERT ((Length - 1) <= (MAX_ADDRESS - (UINTN)Source));
  ASSERT ((Length - 1) <= (MAX_ADDRESS - (UINTN)Destination));
  ASSERT (EfiGetCurrentTpl () <= TPL_NOTIFY);

  gBS->CopyMem (Destination, Source, Length);
}

// EfiSetMem
/** The SetMem() function fills a buffer with a specified value.

  @param[in] Buffer  The pointer to the buffer to fill.
  @param[in] Size    Number of bytes in Buffer to fill.
  @param[in] Value   Value to fill Buffer with.
**/
VOID
EfiSetMem (
  IN VOID   *Buffer,
  IN UINTN  Size,
  IN UINT8  Value
  )
{
  ASSERT (Buffer != 0);
  ASSERT (Size > 0);
  ASSERT ((Size - 1) <= (MAX_ADDRESS - (UINTN)Buffer));
  ASSERT (EfiGetCurrentTpl () <= TPL_NOTIFY);

  gBS->SetMem (Buffer, Size, Value);
}
