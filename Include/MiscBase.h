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

#ifndef MISC_BASE_H_
#define MISC_BASE_H_

// FORWARD_DECLARATION
#if defined (EFI_NO_INTERFACE_DECL) || defined (NO_INTERFACE_DECL)
  #define FORWARD_DECLARATION(x)
#else
  #define FORWARD_DECLARATION(x) typedef struct x x
#endif // (EFI_NO_INTERFACE_DECL || NO_INTERFACE_DECL)

// ARRAY_LENGTH
#define ARRAY_LENGTH(Array) (sizeof (Array) / sizeof (*(Array)))

// CONVERT_LENGTH
#define CONVERT_LENGTH(Size, SourceType, DestinationType) (((Size) / sizeof (SourceType)) * sizeof (DestinationType))

/// @{
#define BIT(Index)                (1 << (Index))
#define JOIN_MASKS(Mask1, Mask2)  ((Mask1) | (Mask2))
#define JOIN_BITS(Mask1, Mask2)   JOIN_MASKS ((Mask1), (Mask2))
#define SET_BIT(Mask1, Mask2)     JOIN_MASKS ((Mask1), (Mask2))
#define SET_BITS(Mask1, Mask2)    SET_BIT ((Mask1), (Mask2))
#define UNSET_BITS(Mask1, Mask2)  ((Mask1) & ~(Mask2))
#define UNSET_BIT(Mask1, Mask2)   UNSET_BITS ((Mask1), (Mask2))
#define DIFF_BITS(Mask1, Mask2)   ((Mask1) ^ (Mask2))
#define TOGGLE_BITS(Mask1, Mask2) DIFF_BITS ((Mask1), (Mask2))
#define TOGGLE_BIT(Mask1, Mask2)  TOGGLE_BITS ((Mask1), (Mask2))
#define APPLY_MASK(Mask1, Mask2)  ((Mask1) & (Mask2))
#define SELECT_BITS(Mask1, Mask2) APPLY_MASK ((Mask1), (Mask2))
#define BIT_SET(Mask1, Mask2)     ((BOOLEAN)(SELECT_BITS ((Mask1), (Mask2)) != 0))
#define BITS_SET(Mask1, Mask2)    ((BOOLEAN)(SELECT_BITS ((Mask1), (Mask2)) == (Mask2)))
/// @}

#endif // MISC_BASE_H_
