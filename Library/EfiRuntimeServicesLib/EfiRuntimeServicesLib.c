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

#include <Library/UefiLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/MiscRuntimeLib.h>

#ifndef MDEPKG_NDEBUG

// mSetVirtualAddressMapExecution
STATIC BOOLEAN mSetVirtualAddressMapExecution = FALSE;

// mSetVirtualAddressMapReturned
STATIC BOOLEAN mSetVirtualAddressMapReturned = FALSE;

#endif // !MDEPKG_NDEBUG

// Time Services

// EfiGetTime
/** Returns the current time and date information, and the time-keeping
    capabilities of the hardware platform.

  @param[out] Time          A pointer to storage to receive a snapshot of the
                            current time.
  @param[out] Capabilities  An optional pointer to a buffer to receive the real
                            time clock device's capabilities.

  @retval EFI_SUCCESS            The operation completed successfully.
  @retval EFI_INVALID_PARAMETER  Time is NULL.
  @retval EFI_DEVICE_ERROR       The time could not be retrieved due to
                                 hardware error.
**/
EFI_STATUS
EfiGetTime (
  OUT EFI_TIME               *Time,
  OUT EFI_TIME_CAPABILITIES  *Capabilities OPTIONAL
  )
{
  EFI_STATUS Status;

  ASSERT (Time != NULL);
  ASSERT (EfiAtRuntime () || (EfiGetCurrentTpl () <= TPL_CALLBACK));

  Status = gRT->GetTime (Time, Capabilities);

  ASSERT_EFI_ERROR (Status);

  return Status;
}

// EfiSetTime
/** Sets the current local time and date information.

  @param[in] Time  A pointer to the current time.

  @retval EFI_SUCCESS            The operation completed successfully.
  @retval EFI_INVALID_PARAMETER  A time field is out of range.
  @retval EFI_DEVICE_ERROR       The time could not be set due due to hardware
                                 error.
**/
EFI_STATUS
EfiSetTime (
  IN EFI_TIME  *Time
  )
{
  EFI_STATUS Status;

  ASSERT (Time != NULL);
  ASSERT ((Time->Year >= 1900) && (Time->Year <= 9999));
  ASSERT (Time->Month <= 12);
  ASSERT (Time->Day <= 31);
  ASSERT (Time->Hour < 24);
  ASSERT (Time->Minute < 60);
  ASSERT (Time->Second < 60);
  ASSERT (Time->Nanosecond < 1000000000);
  ASSERT ((Time->TimeZone >= -1440) && (Time->TimeZone <= 2047));
  ASSERT (EfiAtRuntime () || (EfiGetCurrentTpl () <= TPL_CALLBACK));

  Status = gRT->SetTime (Time);

  ASSERT_EFI_ERROR (Status);

  return Status;
}

// EfiGetWakeupTime
/** Returns the current wakeup alarm clock setting.

  @param[out]  Enabled  Indicates if the alarm is currently enabled or
                        disabled.
  @param[out]  Pending  Indicates if the alarm signal is pending and requires
                        acknowledgement.
  @param[out]  Time     The current alarm setting.

  @retval EFI_SUCCESS            The alarm settings were returned.
  @retval EFI_INVALID_PARAMETER  Enabled is NULL.
  @retval EFI_INVALID_PARAMETER  Pending is NULL.
  @retval EFI_INVALID_PARAMETER  Time is NULL.
  @retval EFI_DEVICE_ERROR       The wakeup time could not be retrieved due to
                                 a hardware error.
  @retval EFI_UNSUPPORTED        A wakeup timer is not supported on this
                                 platform.
**/
EFI_STATUS
EfiGetWakeupTime (
  OUT BOOLEAN   *Enabled,
  OUT BOOLEAN   *Pending,
  OUT EFI_TIME  *Time
  )
{
  EFI_STATUS Status;

  ASSERT (Enabled != NULL);
  ASSERT (Pending != NULL);
  ASSERT (Time != NULL);
  ASSERT (EfiAtRuntime () || (EfiGetCurrentTpl () <= TPL_CALLBACK));

  Status = gRT->GetWakeupTime (Enabled, Pending, Time);

  if (Status != EFI_UNSUPPORTED) {
    ASSERT_EFI_ERROR (Status);
  }

  return Status;
}

// EfiSetWakeupTime
/** Sets the system wakeup alarm clock time.

  @param[in] Enable  Enable or disable the wakeup alarm.
  @param[in] Time    If Enable is TRUE, the time to set the wakeup alarm for.
                     If Enable is FALSE, then this parameter is optional, and
                     may be NULL.

  @retval EFI_SUCCESS            If Enable is TRUE, then the wakeup alarm was
                                 enabled.
                                 If Enable is FALSE, then the wakeup alarm was
                                 disabled.
  @retval EFI_INVALID_PARAMETER  A time field is out of range.
  @retval EFI_DEVICE_ERROR       The wakeup time could not be set due to a
                                 hardware error.
  @retval EFI_UNSUPPORTED        A wakeup timer is not supported on this
                                 platform.
**/
EFI_STATUS
EfiSetWakeupTime (
  IN BOOLEAN   Enable,
  IN EFI_TIME  *Time OPTIONAL
  )
{
  EFI_STATUS Status;

  ASSERT ((Time != NULL) || !Enable);
  ASSERT (EfiAtRuntime () || (EfiGetCurrentTpl () <= TPL_CALLBACK));

  Status = gRT->SetWakeupTime (Enable, Time);

  if (Status != EFI_UNSUPPORTED) {
    ASSERT_EFI_ERROR (Status);
  }

  return Status;
}

// Virtual Memory Services

// EfiSetVirtualAddressMap
/** Changes the runtime addressing mode of EFI firmware from physical to
    virtual.

  @param[in] MemoryMapSize      The size in bytes of VirtualMap.
  @param[in] DescriptorSize     The size in bytes of an entry in the
                                VirtualMap.
  @param[in] DescriptorVersion  The version of the structure entries in
                                VirtualMap.
  @param[in] VirtualMap         An array of memory descriptors which contain
                                new virtual
                                address mapping information for all runtime
                                ranges.

  @retval EFI_SUCCESS            The virtual address map has been applied.
  @retval EFI_UNSUPPORTED        EFI firmware is not at runtime, or the EFI
                                 firmware is already in virtual address mapped
                                 mode.
  @retval EFI_INVALID_PARAMETER  DescriptorSize or DescriptorVersion is
                                 invalid.
  @retval EFI_NO_MAPPING         A virtual address was not supplied for a range
                                 in the memory
                                 map that requires a mapping.
  @retval EFI_NOT_FOUND          A virtual address was supplied for an address
                                 that is not found in the memory map.
**/
EFI_STATUS
EfiSetVirtualAddressMap (
  IN UINTN                  MemoryMapSize,
  IN UINTN                  DescriptorSize,
  IN UINT32                 DescriptorVersion,
  IN EFI_MEMORY_DESCRIPTOR  *VirtualMap
  )
{
  EFI_STATUS Status;

  ASSERT (MemoryMapSize > 0);
  ASSERT (DescriptorSize > 0);
  ASSERT (DescriptorSize < MemoryMapSize);
  ASSERT ((MemoryMapSize % DescriptorSize) == 0);
  ASSERT (VirtualMap != NULL);
  ASSERT (!mSetVirtualAddressMapReturned && !mSetVirtualAddressMapExecution);
  ASSERT (EfiAtRuntime () || EfiGetCurrentTpl () <= TPL_NOTIFY);

  DEBUG_CODE (
    mSetVirtualAddressMapExecution = TRUE;
    );

  Status = gRT->SetVirtualAddressMap (
                  MemoryMapSize,
                  DescriptorSize,
                  DescriptorVersion,
                  VirtualMap
                  );

  if (Status != EFI_BUFFER_TOO_SMALL) {
    ASSERT_EFI_ERROR (Status);
  }

  DEBUG_CODE (
    mSetVirtualAddressMapExecution = FALSE;

    if (!EFI_ERROR (Status)) {
      mSetVirtualAddressMapReturned = TRUE;
    }
    );

  return Status;
}

// EfiConvertPointer
/** This service is a wrapper for the UEFI Runtime Function ConvertPointer().

  The ConvertPointer() function is used by an EFI component during the
  SetVirtualAddressMap() operation.  ConvertPointer() must be called using
  physical address pointers during the execution of SetVirtualAddressMap().

  @param[in]      DebugDisposition  Supplies type information for the pointer
                                    being converted.
  @param[in, out] Address           The pointer to a pointer that is to be
                                    fixed to be the value needed for the new
                                    virtual address mapping being applied.

  @retval EFI_SUCCESS            The pointer pointed to by Address was
                                 modified.
  @retval EFI_NOT_FOUND          The pointer pointed to by Address was not
                                 found to be part of the current memory map.
                                 This is normally fatal.
  @retval EFI_INVALID_PARAMETER  1) Address is NULL.
                                 2) *Address is NULL and DebugDisposition does
                                 not have the EFI_OPTIONAL_PTR bit set.
**/
EFI_STATUS
EfiConvertPointer (
  IN     UINTN  DebugDisposition,
  IN OUT VOID   **Address
  )
{
  EFI_STATUS Status;

  ASSERT (Address != NULL);
  ASSERT ((*Address != NULL) || ((DebugDisposition & EFI_OPTIONAL_PTR) != 0));
  ASSERT (mSetVirtualAddressMapExecution && !mSetVirtualAddressMapReturned);
  ASSERT (EfiAtRuntime () || EfiGetCurrentTpl () <= TPL_NOTIFY);

  Status = gRT->ConvertPointer (DebugDisposition, Address);

  ASSERT_EFI_ERROR (Status);

  return Status;
}

// Variable Services

// EfiGetVariable
/** Returns the value of a variable.

  @param[in]      VariableName  A Null-terminated string that is the name of
                                the vendor's variable.
  @param[in]      VendorGuid    A unique identifier for the vendor.
  @param[out]     Attributes    If not NULL, a pointer to the memory location
                                to return the attributes bitmask for the
                                variable.
  @param[in, out] DataSize      On input, the size in bytes of the return Data
                                Buffer.
                                On output the size of data returned in Data.
  @param[out]     Data          The Buffer to return the contents of the
                                variable.

  @retval EFI_SUCCESS             The function completed successfully.
  @retval EFI_NOT_FOUND           The variable was not found.
  @retval EFI_BUFFER_TOO_SMALL    The DataSize is too small for the result.
  @retval EFI_INVALID_PARAMETER   VariableName is NULL.
  @retval EFI_INVALID_PARAMETER   VendorGuid is NULL.
  @retval EFI_INVALID_PARAMETER   DataSize is NULL.
  @retval EFI_INVALID_PARAMETER   The DataSize is not too small and Data is
                                  NULL.
  @retval EFI_DEVICE_ERROR        The variable could not be retrieved due to a
                                  hardware error.
  @retval EFI_SECURITY_VIOLATION  The variable could not be retrieved due to an
                                  authentication failure.
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
  ASSERT (EfiAtRuntime () || (EfiGetCurrentTpl () <= TPL_CALLBACK));

  Status = gRT->GetVariable (VariableName, VendorGuid, Attributes, DataSize, Data);

  if ((Status != EFI_NOT_FOUND)
   && (Status != EFI_BUFFER_TOO_SMALL)
   && (Status != EFI_SECURITY_VIOLATION)) {
    ASSERT_EFI_ERROR (Status);
  }

  return Status;
}

// EfiGetNextVariableName
/** Enumerates the current variable names.

  @param[in, out] VariableNameSize  The size of the VariableName Buffer.
  @param[in, out] VariableName      On input, supplies the last VariableName
                                    that was returned by
                                    GetNextVariableName().  On output, returns
                                    the Nullterminated string of the current
                                    variable.
  @param[in, out] VendorGuid        On input, supplies the last VendorGuid that
                                    was returned by GetNextVariableName().
                                    On output, returns the VendorGuid of the
                                    current variable.

  @retval EFI_SUCCESS            The function completed successfully.
  @retval EFI_NOT_FOUND          The next variable was not found.
  @retval EFI_BUFFER_TOO_SMALL   The VariableNameSize is too small for the
                                 result.
  @retval EFI_INVALID_PARAMETER  VariableNameSize is NULL.
  @retval EFI_INVALID_PARAMETER  VariableName is NULL.
  @retval EFI_INVALID_PARAMETER  VendorGuid is NULL.
  @retval EFI_DEVICE_ERROR       The variable could not be retrieved due to a
                                 hardware error.
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
  ASSERT (EfiAtRuntime () || (EfiGetCurrentTpl () <= TPL_CALLBACK));

  Status = gRT->GetNextVariableName (
                  VariableNameSize,
                  VariableName,
                  VendorGuid
                  );

  if ((Status != EFI_NOT_FOUND)
   && (Status != EFI_BUFFER_TOO_SMALL)
   && (Status != EFI_DEVICE_ERROR)) {
    ASSERT_EFI_ERROR (Status);
  }

  return Status;
}

// EfiSetVariable
/** Sets the value of a variable.

  @param[in] VariableName  A Null-terminated string that is the name of the
                           vendor's variable.  Each VariableName is unique for
                           each VendorGuid.  VariableName must contain 1 or
                           more characters.  If VariableName is an empty
                           string, then EFI_INVALID_PARAMETER is returned.
  @param[in] VendorGuid    A unique identifier for the vendor.
  @param[in] Attributes    Attributes bitmask to set for the variable.
  @param[in] DataSize      The size in bytes of the Data Buffer.  Unless the
                           EFI_VARIABLE_APPEND_WRITE,
                           EFI_VARIABLE_AUTHENTICATED_WRITE_ACCESS, or
                           EFI_VARIABLE_TIME_BASED_AUTHENTICATED_WRITE_ACCESS
                           attribute is set, a size of zero causes the variable
                           to be deleted.  When the EFI_VARIABLE_APPEND_WRITE
                           attribute is set, then a SetVariable() call with a
                           DataSize of zero will not cause any change to the
                           variable value (the timestamp associated with the
                           variable may be updated however even if no new data
                           value is provided,see the description of the
                           EFI_VARIABLE_AUTHENTICATION_2 descriptor below.  In
                           this case the DataSize will not be zero since the
                           EFI_VARIABLE_AUTHENTICATION_2 descriptor will be
                           populated).
  @param[in] Data          The contents for the variable.

  @retval EFI_SUCCESS             The firmware has successfully stored the
                                  variable and its data as defined by the
                                  Attributes.
  @retval EFI_INVALID_PARAMETER   An invalid combination of attribute bits,
                                  name, and GUID was supplied, or the
                                  DataSize exceeds the maximum allowed.
  @retval EFI_INVALID_PARAMETER   VariableName is an empty string.
  @retval EFI_OUT_OF_RESOURCES    Not enough storage is available to hold the
                                  variable and its data.
  @retval EFI_DEVICE_ERROR        The variable could not be retrieved due to a
                                  hardware error.
  @retval EFI_WRITE_PROTECTED     The variable in question is read-only.
  @retval EFI_WRITE_PROTECTED     The variable in question cannot be deleted.
  @retval EFI_SECURITY_VIOLATION  The variable could not be written due to
                                  EFI_VARIABLE_AUTHENTICATED_WRITE_ACCESS or
                                  EFI_VARIABLE_TIME_BASED_AUTHENTICATED_WRITE_ACESS
                                  being set, but the AuthInfo does NOT pass the
                                  validation check carried out by the firmware.
  @retval EFI_NOT_FOUND           The variable trying to be updated or deleted
                                  was not found.
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
  ASSERT (EfiAtRuntime () || (EfiGetCurrentTpl () <= TPL_CALLBACK));

  Status = gRT->SetVariable (
                  VariableName,
                  VendorGuid,
                  Attributes,
                  DataSize,
                  Data
                  );

  if ((Status != EFI_WRITE_PROTECTED)
   && (Status != EFI_SECURITY_VIOLATION)
   && (Status != EFI_NOT_FOUND)) {
    ASSERT_EFI_ERROR (Status);
  }

  return Status;
}

// Miscellaneous Services

// EfiGetNextHighMonotonicCount
/** Returns the next high 32 bits of the platform's monotonic counter.

  @param[out] HighCount  The pointer to returned value.

  @retval EFI_SUCCESS            The next high monotonic count was returned.
  @retval EFI_INVALID_PARAMETER  HighCount is NULL.
  @retval EFI_DEVICE_ERROR       The device is not functioning properly.
**/
EFI_STATUS
EfiGetNextHighMonotonicCount (
  OUT UINT32  *HighCount
  )
{
  EFI_STATUS Status;

  ASSERT (HighCount != NULL);
  ASSERT (EfiAtRuntime () || (EfiGetCurrentTpl () <= TPL_NOTIFY));

  Status = gRT->GetNextHighMonotonicCount (HighCount);

  ASSERT_EFI_ERROR (Status);

  return Status;
}

// EfiResetSystem
/** Resets the entire platform.

  @param[in] ResetType    The type of reset to perform.
  @param[in] ResetStatus  The status code for the reset.
  @param[in] DataSize     The size, in bytes, of WatchdogData.
  @param[in] ResetData    For a ResetType of EfiResetCold, EfiResetWarm, or
                          EfiResetShutdown the data Buffer starts with a
                          Null-terminated string, optionally followed by
                          additional binary data.
**/
VOID
EfiResetSystem (
  IN EFI_RESET_TYPE  ResetType,
  IN EFI_STATUS      ResetStatus,
  IN UINTN           DataSize,
  IN VOID            *ResetData OPTIONAL
  )
{
  ASSERT ((((DataSize > 0) ? 1 : 0) ^ ((ResetData == NULL) ? 1 : 0)) != 0);
  ASSERT (EfiAtRuntime () || (EfiGetCurrentTpl () <= TPL_NOTIFY));

  gRT->ResetSystem (ResetType, ResetStatus, DataSize, ResetData);

  // ResetSystem may not return.
  _ASSERT (Platform reset has failed.);
}

// Miscellaneous UEFI 2.0 Service

// EfiQueryVariableInfo
/** Returns information about the EFI variables.

  @param[in]  Attributes                    Attributes bitmask to specify the
                                            type of variables on which to return
                                            information.
  @param[out] MaximumVariableStorageSize    On output the maximum size of the
                                            storage space
                                            available for the EFI variables
                                            associated with the attributes
                                            specified.
  @param[out] RemainingVariableStorageSize  Returns the remaining size of the
                                            storage space available for the EFI
                                            variables associated with the
                                            attributes specified.
  @param[out] MaximumVariableSize           Returns the maximum size of the
                                            individual EFI variables associated
                                            with the attributes specified.

  @retval EFI_SUCCESS            Valid answer returned.
  @retval EFI_INVALID_PARAMETER  An invalid combination of attribute bits was
                                 supplied
  @retval EFI_UNSUPPORTED        The attribute is not supported on this
                                 platform, and the MaximumVariableStorageSize,
                                 RemainingVariableStorageSize,
                                 MaximumVariableSize are undefined.
**/
EFI_STATUS
EfiQueryVariableInfo (
  IN  UINT32  Attributes,
  OUT UINT64  *MaximumVariableStorageSize,
  OUT UINT64  *RemainingVariableStorageSize,
  OUT UINT64  *MaximumVariableSize
  )
{
  EFI_STATUS Status;

  ASSERT (MaximumVariableStorageSize != NULL);
  ASSERT (RemainingVariableStorageSize != NULL);
  ASSERT (MaximumVariableSize != NULL);
  ASSERT (EfiAtRuntime () || (EfiGetCurrentTpl () <= TPL_CALLBACK));

  Status = gRT->QueryVariableInfo (
                  Attributes,
                  MaximumVariableStorageSize,
                  RemainingVariableStorageSize,
                  MaximumVariableSize
                  );

  ASSERT_EFI_ERROR (Status);

  return Status;
}
