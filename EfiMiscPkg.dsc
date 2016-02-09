## @file
# Copyright (C) 2015 - 2016, CupertinoNet.  All rights reserved.<BR>
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
#
##

[Defines]
  PLATFORM_NAME           = EfiMisc
  PLATFORM_GUID           = 58F5CF1B-7FE6-4BE4-A9D5-B6EDC415CC9D
  PLATFORM_VERSION        = 1.0
  SUPPORTED_ARCHITECTURES = IA32|IPF|X64|EBC|ARM|AARCH64
  BUILD_TARGETS           = RELEASE|DEBUG|NOOPT
  SKUID_IDENTIFIER        = DEFAULT
  DSC_SPECIFICATION       = 0x00010006

[LibraryClasses]
  DxeServicesLib|EfiMiscPkg/Library/DxeServicesLib/DxeServicesLib.inf
  EfiBootServicesLib|EfiMiscPkg/Library/EfiBootServicesLib/EfiBootServicesLib.inf
  EfiRuntimeServicesLib|EfiMiscPkg/Library/EfiRuntimeServicesLib/EfiRuntimeServicesLib.inf
  MiscEventLib|EfiMiscPkg/Library/MiscEventLib/MiscEventLib.inf
  MiscFileLib|EfiMiscPkg/Library/MiscFileLib/MiscFileLib.inf
  MiscMemoryLib|EfiMiscPkg/Library/MiscMemoryLib/MiscMemoryLib.inf
  MiscOverrideLib|EfiMiscPkg/Library/MiscOverrideLib/MiscOverrideLib.inf
  MiscProtocolLib|EfiMiscPkg/Library/MiscProtocolLib/MiscProtocolLib.inf
  MiscUsbHidLib|EfiMiscPkg/Library/MiscUsbHidLib/MiscUsbHidLib.inf
  MiscVariableLib|EfiMiscPkg/Library/MiscVariableLib/MiscVariableLib.inf

[LibraryClasses.IA32, LibraryClasses.X64]
  SmmServicesLib|EfiMiscPkg/Library/SmmServicesLib/SmmServicesLib.inf
  SmmServicesTableLib|EfiMiscPkg/Library/SmmServicesTableLib/SmmServicesTableLib.inf

[LibraryClasses.Common.DXE_RUNTIME_DRIVER]
  MiscRuntimeLib|EfiMiscPkg/Library/MiscRuntimeLib/MiscRuntimeLib.inf

[LibraryClasses.Common.BASE, LibraryClasses.Common.SEC, LibraryClasses.Common.PEI_CORE, LibraryClasses.Common.PEIM, LibraryClasses.Common.DXE_CORE, LibraryClasses.Common.DXE_DRIVER, LibraryClasses.Common.DXE_RUNTIME_DRIVER, LibraryClasses.Common.DXE_SAL_DRIVER, LibraryClasses.Common.DXE_SMM_DRIVER, LibraryClasses.Common.UEFI_APPLICATION, LibraryClasses.Common.UEFI_DRIVER, LibraryClasses.Common.SMM_CORE, LibraryClasses.Common.DXE_SMM_DRIVER]
  MiscRuntimeLib|EfiMiscPkg/Library/MiscRuntimeLibNull/MiscRuntimeLibNull.inf

[Components]
  EfiMiscPkg/Library/DxeServicesLib/DxeServicesLib.inf
  EfiMiscPkg/Library/EfiBootServicesLib/EfiBootServicesLib.inf
  EfiMiscPkg/Library/EfiRuntimeServicesLib/EfiRuntimeServicesLib.inf
  EfiMiscPkg/Library/MiscEventLib/MiscEventLib.inf
  EfiMiscPkg/Library/MiscFileLib/MiscFileLib.inf
  EfiMiscPkg/Library/MiscMemoryLib/MiscMemoryLib.inf
  EfiMiscPkg/Library/MiscOverrideLib/MiscOverrideLib.inf
  EfiMiscPkg/Library/MiscProtocolLib/MiscProtocolLib.inf
  EfiMiscPkg/Library/MiscRuntimeLib/MiscRuntimeLib.inf
  EfiMiscPkg/Library/MiscRuntimeLibNull/MiscRuntimeLibNull.inf
  EfiMiscPkg/Library/MiscVariableLib/MiscVariableLib.inf
  EfiMiscPkg/Library/MiscUsbHidLib/MiscUsbHidLib.inf
  EfiMiscPkg/Library/SmmServicesLib/SmmServicesLib.inf
  EfiMiscPkg/Library/SmmServicesTableLib/SmmServicesTableLib.inf
