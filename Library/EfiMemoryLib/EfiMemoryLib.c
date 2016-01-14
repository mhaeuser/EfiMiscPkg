/** @file
  Copyright (C) 2015, CupertinoNet.  All rights reserved.<BR>

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

#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/EfiRuntimeLib.h>
#include <Library/EfiMemoryLib.h>

#ifndef MDEPKG_NDEBUG

// mSetVirtualAddressMapReturned
STATIC BOOLEAN mSetVirtualAddressMapReturned = FALSE;

#endif

// EfiGetMemoryMap
/** Returns the current memory map.

  @param[in, out] MemoryMapSize      A pointer to the size, in bytes, of the MemoryMap Buffer.
                                     On input, this is the size of the Buffer allocated by the caller.
                                     On output, it is the size of the Buffer returned by the firmware if
                                     the Buffer was large enough, or the size of the Buffer needed to contain
                                     the map if the Buffer was too small.
  @param[in, out] MemoryMap          A pointer to the Buffer in which firmware places the current memory
                                     map.
  @param[out]     MapKey             A pointer to the location in which firmware returns the key for the
                                     current memory map.
  @param[out]     DescriptorSize     A pointer to the location in which firmware returns the size, in bytes, of
                                     an individual EFI_MEMORY_DESCRIPTOR.
  @param[out]     DescriptorVersion  A pointer to the location in which firmware returns the version number
                                     associated with the EFI_MEMORY_DESCRIPTOR.

  @retval EFI_SUCCESS            The memory map was returned in the MemoryMap Buffer.
  @retval EFI_BUFFER_TOO_SMALL   The MemoryMap Buffer was too small.  The current Buffer size
                                 needed to hold the memory map is returned in MemoryMapSize.
  @retval EFI_INVALID_PARAMETER  1) MemoryMapSize is NULL.
                                 2) The MemoryMap Buffer is not too small and MemoryMap is
                                 NULL.
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
  ASSERT ((*MemoryMapSize == 0) || (MemoryMap != NULL));
  ASSERT (!EfiAtRuntime ());

  Status = gBS->GetMemoryMap (MemoryMapSize, MemoryMap, MapKey, DescriptorSize, DescriptorVersion);

  DEBUG_CODE (
    if (Status != EFI_BUFFER_TOO_SMALL) {
      ASSERT_EFI_ERROR (Status);
    }
    );

  return Status;
}

// EfiSetVirtualAddressMap
/** Changes the runtime addressing mode of EFI firmware from physical to virtual.

  @param[in] MemoryMapSize      The size in bytes of VirtualMap.
  @param[in] DescriptorSize     The size in bytes of an entry in the VirtualMap.
  @param[in] DescriptorVersion  The version of the structure entries in VirtualMap.
  @param[in] VirtualMap         An array of memory descriptors which contain new virtual
                                address mapping information for all runtime ranges.

  @retval EFI_SUCCESS            The virtual address map has been applied.
  @retval EFI_UNSUPPORTED        EFI firmware is not at runtime, or the EFI firmware is already in
                                 virtual address mapped mode.
  @retval EFI_INVALID_PARAMETER  DescriptorSize or DescriptorVersion is invalid.
  @retval EFI_NO_MAPPING         A virtual address was not supplied for a range in the memory
                                 map that requires a mapping.
  @retval EFI_NOT_FOUND          A virtual address was supplied for an address that is not found
                                 in the memory map.
**/
EFI_STATUS
EfiSetVirtualAddressMap (
  IN UINTN                  MemoryMapSize,
  IN UINTN                  DescriptorSize,
  IN UINT32                 DescriptorVersion,
  IN EFI_MEMORY_DESCRIPTOR  *VirtualMap
  )
{
  EFI_STATUS Status;

  ASSERT (MemoryMapSize > 0);
  ASSERT (DescriptorSize > 0);
  ASSERT (VirtualMap != NULL);

  Status = gRT->SetVirtualAddressMap (MemoryMapSize, DescriptorSize, DescriptorVersion, VirtualMap);

  DEBUG_CODE (
    if (Status != EFI_BUFFER_TOO_SMALL) {
      ASSERT_EFI_ERROR (Status);
    }
    );

#ifndef MDEPKG_NDEBUG
  if (!EFI_ERROR (Status)) {
    mSetVirtualAddressMapReturned = TRUE;
  }
#endif

  return Status;
}

// EfiConvertPointer
/** Determines the new virtual address that is to be used on subsequent memory accesses.

  @param[in]      DebugDisposition  Supplies type information for the pointer being converted.
  @param[in, out] Address           A pointer to a pointer that is to be fixed to be the value needed
                                    for the new virtual address mappings being applied.

  @retval EFI_SUCCESS            The pointer pointed to by Address was modified.
  @retval EFI_INVALID_PARAMETER  1) Address is NULL.
                                 2) *Address is NULL and DebugDisposition does
                                 not have the EFI_OPTIONAL_PTR bit set.
  @retval EFI_NOT_FOUND          The pointer pointed to by Address was not found to be part
                                 of the current memory map.  This is normally fatal.
**/
EFI_STATUS
EfiConvertPointer (
  IN     UINTN  DebugDisposition,
  IN OUT VOID   **Address
  )
{
  EFI_STATUS Status;

  ASSERT (Address != NULL);
  ASSERT (((DebugDisposition & EFI_OPTIONAL_PTR) != 0) || (*Address != NULL));
  ASSERT (!mSetVirtualAddressMapReturned);

  // calls to ConvertPointer are illegal after SetVirtualAddressMap returned, so do not use updated RT.
  // also, calls to the updated RT pointer with the virtual address are illegal till SetVirtualAddressMap returned.
  Status = gPhysicalRT->ConvertPointer (DebugDisposition, Address);

  DEBUG_CODE (
    if (Status != EFI_NOT_FOUND) {
      ASSERT_EFI_ERROR (Status);
    }
    );

  return Status;
}

// EfiAllocatePages
/** Allocates one or more 4KB pages of a certain memory type.

  Allocates the number of 4KB pages of a certain memory type and returns a pointer to the allocated
  Buffer.  The Buffer returned is aligned on a 4KB boundary.  If Pages is 0, then NULL is returned.
  If there is not enough memory remaining to satisfy the request, then NULL is returned.

  @param[in] Type        The type of allocation to perform.
  @param[in] MemoryType  The type of memory to allocate.
  @param[in] Pages       The number of 4 KB pages to allocate.

  @return  A pointer to the allocated Buffer or NULL if allocation fails.
**/
VOID *
EfiAllocatePages (
  IN EFI_ALLOCATE_TYPE  Type,
  IN EFI_MEMORY_TYPE    MemoryType,
  IN UINTN              Pages
  )
{
  EFI_PHYSICAL_ADDRESS Memory;

  EFI_STATUS           Status;

  ASSERT ((MemoryType > EfiReservedMemoryType) && (MemoryType < EfiMaxMemoryType));
  ASSERT (Pages > 0);
  ASSERT (!EfiAtRuntime ());

  Status = gBS->AllocatePages (Type, MemoryType, Pages, &Memory);

  ASSERT_EFI_ERROR (Status);

  if (EFI_ERROR (Status)) {
    Memory = 0;
  }

  ASSERT (Memory != 0);

  return (VOID *)(UINTN)Memory;
}
