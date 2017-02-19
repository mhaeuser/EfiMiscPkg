/** @file
  Copyright (C) 2015 - 2016, CupertinoNet.  All rights reserved.<BR>
  Portions Copyright (C) 2012, Damir Mažar.  All rights reserved.<BR>

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
#include <Library/MemoryAllocationLib.h>
#include <Library/MiscRuntimeLib.h>
#include <Library/MiscMemoryLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>

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
  )
{
  EFI_MEMORY_DESCRIPTOR *MemoryMapBuffer;

  UINTN                 Size;
  EFI_STATUS            Status;
  UINTN                 MemoryMapBufferSize;
  UINTN                 Index;

  ASSERT (GetMemoryMap != NULL);
  ASSERT (MemoryMapSize != NULL);
  ASSERT (!EfiAtRuntime ());

  MemoryMapBuffer = NULL;
  Size            = 0;
  Status          = GetMemoryMap (
                      &Size,
                      NULL,
                      MemoryMapKey,
                      DescriptorSize,
                      DescriptorVersion
                      );

  ASSERT (Status == EFI_BUFFER_TOO_SMALL);

  if (Status == EFI_BUFFER_TOO_SMALL) {
    Size                += 512;
    MemoryMapBuffer     = AllocatePool (Size);
    MemoryMapBufferSize = Size;

    if (MemoryMapBuffer != NULL) {
      for (Index = 0; Index < 5; ++Index) {
        Status = GetMemoryMap (
                   &Size,
                   MemoryMapBuffer,
                   MemoryMapKey,
                   DescriptorSize,
                   DescriptorVersion
                   );

        if (EFI_ERROR (Status)) {
          if (Status == EFI_BUFFER_TOO_SMALL) {
            Size               += 512;
            MemoryMapBuffer     = ReallocatePool (
                                    MemoryMapBufferSize,
                                    Size,
                                    (VOID *)MemoryMapBuffer
                                    );

            MemoryMapBufferSize = Size;

            if (MemoryMapBuffer == NULL) {
              Size = 0;
              break;
            }
          } else {
            Size            = 0;
            MemoryMapBuffer = NULL;
          }
        } else {
          break;
        }
      }
    }
  }

  *MemoryMapSize = Size;

  return MemoryMapBuffer;
}

// GetMemoryMapKey
/** Helper function that calls GetMemoryMap() and returns new MapKey.
**/
UINTN
GetMemoryMapKey (
  IN EFI_GET_MEMORY_MAP  GetMemoryMap
  )
{
  UINTN                 MapKey;

  EFI_MEMORY_DESCRIPTOR *MemoryMap;
  UINTN                 MemoryMapSize;
  UINTN                 DescriptorSize;
  UINT32                DescriptorVersion;

  ASSERT (GetMemoryMap != NULL);
  ASSERT (!EfiAtRuntime ());

  MemoryMap = GetMemoryMapBuffer (
                GetMemoryMap,
                &MemoryMapSize,
                &MapKey,
                &DescriptorSize,
                &DescriptorVersion
                );

  FreePool ((VOID *)MemoryMap);

  return MapKey;
}

// AllocatePagesFromTop
/** Allocates one or more 4KB pages of a certain memory type from the top of
    memory.

  Allocates the number of 4KB pages of a certain memory type from the top of
  memory and returns a pointer to the allocated Buffer.  The Buffer returned is
  aligned on a 4KB boundary.  If Pages is 0, then NULL is returned.  If there
  is not enough memory remaining to satisfy the request, then NULL is returned.
  
  @param[in]      MemoryType  The type of memory to allocate.
  @param[in]      Pages       The number of 4 KB pages to allocate.
  @param[in, out] MemoryTop   The pointer to a physical address.  On input, it
                              is the highest desired address.
**/
VOID *
AllocatePagesFromTop (
  IN     EFI_MEMORY_TYPE       MemoryType,
  IN     UINTN                 Pages,
  IN OUT EFI_PHYSICAL_ADDRESS  MemoryTop
  )
{
  EFI_MEMORY_DESCRIPTOR *MemoryMap;
  UINTN                 MemoryMapSize;
  UINTN                 MapKey;
  UINTN                 DescriptorSize;
  UINT32                DescriptorVersion;
  EFI_MEMORY_DESCRIPTOR *MemoryMapEnd;
  EFI_MEMORY_DESCRIPTOR *Descriptor;

  ASSERT ((MemoryType > EfiReservedMemoryType)
       && (MemoryType < EfiMaxMemoryType));

  ASSERT (Pages > 0);
  ASSERT (MemoryTop != 0);
  ASSERT (MemoryTop > Pages);
  ASSERT (!EfiAtRuntime ());

  MemoryMap = GetMemoryMapBuffer (
                gBS->GetMemoryMap,
                &MemoryMapSize,
                &MapKey,
                &DescriptorSize,
                &DescriptorVersion
                );

  if (MemoryMap != NULL) {
    MemoryMapEnd = NEXT_MEMORY_DESCRIPTOR (MemoryMap, MemoryMapSize);
    Descriptor   = PREV_MEMORY_DESCRIPTOR (MemoryMapEnd, DescriptorSize);

    while (((UINTN)Descriptor >= (UINTN)MemoryMap) && (MemoryTop != 0)) {
      if ((Descriptor->Type == EfiConventionalMemory)
       && (Pages <= Descriptor->NumberOfPages)
       && ((Descriptor->PhysicalStart + EFI_PAGES_TO_SIZE (Pages)) <= MemoryTop)) {
        if (Descriptor->PhysicalStart
              + EFI_PAGES_TO_SIZE ((UINTN)Descriptor->NumberOfPages) <= MemoryTop) {
          // The entire range is below Memory. Allocate from the top of the
          // range.
          MemoryTop = (Descriptor->PhysicalStart
                     + EFI_PAGES_TO_SIZE (Descriptor->NumberOfPages - Pages));
        } else {
          // The range contains enough pages under Memory, but spans above it
          // allocate below Memory.
          MemoryTop -= EFI_PAGES_TO_SIZE (Pages);
        }

        EfiAllocatePages (AllocateAddress, MemoryType, Pages, &MemoryTop);

        break;
      }

      Descriptor = PREV_MEMORY_DESCRIPTOR (Descriptor, DescriptorSize);
    }

    FreePool ((VOID *)MemoryMap);
  }

  return (VOID *)(UINTN)MemoryTop;
}
