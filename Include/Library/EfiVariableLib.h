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

#ifndef EFI_VARIABLE_LIB_H_
#define EFI_VARIABLE_LIB_H_

// GetVariable
/** Returns the value of a variable.

  @param[in]      VariableName  A Null-terminated string that is the name of the vendor's
                                variable.
  @param[in]      VendorGuid    A unique identifier for the vendor.
  @param[out]     Attributes    If not NULL, a pointer to the memory location to return the
                                attributes bitmask for the variable.
  @param[in, out] DataSize      On input, the size in bytes of the return Data buffer.
                                On output the size of data returned in Data.
  @param[out]     Data          The buffer to return the contents of the variable.

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
GetVariable (
  IN     CHAR16    *VariableName,
  IN     EFI_GUID  *VendorGuid,
  OUT    UINT32    *Attributes, OPTIONAL
  IN OUT UINTN     *DataSize,
  OUT    VOID      *Data
  );

// GetNextVariableName
/** Enumerates the current variable names.

  @param[in, out] VariableNameSize  The size of the VariableName buffer.
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
GetNextVariableName (
  IN OUT UINTN     *VariableNameSize,
  IN OUT CHAR16    *VariableName,
  IN OUT EFI_GUID  *VendorGuid
  );

// SetVariable
/** Sets the value of a variable.

  @param[in] VariableName  A Null-terminated string that is the name of the vendor's variable.
                           Each VariableName is unique for each VendorGuid.  VariableName must
                           contain 1 or more characters.  If VariableName is an empty string,
                           then EFI_INVALID_PARAMETER is returned.
  @param[in] VendorGuid    A unique identifier for the vendor.
  @param[in] Attributes    Attributes bitmask to set for the variable.
  @param[in] DataSize      The size in bytes of the Data buffer.  Unless the EFI_VARIABLE_APPEND_WRITE,
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
SetVariable (
  IN CHAR16    *VariableName,
  IN EFI_GUID  *VendorGuid,
  IN UINT32    Attributes,
  IN UINTN     DataSize,
  IN VOID      *Data
  );

// VariableExists
BOOLEAN
VariableExists (
  IN CHAR16    *VariableName,
  IN EFI_GUID  *VendorGuid
  );

#endif // EFI_VARIABLE_LIB_H_
