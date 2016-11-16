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

#ifndef MISC_EVENT_LIB_H_
#define MISC_EVENT_LIB_H_

// CreateTimerEvent
EFI_EVENT
CreateTimerEvent (
  IN EFI_EVENT_NOTIFY  NotifyFunction,
  IN VOID              *NotifyContext,
  IN UINT64            TriggerTime,
  IN BOOLEAN           SignalPeriodic,
  IN EFI_TPL           NotifyTpl
  );

// CreateNotifyEvent
EFI_EVENT
CreateNotifyEvent (
  IN EFI_EVENT_NOTIFY  NotifyFunction,
  IN VOID              *NotifyContext,
  IN UINT64            TriggerTime,
  IN BOOLEAN           SignalPeriodic
  );

// CancelEvent
VOID
CancelEvent (
  IN EFI_EVENT  Event
  );

// CancelTimer
EFI_STATUS
CancelTimer (
  IN EFI_EVENT  Event
  );

// CreateSignalEvent
EFI_EVENT
CreateSignalEvent (
  IN EFI_EVENT_NOTIFY  NotifyFunction, OPTIONAL
  IN CONST VOID        *NotifyContext OPTIONAL
  );

// CreateSignalEventEx
EFI_EVENT
CreateSignalEventEx (
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

// CreateExitBootServicesEvent
EFI_EVENT
CreateExitBootServicesEvent (
  IN EFI_EVENT_NOTIFY  NotifyFunction, OPTIONAL
  IN CONST VOID        *NotifyContext OPTIONAL
  );

// CreateVirtualAddressChangeEvent
EFI_EVENT
CreateVirtualAddressChangeEvent (
  IN EFI_EVENT_NOTIFY  NotifyFunction, OPTIONAL
  IN CONST VOID        *NotifyContext OPTIONAL
  );

// CreateEfiExitBootServicesEvent
EFI_EVENT
CreateMemoryMapChangeEvent (
  IN EFI_EVENT_NOTIFY  NotifyFunction, OPTIONAL
  IN CONST VOID        *NotifyContext OPTIONAL
  );

// CreateEfiExitBootServicesEvent
EFI_EVENT
CreateReadyToBootEvent (
  IN EFI_EVENT_NOTIFY  NotifyFunction, OPTIONAL
  IN CONST VOID        *NotifyContext OPTIONAL
  );

// CreateEfiExitBootServicesEvent
EFI_EVENT
CreateDxeDispatchEvent (
  IN EFI_EVENT_NOTIFY  NotifyFunction, OPTIONAL
  IN CONST VOID        *NotifyContext OPTIONAL
  );

// CreateEndOfDxeEvent
EFI_EVENT
CreateEndOfDxeEvent (
  IN EFI_EVENT_NOTIFY  NotifyFunction, OPTIONAL
  IN CONST VOID        *NotifyContext OPTIONAL
  );

#endif // MISC_EVENT_LIB_H_
