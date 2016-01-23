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

#include <Guid/GlobalVariable.h>

#include <Library/UefiRuntimeLib.h>
#include <Library/DebugLib.h>
#include <Library/EfiVariableLib.h>

// EfiGetVariable
/** Returns the value of a variable.

  @param[in]      VariableName  A Null-terminated string that is the name of the vendor's
                                variable.
  @param[in]      VendorGuid    A unique identifier for the vendor.
  @param[out]     Attributes    If not NULL, a pointer to the memory location to return the
                                attributes bitmask for the variable.
  @param[in, out] DataSize      On input, the size in bytes of the return Data Buffer.
                                On output the size of data returned in Data.
  @param[out]     Data          The Buffer to return the contents of the variable.

  @retval EFI_SUCCESS             The function completed successfully.
  @retval EFI_NOT_FOUND           The variable was not found.
  @retval EFI_BUFFER_TOO_SMALL    The DataSize is too small for the result.
  @retval EFI_INVALID_PARAMETER   VariableName is NULL.
  @retval EFI_INVALID_PARAMETER   VendorGuid is NULL.
  @retval EFI_INVALID_PARAMETER   DataSize is NULL.
  @retval EFI_INVALID_PARAMETER   The DataSize is not too small and Data is NULL.
  @retval EFI_DEVICE_ERROR        The variable could not be retrieved due to a hardware error.
  @retval EFI_SECURITY_VIOLATION  The variable could not be retrieved due to an authentication failure.
**/
EFI_STATUS
EfiGetVariable (
  IN     CHAR16    *VariableName,
  IN     EFI_GUID  *VendorGuid,
  OUT    UINT32    *Attributes, OPTIONAL
  IN OUT UINTN     *DataSize,
  OUT    VOID      *Data
  )
{
  EFI_STATUS Status;

  ASSERT (VariableName != NULL);
  ASSERT (VariableName[0] != L'\0');
  ASSERT (VendorGuid != NULL);
  ASSERT (DataSize != NULL);
  ASSERT ((*DataSize == 0) || (Data != NULL));
  ASSERT ((*DataSize != 0) || (Data == NULL));

  Status = EfiGetVariable (VariableName, VendorGuid, Attributes, DataSize, Data);

  if ((Status != EFI_NOT_FOUND) && (Status != EFI_BUFFER_TOO_SMALL) && (Status != EFI_SECURITY_VIOLATION)) {
    ASSERT_EFI_ERROR (Status);
  }

  return Status;
}

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

  return EfiGetVariable (VariableName, &gEfiGlobalVariableGuid, Attributes, DataSize, Data);
}

// EfiGetNextVariableName
/** Enumerates the current variable names.

  @param[in, out] VariableNameSize  The size of the VariableName Buffer.
  @param[in, out] VariableName      On input, supplies the last VariableName that was returned
                                    by GetNextVariableName().  On output, returns the Nullterminated
                                    string of the current variable.
  @param[in, out] VendorGuid        On input, supplies the last VendorGuid that was returned by
                                    GetNextVariableName().  On output, returns the
                                    VendorGuid of the current variable.

  @retval EFI_SUCCESS            The function completed successfully.
  @retval EFI_NOT_FOUND          The next variable was not found.
  @retval EFI_BUFFER_TOO_SMALL   The VariableNameSize is too small for the result.
  @retval EFI_INVALID_PARAMETER  VariableNameSize is NULL.
  @retval EFI_INVALID_PARAMETER  VariableName is NULL.
  @retval EFI_INVALID_PARAMETER  VendorGuid is NULL.
  @retval EFI_DEVICE_ERROR       The variable could not be retrieved due to a hardware error.
**/
EFI_STATUS
EfiGetNextVariableName (
  IN OUT UINTN     *VariableNameSize,
  IN OUT CHAR16    *VariableName,
  IN OUT EFI_GUID  *VendorGuid
  )
{
  EFI_STATUS Status;

  ASSERT (VariableNameSize != NULL);
  ASSERT (VariableName != NULL);
  ASSERT (VariableName[0] != L'\0');
  ASSERT (VendorGuid != NULL);

  Status = EfiGetNextVariableName (VariableNameSize, VariableName, VendorGuid);

  if ((Status != EFI_NOT_FOUND) && (Status != EFI_BUFFER_TOO_SMALL) && (Status != EFI_DEVICE_ERROR)) {
    ASSERT_EFI_ERROR (Status);
  }

  return Status;
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

  return EfiGetNextVariableName (VariableNameSize, VariableName, &gEfiGlobalVariableGuid);
}

// EfiSetVariable
/** Sets the value of a variable.

  @param[in] VariableName  A Null-terminated string that is the name of the vendor's variable.
                           Each VariableName is unique for each VendorGuid.  VariableName must
                           contain 1 or more characters.  If VariableName is an empty string,
                           then EFI_INVALID_PARAMETER is returned.
  @param[in] VendorGuid    A unique identifier for the vendor.
  @param[in] Attributes    Attributes bitmask to set for the variable.
  @param[in] DataSize      The size in bytes of the Data Buffer.  Unless the EFI_VARIABLE_APPEND_WRITE,
                           EFI_VARIABLE_AUTHENTICATED_WRITE_ACCESS, or
                           EFI_VARIABLE_TIME_BASED_AUTHENTICATED_WRITE_ACCESS attribute is set, a size of zero
                           causes the variable to be deleted.  When the EFI_VARIABLE_APPEND_WRITE attribute is
                           set, then a SetVariable() call with a DataSize of zero will not cause any change to
                           the variable value (the timestamp associated with the variable may be updated however
                           even if no new data value is provided,see the description of the
                           EFI_VARIABLE_AUTHENTICATION_2 descriptor below.  In this case the DataSize will not
                           be zero since the EFI_VARIABLE_AUTHENTICATION_2 descriptor will be populated).
  @param[in] Data          The contents for the variable.

  @retval EFI_SUCCESS             The firmware has successfully stored the variable and its data as
                                  defined by the Attributes.
  @retval EFI_INVALID_PARAMETER   An invalid combination of attribute bits, name, and GUID was supplied, or the
                                  DataSize exceeds the maximum allowed.
  @retval EFI_INVALID_PARAMETER   VariableName is an empty string.
  @retval EFI_OUT_OF_RESOURCES    Not enough storage is available to hold the variable and its data.
  @retval EFI_DEVICE_ERROR        The variable could not be retrieved due to a hardware error.
  @retval EFI_WRITE_PROTECTED     The variable in question is read-only.
  @retval EFI_WRITE_PROTECTED     The variable in question cannot be deleted.
  @retval EFI_SECURITY_VIOLATION  The variable could not be written due to EFI_VARIABLE_AUTHENTICATED_WRITE_ACCESS
                                  or EFI_VARIABLE_TIME_BASED_AUTHENTICATED_WRITE_ACESS being set, but the AuthInfo
                                  does NOT pass the validation check carried out by the firmware.
  @retval EFI_NOT_FOUND           The variable trying to be updated or deleted was not found.
**/
EFI_STATUS
EfiSetVariable (
  IN CHAR16    *VariableName,
  IN EFI_GUID  *VendorGuid,
  IN UINT32    Attributes,
  IN UINTN     DataSize,
  IN VOID      *Data
  )
{
  EFI_STATUS Status;

  ASSERT (VariableName != NULL);
  ASSERT (VariableName[0] != L'\0');
  ASSERT (VendorGuid != NULL);
  ASSERT ((((DataSize > 0) ? 1 : 0) ^ ((Data == NULL) ? 1 : 0)) != 0);

  Status = EfiSetVariable (VariableName, VendorGuid, Attributes, DataSize, Data);

  if ((Status != EFI_WRITE_PROTECTED) && (Status != EFI_SECURITY_VIOLATION) && (Status != EFI_NOT_FOUND)) {
    ASSERT_EFI_ERROR (Status);
  }

  return Status;
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

  return EfiSetVariable (VariableName, &gEfiGlobalVariableGuid, Attributes, DataSize, Data);
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
