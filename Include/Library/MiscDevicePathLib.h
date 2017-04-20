/** @file
  Copyright (C) 2017, CupertinoNet.  All rights reserved.<BR>

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

#ifndef MISC_DEVICE_PATH_LIB_H_
#define MISC_DEVICE_PATH_LIB_H_

// MiscFileDevicePathToText
CHAR16 *
MiscFileDevicePathToText (
  IN     EFI_DEVICE_PATH_PROTOCOL  *DevicePath,
  IN OUT UINTN                     *TextSize OPTIONAL
  );

#endif // MISC_DEVICE_PATH_LIB_H_
