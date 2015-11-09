#ifndef _EFI_FILE_LIB_H_
#define _EFI_FILE_LIB_H_

#include <Guid/FileInfo.h>

// FILE_PATH_SEPARATOR
#define FILE_PATH_SEPARATOR  L'\\'

// FileExists
/// Checks whether the given file exists or not.
///
/// @param[in] Root          The volume's opened root.
/// @param[in] FileName      The path of the file to check.
///
/// @return                  Returned is whether the specified file exists or not.
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
  OUT EFI_FILE_INFO    **Buffer,
  IN  CHAR16           *Extension,
  IN  BOOLEAN          PrimaryExtension
  );

#endif // ifndef _EFI_FILE_LIB_H_
