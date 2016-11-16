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

#include <Guid/EventGroup.h>

#include <Library/DebugLib.h>
#include <Library/EfiBootServicesLib.h>
#include <Library/MiscRuntimeLib.h>
#include <Library/MiscEventLib.h>

// CreateTimerEvent
EFI_EVENT
CreateTimerEvent (
  IN EFI_EVENT_NOTIFY  NotifyFunction,
  IN VOID              *NotifyContext,
  IN UINT64            TriggerTime,
  IN BOOLEAN           SignalPeriodic,
  IN EFI_TPL           NotifyTpl
  )
{
  EFI_EVENT  Event;

  EFI_STATUS Status;

  ASSERT (NotifyTpl < TPL_CALLBACK);
  ASSERT (!EfiAtRuntime ());

  Event = NULL;

  if (NotifyTpl < TPL_CALLBACK) {
    Status = EfiCreateEvent (
               ((NotifyFunction != NULL)
                 ? (EVT_TIMER | EVT_NOTIFY_SIGNAL)
                 : EVT_TIMER),
               NotifyTpl,
               NotifyFunction,
               NotifyContext,
               &Event
               );

    if (!EFI_ERROR (Status)) {
      Status = EfiSetTimer (
                 Event,
                 (SignalPeriodic ? TimerPeriodic : TimerRelative),
                 TriggerTime
                 );

      if (EFI_ERROR (Status)) {
        EfiCloseEvent (Event);

        Event = NULL;
      }
    }
  }

  ASSERT (Event != NULL);

  return Event;
}

// CreateNotifyEvent
EFI_EVENT
CreateNotifyEvent (
  IN EFI_EVENT_NOTIFY  NotifyFunction,
  IN VOID              *NotifyContext,
  IN UINT64            TriggerTime,
  IN BOOLEAN           SignalPeriodic
  )
{
  ASSERT (!EfiAtRuntime ());

  return CreateTimerEvent (
           NotifyFunction,
           NotifyContext,
           TriggerTime,
           SignalPeriodic,
           TPL_NOTIFY
           );
}

// CancelTimer
EFI_STATUS
CancelTimer (
  IN EFI_EVENT  Event
  )
{
  return EfiSetTimer (Event, TimerCancel, 0);
}

// CancelEvent
VOID
CancelEvent (
  IN EFI_EVENT  Event
  )
{
  EFI_STATUS Status;

  ASSERT (!EfiAtRuntime ());

  Status = CancelTimer (Event);

  if (!EFI_ERROR (Status)) {
    EfiCloseEvent (Event);
  }
}

// CreateSignalEvent
EFI_EVENT
CreateSignalEvent (
  IN EFI_EVENT_NOTIFY  NotifyFunction, OPTIONAL
  IN CONST VOID        *NotifyContext OPTIONAL
  )
{
  EFI_EVENT Event;

  ASSERT (!EfiAtRuntime ());

  Event = NULL;

  return EfiCreateEvent (
           EVT_NOTIFY_SIGNAL,
           TPL_NOTIFY,
           NotifyFunction,
           NotifyContext,
           &Event
           );

  return Event;
}

// CreateSignalEventEx
EFI_EVENT
CreateSignalEventEx (
  IN EFI_EVENT_NOTIFY  NotifyFunction, OPTIONAL
  IN CONST VOID        *NotifyContext, OPTIONAL
  IN CONST EFI_GUID    *EventGroup OPTIONAL
  )
{
  EFI_EVENT Event;

  ASSERT (!EfiAtRuntime ());

  Event = NULL;

  return EfiCreateEventEx (
           EVT_NOTIFY_SIGNAL,
           TPL_NOTIFY,
           NotifyFunction,
           NotifyContext,
           EventGroup,
           &Event
           );

  return Event;
}

// CreateExitBootServicesEvent
EFI_EVENT
CreateExitBootServicesEvent (
  IN EFI_EVENT_NOTIFY  NotifyFunction, OPTIONAL
  IN CONST VOID        *NotifyContext OPTIONAL
  )
{
  ASSERT (!EfiAtRuntime ());

  return CreateSignalEvent (
           NotifyFunction,
           NotifyContext,
           &gEfiEventExitBootServicesGuid
           );
}

// CreateVirtualAddressChangeEvent
EFI_EVENT
CreateVirtualAddressChangeEvent (
  IN EFI_EVENT_NOTIFY  NotifyFunction, OPTIONAL
  IN CONST VOID        *NotifyContext OPTIONAL
  )
{
  ASSERT (!EfiAtRuntime ());

  return CreateSignalEvent (
           NotifyFunction,
           NotifyContext,
           &gEfiEventVirtualAddressChangeGuid
           );
}

// CreateMemoryMapChangeEvent
EFI_EVENT
CreateMemoryMapChangeEvent (
  IN EFI_EVENT_NOTIFY  NotifyFunction, OPTIONAL
  IN CONST VOID        *NotifyContext OPTIONAL
  )
{
  ASSERT (!EfiAtRuntime ());

  return CreateSignalEvent (
           NotifyFunction,
           NotifyContext,
           &gEfiEventMemoryMapChangeGuid
           );
}

// CreateReadyToBootEvent
EFI_EVENT
CreateReadyToBootEvent (
  IN EFI_EVENT_NOTIFY  NotifyFunction, OPTIONAL
  IN CONST VOID        *NotifyContext OPTIONAL
  )
{
  ASSERT (!EfiAtRuntime ());

  return CreateSignalEvent (
           NotifyFunction,
           NotifyContext,
           &gEfiEventReadyToBootGuid
           );
}

// CreateEventDxeDispatchGuidEvent
EFI_EVENT
CreateDxeDispatchGuidEvent (
  IN EFI_EVENT_NOTIFY  NotifyFunction, OPTIONAL
  IN CONST VOID        *NotifyContext OPTIONAL
  )
{
  ASSERT (!EfiAtRuntime ());

  return CreateSignalEvent (
           NotifyFunction,
           NotifyContext,
           &gEfiEventDxeDispatchGuid
           );
}

// CreateEndOfDxeEvent
EFI_EVENT
CreateEndOfDxeEvent (
  IN EFI_EVENT_NOTIFY  NotifyFunction, OPTIONAL
  IN CONST VOID        *NotifyContext OPTIONAL
  )
{
  ASSERT (!EfiAtRuntime ());

  return CreateSignalEvent (
           NotifyFunction,
           NotifyContext,
           &gEfiEndOfDxeEventGroupGuid
           );
}
