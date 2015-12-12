//
// Copyright 2015 CupertinoNet
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

///
/// @file      Include/Library/EfiMiscLib.h
///
///            
///
/// @author    Download-Fritz
/// @date      15/09/2015: Initial version
/// @copyright Copyright (C) 2015 CupertinoNet. All rights reserved.
///

#ifndef __EFI_MISC_LIB_H__
#define __EFI_MISC_LIB_H__

// RaiseTpl
/// Raises a task's priority level and returns its previous level.
///
/// @param[in] NewTpl  The new task priority level.
///
/// @return Previous task priority level
EFI_TPL
RaiseTpl (
  IN EFI_TPL  NewTpl
  );

// RestoreTpl
/// Restores a task's priority level to its previous value.
///
/// @param[in] OldTpl  The previous task priority level to restore.
VOID
RestoreTpl (
  IN EFI_TPL  OldTpl
  );

// Stall
/// Induces a fine-grained stall.
///
/// @param[in] Microseconds  The number of microseconds to stall execution.
///
/// @retval EFI_SUCCESS  Execution was stalled at least the requested number of
///                      Microseconds.
EFI_STATUS
Stall (
  IN UINTN  Microseconds
  );

// SetWatchdogTimer
/// Sets the system's watchdog timer.
///
/// @param[in] Timeout       The number of seconds to set the watchdog timer to.
/// @param[in] WatchdogCode  The numeric code to log on a watchdog timer timeout event.
/// @param[in] DataSize      The size, in bytes, of WatchdogData.
/// @param[in] WatchdogData  A data buffer that includes a Null-terminated string, optionally
///                          followed by additional binary data.
///
/// @retval EFI_SUCCESS            The timeout has been set.
/// @retval EFI_INVALID_PARAMETER  The supplied WatchdogCode is invalid.
/// @retval EFI_UNSUPPORTED        The system does not have a watchdog timer.
/// @retval EFI_DEVICE_ERROR       The watchdog timer could not be programmed due to a hardware
///                                error.
EFI_STATUS
SetWatchdogTimer (
  IN UINTN   Timeout,
  IN UINT64  WatchdogCode,
  IN UINTN   DataSize,
  IN CHAR16  *WatchdogData OPTIONAL
  );

// ResetSystem
/// Resets the entire platform.
///
/// @param[in] ResetType    The type of reset to perform.
/// @param[in] ResetStatus  The status code for the reset.
/// @param[in] DataSize     The size, in bytes, of WatchdogData.
/// @param[in] ResetData    For a ResetType of EfiResetCold, EfiResetWarm, or
///                         EfiResetShutdown the data buffer starts with a Null-terminated
///                         string, optionally followed by additional binary data.
VOID
ResetSystem (
  IN EFI_RESET_TYPE  ResetType,
  IN EFI_STATUS      ResetStatus,
  IN UINTN           DataSize,
  IN VOID            *ResetData OPTIONAL
  );

// GetNextMonotonicCount
/// Returns a monotonically increasing count for the platform.
///
/// @param[out] Count  The pointer to returned value.
///
/// @retval EFI_SUCCESS            The next monotonic count was returned.
/// @retval EFI_INVALID_PARAMETER  Count is NULL.
/// @retval EFI_DEVICE_ERROR       The device is not functioning properly.
EFI_STATUS
GetNextMonotonicCount (
  OUT UINT64  *Count
  );

// GetNextHighMonotonicCount
/// Returns the next high 32 bits of the platform's monotonic counter.
///
/// @param[out] HighCount  The pointer to returned value.
///
/// @retval EFI_SUCCESS            The next high monotonic count was returned.
/// @retval EFI_INVALID_PARAMETER  HighCount is NULL.
/// @retval EFI_DEVICE_ERROR       The device is not functioning properly.
EFI_STATUS
GetNextHighMonotonicCount (
  OUT UINT32  *HighCount
  );

#endif // ifndef __EFI_MISC_LIB_H__
