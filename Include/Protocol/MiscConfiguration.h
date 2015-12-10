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
/// @file      Include/Protocol/MiscConfiguration.h
///
///            
///
/// @author    Download-Fritz
/// @date      
/// @copyright Copyright (C) 2015 CupertinoNet. All rights reserved.
///

#ifndef __MISC_CONFIGURATION_H__
#define __MISC_CONFIGURATION_H__

#include <Uefi.h>

// EFI_MISC_OPTION_DATA
#define EFI_MISC_OPTION_DATA(EfiMiscOption) \
  ((UINT8)(((UINTN)(EfiMiscOption)) + sizeof (EfiMiscOption->Hdr) + EfiMiscOption->Hdr.NameSize))

// EFI_MISC_NEXT_OPTION
#define EFI_MISC_NEXT_OPTION(EfiMiscOption) \
  ((EFI_MISC_OPTION)(((UINTN)(EFI_MISC_OPTION_DATA (EfiMiscOption))) + EfiMiscOption->Hdr.DataSize))

// _EFI_MISC_OPTION_SOURCE
enum _EFI_MISC_OPTION_SOURCE {
  EfiMiscConfigurationLocationAny,
  EfiMiscConfigurationLocationNvram,
  EfiMiscConfigurationLocationFirmwareVolume,
  EfiMiscConfigurationLocationDisk
};

// EFI_MISC_OPTION_SOURCE
typedef UINT8 EFI_MISC_OPTION_LOCATION;

// _EFI_MISC_OPTION
typedef struct _EFI_MISC_OPTION {
  struct {
    UINTN                    NameSize;
    EFI_GUID                 VendorGuid;
    EFI_MISC_OPTION_LOCATION Location;
    UINTN                    DataSize;
  } Hdr;
  CHAR16                     Name;
//UINT8                      Data;
} EFI_MISC_OPTION;

typedef _EFI_MISC_CONFIGURATION_PROTOCOL EFI_MISC_CONFIGURATION_PROTOCOL;

// EFI_MISC_GET_OPTION
/// 
///
/// @param 
///
/// @return 
/// @retval 
typedef
EFI_STATUS
(EFIAPI *EFI_MISC_GET_OPTION)(
  IN  EFI_MISC_CONFIGURATION_PROTOCOL *This,
  IN  CHAR16                          *Name,
  IN  EFI_GUID                        *VendorGuid, OPTIONAL
  IN  EFI_MISC_OPTION_LOCATION        Location,
  OUT EFI_MISC_OPTION                 *Option
  );

// EFI_MISC_SET_OPTION
/// 
///
/// @param 
///
/// @return 
/// @retval 
typedef
EFI_STATUS
(EFIAPI *EFI_MISC_SET_OPTION)(
  IN EFI_MISC_CONFIGURATION_PROTOCOL *This,
  IN EFI_MISC_OPTION                 *Option
  );

// EFI_MISC_GET_ALL_OPTIONS
/// 
///
/// @param 
///
/// @return 
/// @retval 
typedef
EFI_STATUS
(EFIAPI *EFI_MISC_GET_ALL_OPTIONS)(
  IN  EFI_MISC_CONFIGURATION_PROTOCOL *This,
  IN  EFI_GUID                        *VendorGuid, OPTIONAL
  IN  EFI_MISC_OPTION_LOCATION        Location,
  OUT EFI_MISC_OPTION                 *Option
  );

// _EFI_MISC_CONFIGURATION_PROTOCOL
typedef struct _EFI_MISC_CONFIGURATION_PROTOCOL {
  UINT64              Signature;  ///< 
  UINTN               Revision;   ///< 
  EFI_MISC_GET_OPTION GetOption;  ///< 
  EFI_MISC_SET_OPTION SetOption;  ///< 
};

// gEfiMiscConfigurationProtocolGuid
extern EFI_GUID gEfiMiscConfigurationProtocolGuid;

#endif // ifndef __MISC_CONFIGURATION_H__
