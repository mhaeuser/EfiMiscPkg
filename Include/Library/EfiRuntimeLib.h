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
/// @file      Include/Library/EfiRuntimeLib.h
///
///            
///
/// @author    Download-Fritz
/// @date      16/09/2015: Initial version
/// @copyright Copyright (C) 2015 CupertinoNet. All rights reserved.
///

#ifndef __EFI_RUNTIME_LIB_H__
#define __EFI_RUNTIME_LIB_H__

// EfiAtRuntime
/// This function allows the caller to determine if UEFI ExitBootServices() has been called.
///
/// This function returns TRUE after all the EVT_SIGNAL_EXIT_BOOT_SERVICES functions have
/// executed as a result of the OS calling ExitBootServices().  Prior to this time FALSE
/// is returned. This function is used by runtime code to decide it is legal to access
/// services that go away after ExitBootServices().
///
/// @retval  TRUE  The system has finished executing the EVT_SIGNAL_EXIT_BOOT_SERVICES event.
/// @retval  FALSE The system has not finished executing the EVT_SIGNAL_EXIT_BOOT_SERVICES event.
BOOLEAN
EfiAtRuntime (
  VOID
  );

// EfiGoneVirtual
/// This function allows the caller to determine if UEFI SetVirtualAddressMap() has been called.
///
/// This function returns TRUE after all the EVT_SIGNAL_VIRTUAL_ADDRESS_CHANGE functions have
/// executed as a result of the OS calling SetVirtualAddressMap(). Prior to this time FALSE
/// is returned. This function is used by runtime code to decide it is legal to access services
/// that go away after SetVirtualAddressMap().
///
/// @retval TRUE  The system has finished executing the EVT_SIGNAL_VIRTUAL_ADDRESS_CHANGE event.
/// @retval FALSE The system has not finished executing the EVT_SIGNAL_VIRTUAL_ADDRESS_CHANGE event.
BOOLEAN
EfiGoneVirtual (
  VOID
  );

// ConvertPointer
/// This service is a wrapper for the UEFI Runtime Function ConvertPointer().  
///
/// The ConvertPointer() function is used by an EFI component during the SetVirtualAddressMap() operation.
/// ConvertPointer()must be called using physical address pointers during the execution of SetVirtualAddressMap().
///
/// @param[in]      DebugDisposition Supplies type information for the pointer being converted.
/// @param[in, out] Address          The pointer to a pointer that is to be fixed to be the
///                                  value needed for the new virtual address mapping being
///                                  applied.
///
/// @retval EFI_SUCCESS           The pointer pointed to by Address was modified.
/// @retval EFI_NOT_FOUND         The pointer pointed to by Address was not found to be part of
///                               the current memory map. This is normally fatal.
/// @retval EFI_INVALID_PARAMETER Address is NULL.
/// @retval EFI_INVALID_PARAMETER *Address is NULL and DebugDispositinon
EFI_STATUS
ConvertPointer (
  IN     UINTN  DebugDisposition,
  IN OUT VOID   **Address
  );

// ConvertFunctionPointer
/// Determines the new virtual address that is to be used on subsequent memory accesses.
///
/// For IA32, x64, and EBC, this service is a wrapper for the UEFI Runtime Function
/// ConvertPointer().  See the UEFI Specification for details. 
/// For IPF, this function interprets Address as a pointer to an EFI_PLABEL structure
/// and both the EntryPoint and GP fields of an EFI_PLABEL are converted from physical
/// to virtiual addressing.  Since IPF allows the GP to point to an address outside
/// a PE/COFF image, the physical to virtual offset for the EntryPoint field is used
/// to adjust the GP field.  The UEFI Runtime Function ConvertPointer() is used to convert
/// EntryPoint and the status code for this conversion is always returned.   If the convertion
/// of EntryPoint fails, then neither EntryPoint nor GP are modified.  See the UEFI
/// Specification for details on the UEFI Runtime Function ConvertPointer().
///
/// @param[in]      DebugDisposition Supplies type information for the pointer being converted.
/// @param[in, out] Address          The pointer to a pointer that is to be fixed to be the
///                                  value needed for the new virtual address mapping being
///                                  applied.
///
/// @return EFI_STATUS value from ConvertPointer().
EFI_STATUS
ConvertFunctionPointer (
  IN     UINTN  DebugDisposition,
  IN OUT VOID   **Address
  );

// ConvertList
/// Convert the standard Lib double linked list to a virtual mapping.
///
/// This service uses ConvertPointer() to walk a double linked list and convert all the link
/// pointers to their virtual mappings. This function is only guaranteed to work during the
/// EVT_SIGNAL_VIRTUAL_ADDRESS_CHANGE event and calling it at other times has undefined results.
///
/// @param[in]      DebugDisposition Supplies type information for the pointer being converted.
/// @param[in, out] ListHead         Head of linked list to convert.
///
/// @retval EFI_SUCCESS  Success to execute the function.
/// @retval !EFI_SUCCESS Failed to execute the function.
VOID
ConvertList (
  IN     UINTN       DebugDisposition,
  IN OUT LIST_ENTRY  *ListHead
  );

// gPhysicalRT
extern EFI_RUNTIME_SERVICES *gPhysicalRT;

#endif // ifndef __EFI_RUNTIME_LIB_H__
