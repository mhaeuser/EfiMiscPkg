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

#include <Uefi.h>

#include <Guid/EventGroup.h>

#include <Library/DebugLib.h>
#include <Library/EfiBootServicesLib.h>
#include <Library/MiscRuntimeLib.h>
#include <Library/MiscEventLib.h>

// MiscCreateTimerEvent
EFI_EVENT
MiscCreateTimerEvent (
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

// MiscCreateNotifyTimerEvent
EFI_EVENT
MiscCreateNotifyTimerEvent (
  IN EFI_EVENT_NOTIFY  NotifyFunction,
  IN VOID              *NotifyContext,
  IN UINT64            TriggerTime,
  IN BOOLEAN           SignalPeriodic
  )
{
  ASSERT (!EfiAtRuntime ());

  return MiscCreateTimerEvent (
           NotifyFunction,
           NotifyContext,
           TriggerTime,
           SignalPeriodic,
           TPL_NOTIFY
           );
}

// MiscCancelTimer
EFI_STATUS
MiscCancelTimer (
  IN EFI_EVENT  Event
  )
{
  return EfiSetTimer (Event, TimerCancel, 0);
}

// MiscCancelEvent
VOID
MiscCancelEvent (
  IN EFI_EVENT  Event
  )
{
  EFI_STATUS Status;

  ASSERT (!EfiAtRuntime ());

  Status = MiscCancelTimer (Event);

  if (!EFI_ERROR (Status)) {
    EfiCloseEvent (Event);
  }
}

// MiscCreateNotifySignalEvent
EFI_EVENT
MiscCreateNotifySignalEvent (
  IN EFI_EVENT_NOTIFY  NotifyFunction, OPTIONAL
  IN VOID              *NotifyContext OPTIONAL
  )
{
  EFI_EVENT Event;

  ASSERT (!EfiAtRuntime ());

  Event = NULL;

  EfiCreateEvent (
    EVT_NOTIFY_SIGNAL,
    TPL_NOTIFY,
    NotifyFunction,
    NotifyContext,
    &Event
    );

  return Event;
}

// MiscCreateSignalEventEx
EFI_EVENT
MiscCreateSignalEventEx (
  IN EFI_EVENT_NOTIFY  NotifyFunction, OPTIONAL
  IN CONST VOID        *NotifyContext, OPTIONAL
  IN CONST EFI_GUID    *EventGroup OPTIONAL
  )
{
  EFI_EVENT Event;

  ASSERT (!EfiAtRuntime ());

  Event = NULL;

  EfiCreateEventEx (
    EVT_NOTIFY_SIGNAL,
    TPL_NOTIFY,
    NotifyFunction,
    NotifyContext,
    EventGroup,
    &Event
    );

  return Event;
}

// MiscCreateExitBootServicesEvent
EFI_EVENT
MiscCreateExitBootServicesEvent (
  IN EFI_EVENT_NOTIFY  NotifyFunction, OPTIONAL
  IN CONST VOID        *NotifyContext OPTIONAL
  )
{
  ASSERT (!EfiAtRuntime ());

  return MiscCreateSignalEventEx (
           NotifyFunction,
           NotifyContext,
           &gEfiEventExitBootServicesGuid
           );
}

// MiscCreateVirtualAddressChangeEvent
EFI_EVENT
MiscCreateVirtualAddressChangeEvent (
  IN EFI_EVENT_NOTIFY  NotifyFunction, OPTIONAL
  IN CONST VOID        *NotifyContext OPTIONAL
  )
{
  ASSERT (!EfiAtRuntime ());

  return MiscCreateSignalEventEx (
           NotifyFunction,
           NotifyContext,
           &gEfiEventVirtualAddressChangeGuid
           );
}

// MiscCreateMemoryMapChangeEvent
EFI_EVENT
MiscCreateMemoryMapChangeEvent (
  IN EFI_EVENT_NOTIFY  NotifyFunction, OPTIONAL
  IN CONST VOID        *NotifyContext OPTIONAL
  )
{
  ASSERT (!EfiAtRuntime ());

  return MiscCreateSignalEventEx (
           NotifyFunction,
           NotifyContext,
           &gEfiEventMemoryMapChangeGuid
           );
}

// MiscCreateReadyToBootEvent
EFI_EVENT
MiscCreateReadyToBootEvent (
  IN EFI_EVENT_NOTIFY  NotifyFunction, OPTIONAL
  IN CONST VOID        *NotifyContext OPTIONAL
  )
{
  ASSERT (!EfiAtRuntime ());

  return MiscCreateSignalEventEx (
           NotifyFunction,
           NotifyContext,
           &gEfiEventReadyToBootGuid
           );
}

// CreateEventDxeDispatchGuidEvent
EFI_EVENT
MiscCreateDxeDispatchGuidEvent (
  IN EFI_EVENT_NOTIFY  NotifyFunction, OPTIONAL
  IN CONST VOID        *NotifyContext OPTIONAL
  )
{
  ASSERT (!EfiAtRuntime ());

  return MiscCreateSignalEventEx (
           NotifyFunction,
           NotifyContext,
           &gEfiEventDxeDispatchGuid
           );
}

// MiscCreateEndOfDxeEvent
EFI_EVENT
MiscCreateEndOfDxeEvent (
  IN EFI_EVENT_NOTIFY  NotifyFunction, OPTIONAL
  IN CONST VOID        *NotifyContext OPTIONAL
  )
{
  ASSERT (!EfiAtRuntime ());

  return MiscCreateSignalEventEx (
           NotifyFunction,
           NotifyContext,
           &gEfiEndOfDxeEventGroupGuid
           );
}
