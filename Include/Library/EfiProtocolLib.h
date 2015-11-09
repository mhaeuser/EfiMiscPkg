// 13/09/2015

#ifndef _EFI_PROTOCOL_LIB_H_
#define _EFI_PROTOCOL_LIB_H_

// InstallMultipleProtocolInterfaces
/// Installs one or more protocol interfaces into the boot services environment.
///
/// @param[in, out] Handle The pointer to a handle to install the new protocol interfaces on,
///                        or a pointer to NULL if a new handle is to be allocated.
/// @param  ...            A variable argument list containing pairs of protocol GUIDs and protocol
///                        interfaces.
///
/// @retval EFI_SUCCESS           All the protocol interface was installed.
/// @retval EFI_OUT_OF_RESOURCES  There was not enough memory in pool to install all the protocols.
/// @retval EFI_ALREADY_STARTED   A Device Path Protocol instance was passed in that is already present in
///                               the handle database.
/// @retval EFI_INVALID_PARAMETER Handle is NULL.
/// @retval EFI_INVALID_PARAMETER Protocol is already installed on the handle specified by Handle.
#define InstallMultipleProtocolInterfaces(                                \
  Handle,                                                                 \
  ...                                                                     \
  )                                                                       \
{                                                                         \
  ASSERT (Handle != NULL);                                                \
                                                                          \
  Status = gBS->InstallMultipleProtocolInterfaces (Handle, __VA_ARGS__);  \
                                                                          \
  DEBUG_CODE (                                                            \
    if (Status != EFI_OUT_OF_RESOURCES) {                                 \
      ASSERT_EFI_ERROR (Status);                                          \
    }                                                                     \
    );                                                                    \
}

// UninstallMultipleProtocolInterfaces
/// Removes one or more protocol interfaces into the boot services environment.
///
/// @param[in] Handle The handle to remove the protocol interfaces from.
/// @param     ...    A variable argument list containing pairs of protocol GUIDs and
///                   protocol interfaces.
///
/// @retval EFI_SUCCESS           All the protocol interfaces were removed.
/// @retval EFI_INVALID_PARAMETER One of the protocol interfaces was not previously installed on Handle.
#define InstallMultipleProtocolInterfaces(                                  \
  Handle,                                                                   \
  ...                                                                       \
  )                                                                         \
{                                                                           \
  Status = gBS->UninstallMultipleProtocolInterfaces (Handle, __VA_ARGS__);  \
                                                                            \
  ASSERT_EFI_ERROR (Status);                                                \
}

// InstallProtocolInterface
/// Installs a protocol interface on a device handle. If the handle does not exist, it is created and added
/// to the list of handles in the system. InstallMultipleProtocolInterfaces() performs
/// more error checking than InstallProtocolInterface(), so it is recommended that
/// InstallMultipleProtocolInterfaces() be used in place of
/// InstallProtocolInterface()
///
/// @param[in, out] Handle        A pointer to the EFI_HANDLE on which the interface is to be installed.
/// @param[in]      Protocol      The numeric ID of the protocol interface.
/// @param[in]      InterfaceType Indicates whether Interface is supplied in native form.
/// @param[in]      Interface     A pointer to the protocol interface.
///
/// @retval EFI_SUCCESS           The protocol interface was installed.
/// @retval EFI_OUT_OF_RESOURCES  Space for a new handle could not be allocated.
/// @retval EFI_INVALID_PARAMETER Handle is NULL.
/// @retval EFI_INVALID_PARAMETER Protocol is NULL.
/// @retval EFI_INVALID_PARAMETER InterfaceType is not EFI_NATIVE_INTERFACE.
/// @retval EFI_INVALID_PARAMETER Protocol is already installed on the handle specified by Handle.
EFI_STATUS
InstallProtocolInterface (
  IN OUT EFI_HANDLE          *Handle,
  IN     EFI_GUID            *Protocol,
  IN     EFI_INTERFACE_TYPE  InterfaceType,
  IN     VOID                *Interface
  );

// ReinstallProtocolInterface
/// Reinstalls a protocol interface on a device handle.
///
/// @param[in] Handle       Handle on which the interface is to be reinstalled.
/// @param[in] Protocol     The numeric ID of the interface.
/// @param[in] OldInterface A pointer to the old interface. NULL can be used if a structure is not
///                         associated with Protocol.
/// @param[in] NewInterface A pointer to the new interface.
///
/// @retval EFI_SUCCESS           The protocol interface was reinstalled.
/// @retval EFI_NOT_FOUND         The OldInterface on the handle was not found.
/// @retval EFI_ACCESS_DENIED     The protocol interface could not be reinstalled,
///                               because OldInterface is still being used by a
///                               driver that will not release it.
/// @retval EFI_INVALID_PARAMETER Handle is NULL.
/// @retval EFI_INVALID_PARAMETER Protocol is NULL.
EFI_STATUS
ReinstallProtocolInterface (
  IN EFI_HANDLE  Handle,
  IN EFI_GUID    *Protocol,
  IN VOID        *OldInterface,
  IN VOID        *NewInterface
  );

// UninstallProtocolInterface
/// Removes a protocol interface from a device handle. It is recommended that
/// UninstallMultipleProtocolInterfaces() be used in place of
/// UninstallProtocolInterface().
///
/// @param[in] Handle    The handle on which the interface was installed.
/// @param[in] Protocol  The numeric ID of the interface.
/// @param[in] Interface A pointer to the interface.
///
/// @retval EFI_SUCCESS           The interface was removed.
/// @retval EFI_NOT_FOUND         The interface was not found.
/// @retval EFI_ACCESS_DENIED     The interface was not removed because the interface
///                               is still being used by a driver.
/// @retval EFI_INVALID_PARAMETER Handle is NULL.
/// @retval EFI_INVALID_PARAMETER Protocol is NULL.
EFI_STATUS
UninstallProtocolInterface (
  IN EFI_HANDLE  Handle,
  IN EFI_GUID    *Protocol,
  IN VOID        *Interface
  );

// HandleProtocol
/// Queries a handle to determine if it supports a specified protocol.
///
/// @param[in]  Handle    The handle being queried.
/// @param[in]  Protocol  The published unique identifier of the protocol.
/// @param[out] Interface Supplies the address where a pointer to the corresponding Protocol
///                        Interface is returned.
///
/// @retval EFI_SUCCESS           The interface information for the specified protocol was returned.
/// @retval EFI_UNSUPPORTED       The device does not support the specified protocol.
/// @retval EFI_INVALID_PARAMETER Handle is NULL.
/// @retval EFI_INVALID_PARAMETER Protocol is NULL.
/// @retval EFI_INVALID_PARAMETER Interface is NULL.
EFI_STATUS
HandleProtocol (
  IN     EFI_HANDLE  Handle,
  IN     EFI_GUID    *Protocol,
     OUT VOID        **Interface
  );

// OpenProtocol
/// Queries a handle to determine if it supports a specified protocol. If the protocol is supported by the
/// handle, it opens the protocol on behalf of the calling agent.
///
/// @param[in]  Handle           The handle for the protocol interface that is being opened.
/// @param[in]  Protocol         The published unique identifier of the protocol.
/// @param[out] Interface        Supplies the address where a pointer to the corresponding Protocol
///                              Interface is returned.
/// @param[in]  AgentHandle      The handle of the agent that is opening the protocol interface
///                              specified by Protocol and Interface.
/// @param[in]  ControllerHandle If the agent that is opening a protocol is a driver that follows the
///                              UEFI Driver Model, then this parameter is the controller handle
///                              that requires the protocol interface. If the agent does not follow
///                              the UEFI Driver Model, then this parameter is optional and may
///                              be NULL.
/// @param[in]  Attributes       The open mode of the protocol interface specified by Handle
///                              and Protocol.
///
/// @retval EFI_SUCCESS           An item was added to the open list for the protocol interface, and the
///                               protocol interface was returned in Interface.
/// @retval EFI_UNSUPPORTED       Handle does not support Protocol.
/// @retval EFI_INVALID_PARAMETER One or more parameters are invalid.
/// @retval EFI_ACCESS_DENIED     Required attributes can't be supported in current environment.
/// @retval EFI_ALREADY_STARTED   Item on the open list already has requierd attributes whose agent
///                               handle is the same as AgentHandle.
EFI_STATUS
OpenProtocol (
  IN     EFI_HANDLE  Handle,
  IN     EFI_GUID    *Protocol,
     OUT VOID        **Interface, OPTIONAL
  IN     EFI_HANDLE  AgentHandle,
  IN     EFI_HANDLE  ControllerHandle,
  IN     UINT32      Attributes
  );

// CloseProtocol
/// Closes a protocol on a handle that was opened using OpenProtocol().
///
/// @param[in] Handle           The handle for the protocol interface that was previously opened
///                             with OpenProtocol(), and is now being closed.
/// @param[in] Protocol         The published unique identifier of the protocol.
/// @param[in] AgentHandle      The handle of the agent that is closing the protocol interface.
/// @param[in] ControllerHandle If the agent that opened a protocol is a driver that follows the
///                             UEFI Driver Model, then this parameter is the controller handle
///                             that required the protocol interface.
///
/// @retval EFI_SUCCESS           The protocol instance was closed.
/// @retval EFI_INVALID_PARAMETER 1) Handle is NULL.
///                               2) AgentHandle is NULL.
///                               3) ControllerHandle is not NULL and ControllerHandle is not a valid EFI_HANDLE.
///                               4) Protocol is NULL.
/// @retval EFI_NOT_FOUND         1) Handle does not support the protocol specified by Protocol.
///                               2) The protocol interface specified by Handle and Protocol is not
///                               currently open by AgentHandle and ControllerHandle.
EFI_STATUS
CloseProtocol (
  IN EFI_HANDLE  Handle,
  IN EFI_GUID    *Protocol,
  IN EFI_HANDLE  AgentHandle,
  IN EFI_HANDLE  ControllerHandle
  );

// OpenProtocolInformation
/// Retrieves the list of agents that currently have a protocol interface opened.
///
/// @param[in]  Handle      The handle for the protocol interface that is being queried.
/// @param[in]  Protocol    The published unique identifier of the protocol.
/// @param[out] EntryBuffer A pointer to a buffer of open protocol information in the form of
///                         EFI_OPEN_PROTOCOL_INFORMATION_ENTRY structures.
/// @param[out] EntryCount  A pointer to the number of entries in EntryBuffer.
///
/// @retval EFI_SUCCESS          The open protocol information was returned in EntryBuffer, and the
///                              number of entries was returned EntryCount.
/// @retval EFI_OUT_OF_RESOURCES There are not enough resources available to allocate EntryBuffer.
/// @retval EFI_NOT_FOUND        Handle does not support the protocol specified by Protocol.
EFI_STATUS
OpenProtocolInformation (
  IN     EFI_HANDLE                           Handle,
  IN     EFI_GUID                             *Protocol,
     OUT EFI_OPEN_PROTOCOL_INFORMATION_ENTRY  **EntryBuffer,
     OUT UINTN                                *EntryCount
  );

// ProtocolsPerHandle
/// Retrieves the list of protocol interface GUIDs that are installed on a handle in a buffer allocated
/// from pool.
///
/// @param[in]  Handle              The handle from which to retrieve the list of protocol interface
///                                 GUIDs.
/// @param[out] ProtocolBuffer      A pointer to the list of protocol interface GUID pointers that are
///                                 installed on Handle.
/// @param[out] ProtocolBufferCount A pointer to the number of GUID pointers present in
///                                 ProtocolBuffer.
///
/// @retval EFI_SUCCESS           The list of protocol interface GUIDs installed on Handle was returned in
///                               ProtocolBuffer. The number of protocol interface GUIDs was
///                               returned in ProtocolBufferCount.
/// @retval EFI_OUT_OF_RESOURCES  There is not enough pool memory to store the results.
/// @retval EFI_INVALID_PARAMETER Handle is NULL.
/// @retval EFI_INVALID_PARAMETER Handle is not a valid EFI_HANDLE.
/// @retval EFI_INVALID_PARAMETER ProtocolBuffer is NULL.
/// @retval EFI_INVALID_PARAMETER ProtocolBufferCount is NULL.
EFI_STATUS
ProtocolsPerHandle (
  IN     EFI_HANDLE  Handle,
     OUT EFI_GUID    ***ProtocolBuffer,
     OUT UINTN       *ProtocolBufferCount
  );

// RegisterProtocolNotify
/// Creates an event that is to be signaled whenever an interface is installed for a specified protocol.
///
/// @param[in]  Protocol     The numeric ID of the protocol for which the event is to be registered.
/// @param[in]  Event        Event that is to be signaled whenever a protocol interface is registered
///                          for Protocol.
/// @param[out] Registration A pointer to a memory location to receive the registration value.
///
/// @retval EFI_SUCCESS           The notification event has been registered.
/// @retval EFI_OUT_OF_RESOURCES  Space for the notification event could not be allocated.
/// @retval EFI_INVALID_PARAMETER Protocol is NULL.
/// @retval EFI_INVALID_PARAMETER Event is NULL.
/// @retval EFI_INVALID_PARAMETER Registration is NULL.
EFI_STATUS
RegisterProtocolNotify (
  IN     EFI_GUID   *Protocol,
  IN     EFI_EVENT  Event,
     OUT VOID       **Registration
  );

// LocateHandle
/// Returns an array of handles that support a specified protocol.
///
/// @param[in]      SearchType Specifies which handle(s) are to be returned.
/// @param[in]      Protocol   Specifies the protocol to search by.
/// @param[in]      SearchKey  Specifies the search key.
/// @param[in, out] BufferSize On input, the size in bytes of Buffer. On output, the size in bytes of
///                            the array returned in Buffer (if the buffer was large enough) or the
///                            size, in bytes, of the buffer needed to obtain the array (if the buffer was
///                            not large enough).
/// @param[out]     Buffer     The buffer in which the array is returned.
///
/// @retval EFI_SUCCESS           The array of handles was returned.
/// @retval EFI_NOT_FOUND         No handles match the search.
/// @retval EFI_BUFFER_TOO_SMALL  The BufferSize is too small for the result.
/// @retval EFI_INVALID_PARAMETER SearchType is not a member of EFI_LOCATE_SEARCH_TYPE.
/// @retval EFI_INVALID_PARAMETER SearchType is ByRegisterNotify and SearchKey is NULL.
/// @retval EFI_INVALID_PARAMETER SearchType is ByProtocol and Protocol is NULL.
/// @retval EFI_INVALID_PARAMETER One or more matches are found and BufferSize is NULL.
/// @retval EFI_INVALID_PARAMETER BufferSize is large enough for the result and Buffer is NULL.
EFI_STATUS
LocateHandle (
  IN     EFI_LOCATE_SEARCH_TYPE  SearchType,
  IN     EFI_GUID                *Protocol, OPTIONAL
  IN     VOID                    *SearchKey, OPTIONAL
  IN OUT UINTN                   *BufferSize,
     OUT EFI_HANDLE              *Buffer
  );

// LocateDevicePath
/// Locates the handle to a device on the device path that supports the specified protocol.
///
/// @param[in]      Protocol   Specifies the protocol to search for.
/// @param[in, out] DevicePath On input, a pointer to a pointer to the device path. On output, the device
///                            path pointer is modified to point to the remaining part of the device path.
/// @param[out]     Device     A pointer to the returned device handle.
///
/// @retval EFI_SUCCESS           The resulting handle was returned.
/// @retval EFI_NOT_FOUND         No handles match the search.
/// @retval EFI_INVALID_PARAMETER Protocol is NULL.
/// @retval EFI_INVALID_PARAMETER DevicePath is NULL.
/// @retval EFI_INVALID_PARAMETER A handle matched the search and Device is NULL.
EFI_STATUS
LocateDevicePath (
  IN     EFI_GUID                  *Protocol,
  IN OUT EFI_DEVICE_PATH_PROTOCOL  **DevicePath,
     OUT EFI_HANDLE                *Device
  );

// InstallConfigurationTable
/// Adds, updates, or removes a configuration table entry from the EFI System Table.
///
/// @param[in] Guid  A pointer to the GUID for the entry to add, update, or remove.
/// @param[in] Table A pointer to the configuration table for the entry to add, update, or
///                  remove. May be NULL.
///
/// @retval EFI_SUCCESS           The (Guid, Table) pair was added, updated, or removed.
/// @retval EFI_NOT_FOUND         An attempt was made to delete a nonexistent entry.
/// @retval EFI_INVALID_PARAMETER Guid is NULL.
/// @retval EFI_OUT_OF_RESOURCES  There is not enough memory available to complete the operation.
EFI_STATUS
InstallConfigurationTable (
  IN EFI_GUID  *Guid,
  IN VOID      *Table
  );

// LocateHandleBuffer
/// Returns an array of handles that support the requested protocol in a buffer allocated from pool.
///
/// @param[in]      SearchType Specifies which handle(s) are to be returned.
/// @param[in]      Protocol   Provides the protocol to search by.
///                            This parameter is only valid for a SearchType of ByProtocol.
/// @param[in]      SearchKey  Supplies the search key depending on the SearchType.
/// @param[in, out] NoHandles  The number of handles returned in Buffer.
/// @param[out]     Buffer     A pointer to the buffer to return the requested array of handles that
///                            support Protocol.
///
/// @retval EFI_SUCCESS           The array of handles was returned in Buffer, and the number of
///                               handles in Buffer was returned in NoHandles.
/// @retval EFI_NOT_FOUND         No handles match the search.
/// @retval EFI_OUT_OF_RESOURCES  There is not enough pool memory to store the matching results.
/// @retval EFI_INVALID_PARAMETER NoHandles is NULL.
/// @retval EFI_INVALID_PARAMETER Buffer is NULL.
EFI_STATUS
LocateHandleBuffer (
  IN     EFI_LOCATE_SEARCH_TYPE  SearchType,
  IN     EFI_GUID                *Protocol, OPTIONAL
  IN     VOID                    *SearchKey, OPTIONAL
  IN OUT UINTN                   *NoHandles,
     OUT EFI_HANDLE              **Buffer
  );

// LocateProtocol
/// Returns the first protocol instance that matches the given protocol.
///
/// @param[in]  Protocol     Provides the protocol to search for.
/// @param[in]  Registration Optional registration key returned from
///                          RegisterProtocolNotify().
/// @param[out] Interface    On return, a pointer to the first interface that matches Protocol and
///                          Registration.
///
/// @retval EFI_SUCCESS           A protocol instance matching Protocol was found and returned in
///                               Interface.
/// @retval EFI_NOT_FOUND         No protocol instances were found that match Protocol and
///                               Registration.
/// @retval EFI_INVALID_PARAMETER Interface is NULL.
EFI_STATUS
LocateProtocol (
  IN     EFI_GUID  *Protocol,
  IN     VOID      *Registration, OPTIONAL
     OUT VOID      **Interface
  );

// ConnectController
/// Connects one or more drivers to a controller.
///
/// @param[in] ControllerHandle    The handle of the controller to which driver(s) are to be connected.
/// @param[in] DriverImageHandle   A pointer to an ordered list handles that support the
///                                EFI_DRIVER_BINDING_PROTOCOL.
/// @param[in] RemainingDevicePath A pointer to the device path that specifies a child of the
///                                controller specified by ControllerHandle.
/// @param[in] Recursive           If TRUE, then ConnectController() is called recursively
///                                until the entire tree of controllers below the controller specified
///                                by ControllerHandle have been created. If FALSE, then
///                                the tree of controllers is only expanded one level.
///
/// @retval EFI_SUCCESS            1) One or more drivers were connected to ControllerHandle.
///                                2) No drivers were connected to ControllerHandle, but
///                                RemainingDevicePath is not NULL, and it is an End Device
///                                Path Node.
/// @retval EFI_INVALID_PARAMETER  ControllerHandle is NULL.
/// @retval EFI_NOT_FOUND          1) There are no EFI_DRIVER_BINDING_PROTOCOL instances
///                                present in the system.
///                                2) No drivers were connected to ControllerHandle.
/// @retval EFI_SECURITY_VIOLATION The user has no permission to start UEFI device drivers
///                                on the device path associated with the ControllerHandle or
///                                specified by the RemainingDevicePath.
EFI_STATUS
ConnectController (
  IN EFI_HANDLE                ControllerHandle,
  IN EFI_HANDLE                *DriverImageHandle, OPTIONAL
  IN EFI_DEVICE_PATH_PROTOCOL  *RemainingDevicePath, OPTIONAL
  IN BOOLEAN                   Recursive
  );

// DisconnectController
/// Disconnects one or more drivers from a controller.
///
/// @param[in] ControllerHandle  The handle of the controller from which driver(s) are to be disconnected.
/// @param[in] DriverImageHandle The driver to disconnect from ControllerHandle.
///                              If DriverImageHandle is NULL, then all the drivers currently managing
///                              ControllerHandle are disconnected from ControllerHandle.
/// @param[in] ChildHandle       The handle of the child to destroy.
///                              If ChildHandle is NULL, then all the children of ControllerHandle are
///                              destroyed before the drivers are disconnected from ControllerHandle.
///
/// @retval EFI_SUCCESS           1) One or more drivers were disconnected from the controller.
///                               2) On entry, no drivers are managing ControllerHandle.
///                               3) DriverImageHandle is not NULL, and on entry
///                               DriverImageHandle is not managing ControllerHandle.
/// @retval EFI_INVALID_PARAMETER 1) ControllerHandle is NULL.
///                               2) DriverImageHandle is not NULL, and it is not a valid EFI_HANDLE.
///                               3) ChildHandle is not NULL, and it is not a valid EFI_HANDLE.
///                               4) DriverImageHandle does not support the EFI_DRIVER_BINDING_PROTOCOL.
/// @retval EFI_OUT_OF_RESOURCES  There are not enough resources available to disconnect any drivers from
/// ControllerHandle.
/// @retval EFI_DEVICE_ERROR      The controller could not be disconnected because of a device error.
EFI_STATUS
DisconnectController (
  IN EFI_HANDLE  ControllerHandle,
  IN EFI_HANDLE  DriverImageHandle, OPTIONAL
  IN EFI_HANDLE  ChildHandle OPTIONAL
  );

// SafeInstallProtocolInterface
EFI_STATUS
SafeInstallProtocolInterface (
  IN OUT EFI_HANDLE          *Handle,
  IN     EFI_GUID            *Protocol,
  IN     EFI_INTERFACE_TYPE  InterfaceType,
  IN     VOID                *Interface
  );

// InstallVersionedProtocol
EFI_STATUS
InstallVersionedProtocolInterface (
  IN OUT EFI_HANDLE          *Handle,
  IN     EFI_GUID            *Protocol,
  IN     EFI_INTERFACE_TYPE  InterfaceType,
  IN     VOID                *Interface
  );

#endif // ifndef _EFI_PROTOCOL_LIB_H_
