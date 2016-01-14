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

#ifndef EFI_MISC_LIB_H_
#define EFI_MISC_LIB_H_

// EfiRaiseTpl
/** Raises a task's priority level and returns its previous level.

  @param[in] NewTpl  The new task priority level.

  @return  Previous task priority level
**/
EFI_TPL
EfiRaiseTpl (
  IN EFI_TPL  NewTpl
  );

// EfiRestoreTpl
/** Restores a task's priority level to its previous value.

  @param[in] OldTpl  The previous task priority level to restore.
**/
VOID
EfiRestoreTpl (
  IN EFI_TPL  OldTpl
  );

// EfiStall
/** Induces a fine-grained stall.

  @param[in] Microseconds  The number of microseconds to stall execution.

  @retval EFI_SUCCESS  Execution was stalled at least the requested number of
                       Microseconds.
**/
EFI_STATUS
EfiStall (
  IN UINTN  Microseconds
  );

// EfiSetWatchdogTimer
/** Sets the system's watchdog timer.
 
  @param[in] Timeout       The number of seconds to set the watchdog timer to.
  @param[in] WatchdogCode  The numeric code to log on a watchdog timer timeout event.
  @param[in] DataSize      The size, in bytes, of WatchdogData.
  @param[in] WatchdogData  A data buffer that includes a Null-terminated string, optionally
                           followed by additional binary data.

  @retval EFI_SUCCESS            The timeout has been set.
  @retval EFI_INVALID_PARAMETER  The supplied WatchdogCode is invalid.
  @retval EFI_UNSUPPORTED        The system does not have a watchdog timer.
  @retval EFI_DEVICE_ERROR       The watchdog timer could not be programmed due to a hardware
                                 error.
**/
EFI_STATUS
EfiSetWatchdogTimer (
  IN UINTN   Timeout,
  IN UINT64  WatchdogCode,
  IN UINTN   DataSize,
  IN CHAR16  *WatchdogData OPTIONAL
  );

// EfiResetSystem
/** Resets the entire platform.

  @param[in] ResetType    The type of reset to perform.
  @param[in] ResetStatus  The status code for the reset.
  @param[in] DataSize     The size, in bytes, of WatchdogData.
  @param[in] ResetData    For a ResetType of EfiResetCold, EfiResetWarm, or
                          EfiResetShutdown the data buffer starts with a Null-terminated
                          string, optionally followed by additional binary data.
**/
VOID
EfiResetSystem (
  IN EFI_RESET_TYPE  ResetType,
  IN EFI_STATUS      ResetStatus,
  IN UINTN           DataSize,
  IN VOID            *ResetData OPTIONAL
  );

// EfiGetNextMonotonicCount
/** Returns a monotonically increasing count for the platform.

  @param[out] Count  The pointer to returned value.

  @retval EFI_SUCCESS            The next monotonic count was returned.
  @retval EFI_INVALID_PARAMETER  Count is NULL.
  @retval EFI_DEVICE_ERROR       The device is not functioning properly.
**/
EFI_STATUS
EfiGetNextMonotonicCount (
  OUT UINT64  *Count
  );

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
  );

#endif // EFI_MISC_LIB_H_
