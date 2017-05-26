/** @file
  Copyright (C) 2015 - 2017, CupertinoNet.  All rights reserved.<BR>

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

#ifndef MISC_EVENT_LIB_H_
#define MISC_EVENT_LIB_H_

// MiscCreateTimerEvent
EFI_EVENT
MiscCreateTimerEvent (
  IN EFI_EVENT_NOTIFY  NotifyFunction,
  IN VOID              *NotifyContext,
  IN UINT64            TriggerTime,
  IN BOOLEAN           SignalPeriodic,
  IN EFI_TPL           NotifyTpl
  );

// MiscCreateNotifyTimerEvent
EFI_EVENT
MiscCreateNotifyTimerEvent (
  IN EFI_EVENT_NOTIFY  NotifyFunction,
  IN VOID              *NotifyContext,
  IN UINT64            TriggerTime,
  IN BOOLEAN           SignalPeriodic
  );

// MiscCancelTimerEvent
VOID
MiscCancelTimerEvent (
  IN EFI_EVENT  Event
  );

// MiscCancelTimer
EFI_STATUS
MiscCancelTimer (
  IN EFI_EVENT  Event
  );

// MiscCreateNotifySignalEvent
EFI_EVENT
MiscCreateNotifySignalEvent (
  IN EFI_EVENT_NOTIFY  NotifyFunction, OPTIONAL
  IN VOID              *NotifyContext OPTIONAL
  );

// MiscCreateSignalEventEx
EFI_EVENT
MiscCreateSignalEventEx (
  IN EFI_EVENT_NOTIFY  NotifyFunction, OPTIONAL
  IN CONST VOID        *NotifyContext, OPTIONAL
  IN CONST EFI_GUID    *EventGroup OPTIONAL
  );

// CreateEfiVirtualAddressChangeEvent
EFI_EVENT
CreateEfiVirtualAddressChangeEvent (
  IN EFI_EVENT_NOTIFY  NotifyFunction, OPTIONAL
  IN CONST VOID        *NotifyContext OPTIONAL
  );

// MiscCreateExitBootServicesEvent
EFI_EVENT
MiscCreateExitBootServicesEvent (
  IN EFI_EVENT_NOTIFY  NotifyFunction, OPTIONAL
  IN CONST VOID        *NotifyContext OPTIONAL
  );

// MiscCreateVirtualAddressChangeEvent
EFI_EVENT
MiscCreateVirtualAddressChangeEvent (
  IN EFI_EVENT_NOTIFY  NotifyFunction, OPTIONAL
  IN CONST VOID        *NotifyContext OPTIONAL
  );

// CreateEfiExitBootServicesEvent
EFI_EVENT
MiscCreateMemoryMapChangeEvent (
  IN EFI_EVENT_NOTIFY  NotifyFunction, OPTIONAL
  IN CONST VOID        *NotifyContext OPTIONAL
  );

// CreateEfiExitBootServicesEvent
EFI_EVENT
MiscCreateReadyToBootEvent (
  IN EFI_EVENT_NOTIFY  NotifyFunction, OPTIONAL
  IN CONST VOID        *NotifyContext OPTIONAL
  );

// CreateEfiExitBootServicesEvent
EFI_EVENT
CreateDxeDispatchEvent (
  IN EFI_EVENT_NOTIFY  NotifyFunction, OPTIONAL
  IN CONST VOID        *NotifyContext OPTIONAL
  );

// MiscCreateEndOfDxeEvent
EFI_EVENT
MiscCreateEndOfDxeEvent (
  IN EFI_EVENT_NOTIFY  NotifyFunction, OPTIONAL
  IN CONST VOID        *NotifyContext OPTIONAL
  );

#endif // MISC_EVENT_LIB_H_
