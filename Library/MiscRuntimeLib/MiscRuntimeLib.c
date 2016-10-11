/** @file
  Instance of EFI Runtime Library.

  Instance of EFI Runtime Library, with hooked EVT_SIGNAL_VIRTUAL_ADDRESS_CHANGE and
  EVT_SIGNAL_EXIT_BOOT_SERVICES event, to provide runtime services.
  This instance also supports SAL drivers for better performance.

  Copyright (C) 2006 - 2016, Intel Corporation.  All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.
  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
**/

#include <Uefi.h>

#include <Library/DebugLib.h>
#include <Library/EfiBootServicesLib.h>
#include <Library/EfiRuntimeServicesLib.h>
#include <Library/MiscEventLib.h>
#include <Library/MiscRuntimeLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>

// gPhysicalRT
EFI_RUNTIME_SERVICES *gPhysicalRT = NULL;

// mEfiVirtualNotifyEvent
STATIC EFI_EVENT mEfiVirtualNotifyEvent = NULL;

// mEfiExitBootServicesEvent
STATIC EFI_EVENT mEfiExitBootServicesEvent = NULL;

// mEfiGoneVirtual
STATIC BOOLEAN mEfiGoneVirtual = FALSE;

// mEfiAtRuntime
STATIC BOOLEAN mEfiAtRuntime = FALSE;

// RuntimeLibExitBootServicesEvent
/** Set AtRuntime flag as TRUE after ExitBootServices.

  @param[in] Event    The Event that is being processed.
  @param[in] Context  The Event Context.
**/
VOID
EFIAPI
RuntimeLibExitBootServicesEvent (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  mEfiAtRuntime = TRUE;

  DEBUG_CODE (
    gBS = NULL;
    );
}

// RuntimeLibVirtualNotifyEvent
/** Fixup internal data so that EFI can be call in virtual mode.
    Call the passed in Child Notify event and convert any pointers in
    lib to virtual mode.

  @param[in] Event    The Event that is being processed.
  @param[in] Context  The Event Context.
**/
VOID
EFIAPI
RuntimeLibVirtualNotifyEvent (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  // Update global for Runtime Services Table and IO

  EfiConvertPointer (0, (VOID **)&gST);
  EfiConvertPointer (0, (VOID **)&gRT);

  mEfiGoneVirtual = TRUE;
}

// RuntimeDriverLibConstruct
/** Initialize runtime Driver Lib if it has not yet been initialized.
    It will ASSERT() if gRT is NULL or gBS is NULL.
    It will ASSERT() if that operation fails.

  @param[in] ImageHandle  The firmware allocated handle for the EFI image.
  @param[in] SystemTable  A pointer to the EFI System Table.

  @return  EFI_STATUS  always returns EFI_SUCCESS except EFI_ALREADY_STARTED if
                       already started.
**/
EFI_STATUS
EFIAPI
RuntimeDriverLibConstruct (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  ASSERT (gRT != NULL);
  ASSERT (gBS != NULL);

  gPhysicalRT               = gRT;
  mEfiVirtualNotifyEvent    = CreateVirtualAddressChangeEvent (
                                RuntimeLibVirtualNotifyEvent,
                                NULL
                                );

  mEfiExitBootServicesEvent = CreateExitBootServicesEvent (
                                RuntimeLibExitBootServicesEvent,
                                NULL
                                );

  return EFI_SUCCESS;
}

// RuntimeDriverLibDeCONSTruct
/** If a runtime driver exits with an error, it must call this routine 
    to free the allocated resource before the exiting.
    It will ASSERT() if gBS is NULL.
    It will ASSERT() if that operation fails.

  @param[in] ImageHandle  The firmware allocated handle for the EFI image.
  @param[in] SystemTable  A pointer to the EFI System Table.

  @retval EFI_SUCCESS      The Runtime Driver Lib shutdown successfully.
  @retval EFI_UNSUPPORTED  Runtime Driver lib was not initialized.
**/
EFI_STATUS
EFIAPI
RuntimeDriverLibDeCONSTruct (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  // Close SetVirtualAddressMap () notify function

  EfiCloseEvent (mEfiVirtualNotifyEvent);
  EfiCloseEvent (mEfiExitBootServicesEvent);

  return EFI_SUCCESS;
}

// EfiAtRuntime
/** This function allows the caller to determine if UEFI ExitBootServices() has
    been called.

  This function returns TRUE after all the EVT_SIGNAL_EXIT_BOOT_SERVICES
  functions have executed as a result of the OS calling ExitBootServices().
  Prior to this time FALSE is returned.  This function is used by runtime code
  to decide it is legal to access services that go away after
  ExitBootServices().

  @retval TRUE  The system has finished executing the
                EVT_SIGNAL_EXIT_BOOT_SERVICES event.
  @retval FALSE The system has not finished executing the
                EVT_SIGNAL_EXIT_BOOT_SERVICES event.
**/
BOOLEAN
EfiAtRuntime (
  VOID
  )
{
  return mEfiAtRuntime;
}

// EfiGoneVirtual
/** This function allows the caller to determine if UEFI SetVirtualAddressMap()
    has been called.

  This function returns TRUE after all the EVT_SIGNAL_VIRTUAL_ADDRESS_CHANGE
  functions have executed as a result of the OS calling SetVirtualAddressMap().
  Prior to this time FALSE is returned.  This function is used by runtime code
  to decide it is legal to access services that go away after
  SetVirtualAddressMap().

  @retval TRUE   The system has finished executing the
                 EVT_SIGNAL_VIRTUAL_ADDRESS_CHANGE event.
  @retval FALSE  The system has not finished executing the
                 EVT_SIGNAL_VIRTUAL_ADDRESS_CHANGE event.
**/
BOOLEAN
EfiGoneVirtual (
  VOID
  )
{
  return mEfiGoneVirtual;
}

// ConvertFunctionPointer
/**
  Determines the new virtual address that is to be used on subsequent memory
  accesses.

  For IA32, x64, and EBC, this service is a wrapper for the UEFI Runtime
  Service ConvertPointer().  See the UEFI Specification for details.
  For IPF, this function interprets Address as a pointer to an EFI_PLABEL
  structure and both the EntryPoint and GP fields of an EFI_PLABEL are
  converted from physical to virtiual addressing.  Since IPF allows the GP to
  point to an address outside a PE/COFF image, the physical to virtual offset
  for the EntryPoint field is used to adjust the GP field.  The UEFI Runtime
  Service ConvertPointer() is used to convert EntryPoint and the status code
  for this conversion is always returned.  If the convertion of EntryPoint
  fails, then neither EntryPoint nor GP are modified.  See the UEFI
  Specification for details on the UEFI Runtime Service ConvertPointer().

  @param[in]      DebugDisposition  Supplies type information for the pointer
                                    being converted.
  @param[in, out] Address           The pointer to a pointer that is to be
                                    fixed to be the value needed for the new
                                    virtual address mapping being applied.

  @return  EFI_STATUS value from EfiConvertPointer().
**/
EFI_STATUS
ConvertFunctionPointer (
  IN     UINTN  DebugDisposition,
  IN OUT VOID   **Address
  )
{
  return EfiConvertPointer (DebugDisposition, Address);
}

// ConvertList
/** Convert the standard Lib double linked list to a virtual mapping.

  This service uses ConvertPointer() to walk a double linked list and convert
  all the link pointers to their virtual mappings.  This function is only
  guaranteed to work during the EVT_SIGNAL_VIRTUAL_ADDRESS_CHANGE event and
  calling it at other times has undefined results.

  @param[in]      DebugDisposition  Supplies type information for the pointer
                                    being converted.
  @param[in, out] ListHead          Head of linked list to convert.

  @retval EFI_SUCCESS   Success to execute the function.
  @retval !EFI_SUCCESS  Failed to execute the function.
**/
VOID
ConvertList (
  IN     UINTN       DebugDisposition,
  IN OUT LIST_ENTRY  *ListHead
  )
{
  LIST_ENTRY *Link;
  LIST_ENTRY *NextLink;

  ASSERT (ListHead != NULL);

  // Convert all the ForwardLink & BackLink pointers in the list
  Link = ListHead;

  do {
    NextLink = Link->ForwardLink;

    EfiConvertPointer (
      ((Link->ForwardLink == ListHead) ? DebugDisposition : 0),
      (VOID **)&Link->ForwardLink
      );

    EfiConvertPointer (
      ((Link->BackLink == ListHead) ? DebugDisposition : 0),
      (VOID **)&Link->BackLink
      );

    Link = NextLink;
  } while (Link != ListHead);
}
