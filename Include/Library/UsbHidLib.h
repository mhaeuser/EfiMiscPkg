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

#ifndef USB_HID_LIB_H_
#define USB_HID_LIB_H_

#include <IndustryStandard/UsbHid.h>

// EfiKeyToUsbKeyCodeConvertionTable
/// EFI_KEY to USB Keycode conversion table
/// EFI_KEY is defined in UEFI spec.
/// USB Keycode is defined in USB HID Firmware spec.
extern USB_HID_USAGE_ID EfiKeyToUsbKeyCodeConvertionTable[];

#endif // USB_HID_LIB_H_
