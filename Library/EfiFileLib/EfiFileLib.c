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

#include <Guid/FileInfo.h>

#include <Protocol/SimpleFileSystem.h>

#include <Library/BaseLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/FileHandleLib.h>
#include <Library/EfiFileLib.h>

// FILE_INFO_IS_DIRECTORY
#define FILE_INFO_IS_DIRECTORY(DirInfo) (((DirInfo)->Attribute & EFI_FILE_DIRECTORY) != 0)

// FileExists
/** Checks whether the given file exists or not.

  @param[in] Root      The volume's opened root.
  @param[in] FileName  The path of the file to check.

  @return  Returned is whether the specified file exists or not.
**/
BOOLEAN
FileExists (
  IN EFI_FILE_HANDLE  Root,
  IN CHAR16           *FileName
  )
{
  BOOLEAN         Exists;

  EFI_STATUS      Status;
  EFI_FILE_HANDLE FileHandle;

  ASSERT (Root != NULL);
  ASSERT (FileName != NULL);

  Status = Root->Open (Root, &FileHandle, FileName, EFI_FILE_MODE_READ, 0);
  Exists = !EFI_ERROR (Status);

  if (Exists) {
    FileHandle->Close (FileHandle);
  }

  return Exists;
}

// LoadFile
EFI_STATUS
LoadFile (
  IN  EFI_FILE_HANDLE  Root,
  IN  CHAR16           *FileName,
  OUT UINTN            *BufferSize,
  OUT VOID             **Buffer
  )
{
  EFI_STATUS      Status;

  EFI_FILE_HANDLE FileHandle;
  UINT64          ReadSize;
  UINTN           FileDataSize;
  VOID            *FileData;

  ASSERT (Root != NULL);
  ASSERT (FileName != NULL);
  ASSERT (BufferSize != NULL);
  ASSERT (Buffer != NULL);

  Status = Root->Open (Root, &FileHandle, FileName, EFI_FILE_MODE_READ, 0);

  if (!EFI_ERROR (Status)) {
    Status = FileHandleGetSize (FileHandle, &ReadSize);

    if (!EFI_ERROR (Status)) {
      ReadSize += sizeof (CHAR16); // HFSPlus

      if (ReadSize > MAX_UINTN) {
        ReadSize = MAX_UINTN;
      }

      FileDataSize = (UINTN)ReadSize;
      FileData     = AllocateZeroPool (FileDataSize);
      Status       = EFI_OUT_OF_RESOURCES;

      if (FileData != NULL) {
        Status = FileHandleRead (FileHandle, &FileDataSize, FileData);

        if (!EFI_ERROR (Status)) {
          *BufferSize = FileDataSize;
          *Buffer     = FileData;
          Status      = EFI_SUCCESS;
        } else {
          FreePool (FileData);
        }
      }
    }

    FileHandleClose (FileHandle);
  }

  return Status;
}

// GetFileExtension
CHAR16 *
GetFileExtension (
  IN CHAR16  *FileName
  )
{
  ASSERT (FileName != NULL);

  while (FileName[0] == L'.') {
    ++FileName;
  }

  return StrStr (FileName, L".");
}

// GetFilePrimaryExtension
CHAR16 *
GetFilePrimaryExtension (
  IN CHAR16  *FileName
  )
{
  CHAR16 *Extension;

  ASSERT (FileName != NULL);

  Extension = GetFileExtension (FileName);

  while (TRUE) {
    FileName  = &Extension[1];
    Extension = StrStr (FileName, L".");

    if (Extension == NULL) {
      Extension = &FileName[-1];
      break;
    }
  }

  return Extension;
}

// CompareExtension
INTN
CompareExtension (
  IN CHAR16   *FileName,
  IN CHAR16   *Extension,
  IN BOOLEAN  PrimaryExtension
  )
{
  CHAR16 *CurrentExtension;

  ASSERT (FileName != NULL);
  ASSERT (Extension != NULL);

  CurrentExtension = (PrimaryExtension ? GetFilePrimaryExtension (FileName) : GetFileExtension (FileName));

  return StrCmp (CurrentExtension, Extension);
}

// FindNextFileByExtension
EFI_STATUS
FindNextFileByExtension (
  IN  EFI_FILE_HANDLE  DirHandle,
  OUT EFI_FILE_INFO    *Buffer,
  IN  CHAR16           *Extension,
  IN  BOOLEAN          PrimaryExtension
  )
{
  EFI_STATUS Status;

  BOOLEAN    NoFile;

  ASSERT (DirHandle != NULL);
  ASSERT (Buffer != NULL);
  ASSERT (Extension != NULL);

  NoFile = FALSE;

  do {
    Status = FileHandleFindNextFile (DirHandle, Buffer, &NoFile);

    if (EFI_ERROR (Status)) {
      FreePool ((VOID *)Buffer);
      break;
    }

    if (NoFile) {
      Status = EFI_NOT_FOUND;
      break;
    }
  } while (CompareExtension (Buffer->FileName, Extension, PrimaryExtension) != 0);

  return Status;
}

// FindFirstFileByExtension
EFI_STATUS
FindFirstFileByExtension (
  IN  EFI_FILE_HANDLE  DirHandle,
  OUT EFI_FILE_INFO    **Buffer,
  IN  CHAR16           *Extension,
  IN  BOOLEAN          PrimaryExtension
  )
{
  EFI_STATUS Status;

  ASSERT (DirHandle != NULL);
  ASSERT (Buffer != NULL);
  ASSERT (Extension != NULL);

  Status = FileHandleFindFirstFile (DirHandle, Buffer);

  if (!EFI_ERROR (Status)) {
    if (CompareExtension ((*Buffer)->FileName, Extension, PrimaryExtension) != 0) {
      Status = FindNextFileByExtension (DirHandle, *Buffer, Extension, PrimaryExtension);
    }
  } else {
    FreePool ((VOID *)Buffer);
  }

  return Status;
}

// FindNextDirectory
EFI_STATUS
FindNextDirectory (
  IN  EFI_FILE_HANDLE  DirHandle,
  OUT EFI_FILE_INFO    *Buffer
  )
{
  EFI_STATUS      Status;

  BOOLEAN         NoFile;

  ASSERT (DirHandle != NULL);
  ASSERT (Buffer != NULL);

  NoFile = FALSE;

  do {
    Status = FileHandleFindNextFile (DirHandle, Buffer, &NoFile);

    if (EFI_ERROR (Status)) {
      FreePool ((VOID *)Buffer);

      break;
    }

    if (NoFile) {
      Status = EFI_NOT_FOUND;

      break;
    }
  } while (!FILE_INFO_IS_DIRECTORY (Buffer));

  return Status;
}

// FindFirstDirectory
EFI_STATUS
FindFirstDirectory (
  IN  EFI_FILE_HANDLE  DirHandle,
  OUT EFI_FILE_INFO    **Buffer
  )
{
  EFI_STATUS Status;

  ASSERT (DirHandle != NULL);
  ASSERT (Buffer != NULL);

  Status = FileHandleFindFirstFile (DirHandle, Buffer);

  if (!EFI_ERROR (Status)) {
    if (!FILE_INFO_IS_DIRECTORY (*Buffer)) {
      Status = FindNextDirectory (DirHandle, *Buffer);
    }
  } else {
    FreePool ((VOID *)Buffer);
  }

  return Status;
}

// FindNextDirectoryByExtension
EFI_STATUS
FindNextDirectoryByExtension (
  IN  EFI_FILE_HANDLE  DirHandle,
  OUT EFI_FILE_INFO    *Buffer,
  IN  CHAR16           *Extension,
  IN  BOOLEAN          PrimaryExtension
  )
{
  EFI_STATUS Status;

  ASSERT (DirHandle != NULL);
  ASSERT (Buffer != NULL);
  ASSERT (Extension != NULL);

  do {
    Status = FindNextFileByExtension (DirHandle, Buffer, Extension, PrimaryExtension);
  } while (!EFI_ERROR (Status) && !FILE_INFO_IS_DIRECTORY (Buffer));

  return Status;
}

// FindFirstDirectoryByExtension
EFI_STATUS
FindFirstDirectoryByExtension (
  IN  EFI_FILE_HANDLE  DirHandle,
  OUT EFI_FILE_INFO    **Buffer,
  IN  CHAR16           *Extension,
  IN  BOOLEAN          PrimaryExtension
  )
{
  EFI_STATUS Status;

  ASSERT (DirHandle != NULL);
  ASSERT (Buffer != NULL);
  ASSERT (Extension != NULL);

  Status = FileHandleFindFirstFile (DirHandle, Buffer);

  if (!EFI_ERROR (Status)) {
    if (!FILE_INFO_IS_DIRECTORY (*Buffer)
     || (CompareExtension ((*Buffer)->FileName, Extension, PrimaryExtension) == 0)) {
      Status = FindNextDirectoryByExtension (DirHandle, *Buffer, Extension, PrimaryExtension);
    }
  } else {
    FreePool ((VOID *)Buffer);
  }

  return Status;
}
