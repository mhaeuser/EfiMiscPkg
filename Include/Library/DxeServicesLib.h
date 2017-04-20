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

#ifndef DXE_SERVICES_LIB_H_
#define DXE_SERVICES_LIB_H_

// DxeAddMemorySpace
/** Adds reserved memory, system memory, or memory-mapped I/O resources to the
    global coherency domain of the processor.

  @param[in] GcdMemoryType  The type of memory resource being added.
  @param[in] BaseAddress    The physical address that is the start address of
                            the memory resource being added.
  @param[in] Length         The size, in bytes, of the memory resource that is
                            being added.
  @param[in] Capabilities   The bit mask of attributes that the memory resource
                            region supports.

  @retval EFI_SUCCESS            The memory resource was added to the global
                                 coherency domain of the processor.
  @retval EFI_INVALID_PARAMETER  GcdMemoryType is invalid.
  @retval EFI_INVALID_PARAMETER  Length is zero.
  @retval EFI_OUT_OF_RESOURCES   There are not enough system resources to add
                                 the memory resource to the global coherency
                                 domain of the processor.
  @retval EFI_UNSUPPORTED        The processor does not support one or more
                                 bytes of the memory resource range specified
                                 by BaseAddress and Length.
  @retval EFI_ACCESS_DENIED      One or more bytes of the memory resource range
                                 specified by BaseAddress and Length conflicts
                                 with a memory resource range that was
                                 previously added to the global coherency
                                 domain of the processor.
  @retval EFI_ACCESS_DENIED      One or more bytes of the memory resource range
                                 specified by BaseAddress and Length was
                                 allocated in a prior call to
                                 AllocateMemorySpace().
**/
EFI_STATUS
DxeAddMemorySpace (
  IN EFI_GCD_MEMORY_TYPE   GcdMemoryType,
  IN EFI_PHYSICAL_ADDRESS  BaseAddress,
  IN UINT64                Length,
  IN UINT64                Capabilities
  );

// DxeAllocateMemorySpace
/** Allocates nonexistent memory, reserved memory, system memory, or
    memorymapped I/O resources from the global coherency domain of the
    processor.

  @param[in]      GcdAllocateType  The type of allocation to perform.
  @param[in]      GcdMemoryType    The type of memory resource being allocated.
  @param[in]      Alignment        The log base 2 of the boundary that
                                   BaseAddress must be aligned on output.
                                   Align with 2^Alignment.
  @param[in]      Length           The size in bytes of the memory resource
                                   range that is being allocated.
  @param[in, out] BaseAddress      A pointer to a physical address to allocate.
  @param[in]      ImageHandle      The image handle of the agent that is
                                   allocating the memory resource.
  @param[in]      DeviceHandle     The device handle for which the memory
                                   resource is being allocated.

  @retval EFI_INVALID_PARAMETER  GcdAllocateType is invalid.
  @retval EFI_INVALID_PARAMETER  GcdMemoryType is invalid.
  @retval EFI_INVALID_PARAMETER  Length is zero.
  @retval EFI_INVALID_PARAMETER  BaseAddress is NULL.
  @retval EFI_INVALID_PARAMETER  ImageHandle is NULL.
  @retval EFI_NOT_FOUND          The memory resource request could not be
                                 satisfied.  No descriptor contains the desired
                                 space.
  @retval EFI_OUT_OF_RESOURCES   There are not enough system resources to
                                 allocate the memory resource from the global
                                 coherency domain of the processor.
  @retval EFI_SUCCESS            The memory resource was allocated from the
                                 global coherency domain of the processor.
**/
EFI_STATUS
DxeAllocateMemorySpace (
  IN     EFI_GCD_ALLOCATE_TYPE  GcdAllocateType,
  IN     EFI_GCD_MEMORY_TYPE    GcdMemoryType,
  IN     UINTN                  Alignment,
  IN     UINT64                 Length,
  IN OUT EFI_PHYSICAL_ADDRESS   *BaseAddress,
  IN     EFI_HANDLE             ImageHandle,
  IN     EFI_HANDLE             DeviceHandle OPTIONAL
  );

// DxeFreeMemorySpace
/** Frees nonexistent memory, reserved memory, system memory, or memory-mapped
    I/O resources from the global coherency domain of the processor.

  @param[in] BaseAddress  The physical address that is the start address of the
                          memory resource being freed.
  @param[in] Length       The size in bytes of the memory resource range that
                          is being freed.

  @retval EFI_SUCCESS            The memory resource was freed from the global
                                 coherency domain of the processor.
  @retval EFI_INVALID_PARAMETER  Length is zero.
  @retval EFI_UNSUPPORTED        The processor does not support one or more
                                 bytes of the memory resource range specified
                                 by BaseAddress and Length.
  @retval EFI_NOT_FOUND          The memory resource range specified by
                                 BaseAddress and Length was not allocated with
                                 previous calls to AllocateMemorySpace().
  @retval EFI_OUT_OF_RESOURCES   There are not enough system resources to free
                                 the memory resource from the global
                                 coherency domain of the processor.
**/
EFI_STATUS
DxeFreeMemorySpace (
  IN EFI_PHYSICAL_ADDRESS  BaseAddress,
  IN UINT64                Length
  );

// DxeRemoveMemorySpace
/** Removes reserved memory, system memory, or memory-mapped I/O resources from
    the global coherency domain of the processor.

  @param[in] BaseAddress  The physical address that is the start address of the
                          memory resource being removed.
  @param[in] Length       The size in bytes of the memory resource that is
                          being removed.

  @retval EFI_SUCCESS            The memory resource was removed from the
                                 global coherency domain of the processor.
  @retval EFI_INVALID_PARAMETER  Length is zero.
  @retval EFI_UNSUPPORTED        The processor does not support one or more
                                 bytes of the memory resource range specified
                                 by BaseAddress and Length.
  @retval EFI_NOT_FOUND          One or more bytes of the memory resource range
                                 specified by BaseAddress and Length was
                                 not added with previous calls to
                                 AddMemorySpace().
  @retval EFI_ACCESS_DEFINED     One or more bytes of the memory resource range
                                 specified by BaseAddress and Length has
                                 been allocated with AllocateMemorySpace().
  @retval EFI_OUT_OF_RESOURCES   There are not enough system resources to
                                 remove the memory  resource from the global
                                 coherency domain of the processor.
**/
EFI_STATUS
DxeRemoveMemorySpace (
  IN EFI_PHYSICAL_ADDRESS  BaseAddress,
  IN UINT64                Length
  );

// DxeGetMemorySpaceDescriptor
/** Retrieves the descriptor for a memory region containing a specified
    address.

  @param[in]  BaseAddress  The physical address that is the start address of a
                           memory region.
  @param[out] Descriptor   A pointer to a caller allocated descriptor.

  @retval EFI_SUCCESS            The descriptor for the memory resource region
                                 containing BaseAddress was returned in
                                 Descriptor.
  @retval EFI_INVALID_PARAMETER  Descriptor is NULL.
  @retval EFI_NOT_FOUND          A memory resource range containing BaseAddress
                                 was not found.
**/
EFI_STATUS
DxeGetMemorySpaceDescriptor (
  IN  EFI_PHYSICAL_ADDRESS             BaseAddress,
  OUT EFI_GCD_MEMORY_SPACE_DESCRIPTOR  *Descriptor
  );

// DxeSetMemorySpaceAttributes
/** Modifies the attributes for a memory region in the global coherency domain
    of the processor.

  @param[in] BaseAddress  The physical address that is the start address of a
                          memory region.
  @param[in] Length       The size in bytes of the memory region.
  @param[in] Attributes   The bit mask of attributes to set for the memory
                          region.

  @retval EFI_SUCCESS            The attributes were set for the memory region.
  @retval EFI_INVALID_PARAMETER  Length is zero.
  @retval EFI_UNSUPPORTED        The processor does not support one or more
                                 bytes of the memory resource range specified
                                 by BaseAddress and Length.
  @retval EFI_UNSUPPORTED        The bit mask of attributes is not support for
                                 the memory resource range specified by
                                 BaseAddress and Length.
  @retval EFI_ACCESS_DENIED      The attributes for the memory resource range
                                 specified by BaseAddress and Length cannot be
                                 modified.
  @retval EFI_OUT_OF_RESOURCES   There are not enough system resources to
                                 modify the attributes of the memory resource
                                 range.
  @retval EFI_NOT_AVAILABLE_YET  The attributes cannot be set because CPU
                                 architectural protocol is not available yet.
**/
EFI_STATUS
DxeSetMemorySpaceAttributes (
  IN EFI_PHYSICAL_ADDRESS  BaseAddress,
  IN UINT64                Length,
  IN UINT64                Attributes
  );

// DxeSetMemorySpaceCapabilities
/** Modifies the capabilities for a memory region in the global coherency
    domain of the processor.

  @param[in] BaseAddress   The physical address that is the start address of a
                           memory region.
  @param[in] Length        The size in bytes of the memory region.
  @param[in] Capabilities  The bit mask of capabilities that the memory region
                           supports.

  @retval EFI_SUCCESS            The capabilities were set for the memory
                                 region.
  @retval EFI_INVALID_PARAMETER  Length is zero.
  @retval EFI_UNSUPPORTED        The capabilities specified by Capabilities do
                                 not include the memory region attributes
                                 currently in use.
  @retval EFI_ACCESS_DENIED      The capabilities for the memory resource range
                                 specified by BaseAddress and Length cannot be
                                 modified.
  @retval EFI_OUT_OF_RESOURCES   There are not enough system resources to
                                 modify the capabilities of the memory resource
                                 range.
**/
EFI_STATUS
DxeSetMemorySpaceCapabilities (
  IN EFI_PHYSICAL_ADDRESS  BaseAddress,
  IN UINT64                Length,
  IN UINT64                Capabilities
  );

// DxeGetMemorySpaceMap
/** Returns a map of the memory resources in the global coherency domain of the
    processor.

  @param[out] NumberOfDescriptors  A pointer to number of descriptors returned
                                   in the MemorySpaceMap buffer.
  @param[out] MemorySpaceMap       A pointer to the array of
                                   EFI_GCD_MEMORY_SPACE_DESCRIPTORs.

  @retval EFI_SUCCESS            The memory space map was returned in the
                                 MemorySpaceMap buffer, and the number of
                                 descriptors in MemorySpaceMap was returned in
                                 NumberOfDescriptors.
  @retval EFI_INVALID_PARAMETER  NumberOfDescriptors is NULL.
  @retval EFI_INVALID_PARAMETER  MemorySpaceMap is NULL.
  @retval EFI_OUT_OF_RESOURCES   There are not enough resources to allocate
                                 MemorySpaceMap.
**/
EFI_STATUS
DxeGetMemorySpaceMap (
  OUT UINTN                            *NumberOfDescriptors,
  OUT EFI_GCD_MEMORY_SPACE_DESCRIPTOR  **MemorySpaceMap
  );

// DxeAddIoSpace
/** Adds reserved I/O or I/O resources to the global coherency domain of the
    processor.

  @param[in] GcdIoType    The type of I/O resource being added.
  @param[in] BaseAddress  The physical address that is the start address of the
                          I/O resource being added.
  @param[in] Length       The size in bytes of the I/O resource that is being
                          added.

  @retval EFI_SUCCESS            The I/O resource was added to the global
                                 coherency domain of the processor.
  @retval EFI_INVALID_PARAMETER  GcdIoType is invalid.
  @retval EFI_INVALID_PARAMETER  Length is zero.
  @retval EFI_OUT_OF_RESOURCES   There are not enough system resources to add
                                 the I/O resource to the global coherency domain
                                 of the processor.
  @retval EFI_UNSUPPORTED        The processor does not support one or more
                                 bytes of the I/O resource range specified by
                                 BaseAddress and Length.
  @retval EFI_ACCESS_DENIED      One or more bytes of the I/O resource range
                                 specified by BaseAddress and Length conflicts
                                 with an I/O resource range that was previously
                                 added to the global coherency domain of the
                                 processor.
  @retval EFI_ACCESS_DENIED      One or more bytes of the I/O resource range
                                 specified by BaseAddress and Length was
                                 allocated in a prior call to
                                 AllocateIoSpace().

**/
EFI_STATUS
DxeAddIoSpace (
  IN EFI_GCD_IO_TYPE       GcdIoType,
  IN EFI_PHYSICAL_ADDRESS  BaseAddress,
  IN UINT64                Length
  );

// DxeAllocateIoSpace
/** Allocates nonexistent I/O, reserved I/O, or I/O resources from the global
    coherency domain of the processor.

  @param[in]      GcdAllocateType  The type of allocation to perform.
  @param[in]      GcdIoType        The type of I/O resource being allocated.
  @param[in]      Alignment        The log base 2 of the boundary that
                                   BaseAddress must be aligned on output.
  @param[in]      Length           The size in bytes of the I/O resource range
                                   that is being allocated.
  @param[in, out] BaseAddress      A pointer to a physical address.
  @param[in]      ImageHandle      The image handle of the agent that is
                                   allocating the I/O resource.
  @param[in]      DeviceHandle     The device handle for which the I/O resource
                                   is being allocated.

  @retval EFI_SUCCESS            The I/O resource was allocated from the global
                                 coherency domain of the processor.
  @retval EFI_INVALID_PARAMETER  GcdAllocateType is invalid.
  @retval EFI_INVALID_PARAMETER  GcdIoType is invalid.
  @retval EFI_INVALID_PARAMETER  Length is zero.
  @retval EFI_INVALID_PARAMETER  BaseAddress is NULL.
  @retval EFI_INVALID_PARAMETER  ImageHandle is NULL.
  @retval EFI_OUT_OF_RESOURCES   There are not enough system resources to
                                 allocate the I/O resource from the global
                                 coherency domain of the processor.
  @retval EFI_NOT_FOUND          The I/O resource request could not be
                                 satisfied.
**/
EFI_STATUS
DxeAllocateIoSpace (
  IN     EFI_GCD_ALLOCATE_TYPE  GcdAllocateType,
  IN     EFI_GCD_IO_TYPE        GcdIoType,
  IN     UINTN                  Alignment,
  IN     UINT64                 Length,
  IN OUT EFI_PHYSICAL_ADDRESS   *BaseAddress,
  IN     EFI_HANDLE             ImageHandle,
  IN     EFI_HANDLE             DeviceHandle OPTIONAL
  );

// DxeFreeIoSpace
/** Frees nonexistent I/O, reserved I/O, or I/O resources from the global
    coherency domain of the processor.

  @param[in] BaseAddress  The physical address that is the start address of the
                          I/O resource being freed.
  @param[in] Length       The size in bytes of the I/O resource range that is
                          being freed.

  @retval EFI_SUCCESS            The I/O resource was freed from the global
                                 coherency domain of the processor.
  @retval EFI_INVALID_PARAMETER  Length is zero.
  @retval EFI_UNSUPPORTED        The processor does not support one or more
                                 bytes of the I/O resource range specified
                                 by BaseAddress and Length.
  @retval EFI_NOT_FOUND          The I/O resource range specified by
                                 BaseAddress and Length was not allocated with
                                 previous calls to AllocateIoSpace().
  @retval EFI_OUT_OF_RESOURCES   There are not enough system resources to free
                                 the I/O resource from the global coherency
                                 domain of the processor.
**/
EFI_STATUS
DxeFreeIoSpace (
  IN EFI_PHYSICAL_ADDRESS  BaseAddress,
  IN UINT64                Length
  );

// DxeRemoveIoSpace
/** Removes reserved I/O or I/O resources from the global coherency domain of
    the processor.

  @param[in] BaseAddress  A pointer to a physical address that is the start
                          address of the I/O resource being removed.
  @param[in] Length       The size in bytes of the I/O resource that is being
                          removed.

  @retval EFI_SUCCESS            The I/O resource was removed from the global
                                 coherency domain of the processor.
  @retval EFI_INVALID_PARAMETER  Length is zero.
  @retval EFI_UNSUPPORTED        The processor does not support one or more
                                 bytes of the I/O resource range specified by
                                 BaseAddress and Length.
  @retval EFI_NOT_FOUND          One or more bytes of the I/O resource range
                                 specified by BaseAddress and Length was
                                 not added with previous calls to AddIoSpace().
  @retval EFI_ACCESS_DENIED      One or more bytes of the I/O resource range
                                 specified by BaseAddress and Length has been
                                 allocated with AllocateIoSpace().
  @retval EFI_OUT_OF_RESOURCES   There are not enough system resources to
                                 remove the I/O resource from the global
                                 coherency domain of the processor.
**/
EFI_STATUS
DxeRemoveIoSpace (
  IN EFI_PHYSICAL_ADDRESS  BaseAddress,
  IN UINT64                Length
  );

// DxeGetIoSpaceDescriptor
/** Retrieves the descriptor for an I/O region containing a specified address.

  @param[in]  BaseAddress  The physical address that is the start address of an
                           I/O region.
  @param[out] Descriptor   A pointer to a caller allocated descriptor.

  @retval EFI_SUCCESS            The descriptor for the I/O resource region
                                 containing BaseAddress was returned in
                                 Descriptor.
  @retval EFI_INVALID_PARAMETER  Descriptor is NULL.
  @retval EFI_NOT_FOUND          An I/O resource range containing BaseAddress
                                 was not found.
**/
EFI_STATUS
DxeGetIoSpaceDescriptor (
  IN  EFI_PHYSICAL_ADDRESS         BaseAddress,
  OUT EFI_GCD_IO_SPACE_DESCRIPTOR  *Descriptor
  );

// DxeGetIoSpaceMap
/** Returns a map of the I/O resources in the global coherency domain of the
    processor.

  @param[out] NumberOfDescriptors  A pointer to number of descriptors returned
                                   in the IoSpaceMap buffer.
  @param[out] IoSpaceMap           A pointer to the array of
                                   EFI_GCD_IO_SPACE_DESCRIPTORs.

  @retval EFI_SUCCESS            The I/O space map was returned in the
                                 IoSpaceMap buffer, and the number of
                                 descriptors in IoSpaceMap was returned in
                                 NumberOfDescriptors.
  @retval EFI_INVALID_PARAMETER  NumberOfDescriptors is NULL.
  @retval EFI_INVALID_PARAMETER  IoSpaceMap is NULL.
  @retval EFI_OUT_OF_RESOURCES   There are not enough resources to allocate
                                 IoSpaceMap.
**/
EFI_STATUS
DxeGetIoSpaceMap (
  OUT UINTN                        *NumberOfDescriptors,
  OUT EFI_GCD_IO_SPACE_DESCRIPTOR  **IoSpaceMap
  );

// DxeDispatch
/** Loads and executed DXE drivers from firmware volumes.

  The Dispatch() function searches for DXE drivers in firmware volumes that
  have been installed since the last time the Dispatch() service was called.
  It then evaluates the dependency expressions of all the DXE drivers and loads
  and executes those DXE drivers whose dependency expression evaluate to TRUE.
  This service must interact with the Security Architectural Protocol to
  authenticate DXE drivers before they are executed.  This process is continued
  until no more DXE drivers can be executed.

  @retval EFI_SUCCESS          One or more DXE driver were dispatched.
  @retval EFI_NOT_FOUND        No DXE drivers were dispatched.
  @retval EFI_ALREADY_STARTED  An attempt is being made to start the DXE
                               Dispatcher recursively. Thus, no action was
                               taken.
**/
EFI_STATUS
DxeDispatch (
  VOID
  );

// DxeSchedule
/** Clears the Schedule on Request (SOR) flag for a component that is stored in
    a firmware volume.

  @param[in] FirmwareVolumeHandle  The handle of the firmware volume that
                                   contains the file specified by FileName.
  @param[in] FileName              A pointer to the name of the file in a
                                   firmware volume.

  @retval EFI_SUCCESS    The DXE driver was found and its SOR bit was cleared.
  @retval EFI_NOT_FOUND  The DXE driver does not exist, or the DXE driver
                         exists and its SOR bit is not set.
**/
EFI_STATUS
DxeSchedule (
  IN EFI_HANDLE      FirmwareVolumeHandle,
  IN CONST EFI_GUID  *FileName
  );

// DxeTrust
/** Promotes a file stored in a firmware volume from the untrusted to the
    trusted state.

  @param[in] FirmwareVolumeHandle  The handle of the firmware volume that
                                   contains the file specified by FileName.
  @param[in] FileName              A pointer to the name of the file in a
                                   firmware volume.

  @return                Status of promoting FFS from untrusted to trusted
                         state.
  @retval EFI_NOT_FOUND  The file was not found in the untrusted state.
**/
EFI_STATUS
DxeTrust (
  IN EFI_HANDLE      FirmwareVolumeHandle,
  IN CONST EFI_GUID  *FileName
  );

// DxeProcessFirmwareVolume
/** Creates a firmware volume handle for a firmware volume that is present in
    system memory.

  @param[in]  FirmwareVolumeHeader  A pointer to the header of the firmware
                                    volume.
  @param[in]  Size                  The size, in bytes, of the firmware volume.
  @param[out] FirmwareVolumeHandle  On output, a pointer to the created handle.

  @retval EFI_SUCCESS           The EFI_FIRMWARE_VOLUME_PROTOCOL and
                                EFI_DEVICE_PATH_PROTOCOL were installed onto
                                FirmwareVolumeHandle for the firmware volume
                                described by FirmwareVolumeHeader and Size.
  @retval EFI_VOLUME_CORRUPTED  The firmware volume described by
                                FirmwareVolumeHeader and Size is corrupted.
  @retval EFI_OUT_OF_RESOURCES  There are not enough system resources available
                                to produce the EFI_FIRMWARE_VOLUME_PROTOCOL and
                                EFI_DEVICE_PATH_PROTOCOL for the firmware
                                volume described by FirmwareVolumeHeader and
                                Size.
**/
EFI_STATUS
DxeProcessFirmwareVolume (
  IN  CONST VOID  *FirmwareVolumeHeader,
  IN  UINTN       Size,
  OUT EFI_HANDLE  *FirmwareVolumeHandle
  );

#endif // DXE_SERVICES_LIB_H_