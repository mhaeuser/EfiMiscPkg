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

#ifndef MISC_FILE_LIB_H_
#define MISC_FILE_LIB_H_

#include <Guid/FileInfo.h>

#include <Protocol/SimpleFileSystem.h>

// FILE_PATH_SEPARATOR
#define FILE_PATH_SEPARATOR  L'\\'

// FileExists
/** Checks whether the given file exists or not.

  @param[in] Root      The volume's opened root.
  @param[in] FileName  The path of the file to check.

  @return  Returned is whether the specified file exists or not.
**/
BOOLEAN
FileExists (
  IN EFI_FILE_PROTOCOL  *Root,
  IN CHAR16             *FileName
  );

// LoadFile
EFI_STATUS
LoadFile (
  IN  EFI_FILE_HANDLE  Root,
  IN  CHAR16           *FileName,
  OUT UINTN            *BufferSize,
  OUT VOID             **Buffer
  );

// GetFileExtension
CHAR16 *
GetFileExtension (
  IN CHAR16  *FileName
  );

// GetFilePrimaryExtension
CHAR16 *
GetFilePrimaryExtension (
  IN CHAR16  *FileName
  );

// FindFirstFileByExtension
EFI_STATUS
FindFirstFileByExtension (
  IN  EFI_FILE_HANDLE  DirHandle,
  OUT EFI_FILE_INFO    **Buffer,
  IN  CHAR16           *Extension,
  IN  BOOLEAN          PrimaryExtension
  );

// FindNextFileByExtension
EFI_STATUS
FindNextFileByExtension (
  IN  EFI_FILE_HANDLE  DirHandle,
  OUT EFI_FILE_INFO    *Buffer,
  IN  CHAR16           *Extension,
  IN  BOOLEAN          PrimaryExtension
  );

// FindFirstDirectory
EFI_STATUS
FindFirstDirectory (
  IN  EFI_FILE_HANDLE  DirHandle,
  OUT EFI_FILE_INFO    **Buffer
  );

// FindNextDirectory
EFI_STATUS
FindNextDirectory (
  IN  EFI_FILE_HANDLE  DirHandle,
  OUT EFI_FILE_INFO    *Buffer
  );

// FindFirstDirectoryByExtension
EFI_STATUS
FindFirstDirectoryByExtension (
  IN  EFI_FILE_HANDLE  DirHandle,
  OUT EFI_FILE_INFO    **Buffer,
  IN  CHAR16           *Extension,
  IN  BOOLEAN          PrimaryExtension
  );

// FindNextDirectoryByExtension
EFI_STATUS
FindNextDirectoryByExtension (
  IN  EFI_FILE_HANDLE  DirHandle,
  OUT EFI_FILE_INFO    *Buffer,
  IN  CHAR16           *Extension,
  IN  BOOLEAN          PrimaryExtension
  );

// MiscGetFileInformation
VOID *
MiscGetFileInformation (
  IN EFI_FILE  *Root,
  IN EFI_GUID  *InformationType
);

#endif // MISC_FILE_LIB_H_
