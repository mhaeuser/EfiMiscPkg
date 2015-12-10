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
/// @file      Include/Library/EfiChecksumLib.h
///
///            
///
/// @author    Download-Fritz
/// @date      18/07/2015: Initial version
/// @copyright Copyright (C) 2015 CupertinoNet. All rights reserved.
///

#ifndef __EFI_CHECKSUM_LIB_H__
#define __EFI_CHECKSUM_LIB_H__

// CALCULATE_CRC32
#define CALCULATE_CRC32(Struct, Size, Crc32)                  \
  Crc32 = 0;                                                  \
                                                              \
  CalculateCrc32 ((VOID *)&(Struct), (UINTN)(Size), &Crc32);

// CALCULATE_EFI_TABLE_HEADER_CRC32
#define CALCULATE_EFI_TABLE_HEADER_CRC32(Header)                  \
  CALCULATE_CRC32 (Header, (Header).HeaderSize, (Header).CRC32);

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
  );

#endif // ifndef __EFI_CHECKSUM_LIB_H__
