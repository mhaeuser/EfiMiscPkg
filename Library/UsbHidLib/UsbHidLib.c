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

#include <IndustryStandard/UsbHid.h>

// EfiKeyToUsbKeyCodeConvertionTable
/// EFI_KEY to USB Keycode conversion table
/// EFI_KEY is defined in UEFI spec.
/// USB Keycode is defined in USB HID Firmware spec.
GLOBAL_REMOVE_IF_UNREFERENCED USB_HID_USAGE_ID EfiKeyToUsbKeyCodeConvertionTable[] = {
  UsbHidUsageIdKbKpModifierKeyLeftControl,  // EfiKeyLCtrl
  UsbHidUsageIdKbKpModifierKeyLeftGui,  // EfiKeyA0
  UsbHidUsageIdKbKpModifierKeyLeftAlt,  // EfiKeyLAlt
  UsbHidUsageIdKbKpKeySpaceBar,  // EfiKeySpaceBar
  UsbHidUsageIdKbKpModifierKeyRightAlt,  // EfiKeyA2
  UsbHidUsageIdKbKpModifierKeyRightGui,  // EfiKeyA3
  UsbHidUsageIdKbKpPadKeyApplication,  // EfiKeyA4
  UsbHidUsageIdKbKpModifierKeyRightControl,  // EfiKeyRCtrl
  UsbHidUsageIdKbKpKeyLeftArrow,  // EfiKeyLeftArrow
  UsbHidUsageIdKbKpKeyDownArrow,  // EfiKeyDownArrow
  UsbHidUsageIdKbKpKeyRightArrow,  // EfiKeyRightArrow
  UsbHidUsageIdKbKpKeyZero,  // EfiKeyZero
  UsbHidUsageIdKbKpKeyPeriod,  // EfiKeyPeriod
  UsbHidUsageIdKbKpKeyEnter,  // EfiKeyEnter
  UsbHidUsageIdKbKpModifierKeyLeftShift,  // EfiKeyLShift
  UsbHidUsageIdKbKpPadKeyNonUsBackslash,  // EfiKeyB0
  UsbHidUsageIdKbKpKeyZ,  // EfiKeyB1
  UsbHidUsageIdKbKpKeyX,  // EfiKeyB2
  UsbHidUsageIdKbKpKeyC,  // EfiKeyB3
  UsbHidUsageIdKbKpKeyV,  // EfiKeyB4
  UsbHidUsageIdKbKpKeyB,  // EfiKeyB5
  UsbHidUsageIdKbKpKeyN,  // EfiKeyB6
  UsbHidUsageIdKbKpKeyM,  // EfiKeyB7
  UsbHidUsageIdKbKpKeyComma,  // EfiKeyB8
  UsbHidUsageIdKbKpKeyPeriod,  // EfiKeyB9
  UsbHidUsageIdKbKpKeySlash,  // EfiKeyB10
  UsbHidUsageIdKbKpModifierKeyRightShift,  // EfiKeyRShift
  UsbHidUsageIdKbKpKeyUpArrow,  // EfiKeyUpArrow
  UsbHidUsageIdKbKpKeyOne,  // EfiKeyOne
  UsbHidUsageIdKbKpKeyTwo,  // EfiKeyTwo
  UsbHidUsageIdKbKpKeyThree,  // EfiKeyThree
  UsbHidUsageIdKbKpKeyCLock,  // EfiKeyCapsLock
  UsbHidUsageIdKbKpKeyA,  // EfiKeyC1
  UsbHidUsageIdKbKpKeyS,  // EfiKeyC2
  UsbHidUsageIdKbKpKeyD,  // EfiKeyC3
  UsbHidUsageIdKbKpKeyF,  // EfiKeyC4
  UsbHidUsageIdKbKpKeyG,  // EfiKeyC5
  UsbHidUsageIdKbKpKeyH,  // EfiKeyC6
  UsbHidUsageIdKbKpKeyJ,  // EfiKeyC7
  UsbHidUsageIdKbKpKeyK,  // EfiKeyC8
  UsbHidUsageIdKbKpKeyL,  // EfiKeyC9
  UsbHidUsageIdKbKpKeySemicolon,  // EfiKeyC10
  UsbHidUsageIdKbKpKeyQuotation,  // EfiKeyC11
  UsbHidUsageIdKbKpKeyNonUsHash,  // EfiKeyC12
  UsbHidUsageIdKbKpKeyFour,  // EfiKeyFour
  UsbHidUsageIdKbKpKeyFive,  // EfiKeyFive
  UsbHidUsageIdKbKpKeySix,  // EfiKeySix
  UsbHidUsageIdKbKpPadKeyPlus,  // EfiKeyPlus
  UsbHidUsageIdKbKpKeyTab,  // EfiKeyTab
  UsbHidUsageIdKbKpKeyQ,  // EfiKeyD1
  UsbHidUsageIdKbKpKeyW,  // EfiKeyD2
  UsbHidUsageIdKbKpKeyE,  // EfiKeyD3
  UsbHidUsageIdKbKpKeyR,  // EfiKeyD4
  UsbHidUsageIdKbKpKeyT,  // EfiKeyD5
  UsbHidUsageIdKbKpKeyY,  // EfiKeyD6
  UsbHidUsageIdKbKpKeyU,  // EfiKeyD7
  UsbHidUsageIdKbKpKeyI,  // EfiKeyD8
  UsbHidUsageIdKbKpKeyO,  // EfiKeyD9
  UsbHidUsageIdKbKpKeyP,  // EfiKeyD10
  UsbHidUsageIdKbKpKeyLeftBracket,  // EfiKeyD11
  UsbHidUsageIdKbKpKeyRightBracket,  // EfiKeyD12
  UsbHidUsageIdKbKpKeyBackslash,  // EfiKeyD13
  UsbHidUsageIdKbKpKeyDel,  // EfiKeyDel
  UsbHidUsageIdKbKpKeyEnd,  // EfiKeyEnd
  UsbHidUsageIdKbKpKeyPgDn,  // EfiKeyPgDn
  UsbHidUsageIdKbKpKeySeven,  // EfiKeySeven
  UsbHidUsageIdKbKpKeyEight,  // EfiKeyEight
  UsbHidUsageIdKbKpKeyNine,  // EfiKeyNine
  UsbHidUsageIdKbKpKeyAcute,  // EfiKeyE0
  UsbHidUsageIdKbKpKeyOne,  // EfiKeyE1
  UsbHidUsageIdKbKpKeyTwo,  // EfiKeyE2
  UsbHidUsageIdKbKpKeyThree,  // EfiKeyE3
  UsbHidUsageIdKbKpKeyFour,  // EfiKeyE4
  UsbHidUsageIdKbKpKeyFive,  // EfiKeyE5
  UsbHidUsageIdKbKpKeySix,  // EfiKeyE6
  UsbHidUsageIdKbKpKeySeven,  // EfiKeyE7
  UsbHidUsageIdKbKpKeyEight,  // EfiKeyE8
  UsbHidUsageIdKbKpKeyNine,  // EfiKeyE9
  UsbHidUsageIdKbKpKeyZero,  // EfiKeyE10
  UsbHidUsageIdKbKpKeyMinus,  // EfiKeyE11
  UsbHidUsageIdKbKpKeyEquals,  // EfiKeyE12
  UsbHidUsageIdKbKpKeyBackSpace,  // EfiKeyBackSpace
  UsbHidUsageIdKbKpKeyIns,  // EfiKeyIns
  UsbHidUsageIdKbKpKeyHome,  // EfiKeyHome
  UsbHidUsageIdKbKpKeyPgUp,  // EfiKeyPgUp
  UsbHidUsageIdKbKpPadKeyNLck,  // EfiKeyNLck
  UsbHidUsageIdKbKpKeySlash,  // EfiKeySlash
  UsbHidUsageIdKbKpPadKeyAsterisk,  // EfiKeyAsterisk
  UsbHidUsageIdKbKpPadKeyMinus,  // EfiKeyMinus
  UsbHidUsageIdKbKpKeyEsc,  // EfiKeyEsc
  UsbHidUsageIdKbKpKeyF1,  // EfiKeyF1
  UsbHidUsageIdKbKpKeyF2,  // EfiKeyF2
  UsbHidUsageIdKbKpKeyF3,  // EfiKeyF3
  UsbHidUsageIdKbKpKeyF4,  // EfiKeyF4
  UsbHidUsageIdKbKpKeyF5,  // EfiKeyF5
  UsbHidUsageIdKbKpKeyF6,  // EfiKeyF6
  UsbHidUsageIdKbKpKeyF7,  // EfiKeyF7
  UsbHidUsageIdKbKpKeyF8,  // EfiKeyF8
  UsbHidUsageIdKbKpKeyF9,  // EfiKeyF9
  UsbHidUsageIdKbKpKeyF10,  // EfiKeyF10
  UsbHidUsageIdKbKpKeyF11,  // EfiKeyF11
  UsbHidUsageIdKbKpKeyF12,  // EfiKeyF12
  UsbHidUsageIdKbKpKeyPrint,  // EfiKeyPrint
  UsbHidUsageIdKbKpKeySLock,  // EfiKeySLck
  UsbHidUsageIdKbKpKeyPause   // EfiKeyPause
};
