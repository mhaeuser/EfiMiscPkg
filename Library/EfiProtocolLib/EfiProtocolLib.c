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

#include <Uefi.h>

#include <MiscBase.h>

#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/EfiProtocolLib.h>

// InstallProtocolInterface
/** Installs a protocol interface on a device handle.  If the handle does not exist, it is created and added
    to the list of handles in the system.  InstallMultipleProtocolInterfaces() performs
    more error checking than InstallProtocolInterface(), so it is recommended that
    InstallMultipleProtocolInterfaces() be used in place of
    InstallProtocolInterface()

  @param[in, out] Handle         A pointer to the EFI_HANDLE on which the interface is to be installed.
  @param[in]      Protocol       The numeric ID of the protocol interface.
  @param[in]      InterfaceType  Indicates whether Interface is supplied in native form.
  @param[in]      Interface      A pointer to the protocol interface.

  @retval EFI_SUCCESS            The protocol interface was installed.
  @retval EFI_OUT_OF_RESOURCES   Space for a new handle could not be allocated.
  @retval EFI_INVALID_PARAMETER  Handle is NULL.
  @retval EFI_INVALID_PARAMETER  Protocol is NULL.
  @retval EFI_INVALID_PARAMETER  InterfaceType is not EFI_NATIVE_INTERFACE.
  @retval EFI_INVALID_PARAMETER  Protocol is already installed on the handle specified by Handle.
**/
EFI_STATUS
InstallProtocolInterface (
  IN OUT EFI_HANDLE          *Handle,
  IN     EFI_GUID            *Protocol,
  IN     EFI_INTERFACE_TYPE  InterfaceType,
  IN     VOID                *Interface
  )
{
  EFI_STATUS Status;

  ASSERT (Handle != NULL);
  ASSERT (Protocol != NULL);
  ASSERT (InterfaceType == EFI_NATIVE_INTERFACE);

  Status = gBS->InstallProtocolInterface (Handle, Protocol, InterfaceType, Interface);

  ASSERT_EFI_ERROR (Status);

  return Status;
}

// ReinstallProtocolInterface
/** Reinstalls a protocol interface on a device handle.

  @param[in] Handle        Handle on which the interface is to be reinstalled.
  @param[in] Protocol      The numeric ID of the interface.
  @param[in] OldInterface  A pointer to the old interface.  NULL can be used if a structure is not
                           associated with Protocol.
  @param[in] NewInterface  A pointer to the new interface.

  @retval EFI_SUCCESS            The protocol interface was reinstalled.
  @retval EFI_NOT_FOUND          The OldInterface on the handle was not found.
  @retval EFI_ACCESS_DENIED      The protocol interface could not be reinstalled,
                                 because OldInterface is still being used by a
                                 driver that will not release it.
  @retval EFI_INVALID_PARAMETER  Handle is NULL.
  @retval EFI_INVALID_PARAMETER  Protocol is NULL.
**/
EFI_STATUS
ReinstallProtocolInterface (
  IN EFI_HANDLE  Handle,
  IN EFI_GUID    *Protocol,
  IN VOID        *OldInterface,
  IN VOID        *NewInterface
  )
{
  EFI_STATUS Status;

  ASSERT (Handle != NULL);
  ASSERT (Protocol != NULL);

  Status = gBS->ReinstallProtocolInterface (Handle, Protocol, OldInterface, NewInterface);

  DEBUG_CODE (
    if (Status != EFI_ACCESS_DENIED) {
      ASSERT_EFI_ERROR (Status);
    }
    );

  return Status;
}

// UninstallProtocolInterface
/** Removes a protocol interface from a device handle.  It is recommended that
    UninstallMultipleProtocolInterfaces() be used in place of
    UninstallProtocolInterface().

  @param[in] Handle     The handle on which the interface was installed.
  @param[in] Protocol   The numeric ID of the interface.
  @param[in] Interface  A pointer to the interface.

  @retval EFI_SUCCESS            The interface was removed.
  @retval EFI_NOT_FOUND          The interface was not found.
  @retval EFI_ACCESS_DENIED      The interface was not removed because the interface
                                 is still being used by a driver.
  @retval EFI_INVALID_PARAMETER  Handle is NULL.
  @retval EFI_INVALID_PARAMETER  Protocol is NULL.
**/
EFI_STATUS
UninstallProtocolInterface (
  IN EFI_HANDLE  Handle,
  IN EFI_GUID    *Protocol,
  IN VOID        *Interface
  )
{
  EFI_STATUS Status;

  ASSERT (Handle != NULL);
  ASSERT (Protocol != NULL);

  Status = gBS->UninstallProtocolInterface (Handle, Protocol, Interface);

  DEBUG_CODE (
    if (Status != EFI_ACCESS_DENIED) {
      ASSERT_EFI_ERROR (Status);
    }
  );

  return Status;
}

// HandleProtocol
/** Queries a handle to determine if it supports a specified protocol.

  @param[in]  Handle     The handle being queried.
  @param[in]  Protocol   The published unique identifier of the protocol.
  @param[out] Interface  Supplies the address where a pointer to the corresponding Protocol
                         Interface is returned.

  @retval EFI_SUCCESS            The interface information for the specified protocol was returned.
  @retval EFI_UNSUPPORTED        The device does not support the specified protocol.
  @retval EFI_INVALID_PARAMETER  Handle is NULL.
  @retval EFI_INVALID_PARAMETER  Protocol is NULL.
  @retval EFI_INVALID_PARAMETER  Interface is NULL.
**/
EFI_STATUS
HandleProtocol (
  IN  EFI_HANDLE  Handle,
  IN  EFI_GUID    *Protocol,
  OUT VOID        **Interface
  )
{
  EFI_STATUS Status;

  ASSERT (Handle != NULL);
  ASSERT (Protocol != NULL);
  ASSERT (Interface != NULL);

  Status = gBS->HandleProtocol (Handle, Protocol, Interface);

  DEBUG_CODE (
    if (Status != EFI_UNSUPPORTED) {
      ASSERT_EFI_ERROR (Status);
    }
  );

  return Status;
}

// OpenProtocol
/** Queries a handle to determine if it supports a specified protocol.  If the protocol is supported by the
    handle, it opens the protocol on behalf of the calling agent.

  @param[in]  Handle            The handle for the protocol interface that is being opened.
  @param[in]  Protocol          The published unique identifier of the protocol.
  @param[out] Interface         Supplies the address where a pointer to the corresponding Protocol
                                Interface is returned.
  @param[in]  AgentHandle       The handle of the agent that is opening the protocol interface
                                specified by Protocol and Interface.
  @param[in]  ControllerHandle  If the agent that is opening a protocol is a driver that follows the
                                UEFI Driver Model, then this parameter is the controller handle
                                that requires the protocol interface.  If the agent does not follow
                                the UEFI Driver Model, then this parameter is optional and may
                                be NULL.
  @param[in]  Attributes        The open mode of the protocol interface specified by Handle
                                and Protocol.

  @retval EFI_SUCCESS            An item was added to the open list for the protocol interface, and the
                                 protocol interface was returned in Interface.
  @retval EFI_UNSUPPORTED        Handle does not support Protocol.
  @retval EFI_INVALID_PARAMETER  One or more parameters are invalid.
  @retval EFI_ACCESS_DENIED      Required attributes can't be supported in current environment.
  @retval EFI_ALREADY_STARTED    Item on the open list already has requierd attributes whose agent
                                 handle is the same as AgentHandle.
**/
EFI_STATUS
OpenProtocol (
  IN  EFI_HANDLE  Handle,
  IN  EFI_GUID    *Protocol,
  OUT VOID        **Interface, OPTIONAL
  IN  EFI_HANDLE  AgentHandle,
  IN  EFI_HANDLE  ControllerHandle,
  IN  UINT32      Attributes
  )
{
  EFI_STATUS Status;

  /*
  If Attributes is not a legal value, then EFI_INVALID_PARAMETER is returned
  If Attributes is BY_CHILD_CONTROLLER, BY_DRIVER, EXCLUSIVE, or BY_DRIVER|EXCULSIVE, and AgentHandle is not a valid EFI_HANDLE, then EFI_INVALID_PARAMETER is returned.
  If Attributes is BY_CHILD_CONTROLLER, BY_DRIVER, or BY_DRIVER|EXCULSIVE, and ControllerHandle is not a valid EFI_HANDLE, then EFI_INVALID_PARAMETER is returned.
  If Attributes is BY_CHILD_CONTROLLER and Handle is identical to ControllerHandle, then EFI_INVALID_PARAMETER is returned.
  If Attributes is BY_DRIVER , BY_DRIVER|EXCLUSIVE, or EXCLUSIVE, and there are any items on the open list of the protocol interface with an attribute of EXCLUSIVE or BY_DRIVER|EXCLUSIVE, then EFI_ACCESS_DENIED is returned.
  If Attributes is BY_DRIVER, and there are any items on the open list of the protocol interface with an attribute of BY_DRIVER, and AgentHandle is the same agent handle in the open list item, then EFI_ALREADY_STARTED is returned.
  If Attributes is BY_DRIVER, and there are any items on the open list of the protocol interface with an attribute of BY_DRIVER, and AgentHandle is different than the agent handle in the open list item, then EFI_ACCESS_DENIED is returned.
  If Attributes is BY_DRIVER|EXCLUSIVE, and there are any items on the open list of the protocol interface with an attribute of BY_DRIVER|EXCLUSIVE, and AgentHandle is the same agent handle in the open list item, then EFI_ALREADY_STARTED is returned.
  If Attributes is BY_DRIVER|EXCLUSIVE, and there are any items on the open list of the protocol interface with an attribute of BY_DRIVER|EXCLUSIVE, and AgentHandle is different than the agent handle in the open list item, then EFI_ACCESS_DENIED is returned.
  If Attributes is BY_DRIVER|EXCLUSIVE or EXCLUSIVE, and there is an item on the open list of the protocol interface with an attribute of BY_DRIVER, then the boot service DisconnectController() is called for the driver on the open list. If there is an item in the open list of the protocol interface with an attribute of BY_DRIVER remaining after the DisconnectController() call has been made, EFI_ACCESS_DENIED is returned.
  */

  ASSERT (Handle != NULL);
  ASSERT (Protocol != NULL);
  ASSERT ((Attributes != 0) && ((Attributes & ~0x3F) == 0));
  ASSERT (((((Attributes & EFI_OPEN_PROTOCOL_TEST_PROTOCOL) == 0) ? 1 : 0) ^ ((Interface != NULL) ? 1 : 0)) != 0);
  ASSERT (!((Attributes == EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER) || (Attributes == EFI_OPEN_PROTOCOL_BY_DRIVER) || (Attributes == EFI_OPEN_PROTOCOL_EXCLUSIVE) || (Attributes == (EFI_OPEN_PROTOCOL_BY_DRIVER | EFI_OPEN_PROTOCOL_EXCLUSIVE))) || (AgentHandle != NULL));
  ASSERT (!((Attributes == EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER) || (Attributes == EFI_OPEN_PROTOCOL_BY_DRIVER) || (Attributes == (EFI_OPEN_PROTOCOL_BY_DRIVER | EFI_OPEN_PROTOCOL_EXCLUSIVE))) || (ControllerHandle != NULL));
  ASSERT ((Attributes != EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER) || (ControllerHandle != Handle));

  Status = gBS->OpenProtocol (Handle, Protocol, Interface, AgentHandle, ControllerHandle, Attributes);

  DEBUG_CODE (
    if (Status != EFI_UNSUPPORTED) {
      ASSERT_EFI_ERROR (Status);
    }
  );

  return Status;
}

// CloseProtocol
/** Closes a protocol on a handle that was opened using OpenProtocol().

  @param[in] Handle            The handle for the protocol interface that was previously opened
                               with OpenProtocol(), and is now being closed.
  @param[in] Protocol          The published unique identifier of the protocol.
  @param[in] AgentHandle       The handle of the agent that is closing the protocol interface.
  @param[in] ControllerHandle  If the agent that opened a protocol is a driver that follows the
                               UEFI Driver Model, then this parameter is the controller handle
                               that required the protocol interface.

  @retval EFI_SUCCESS            The protocol instance was closed.
  @retval EFI_INVALID_PARAMETER  1) Handle is NULL.
                                 2) AgentHandle is NULL.
                                 3) ControllerHandle is not NULL and ControllerHandle is not a valid EFI_HANDLE.
                                 4) Protocol is NULL.
  @retval EFI_NOT_FOUND          1) Handle does not support the protocol specified by Protocol.
                                 2) The protocol interface specified by Handle and Protocol is not
                                 currently open by AgentHandle and ControllerHandle.
**/
EFI_STATUS
CloseProtocol (
  IN EFI_HANDLE  Handle,
  IN EFI_GUID    *Protocol,
  IN EFI_HANDLE  AgentHandle,
  IN EFI_HANDLE  ControllerHandle
  )
{
  EFI_STATUS Status;

  ASSERT (Handle != NULL);
  ASSERT (Protocol != NULL);
  ASSERT (AgentHandle != NULL);

  Status = gBS->CloseProtocol (Handle, Protocol, AgentHandle, ControllerHandle);

  ASSERT_EFI_ERROR (Status);

  return Status;
}

// OpenProtocolInformation
/** Retrieves the list of agents that currently have a protocol interface opened.

  @param[in]  Handle       The handle for the protocol interface that is being queried.
  @param[in]  Protocol     The published unique identifier of the protocol.
  @param[out] EntryBuffer  A pointer to a Buffer of open protocol information in the form of
                           EFI_OPEN_PROTOCOL_INFORMATION_ENTRY structures.
  @param[out] EntryCount   A pointer to the number of entries in EntryBuffer.

  @retval EFI_SUCCESS           The open protocol information was returned in EntryBuffer, and the
                                number of entries was returned EntryCount.
  @retval EFI_OUT_OF_RESOURCES  There are not enough resources available to allocate EntryBuffer.
  @retval EFI_NOT_FOUND         Handle does not support the protocol specified by Protocol.
**/
EFI_STATUS
OpenProtocolInformation (
  IN  EFI_HANDLE                           Handle,
  IN  EFI_GUID                             *Protocol,
  OUT EFI_OPEN_PROTOCOL_INFORMATION_ENTRY  **EntryBuffer,
  OUT UINTN                                *EntryCount
  )
{
  EFI_STATUS Status;

  ASSERT (Handle != NULL);
  ASSERT (Protocol != NULL);
  ASSERT (EntryBuffer != NULL);
  ASSERT (EntryCount != NULL);
  ASSERT (*EntryCount > 0);

  Status = gBS->OpenProtocolInformation (Handle, Protocol, EntryBuffer, EntryCount);

  DEBUG_CODE (
    if (Status != EFI_NOT_FOUND) {
      ASSERT_EFI_ERROR (Status);
    }
    );

  return Status;
}

// ProtocolsPerHandle
/** Retrieves the list of protocol interface GUIDs that are installed on a handle in a Buffer allocated
    from pool.

  @param[in]  Handle               The handle from which to retrieve the list of protocol interface
                                   GUIDs.
  @param[out] ProtocolBuffer       A pointer to the list of protocol interface GUID pointers that are
                                   installed on Handle.
  @param[out] ProtocolBufferCount  A pointer to the number of GUID pointers present in
                                   ProtocolBuffer.

  @retval EFI_SUCCESS            The list of protocol interface GUIDs installed on Handle was returned in
                                 ProtocolBuffer.  The number of protocol interface GUIDs was
                                 returned in ProtocolBufferCount.
  @retval EFI_OUT_OF_RESOURCES   There is not enough pool memory to store the results.
  @retval EFI_INVALID_PARAMETER  Handle is NULL.
  @retval EFI_INVALID_PARAMETER  Handle is not a valid EFI_HANDLE.
  @retval EFI_INVALID_PARAMETER  ProtocolBuffer is NULL.
  @retval EFI_INVALID_PARAMETER  ProtocolBufferCount is NULL.
**/
EFI_STATUS
ProtocolsPerHandle (
  IN  EFI_HANDLE  Handle,
  OUT EFI_GUID    ***ProtocolBuffer,
  OUT UINTN       *ProtocolBufferCount
  )
{
  EFI_STATUS Status;

  ASSERT (Handle != NULL);
  ASSERT (ProtocolBuffer != NULL);
  ASSERT (ProtocolBufferCount != NULL);

  Status = gBS->ProtocolsPerHandle (Handle, ProtocolBuffer, ProtocolBufferCount);

  ASSERT_EFI_ERROR (Status);

  return Status;
}

// RegisterProtocolNotify
/** Creates an event that is to be signaled whenever an interface is installed for a specified protocol.

  @param[in]  Protocol      The numeric ID of the protocol for which the event is to be registered.
  @param[in]  Event         Event that is to be signaled whenever a protocol interface is registered
                            for Protocol.
  @param[out] Registration  A pointer to a memory location to receive the registration value.

  @retval EFI_SUCCESS            The notification event has been registered.
  @retval EFI_OUT_OF_RESOURCES   Space for the notification event could not be allocated.
  @retval EFI_INVALID_PARAMETER  Protocol is NULL.
  @retval EFI_INVALID_PARAMETER  Event is NULL.
  @retval EFI_INVALID_PARAMETER  Registration is NULL.
**/
EFI_STATUS
RegisterProtocolNotify (
  IN  EFI_GUID   *Protocol,
  IN  EFI_EVENT  Event,
  OUT VOID       **Registration
  )
{
  EFI_STATUS Status;

  ASSERT (Protocol != NULL);
  ASSERT (Event != NULL);
  ASSERT (Registration != NULL);

  Status = gBS->RegisterProtocolNotify (Protocol, Event, Registration);

  ASSERT_EFI_ERROR (Status);

  return Status;
}

// LocateHandle
/** Returns an array of handles that support a specified protocol.

  @param[in]      SearchType  Specifies which handle(s) are to be returned.
  @param[in]      Protocol    Specifies the protocol to search by.
  @param[in]      SearchKey   Specifies the search key.
  @param[in, out] BufferSize  On input, the size in bytes of Buffer.  On output, the size in bytes of
                              the array returned in Buffer (if the Buffer was large enough) or the
                              size, in bytes, of the Buffer needed to obtain the array (if the Buffer was
                              not large enough).
  @param[out]     Buffer      The Buffer in which the array is returned.

  @retval EFI_SUCCESS            The array of handles was returned.
  @retval EFI_NOT_FOUND          No handles match the search.
  @retval EFI_BUFFER_TOO_SMALL   The BufferSize is too small for the result.
  @retval EFI_INVALID_PARAMETER  SearchType is not a member of EFI_LOCATE_SEARCH_TYPE.
  @retval EFI_INVALID_PARAMETER  SearchType is ByRegisterNotify and SearchKey is NULL.
  @retval EFI_INVALID_PARAMETER  SearchType is ByProtocol and Protocol is NULL.
  @retval EFI_INVALID_PARAMETER  One or more matches are found and BufferSize is NULL.
  @retval EFI_INVALID_PARAMETER  BufferSize is large enough for the result and Buffer is NULL.
**/
EFI_STATUS
LocateHandle (
  IN     EFI_LOCATE_SEARCH_TYPE  SearchType,
  IN     EFI_GUID                *Protocol, OPTIONAL
  IN     VOID                    *SearchKey, OPTIONAL
  IN OUT UINTN                   *BufferSize,
  OUT    EFI_HANDLE              *Buffer
  )
{
  EFI_STATUS Status;

  ASSERT ((SearchType >= AllHandles) && (SearchType <= ByProtocol));
  ASSERT ((SearchType != ByRegisterNotify) || (SearchKey != NULL));
  ASSERT ((SearchType != ByProtocol) || (Protocol != NULL));

  Status = gBS->LocateHandle (SearchType, Protocol, SearchKey, BufferSize, Buffer);

  DEBUG_CODE (
    if ((Status != EFI_NOT_FOUND) && (Status != EFI_BUFFER_TOO_SMALL)) {
      ASSERT_EFI_ERROR (Status);
    }
    );

  return Status;
}

// LocateDevicePath
/** Locates the handle to a device on the device path that supports the specified protocol.

  @param[in]      Protocol    Specifies the protocol to search for.
  @param[in, out] DevicePath  On input, a pointer to a pointer to the device path.  On output, the device
                              path pointer is modified to point to the remaining part of the device path.
  @param[out]     Device      A pointer to the returned device handle.

  @retval EFI_SUCCESS            The resulting handle was returned.
  @retval EFI_NOT_FOUND          No handles match the search.
  @retval EFI_INVALID_PARAMETER  Protocol is NULL.
  @retval EFI_INVALID_PARAMETER  DevicePath is NULL.
  @retval EFI_INVALID_PARAMETER  A handle matched the search and Device is NULL.
**/
EFI_STATUS
LocateDevicePath (
  IN     EFI_GUID                  *Protocol,
  IN OUT EFI_DEVICE_PATH_PROTOCOL  **DevicePath,
  OUT    EFI_HANDLE                *Device
  )
{
  EFI_STATUS Status;

  ASSERT (Protocol != NULL);
  ASSERT (DevicePath != NULL);
  ASSERT (Device != NULL);

  Status = gBS->LocateDevicePath (Protocol, DevicePath, Device);

  DEBUG_CODE (
    if (Status != EFI_NOT_FOUND) {
      ASSERT_EFI_ERROR (Status);
    }
    );

  return Status;
}

// InstallConfigurationTable
/** Adds, updates, or removes a configuration table entry from the EFI System Table.

  @param[in] Guid   A pointer to the GUID for the entry to add, update, or remove.
  @param[in] Table  A pointer to the configuration table for the entry to add, update, or
                    remove.  May be NULL.

  @retval EFI_SUCCESS            The (Guid, Table) pair was added, updated, or removed.
  @retval EFI_NOT_FOUND          An attempt was made to delete a nonexistent entry.
  @retval EFI_INVALID_PARAMETER  Guid is NULL.
  @retval EFI_OUT_OF_RESOURCES   There is not enough memory available to complete the operation.
**/
EFI_STATUS
InstallConfigurationTable (
  IN EFI_GUID  *Guid,
  IN VOID      *Table
  )
{
  EFI_STATUS Status;

  ASSERT (Guid != NULL);

  Status = gBS->InstallConfigurationTable (Guid, Table);

  DEBUG_CODE (
    if (Status != EFI_NOT_FOUND) {
      ASSERT_EFI_ERROR (Status);
    }
    );

  return Status;
}

// LocateHandleBuffer
/** Returns an array of handles that support the requested protocol in a Buffer allocated from pool.

  @param[in]      SearchType  Specifies which handle(s) are to be returned.
  @param[in]      Protocol    Provides the protocol to search by.
                              This parameter is only valid for a SearchType of ByProtocol.
  @param[in]      SearchKey   Supplies the search key depending on the SearchType.
  @param[in, out] NoHandles   The number of handles returned in Buffer.
  @param[out]     Buffer      A pointer to the Buffer to return the requested array of handles that
                              support Protocol.

  @retval EFI_SUCCESS            The array of handles was returned in Buffer, and the number of
                                 handles in Buffer was returned in NoHandles.
  @retval EFI_NOT_FOUND          No handles match the search.
  @retval EFI_OUT_OF_RESOURCES   There is not enough pool memory to store the matching results.
  @retval EFI_INVALID_PARAMETER  NoHandles is NULL.
  @retval EFI_INVALID_PARAMETER  Buffer is NULL.
**/
EFI_STATUS
LocateHandleBuffer (
  IN     EFI_LOCATE_SEARCH_TYPE  SearchType,
  IN     EFI_GUID                *Protocol, OPTIONAL
  IN     VOID                    *SearchKey, OPTIONAL
  IN OUT UINTN                   *NoHandles,
  OUT    EFI_HANDLE              **Buffer
  )
{
  EFI_STATUS Status;

  ASSERT ((((SearchType == ByProtocol) ? 1 : 0) ^ ((Protocol == NULL) ? 1 : 0)) != 0);
  ASSERT (NoHandles != NULL);
  ASSERT (Buffer != NULL);

  Status = gBS->LocateHandleBuffer (SearchType, Protocol, SearchKey, NoHandles, Buffer);

  DEBUG_CODE (
    if (Status != EFI_NOT_FOUND) {
      ASSERT_EFI_ERROR (Status);
    }
    );

  return Status;
}

// LocateProtocol
/** Returns the first protocol instance that matches the given protocol.

  @param[in]  Protocol      Provides the protocol to search for.
  @param[in]  Registration  Optional registration key returned from
                            RegisterProtocolNotify().
  @param[out] Interface     On return, a pointer to the first interface that matches Protocol and
                            Registration.

  @retval EFI_SUCCESS            A protocol instance matching Protocol was found and returned in
                                 Interface.
  @retval EFI_NOT_FOUND          No protocol instances were found that match Protocol and
                                 Registration.
  @retval EFI_INVALID_PARAMETER  Interface is NULL.
**/
EFI_STATUS
LocateProtocol (
  IN  EFI_GUID  *Protocol,
  IN  VOID      *Registration, OPTIONAL
  OUT VOID      **Interface
  )
{
  EFI_STATUS Status;

  ASSERT (Protocol != NULL);
  ASSERT (Interface != NULL);

  Status = gBS->LocateProtocol (Protocol, Registration, Interface);

  DEBUG_CODE (
    if (Status != EFI_NOT_FOUND) {
      ASSERT_EFI_ERROR (Status);
    }
    );

  return Status;
}

// ConnectController
/** Connects one or more drivers to a controller.

  @param[in] ControllerHandle     The handle of the controller to which driver(s) are to be connected.
  @param[in] DriverImageHandle    A pointer to an ordered list handles that support the
                                  EFI_DRIVER_BINDING_PROTOCOL.
  @param[in] RemainingDevicePath  A pointer to the device path that specifies a child of the
                                  controller specified by ControllerHandle.
  @param[in] Recursive            If TRUE, then ConnectController() is called recursively
                                  until the entire tree of controllers below the controller specified
                                  by ControllerHandle have been created.  If FALSE, then
                                  the tree of controllers is only expanded one level.

  @retval EFI_SUCCESS             1) One or more drivers were connected to ControllerHandle.
                                  2) No drivers were connected to ControllerHandle, but
                                  RemainingDevicePath is not NULL, and it is an End Device
                                  Path Node.
  @retval EFI_INVALID_PARAMETER   ControllerHandle is NULL.
  @retval EFI_NOT_FOUND           1) There are no EFI_DRIVER_BINDING_PROTOCOL instances
                                  present in the system.
                                  2) No drivers were connected to ControllerHandle.
  @retval EFI_SECURITY_VIOLATION  The user has no permission to start UEFI device drivers
                                  on the device path associated with the ControllerHandle or
                                  specified by the RemainingDevicePath.
**/
EFI_STATUS
ConnectController (
  IN EFI_HANDLE                ControllerHandle,
  IN EFI_HANDLE                *DriverImageHandle, OPTIONAL
  IN EFI_DEVICE_PATH_PROTOCOL  *RemainingDevicePath, OPTIONAL
  IN BOOLEAN                   Recursive
  )
{
  EFI_STATUS Status;

  ASSERT (ControllerHandle != NULL);

  Status = gBS->ConnectController (ControllerHandle, DriverImageHandle, RemainingDevicePath, Recursive);

  DEBUG_CODE (
    if (Status != EFI_NOT_FOUND) {
      ASSERT_EFI_ERROR (Status);
    }
  );

  return Status;
}

// DisconnectController
/** Disconnects one or more drivers from a controller.

  @param[in] ControllerHandle   The handle of the controller from which driver(s) are to be disconnected.
  @param[in] DriverImageHandle  The driver to disconnect from ControllerHandle.
                                If DriverImageHandle is NULL, then all the drivers currently managing
                                ControllerHandle are disconnected from ControllerHandle.
  @param[in] ChildHandle        The handle of the child to destroy.
                                If ChildHandle is NULL, then all the children of ControllerHandle are
                                destroyed before the drivers are disconnected from ControllerHandle.

  @retval EFI_SUCCESS            1) One or more drivers were disconnected from the controller.
                                 2) On entry, no drivers are managing ControllerHandle.
                                 3) DriverImageHandle is not NULL, and on entry
                                 DriverImageHandle is not managing ControllerHandle.
  @retval EFI_INVALID_PARAMETER  1) ControllerHandle is NULL.
                                 2) DriverImageHandle is not NULL, and it is not a valid EFI_HANDLE.
                                 3) ChildHandle is not NULL, and it is not a valid EFI_HANDLE.
                                 4) DriverImageHandle does not support the EFI_DRIVER_BINDING_PROTOCOL.
  @retval EFI_OUT_OF_RESOURCES   There are not enough resources available to disconnect any drivers from
                                 ControllerHandle.
  @retval EFI_DEVICE_ERROR       The controller could not be disconnected because of a device error.
**/
EFI_STATUS
DisconnectController (
  IN EFI_HANDLE  ControllerHandle,
  IN EFI_HANDLE  DriverImageHandle, OPTIONAL
  IN EFI_HANDLE  ChildHandle OPTIONAL
  )
{
  EFI_STATUS Status;

  ASSERT (ControllerHandle != NULL);

  Status = gBS->DisconnectController (ControllerHandle, DriverImageHandle, ChildHandle);

  DEBUG_CODE (
    if ((Status != EFI_OUT_OF_RESOURCES) && (Status != EFI_DEVICE_ERROR)) {
      ASSERT_EFI_ERROR (Status);
    }
    );

  return Status;
}

// SafeInstallProtocolInterface
EFI_STATUS
SafeInstallProtocolInterface (
  IN OUT EFI_HANDLE          *Handle,
  IN     EFI_GUID            *Protocol,
  IN     EFI_INTERFACE_TYPE  InterfaceType,
  IN     VOID                *Interface
  )
{
  EFI_STATUS Status;

  UINTN      NoHandles;
  EFI_HANDLE *Buffer;

  ASSERT (Handle != NULL);
  ASSERT (Protocol != NULL);
  ASSERT (InterfaceType == EFI_NATIVE_INTERFACE);

  DEBUG_CODE (
    Buffer = NULL;
    );

  Status = LocateHandleBuffer (ByProtocol, Protocol, NULL, &NoHandles, &Buffer);

  if (!EFI_ERROR (Status)) {
    FreePool ((VOID *)Buffer);
  }

  return ((Status == EFI_NOT_FOUND)
           ? InstallProtocolInterface (Handle, Protocol, InterfaceType, Interface)
           : EFI_ALREADY_STARTED);
}

// InstallVersionedProtocol
EFI_STATUS
InstallVersionedProtocolInterface (
  IN OUT EFI_HANDLE          *Handle,
  IN     EFI_GUID            *Protocol,
  IN     EFI_INTERFACE_TYPE  InterfaceType,
  IN     VOID                *Interface
  )
{
  EFI_STATUS Status;

  VOID       *OldInterface;
  UINTN      NoHandles;
  EFI_HANDLE *Buffer;

  ASSERT (Handle != NULL);
  ASSERT (Protocol != NULL);
  ASSERT (InterfaceType == EFI_NATIVE_INTERFACE);
  ASSERT (Interface != NULL);

  if (*Handle != NULL) {
    Status = HandleProtocol (*Handle, Protocol, &OldInterface);

    if (!EFI_ERROR (Status)) {
      Status = EFI_ALREADY_STARTED;

      if (*(UINTN *)OldInterface < *(UINTN *)Interface) {
        Status = ReinstallProtocolInterface (*Handle, Protocol, OldInterface, Interface);
      }
    } else if (Status == EFI_UNSUPPORTED) {
      goto InstallProtocol;
    }
  } else {
    Status = LocateHandleBuffer (ByProtocol, Protocol, NULL, &NoHandles, &Buffer);

    if (!EFI_ERROR (Status)) {
      Status = EFI_ALREADY_STARTED;

      if (NoHandles == 1) {
        Status = HandleProtocol (Buffer[0], Protocol, &OldInterface);

        if (!EFI_ERROR (Status)) {
          Status = EFI_ALREADY_STARTED;

          if (*(UINTN *)OldInterface < *(UINTN *)Interface) {
            Status = UninstallProtocolInterface (Buffer[0], Protocol, OldInterface);

            if (!EFI_ERROR (Status)) {
              goto InstallProtocol;
            }
          }
        }
      }

      FreePool ((VOID *)Buffer);
    } else if (Status == EFI_NOT_FOUND) {
    InstallProtocol:
      Status = InstallProtocolInterface (Handle, Protocol, InterfaceType, Interface);
    }
  }

  DEBUG_CODE (
    if (Status != EFI_ALREADY_STARTED) {
      ASSERT_EFI_ERROR (Status);
    }
    );

  return Status;
}
