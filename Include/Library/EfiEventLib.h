/** @file
  Copyright (C) 2015 CupertinoNet.  All rights reserved.<BR>

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

#ifndef EFI_EVENT_LIB_H_
#define EFI_EVENT_LIB_H_

#ifndef TPL_APPLICATION
  #define TPL_APPLICATION EFI_TPL_APPLICATION
#endif

#ifndef TPL_CALLBACK
  #define TPL_CALLBACK EFI_TPL_CALLBACK
#endif

#ifndef TPL_HIGH_LEVEL
  #define TPL_HIGH_LEVEL EFI_TPL_HIGH_LEVEL
#endif

#ifndef TPL_NOTIFY
  #define TPL_NOTIFY EFI_TPL_NOTIFY
#endif

#ifndef EFI_TIMER_PERIOD_MILLISECONDS

// EFI_TIMER_PERIOD_MILLISECONDS
/** Macro that returns the number of 100 ns units for a specified number of milliseconds.
  This is useful for managing EFI timer events.

  @param Milliseconds The number of milliseconds.

  @return  The number of 100 ns units equivalent to the number of milliseconds specified
  by Milliseconds.
**/
#define EFI_TIMER_PERIOD_MILLISECONDS(Milliseconds) MultU64x32 ((UINT64)(Milliseconds), 10000)

#endif // EFI_TIMER_PERIOD_MILLISECONDS

// CreateEvent
/** Creates an event.

  @param[in] Type            The type of event to create and its mode and attributes.
  @param[in] NotifyTpl       The task priority level of event notifications, if needed.
  @param[in] NotifyFunction  The pointer to the event's notification function, if any.
  @param[in] NotifyContext   The pointer to the notification function's context; corresponds to parameter
                             Context in the notification function.

  @retval EFI_SUCCESS            The event structure was created.
  @retval EFI_INVALID_PARAMETER  One or more parameters are invalid.
  @retval EFI_OUT_OF_RESOURCES   The event could not be allocated.
**/
EFI_EVENT
CreateEvent (
  IN UINT32            Type,
  IN EFI_TPL           NotifyTpl,
  IN EFI_EVENT_NOTIFY  NotifyFunction,
  IN VOID              *NotifyContext
  );

// CreateEventEx
/** Creates an event in a group.

  @param[in]  Type            The type of event to create and its mode and attributes.
  @param[in]  NotifyTpl       The task priority level of event notifications,if needed.
  @param[in]  NotifyFunction  The pointer to the event's notification function, if any.
  @param[in]  NotifyContext   The pointer to the notification function's context; corresponds to parameter
                              Context in the notification function.
  @param[in]  EventGroup      The pointer to the unique identifier of the group to which this event belongs.
                              If this is NULL, then the function behaves as if the parameters were passed
                              to CreateEvent.
  @param[out] Event           The pointer to the newly created event if the call succeeds; undefined
                              otherwise.

  @retval EFI_SUCCESS            The event structure was created.
  @retval EFI_INVALID_PARAMETER  One or more parameters are invalid.
  @retval EFI_OUT_OF_RESOURCES   The event could not be allocated.
**/
EFI_EVENT
CreateEventEx (
  IN UINT32            Type,
  IN EFI_TPL           NotifyTpl,
  IN EFI_EVENT_NOTIFY  NotifyFunction, OPTIONAL
  IN CONST VOID        *NotifyContext, OPTIONAL
  IN CONST EFI_GUID    *EventGroup OPTIONAL
  );

// SetTimer
/** Sets the type of timer and the trigger time for a timer event.

  @param[in] Event        The timer event that is to be signaled at the specified time.
  @param[in] Type         The type of time that is specified in TriggerTime.
  @param[in] TriggerTime  The number of 100ns units until the timer expires.
                          A TriggerTime of 0 is legal.
                          If Type is TimerRelative and TriggerTime is 0, then the timer
                          event will be signaled on the next timer tick.
                          If Type is TimerPeriodic and TriggerTime is 0, then the timer
                          event will be signaled on every timer tick.

  @retval EFI_SUCCESS            The event has been set to be signaled at the requested time.
  @retval EFI_INVALID_PARAMETER  Event or Type is not valid.
**/
EFI_STATUS
SetTimer (
  IN EFI_EVENT        Event,
  IN EFI_TIMER_DELAY  Type,
  IN UINT64           TriggerTime
  );

// SignalEvent
/** Signals an event.

  @param[in] Event  The event to signal.

  @retval EFI_SUCCESS The event has been signaled.
**/
EFI_STATUS
SignalEvent (
  IN EFI_EVENT  Event
  );

// WaitForEvent
/** Stops execution until an event is signaled.

  @param[in]  NumberOfEvents  The number of events in the Event array.
  @param[in]  Event           An array of EFI_EVENT.
  @param[out] Index           The pointer to the index of the event which satisfied the wait condition.

  @retval EFI_SUCCESS            The event indicated by Index was signaled.
  @retval EFI_INVALID_PARAMETER  1) NumberOfEvents is 0.
                                 2) The event indicated by Index is of type
                                 EVT_NOTIFY_SIGNAL.
  @retval EFI_UNSUPPORTED        The current TPL is not TPL_APPLICATION.
**/
EFI_STATUS
WaitForEvent (
  IN  UINTN      NumberOfEvents,
  IN  EFI_EVENT  *Event,
  OUT UINTN      *Index
  );

// CloseEvent
/** Closes an event.

  @param[in] Event  The event to close.

  @retval EFI_SUCCESS  The event has been closed.
**/
EFI_STATUS
CloseEvent (
  IN EFI_EVENT  Event
  );

// CheckEvent
/** Checks whether an event is in the signaled state.

  @param[in] Event  The event to check.

  @retval EFI_SUCCESS            The event is in the signaled state.
  @retval EFI_NOT_READY          The event is not in the signaled state.
  @retval EFI_INVALID_PARAMETER  Event is of type EVT_NOTIFY_SIGNAL.
**/
EFI_STATUS
CheckEvent (
  IN EFI_EVENT  Event
  );

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

#endif // EFI_EVENT_LIB_H_
