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
/// @file      Library/EfiOverrideLib/EfiOverrideLib.c
///
///            
///
/// @author    Download-Fritz
/// @date      07/10/2015: initial version
/// @copyright Copyright (C) 2015 CupertinoNet. All rights reserved.
///

#include <Uefi.h>
#include <PiDxe.h>
#include <PiSmm.h>

#include <Framework/SmmCis.h>

#include <Library/MemoryAllocationLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>

/// @{
#define OVERRIDE_IDENTIFIER_SIGNATURE                  SIGNATURE_32 ('o', 'v', 'r', 'r')
#define OVERRIDE_IDENTIFIER_FROM_LIST_ENTRY(ListEntry) CR ((ListEntry), EFI_OVERRIDE_IDENTIFIER, This, OVERRIDE_IDENTIFIER_SIGNATURE)
#define OVERRIDE_IDENTIFIER_FUNCTION (Identfier, Type) ((Type)OVERRIDE_IDENTIFIER_FROM_LIST_ENTRY ((Identifier)->This.BackLink)->Function)
/// @}

// _EFI_SMM_SERVICE
typedef union _EFI_SMM_SERVICE {
  EFI_SMM_INSTALL_CONFIGURATION_TABLE SmmInstallConfigurationTable;
  EFI_SMMCORE_ALLOCATE_POOL           SmmAllocatePool;
  EFI_SMMCORE_FREE_POOL               SmmFreePool;
  EFI_SMMCORE_ALLOCATE_PAGES          SmmAllocatePages;
  EFI_SMMCORE_FREE_PAGES              SmmFreePages;
} EFI_SMM_SERVICE;

// _EFI_SMM_SERVICE2
typedef union _EFI_SMM_SERVICE2 {
  EFI_SMM_INSTALL_CONFIGURATION_TABLE2 InstallConfigurationTable;
  EFI_ALLOCATE_POOL                    AllocatePool;
  EFI_FREE_POOL                        FreePool;
  EFI_ALLOCATE_PAGES                   AllocatePages;
  EFI_FREE_PAGES                       FreePages;
  EFI_INSTALL_PROTOCOL_INTERFACE       InstallProtocolInterface;
  EFI_UNINSTALL_PROTOCOL_INTERFACE     UninstallProtocolInterface;
  EFI_HANDLE_PROTOCOL                  HandleProtocol;
  EFI_SMM_REGISTER_PROTOCOL_NOTIFY     RegisterProtocolNotify;
  EFI_LOCATE_HANDLE                    LocateHandle;
  EFI_LOCATE_PROTOCOL                  LocateProtocol;
  EFI_SMM_INTERRUPT_MANAGE             SmiManage;
  EFI_SMM_INTERRUPT_REGISTER           SmiHandlerRegister;
  EFI_SMM_INTERRUPT_UNREGISTER         SmiHandlerUnRegister;
} EFI_SMM_SERVICE2;

// _EFI_DXE_SERVICE
typedef union _EFI_DXE_SERVICE {
  EFI_ADD_MEMORY_SPACE              AddMemorySpace;
  EFI_ALLOCATE_MEMORY_SPACE         AllocateMemorySpace;
  EFI_FREE_MEMORY_SPACE             FreeMemorySpace;
  EFI_REMOVE_MEMORY_SPACE           RemoveMemorySpace;
  EFI_GET_MEMORY_SPACE_DESCRIPTOR   GetMemorySpaceDescriptor;
  EFI_SET_MEMORY_SPACE_ATTRIBUTES   SetMemorySpaceAttributes;
  EFI_GET_MEMORY_SPACE_MAP          GetMemorySpaceMap;
  EFI_ADD_IO_SPACE                  AddIoSpace;
  EFI_ALLOCATE_IO_SPACE             AllocateIoSpace;
  EFI_FREE_IO_SPACE                 FreeIoSpace;
  EFI_REMOVE_IO_SPACE               RemoveIoSpace;
  EFI_GET_IO_SPACE_DESCRIPTOR       GetIoSpaceDescriptor;
  EFI_GET_IO_SPACE_MAP              GetIoSpaceMap;
  EFI_DISPATCH                      Dispatch;
  EFI_SCHEDULE                      Schedule;
  EFI_TRUST                         Trust;
  EFI_PROCESS_FIRMWARE_VOLUME       ProcessFirmwareVolume;
  EFI_SET_MEMORY_SPACE_CAPABILITIES SetMemorySpaceCapabilities;
} EFI_DXE_SERVICE;

// _EFI_BOOT_SERVICE
typedef union _EFI_BOOT_SERVICE {
  EFI_RAISE_TPL                              RaiseTPL;
  EFI_RESTORE_TPL                            RestoreTPL;
  EFI_ALLOCATE_PAGES                         AllocatePages;
  EFI_FREE_PAGES                             FreePages;
  EFI_GET_MEMORY_MAP                         GetMemoryMap;
  EFI_ALLOCATE_POOL                          AllocatePool;
  EFI_FREE_POOL                              FreePool;
  EFI_CREATE_EVENT                           CreateEvent;
  EFI_SET_TIMER                              SetTimer;
  EFI_WAIT_FOR_EVENT                         WaitForEvent;
  EFI_SIGNAL_EVENT                           SignalEvent;
  EFI_CLOSE_EVENT                            CloseEvent;
  EFI_CHECK_EVENT                            CheckEvent;
  EFI_INSTALL_PROTOCOL_INTERFACE             InstallProtocolInterface;
  EFI_REINSTALL_PROTOCOL_INTERFACE           ReinstallProtocolInterface;
  EFI_UNINSTALL_PROTOCOL_INTERFACE           UninstallProtocolInterface;
  EFI_HANDLE_PROTOCOL                        HandleProtocol;
  EFI_REGISTER_PROTOCOL_NOTIFY               RegisterProtocolNotify;
  EFI_LOCATE_HANDLE                          LocateHandle;
  EFI_LOCATE_DEVICE_PATH                     LocateDevicePath;
  EFI_INSTALL_CONFIGURATION_TABLE            InstallConfigurationTable;
  EFI_IMAGE_LOAD                             LoadImage;
  EFI_IMAGE_START                            StartImage;
  EFI_EXIT                                   Exit;
  EFI_IMAGE_UNLOAD                           UnloadImage;
  EFI_EXIT_BOOT_SERVICES                     ExitBootServices;
  EFI_GET_NEXT_MONOTONIC_COUNT               GetNextMonotonicCount;
  EFI_STALL                                  Stall;
  EFI_SET_WATCHDOG_TIMER                     SetWatchdogTimer;
  EFI_CONNECT_CONTROLLER                     ConnectController;
  EFI_DISCONNECT_CONTROLLER                  DisconnectController;
  EFI_OPEN_PROTOCOL                          OpenProtocol;
  EFI_CLOSE_PROTOCOL                         CloseProtocol;
  EFI_OPEN_PROTOCOL_INFORMATION              OpenProtocolInformation;
  EFI_PROTOCOLS_PER_HANDLE                   ProtocolsPerHandle;
  EFI_LOCATE_HANDLE_BUFFER                   LocateHandleBuffer;
  EFI_LOCATE_PROTOCOL                        LocateProtocol;
  EFI_INSTALL_MULTIPLE_PROTOCOL_INTERFACES   InstallMultipleProtocolInterfaces;
  EFI_UNINSTALL_MULTIPLE_PROTOCOL_INTERFACES UninstallMultipleProtocolInterfaces;
  EFI_CALCULATE_CRC32                        CalculateCrc32;
  EFI_COPY_MEM                               CopyMem;
  EFI_SET_MEM                                SetMem;
  EFI_CREATE_EVENT_EX                        CreateEventEx;
} EFI_BOOT_SERVICE;

// _EFI_RUNTIME_SERVICE
typedef union _EFI_RUNTIME_SERVICE {
  EFI_GET_TIME                   GetTime;
  EFI_SET_TIME                   SetTime;
  EFI_GET_WAKEUP_TIME            GetWakeupTime;
  EFI_SET_WAKEUP_TIME            SetWakeupTime;
  EFI_SET_VIRTUAL_ADDRESS_MAP    SetVirtualAddressMap;
  EFI_CONVERT_POINTER            ConvertPointer;
  EFI_GET_VARIABLE               GetVariable;
  EFI_GET_NEXT_VARIABLE_NAME     GetNextVariableName;
  EFI_SET_VARIABLE               SetVariable;
  EFI_GET_NEXT_HIGH_MONO_COUNT   GetNextHighMonotonicCount;
  EFI_RESET_SYSTEM               ResetSystem;
  EFI_UPDATE_CAPSULE             UpdateCapsule;
  EFI_QUERY_CAPSULE_CAPABILITIES QueryCapsuleCapabilities;
  EFI_QUERY_VARIABLE_INFO        QueryVariableInfo;
} EFI_RUNTIME_SERVICE;

// _EFI_SERVICE
typedef union _EFI_SERVICE {
  EFI_SMM_SERVICE     SmmService;
  EFI_SMM_SERVICE2    SmmService2;
  EFI_DXE_SERVICE     DxeService;
  EFI_BOOT_SERVICE    BootService;
  EFI_RUNTIME_SERVICE RuntimeService;
} EFI_SERVICE;

// _EFI_OVERRIDE
typedef struct _EFI_OVERRIDE {
  VOID       **Destination;  ///< 
  VOID       *Original;      ///< 
  LIST_ENTRY Identifiers;    ///< 
} EFI_OVERRIDE;

// _EFI_OVERRIDE_IDENTIFIER
typedef struct _EFI_OVERRIDE_IDENTIFIER {
  UINTN      Signature;    ///< 
  LIST_ENTRY This;         ///< 
  VOID       **Function;   ///< 
  CHAR8      *Identifier;  ///< 
} EFI_OVERRIDE_IDENTIFIER;

// InitializeOverride
/// 
///
/// @param 
///
/// @return 
/// @retval 
EFI_OVERRIDE *
InitializeOverride (
  IN VOID **Function
  )
{
  EFI_OVERRIDE *Override;

  ASSERT (Function != NULL);

  Override = (EFI_OVERRIDE *)AllocatePool (sizeof (*Override));

  if (Override != NULL) {
    Override->Destination = Function;
    Override->Original   = *Function;

    InitializeListHead (&Override->Identifiers);
  }

  return Override;
}

// AddOverride
/// 
///
/// @param 
///
/// @return 
/// @retval 
EFI_OVERRIDE_IDENTIFIER *
AddOverride (
  IN OUT EFI_OVERRIDE *Override,
  IN     VOID         *Function
  )
{
  EFI_OVERRIDE_IDENTIFIER *Identifier;

  ASSERT (Override != NULL);
  ASSERT (Function != NULL);

  Identifier             = (EFI_OVERRIDE_IDENTIFIER *)AllocatePool (sizeof (*Identifier));
  Identifier->Signature  = OVERRIDE_IDENTIFIER_SIGNATURE;
  Identifier->Function   = Function;
  Identifier->Identifier = "OvrFuncIdent";
  *Override->Destination = Function;
  
  InsertTailList (&Override->Identifiers, &Identifier->This);

  return Identifier;
}

// RemoveOverride
/// 
///
/// @param 
///
/// @return 
/// @retval 
VOID
RemoveOverride (
  IN OUT EFI_OVERRIDE            *Override,
  IN     EFI_OVERRIDE_IDENTIFIER *Identifier
  )
{
  ASSERT (Override != NULL);
  ASSERT (Identifier != NULL);

  *Override->Destination = (OVERRIDE_IDENTIFIER_FROM_LIST_ENTRY (Identifier->This.BackLink))->Function;

  RemoveEntryList (&Identifier->This);
  FreePool ((VOID *)Identifier);
}

// RestoreOriginal
/// 
///
/// @param 
///
/// @return 
/// @retval 
VOID
RestoreOriginal (
  IN OUT EFI_OVERRIDE *Override
  )
{
  EFI_OVERRIDE_IDENTIFIER *Identifier;
  EFI_OVERRIDE_IDENTIFIER *NextIdentifier;

  if (!IsListEmpty (&Override->Identifiers)) {
    Identifier = OVERRIDE_IDENTIFIER_FROM_LIST_ENTRY (Override->Identifiers.ForwardLink);

    do {
      NextIdentifier = OVERRIDE_IDENTIFIER_FROM_LIST_ENTRY (Identifier->This.ForwardLink);

      RemoveEntryList (&Identifier->This);
      FreePool ((VOID *)Identifier);

      Identifier = NextIdentifier;
    } while (!IsNull (&Override->Identifiers, &Identifier->This));
  }

  *Override->Destination = Override->Original;
}

// UpdatePointers
/// 
///
/// @param 
///
/// @return 
/// @retval 
VOID
UpdatePointers (
  
  )
{
  
}
