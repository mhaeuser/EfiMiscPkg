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
/// @file      Library/EfiEventLib/EfiEventLib.c
///
///            
///
/// @author    Download-Fritz
/// @date      18/08/2015: Initial version
/// @date      14/09/2015: Added BootServices wrappers
/// @copyright Copyright (C) 2015 CupertinoNet. All rights reserved.
///

#include <Uefi.h>

#include <Guid/EventGroup.h>

#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/EfiRuntimeLib.h>
#include <Library/EfiEventLib.h>

// CreateEvent
/// Creates an event.
///
/// @param[in] Type            The type of event to create and its mode and attributes.
/// @param[in] NotifyTpl       The task priority level of event notifications, if needed.
/// @param[in] NotifyFunction  The pointer to the event's notification function, if any.
/// @param[in] NotifyContext   The pointer to the notification function's context; corresponds to parameter
///                            Context in the notification function.
///
/// @retval EFI_SUCCESS            The event structure was created.
/// @retval EFI_INVALID_PARAMETER  One or more parameters are invalid.
/// @retval EFI_OUT_OF_RESOURCES   The event could not be allocated.
EFI_EVENT
CreateEvent (
  IN UINT32            Type,
  IN EFI_TPL           NotifyTpl,
  IN EFI_EVENT_NOTIFY  NotifyFunction,
  IN VOID              *NotifyContext
  )
{
  EFI_EVENT  Event;

  EFI_STATUS Status;

  ASSERT (!EfiAtRuntime ());

  Status = gBS->CreateEvent (Type, NotifyTpl, NotifyFunction, NotifyContext, &Event);

  DEBUG_CODE (
    if (Status != EFI_OUT_OF_RESOURCES) {
      ASSERT_EFI_ERROR (Status);
    }
    );

  if (EFI_ERROR (Status)) {
    Event = NULL;
  }

  return Event;
}

// CreateEventEx
/// Creates an event in a group.
///
/// @param[in]  Type            The type of event to create and its mode and attributes.
/// @param[in]  NotifyTpl       The task priority level of event notifications,if needed.
/// @param[in]  NotifyFunction  The pointer to the event's notification function, if any.
/// @param[in]  NotifyContext   The pointer to the notification function's context; corresponds to parameter
///                             Context in the notification function.
/// @param[in]  EventGroup      The pointer to the unique identifier of the group to which this event belongs.
///                             If this is NULL, then the function behaves as if the parameters were passed
///                             to CreateEvent.
/// @param[out] Event           The pointer to the newly created event if the call succeeds; undefined
///                             otherwise.
///
/// @retval EFI_SUCCESS            The event structure was created.
/// @retval EFI_INVALID_PARAMETER  One or more parameters are invalid.
/// @retval EFI_OUT_OF_RESOURCES   The event could not be allocated.
EFI_EVENT
CreateEventEx (
  IN UINT32            Type,
  IN EFI_TPL           NotifyTpl,
  IN EFI_EVENT_NOTIFY  NotifyFunction, OPTIONAL
  IN CONST VOID        *NotifyContext, OPTIONAL
  IN CONST EFI_GUID    *EventGroup OPTIONAL
  )
{
  EFI_EVENT  Event;

  EFI_STATUS Status;

  ASSERT (!EfiAtRuntime ());
  ASSERT ((NotifyFunction != NULL) || ((Type & (EVT_NOTIFY_SIGNAL| EVT_NOTIFY_WAIT)) == 0));

  Status = gBS->CreateEventEx (Type, NotifyTpl, NotifyFunction, NotifyContext, EventGroup, &Event);

  DEBUG_CODE (
    if (Status != EFI_OUT_OF_RESOURCES) {
      ASSERT_EFI_ERROR (Status);
    }
    );

  if (EFI_ERROR (Status)) {
    Event = NULL;
  }

  return Event;
}

// SetTimer
/// Sets the type of timer and the trigger time for a timer event.
///
/// @param[in] Event        The timer event that is to be signaled at the specified time.
/// @param[in] Type         The type of time that is specified in TriggerTime.
/// @param[in] TriggerTime  The number of 100ns units until the timer expires.
///                         A TriggerTime of 0 is legal.
///                         If Type is TimerRelative and TriggerTime is 0, then the timer
///                         event will be signaled on the next timer tick.
///                         If Type is TimerPeriodic and TriggerTime is 0, then the timer
///                         event will be signaled on every timer tick.
///
/// @retval EFI_SUCCESS            The event has been set to be signaled at the requested time.
/// @retval EFI_INVALID_PARAMETER  Event or Type is not valid.
EFI_STATUS
SetTimer (
  IN EFI_EVENT        Event,
  IN EFI_TIMER_DELAY  Type,
  IN UINT64           TriggerTime
  )
{
  EFI_STATUS Status;

  ASSERT (!EfiAtRuntime ());
  ASSERT (Event != NULL);
  ASSERT ((Type >= TimerCancel) && (Type <= TimerRelative));

  Status = gBS->SetTimer (Event, Type, TriggerTime);

  ASSERT_EFI_ERROR (Status);

  return Status;
}

// SignalEvent
/// Signals an event.
///
/// @param[in] Event  The event to signal.
///
/// @retval EFI_SUCCESS  The event has been signaled.
EFI_STATUS
SignalEvent (
  IN EFI_EVENT  Event
  )
{
  EFI_STATUS Status;

  ASSERT (!EfiAtRuntime ());
  ASSERT (Event != NULL);

  Status = gBS->SignalEvent (Event);

  return Status;
}

// WaitForEvent
/// Stops execution until an event is signaled.
///
/// @param[in]  NumberOfEvents  The number of events in the Event array.
/// @param[in]  Event           An array of EFI_EVENT.
/// @param[out] Index           The pointer to the index of the event which satisfied the wait condition.
///
/// @retval EFI_SUCCESS            The event indicated by Index was signaled.
/// @retval EFI_INVALID_PARAMETER  1) NumberOfEvents is 0.
///                                2) The event indicated by Index is of type
///                                EVT_NOTIFY_SIGNAL.
/// @retval EFI_UNSUPPORTED        The current TPL is not TPL_APPLICATION.
EFI_STATUS
WaitForEvent (
  IN     UINTN      NumberOfEvents,
  IN     EFI_EVENT  *Event,
     OUT UINTN      *Index
  )
{
  EFI_STATUS Status;

  ASSERT (!EfiAtRuntime ());
  ASSERT (NumberOfEvents > 0);
  ASSERT (Event != NULL);
  ASSERT (Index != NULL);

  Status = gBS->WaitForEvent (NumberOfEvents, Event, Index);

  ASSERT_EFI_ERROR (Status);

  return Status;
}

// CloseEvent
/// Closes an event.
///
/// @param[in] Event  The event to close.
///
/// @retval EFI_SUCCESS  The event has been closed.
EFI_STATUS
CloseEvent (
  IN EFI_EVENT  Event
  )
{
  EFI_STATUS Status;

  ASSERT (!EfiAtRuntime ());
  ASSERT (Event != NULL);

  Status = gBS->CloseEvent (Event);

  ASSERT_EFI_ERROR (Status);

  return Status;
}

// CheckEvent
/// Checks whether an event is in the signaled state.
///
/// @param[in] Event  The event to check.
///
/// @retval EFI_SUCCESS            The event is in the signaled state.
/// @retval EFI_NOT_READY          The event is not in the signaled state.
/// @retval EFI_INVALID_PARAMETER  Event is of type EVT_NOTIFY_SIGNAL.
EFI_STATUS
CheckEvent (
  IN EFI_EVENT  Event
  )
{
  EFI_STATUS Status;

  ASSERT (!EfiAtRuntime ());
  ASSERT (Event != NULL);

  Status = gBS->CheckEvent (Event);

  DEBUG_CODE (
    if (Status != EFI_NOT_READY) {
      ASSERT_EFI_ERROR (Status);
    }
    );

  return Status;
}

// CreateTimerEvent
/// 
///
/// @param 
///
/// @return 
/// @retval 
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

  Event = NULL;

  if (NotifyTpl < TPL_CALLBACK) {
    Event = CreateEvent (
              ((NotifyFunction != NULL) ? (EVT_TIMER | EVT_NOTIFY_SIGNAL) : EVT_TIMER),
              NotifyTpl,
              NotifyFunction,
              NotifyContext
              );

    if (Event != NULL) {
      Status = SetTimer (Event, (SignalPeriodic ? TimerPeriodic : TimerRelative), TriggerTime);

      if (EFI_ERROR (Status)) {
        CloseEvent (Event);

        Event = NULL;
      }
    }
  }

  return Event;
}

// CreateNotifyEvent
/// 
///
/// @param 
///
/// @return 
/// @retval 
EFI_EVENT
CreateNotifyEvent (
  IN EFI_EVENT_NOTIFY  NotifyFunction,
  IN VOID              *NotifyContext,
  IN UINT64            TriggerTime,
  IN BOOLEAN           SignalPeriodic
  )
{
  return CreateTimerEvent (NotifyFunction, NotifyContext, TriggerTime, SignalPeriodic, TPL_NOTIFY);
}

// CancelTimer
/// 
///
/// @param 
///
/// @return 
/// @retval 
EFI_STATUS
CancelTimer (
  IN EFI_EVENT  Event
  )
{
  return SetTimer (Event, TimerCancel, 0);
}

// CancelEvent
/// 
///
/// @param 
///
/// @return 
/// @retval 
VOID
CancelEvent (
  IN EFI_EVENT  Event
  ) // sub_309
{
  EFI_STATUS Status;

  Status = CancelTimer (Event);

  if (!EFI_ERROR (Status)) {
    CloseEvent (Event);
  }
}

// CreateSignalEvent
/// 
///
/// @param 
///
/// @return 
/// @retval 
EFI_EVENT
CreateSignalEvent (
  IN EFI_EVENT_NOTIFY  NotifyFunction, OPTIONAL
  IN CONST VOID        *NotifyContext, OPTIONAL
  IN CONST EFI_GUID    *EventGroup OPTIONAL
  )
{
  return CreateEventEx (
           EVT_NOTIFY_SIGNAL,
           TPL_NOTIFY,
           NotifyFunction,
           NotifyContext,
           EventGroup
           );
}

// CreateExitBootServicesEvent
/// 
///
/// @param 
///
/// @return 
/// @retval 
EFI_EVENT
CreateExitBootServicesEvent (
  IN EFI_EVENT_NOTIFY  NotifyFunction, OPTIONAL
  IN CONST VOID        *NotifyContext OPTIONAL
  )
{
  return CreateSignalEvent (
           NotifyFunction,
           NotifyContext,
           &gEfiEventExitBootServicesGuid
           );
}

// CreateVirtualAddressChangeEvent
/// 
///
/// @param 
///
/// @return 
/// @retval 
EFI_EVENT
CreateVirtualAddressChangeEvent (
  IN EFI_EVENT_NOTIFY  NotifyFunction, OPTIONAL
  IN CONST VOID        *NotifyContext OPTIONAL
  )
{
  return CreateSignalEvent (
           NotifyFunction,
           NotifyContext,
           &gEfiEventVirtualAddressChangeGuid
           );
}

// CreateMemoryMapChangeEvent
/// 
///
/// @param 
///
/// @return 
/// @retval 
EFI_EVENT
CreateMemoryMapChangeEvent (
  IN EFI_EVENT_NOTIFY  NotifyFunction, OPTIONAL
  IN CONST VOID        *NotifyContext OPTIONAL
  )
{
  return CreateSignalEvent (
           NotifyFunction,
           NotifyContext,
           &gEfiEventMemoryMapChangeGuid
           );
}

// CreateReadyToBootEvent
/// 
///
/// @param 
///
/// @return 
/// @retval 
EFI_EVENT
CreateReadyToBootEvent (
  IN EFI_EVENT_NOTIFY  NotifyFunction, OPTIONAL
  IN CONST VOID        *NotifyContext OPTIONAL
  )
{
  return CreateSignalEvent (
           NotifyFunction,
           NotifyContext,
           &gEfiEventReadyToBootGuid
           );
}

// CreateEventDxeDispatchGuidEvent
/// 
///
/// @param 
///
/// @return 
/// @retval 
EFI_EVENT
CreateDxeDispatchGuidEvent (
  IN EFI_EVENT_NOTIFY  NotifyFunction, OPTIONAL
  IN CONST VOID        *NotifyContext OPTIONAL
  )
{
  return CreateSignalEvent (
           NotifyFunction,
           NotifyContext,
           &gEfiEventDxeDispatchGuid
           );
}

// CreateEndOfDxeEvent
/// 
///
/// @param 
///
/// @return 
/// @retval 
EFI_EVENT
CreateEndOfDxeEvent (
  IN EFI_EVENT_NOTIFY  NotifyFunction, OPTIONAL
  IN CONST VOID        *NotifyContext OPTIONAL
  )
{
  return CreateSignalEvent (
           NotifyFunction,
           NotifyContext,
           &gEfiEndOfDxeEventGroupGuid
           );
}
