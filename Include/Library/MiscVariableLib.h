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

#ifndef MISC_VARIABLE_LIB_H_
#define MISC_VARIABLE_LIB_H_

// GetEfiGlobalVariable
EFI_STATUS
GetEfiGlobalVariable (
  IN     CHAR16  *VariableName,
  OUT    UINT32  *Attributes, OPTIONAL
  IN OUT UINTN   *DataSize,
  OUT    VOID    *Data
  );

// GetNextEfiGlobalVariableName
EFI_STATUS
GetNextEfiGlobalVariableName (
  IN OUT UINTN   *VariableNameSize,
  IN OUT CHAR16  *VariableName
  );

// SetEfiGlobalvariable
EFI_STATUS
SetEfiGlobalVariable (
  IN CHAR16  *VariableName,
  IN UINT32  Attributes,
  IN UINTN   DataSize,
  IN VOID    *Data
  );

// DeleteVariable
EFI_STATUS
DeleteVariable (
  IN CHAR16    *VariableName,
  IN EFI_GUID  *VendorGuid
  );

// DeleteEfiGlobalVariable
EFI_STATUS
DeleteEfiGlobalVariable (
  IN CHAR16  *VariableName
  );

// VariableExists
BOOLEAN
VariableExists (
  IN CHAR16    *VariableName,
  IN EFI_GUID  *VendorGuid
  );

#endif // MISC_VARIABLE_LIB_H_
