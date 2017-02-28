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

#include <Guid/FileInfo.h>

#include <Protocol/SimpleFileSystem.h>

#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/FileHandleLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/MiscFileLib.h>
#include <Library/MiscRuntimeLib.h>

// FILE_INFO_IS_DIRECTORY
#define FILE_INFO_IS_DIRECTORY(DirInfo)  \
  (((DirInfo)->Attribute & EFI_FILE_DIRECTORY) != 0)

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
  ASSERT (FileName[0] != L'\0');
  ASSERT (!EfiAtRuntime ());

  Status = Root->Open (Root, &FileHandle, FileName, EFI_FILE_MODE_READ, 0);

  if ((Status != EFI_NOT_FOUND)
   && (Status != EFI_NO_MEDIA)
   && (Status != EFI_MEDIA_CHANGED)) {
    ASSERT_EFI_ERROR (Status);
  }
  
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
  ASSERT (FileName[0] != L'\0');
  ASSERT (BufferSize != NULL);
  ASSERT (Buffer != NULL);
  ASSERT (!EfiAtRuntime ());

  Status = Root->Open (Root, &FileHandle, FileName, EFI_FILE_MODE_READ, 0);

  if ((Status != EFI_NOT_FOUND)
   && (Status != EFI_NO_MEDIA)
   && (Status != EFI_MEDIA_CHANGED)) {
    ASSERT_EFI_ERROR (Status);
  }

  if (!EFI_ERROR (Status)) {
    Status = FileHandleGetSize (FileHandle, &ReadSize);

    if (!EFI_ERROR (Status)) {
      if (sizeof (FileDataSize) < sizeof (ReadSize)) {
        FileDataSize = (UINTN)MIN (ReadSize, MAX_UINTN);
      } else {
        FileDataSize = (UINTN)ReadSize;
      }

      FileData = AllocatePool (FileDataSize);
      Status   = EFI_OUT_OF_RESOURCES;

      if (FileData != NULL) {
        Status = FileHandleRead (FileHandle, &FileDataSize, FileData);

        if (!EFI_ERROR (Status)) {
          *BufferSize = FileDataSize;
          *Buffer     = FileData;

          Status = EFI_SUCCESS;
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
  CHAR16 *Extension;
  ASSERT (FileName != NULL);
  ASSERT (FileName[0] != L'\0');

  Extension = StrStr (FileName, L".");

  if (Extension != NULL) {
    ++Extension;
  }

  return Extension;
}

// GetFilePrimaryExtension
CHAR16 *
GetFilePrimaryExtension (
  IN CHAR16  *FileName
  )
{
  CHAR16 *Extension;
  CHAR16 *NextExtension;

  ASSERT (FileName != NULL);
  ASSERT (FileName[0] != L'\0');

  Extension     = NULL;
  NextExtension = GetFileExtension (FileName);

  while (NextExtension != NULL) {
    Extension     = NextExtension;
    NextExtension = GetFileExtension (Extension);
  }

  return Extension;
}

// InternalCompareExtension
STATIC
INTN
InternalCompareExtension (
  IN CHAR16   *FileName,
  IN CHAR16   *Extension,
  IN BOOLEAN  PrimaryExtension
  )
{
  CHAR16 *CurrentExtension;

  ASSERT (FileName != NULL);
  ASSERT (FileName[0] != L'\0');
  ASSERT (Extension != NULL);
  ASSERT (Extension[0] != L'\0');

  CurrentExtension = (PrimaryExtension
                       ? GetFilePrimaryExtension (FileName)
                       : GetFileExtension (FileName));

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
  INTN       Result;

  ASSERT (DirHandle != NULL);
  ASSERT (Buffer != NULL);
  ASSERT (Extension != NULL);
  ASSERT (Extension[0] != L'\0');
  ASSERT (!EfiAtRuntime ());

  NoFile = FALSE;

  do {
    Status = FileHandleFindNextFile (DirHandle, Buffer, &NoFile);

    if (NoFile) {
      Status = EFI_NOT_FOUND;
      break;
    }

    if (EFI_ERROR (Status)) {
      FreePool ((VOID *)Buffer);

      break;
    }

    Result = InternalCompareExtension (
               Buffer->FileName,
               Extension,
               PrimaryExtension
               );
  } while (Result != 0);

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

  INTN       Result;

  ASSERT (DirHandle != NULL);
  ASSERT (Buffer != NULL);
  ASSERT (Extension != NULL);
  ASSERT (Extension[0] != L'\0');
  ASSERT (!EfiAtRuntime ());

  Status = FileHandleFindFirstFile (DirHandle, Buffer);

  if (!EFI_ERROR (Status)) {
    Result = InternalCompareExtension (
               (*Buffer)->FileName,
               Extension,
               PrimaryExtension
               );

    if (Result != 0) {
      Status = FindNextFileByExtension (
                 DirHandle,
                 *Buffer,
                 Extension,
                 PrimaryExtension
                 );
    }
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
  EFI_STATUS Status;

  BOOLEAN    NoFile;

  ASSERT (DirHandle != NULL);
  ASSERT (Buffer != NULL);
  ASSERT (!EfiAtRuntime ());

  NoFile = FALSE;

  do {
    Status = FileHandleFindNextFile (DirHandle, Buffer, &NoFile);

    if (NoFile) {
      Status = EFI_NOT_FOUND;
      break;
    }

    if (EFI_ERROR (Status)) {
      FreePool ((VOID *)Buffer);

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
  ASSERT (!EfiAtRuntime ());

  Status = FileHandleFindFirstFile (DirHandle, Buffer);

  if (!EFI_ERROR (Status) && !FILE_INFO_IS_DIRECTORY (*Buffer)) {
    Status = FindNextDirectory (DirHandle, *Buffer);
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

  INTN       Result;

  ASSERT (DirHandle != NULL);
  ASSERT (Buffer != NULL);
  ASSERT (Extension != NULL);
  ASSERT (Extension[0] != L'\0');
  ASSERT (!EfiAtRuntime ());

  do {
    Status = FindNextDirectory (DirHandle, Buffer);

    if (EFI_ERROR (Status)) {
      break;
    }

    Result = InternalCompareExtension (
               Buffer->FileName,
               Extension,
               PrimaryExtension
               );
  } while (Result != 0);

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

  INTN       Result;

  ASSERT (DirHandle != NULL);
  ASSERT (Buffer != NULL);
  ASSERT (Extension != NULL);
  ASSERT (!EfiAtRuntime ());

  Status = FindFirstDirectory (DirHandle, Buffer);

  if (!EFI_ERROR (Status)) {
    Result = InternalCompareExtension (
               (*Buffer)->FileName,
               Extension,
               PrimaryExtension
               );

    if (Result != 0) {
      Status = FindNextDirectoryByExtension (
                 DirHandle,
                 *Buffer,
                 Extension,
                 PrimaryExtension
                 );
    }
  }

  return Status;
}

// MiscGetFileInformation
VOID *
MiscGetFileInformation (
  IN EFI_FILE  *Root,
  IN EFI_GUID  *InformationType
  )
{
  VOID       *Buffer;

  UINTN      Size;
  EFI_STATUS Status;

  ASSERT (Root != NULL);

  Buffer = NULL;

  Status = Root->GetInfo (Root, InformationType, &Size, NULL);

  ASSERT (Status == EFI_BUFFER_TOO_SMALL);

  if (Status == EFI_BUFFER_TOO_SMALL) {
    Buffer = AllocatePool (Size);

    if (Buffer != NULL) {
      Status = Root->GetInfo (
                       Root,
                       InformationType,
                       &Size,
                       Buffer
                       );

      if (EFI_ERROR (Status)) {
        FreePool (Buffer);

        Buffer = NULL;
      }
    }
  }

  return Buffer;
}
