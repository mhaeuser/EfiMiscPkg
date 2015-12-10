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
/// @file      Library/EfiChecksumLib/EfiChecksumLib.c
///
///            
///
/// @author    Download-Fritz
/// @date      18/07/2015: Initial version
/// @copyright Copyright (C) 2015 CupertinoNet. All rights reserved.
///

#include <Uefi.h>

#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/EfiRuntimeLib.h>
#include <Library/EfiChecksumLib.h>

// CalculateCrc32
/// 
///
/// @param 
///
/// @return 
/// @retval
EFI_STATUS
CalculateCrc32 (
  IN  VOID    *Data,
  IN  UINTN   DataSize,
  OUT UINT32  *Crc32
  )
{
  EFI_STATUS Status;

  ASSERT (!EfiAtRuntime ());
  ASSERT (Data != NULL);
  ASSERT (DataSize > 0);
  ASSERT (Crc32 != NULL);

  *Crc32 = 0;
  Status = gBS->CalculateCrc32 (Data, DataSize, Crc32);

  ASSERT_EFI_ERROR (Status);

  return Status;
}