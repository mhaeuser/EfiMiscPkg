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

#ifndef MISC_MEMORY_LIB_H_
#define MISC_MEMORY_LIB_H_

// PREV_MEMORY_DESCRIPTOR
/** Macro that returns the a pointer to the next EFI_MEMORY_DESCRIPTOR in an
    array returned from GetMemoryMap().  

  @param[in] MemoryDescriptor  A pointer to an EFI_MEMORY_DESCRIPTOR.

  @param[in] Size              The size, in bytes, of the current
                               EFI_MEMORY_DESCRIPTOR.

  @return A pointer to the next EFI_MEMORY_DESCRIPTOR.
**/
#define PREV_MEMORY_DESCRIPTOR(MemoryDescriptor, Size) \
  ((EFI_MEMORY_DESCRIPTOR *)((UINTN)(MemoryDescriptor) - (Size)))

// GetMemoryMapBuffer
/** Helper function that calls GetMemoryMap(), allocates space for the Memory
    Map and returns it.
**/
EFI_MEMORY_DESCRIPTOR *
GetMemoryMapBuffer (
  IN  EFI_GET_MEMORY_MAP  GetMemoryMap,
  OUT UINTN               *MemoryMapSize,
  OUT UINTN               *MemoryMapKey,
  OUT UINTN               *DescriptorSize,
  OUT UINT32              *DescriptorVersion
  );

// GetMemoryMapKey
/** Helper function that calls GetMemoryMap() and returns new MapKey.
**/
UINTN
GetMemoryMapKey (
  IN EFI_GET_MEMORY_MAP  GetMemoryMap
  );

// AllocatePagesFromTop
/** Allocates one or more 4KB pages of a certain memory type from the top of
    memory.

  Allocates the number of 4KB pages of a certain memory type from the top of
  memory and returns a pointer to the allocated Buffer.  The Buffer returned is
  aligned on a 4KB boundary.  If Pages is 0, then NULL is returned.  If there
  is not enough memory remaining to satisfy the request, then NULL is returned.
  
  @param[in]      MemoryType  The type of memory to allocate.
  @param[in]      Pages       The number of 4 KB pages to allocate.
  @param[in, out] Memory      The pointer to a physical address.  On input, it
                              is the highest desired address.
**/
VOID *
AllocatePagesFromTop (
  IN     EFI_MEMORY_TYPE       MemoryType,
  IN     UINTN                 Pages,
  IN OUT EFI_PHYSICAL_ADDRESS  Memory
  );

#endif // MISC_MEMORY_LIB_H_
