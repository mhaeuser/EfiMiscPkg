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

#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/EfiRuntimeLib.h>
#include <Library/EfiChecksumLib.h>

// CalculateCrc32
/**
  Computes and returns a 32-bit CRC for a data Buffer.

  @param[in]  Data      A pointer to the Buffer on which the 32-bit CRC is to be computed.
  @param[in]  DataSize  The number of bytes in the Buffer Data.
  @param[out] Crc32     The 32-bit CRC that was computed for the data Buffer specified by Data
                        and DataSize.

  @retval EFI_SUCCESS            The 32-bit CRC was computed for the data Buffer and returned in
                                 Crc32.
  @retval EFI_INVALID_PARAMETER  Data is NULL.
  @retval EFI_INVALID_PARAMETER  Crc32 is NULL.
  @retval EFI_INVALID_PARAMETER  DataSize is 0.
**/
EFI_STATUS
CalculateCrc32 (
  IN  VOID    *Data,
  IN  UINTN   DataSize,
  OUT UINT32  *Crc32
  )
{
  EFI_STATUS Status;

  ASSERT (Data != NULL);
  ASSERT (DataSize > 0);
  ASSERT (Crc32 != NULL);
  ASSERT (!EfiAtRuntime ());

  *Crc32 = 0;
  Status = gBS->CalculateCrc32 (Data, DataSize, Crc32);

  ASSERT_EFI_ERROR (Status);

  return Status;
}
