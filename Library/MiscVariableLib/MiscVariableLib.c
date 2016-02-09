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

#include <Guid/GlobalVariable.h>

#include <Library/UefiRuntimeLib.h>
#include <Library/DebugLib.h>
#include <Library/MiscVariableLib.h>

// GetEfiGlobalVariable
EFI_STATUS
GetEfiGlobalVariable (
  IN     CHAR16  *VariableName,
  OUT    UINT32  *Attributes, OPTIONAL
  IN OUT UINTN   *DataSize,
  OUT    VOID    *Data
  )
{
  ASSERT (VariableName != NULL);
  ASSERT (VariableName[0] != L'\0');
  ASSERT (DataSize != NULL);

  return EfiGetVariable (
           VariableName,
           &gEfiGlobalVariableGuid,
           Attributes,
           DataSize,
           Data
           );
}

// GetNextEfiGlobalVariableName
EFI_STATUS
GetNextEfiGlobalVariableName (
  IN OUT UINTN   *VariableNameSize,
  IN OUT CHAR16  *VariableName
  )
{
  ASSERT (VariableNameSize != NULL);
  ASSERT (VariableName != NULL);
  ASSERT (VariableName[0] != L'\0');

  return EfiGetNextVariableName (
           VariableNameSize,
           VariableName,
           &gEfiGlobalVariableGuid
           );
}

// SetEfiGlobalvariable
EFI_STATUS
SetEfiGlobalVariable (
  IN CHAR16  *VariableName,
  IN UINT32  Attributes,
  IN UINTN   DataSize,
  IN VOID    *Data
  )
{
  ASSERT (VariableName != NULL);
  ASSERT (VariableName[0] != L'\0');
  ASSERT ((((DataSize > 0) ? 1 : 0) ^ ((Data == NULL) ? 1 : 0)) != 0);

  return EfiSetVariable (
           VariableName,
           &gEfiGlobalVariableGuid,
           Attributes,
           DataSize,
           Data
           );
}

// DeleteVariable
EFI_STATUS
DeleteVariable (
  IN CHAR16    *VariableName,
  IN EFI_GUID  *VendorGuid
  )
{
  ASSERT (VariableName != NULL);
  ASSERT (VariableName[0] != L'\0');
  ASSERT (VendorGuid != NULL);

  return EfiSetVariable (VariableName, VendorGuid, 0, 0, NULL);
}

// DeleteEfiGlobalVariable
EFI_STATUS
DeleteEfiGlobalVariable (
  IN CHAR16  *VariableName
  )
{
  ASSERT (VariableName != NULL);
  ASSERT (VariableName[0] != L'\0');

  return DeleteVariable (VariableName, &gEfiGlobalVariableGuid);
}

// VariableExists
BOOLEAN
VariableExists (
  IN CHAR16    *VariableName,
  IN EFI_GUID  *VendorGuid
  )
{
  UINTN      Size;
  EFI_STATUS Status;

  Size   = 0;
  Status = EfiGetVariable (VariableName, VendorGuid, 0, &Size, NULL);

  return (Status == EFI_BUFFER_TOO_SMALL);
}
