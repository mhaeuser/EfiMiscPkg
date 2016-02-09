/** @file
  Copyright (C) 2015, CupertinoNet.  All rights reserved.<BR>

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

#include <Library/EfiBootServicesLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/MiscRuntimeLib.h>
#include <Library/MiscProtocolLib.h>

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
  ASSERT (!EfiAtRuntime ());

  Status = EfiLocateHandleBuffer (
             ByProtocol,
             Protocol,
             NULL,
             &NoHandles,
             &Buffer
             );

  if (!EFI_ERROR (Status)) {
    FreePool ((VOID *)Buffer);
  }

  return ((Status == EFI_NOT_FOUND)
            ? EfiInstallProtocolInterface (
                Handle,
                Protocol,
                InterfaceType,
                Interface
                )
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
  ASSERT (!EfiAtRuntime ());

  if (*Handle != NULL) {
    Status = EfiHandleProtocol (*Handle, Protocol, &OldInterface);

    if (!EFI_ERROR (Status)) {
      Status = EFI_ALREADY_STARTED;

      if (*(UINTN *)OldInterface < *(UINTN *)Interface) {
        Status = EfiReinstallProtocolInterface (
                   *Handle,
                   Protocol,
                   OldInterface,
                   Interface
                   );
      }
    } else if (Status == EFI_UNSUPPORTED) {
      goto InstallProtocol;
    }
  } else {
    Status = EfiLocateHandleBuffer (
               ByProtocol,
               Protocol,
               NULL,
               &NoHandles,
               &Buffer
               );

    if (!EFI_ERROR (Status)) {
      Status = EFI_ALREADY_STARTED;

      if (NoHandles == 1) {
        Status = EfiHandleProtocol (Buffer[0], Protocol, &OldInterface);

        if (!EFI_ERROR (Status)) {
          Status = EFI_ALREADY_STARTED;

          if (*(UINTN *)OldInterface < *(UINTN *)Interface) {
            Status = EfiUninstallProtocolInterface (
                       Buffer[0],
                       Protocol,
                       OldInterface
                       );

            if (!EFI_ERROR (Status)) {
              goto InstallProtocol;
            }
          }
        }
      }

      FreePool ((VOID *)Buffer);
    } else if (Status == EFI_NOT_FOUND) {
    InstallProtocol:
      Status = EfiInstallProtocolInterface (
                 Handle,
                 Protocol,
                 InterfaceType,
                 Interface
                 );
    }
  }

  if (Status != EFI_ALREADY_STARTED) {
    ASSERT_EFI_ERROR (Status);
  }

  return Status;
}
