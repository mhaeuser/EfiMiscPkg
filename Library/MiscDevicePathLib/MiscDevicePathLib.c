/** @file
  Copyright (C) 2017, CupertinoNet.  All rights reserved.<BR>

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

#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>

// TODO: Rather split the functions than using a PCDs?
#define _PCD_GET_MODE_BOOL_PcdSupportMultiNodeFilePaths 1
#define _PCD_GET_MODE_BOOL_PcdSupportNonterminatedPaths 1

// MiscFileDevicePathToText
// TODO: This could use a cleanup.
CHAR16 *
MiscFileDevicePathToText (
  IN     EFI_DEVICE_PATH_PROTOCOL  *DevicePath,
  IN OUT UINTN                     *TextSize OPTIONAL
  )
{
  CHAR16           *FilePath;

  UINTN            FilePathSize;
  EFI_DEV_PATH_PTR DevPath;
  UINTN            DevFilePathSize;
  CHAR16           *DevFilePath;
  CHAR16           *TempFilePath;
  UINTN            Index;
  BOOLEAN          FilePathFound;

  ASSERT (DevicePath != NULL);

  FilePathSize = 0;
  FilePath     = NULL;

  DevPath.DevPath = DevicePath;

  if (PcdGetBool (PcdSupportMultiNodeFilePaths)) {
    FilePathFound = FALSE;
  }

  while (!IsDevicePathEnd ((CONST VOID *)DevPath.Raw)) {
    if ((DevicePathType ((CONST VOID *)DevPath.Raw) == MEDIA_DEVICE_PATH)
     && (DevicePathSubType ((CONST VOID *)DevPath.Raw) == MEDIA_FILEPATH_DP)) {
      if (PcdGetBool (PcdSupportMultiNodeFilePaths)) {
        FilePathFound = TRUE;
      }

      DevFilePathSize  = DevicePathNodeLength ((CONST VOID *)DevPath.Raw);
      DevFilePathSize -= sizeof (*DevPath.DevPath);
      DevFilePath      = &DevPath.FilePath->PathName[0];

      // Ensure the size does not include \0.

      if (PcdGetBool (PcdSupportNonterminatedPaths)) {
        Index = ((DevFilePathSize / sizeof (DevFilePath[0])) - 1);

        if (DevFilePath[Index] == L'\0') {
          DevFilePathSize -= sizeof (DevFilePath[0]);
        }
      } else {
        DevFilePathSize -= sizeof (DevFilePath[0]);
      }

      // Ensure FilePath ends with '\' and DevFilePath doesn't start with '\'
      // for all iterations but the first.
      // This is only required for FilePaths consisting of multiple Nodes.

      if (PcdGetBool (PcdSupportMultiNodeFilePaths) && (FilePathSize > 0)) {
        Index = ((FilePathSize / sizeof (FilePath[0])) - 1);

        if (FilePath[Index] != L'\\') {
          FilePathSize += sizeof (L'\\');

          // Overwrite '\0' with '\' - a new will be appended anyway.
          FilePath[Index + 1] = L'\\';
        }

        if (DevFilePath[0] == L'\\') {
          ++DevFilePath;
          --DevFilePathSize;
        }
      }

      TempFilePath = AllocatePool (
                       FilePathSize + DevFilePathSize + sizeof (L'\0')
                       );

      if (TempFilePath == NULL) {
        // FilePath cannot be non-NULL in the first iteration.
        if (PcdGetBool (PcdSupportMultiNodeFilePaths)) {
          FilePath = NULL;
        }

        break;
      }

      if (PcdGetBool (PcdSupportMultiNodeFilePaths) && (FilePath != NULL)) {
        CopyMem ((VOID *)TempFilePath, (VOID *)FilePath, FilePathSize);
        FreePool ((VOID *)FilePath);
      }

      FilePath = TempFilePath;

      // Index of the first character after FilePath.
      if (PcdGetBool (PcdSupportMultiNodeFilePaths)) {
        Index = (FilePathSize / sizeof (FilePath[0]));
      } else {
        Index = 0;
      }

      CopyMem (
        (VOID *)&FilePath[Index],
        (VOID *)DevFilePath,
        DevFilePathSize
        );

      FilePathSize += DevFilePathSize;

      // Index of the first character after FilePath.
      Index = (FilePathSize / sizeof (FilePath[0]));

      FilePath[Index] = L'\0';

      if (!PcdGetBool (PcdSupportMultiNodeFilePaths)) {
        break;
      }
    } else if (PcdGetBool (PcdSupportMultiNodeFilePaths)) {
      // No non-FilePath Nodes should be present after the first.
      ASSERT (!FilePathFound);
    }

    DevPath.DevPath = NextDevicePathNode ((CONST VOID *)DevPath.Raw);
  }

  if (TextSize != NULL) {
    *TextSize = FilePathSize;
  }

  return FilePath;
}
