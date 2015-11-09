#ifndef _EFI_RUNTIME_LIB_H_
#define _EFI_RUNTIME_LIB_H_

/**
This function allows the caller to determine if UEFI ExitBootServices() has been called.

This function returns TRUE after all the EVT_SIGNAL_EXIT_BOOT_SERVICES functions have
executed as a result of the OS calling ExitBootServices().  Prior to this time FALSE
is returned. This function is used by runtime code to decide it is legal to access
services that go away after ExitBootServices().

@retval  TRUE  The system has finished executing the EVT_SIGNAL_EXIT_BOOT_SERVICES event.
@retval  FALSE The system has not finished executing the EVT_SIGNAL_EXIT_BOOT_SERVICES event.

**/
BOOLEAN
EfiAtRuntime (
  VOID
  );

/**
This function allows the caller to determine if UEFI SetVirtualAddressMap() has been called.

This function returns TRUE after all the EVT_SIGNAL_VIRTUAL_ADDRESS_CHANGE functions have
executed as a result of the OS calling SetVirtualAddressMap(). Prior to this time FALSE
is returned. This function is used by runtime code to decide it is legal to access services
that go away after SetVirtualAddressMap().

@retval  TRUE  The system has finished executing the EVT_SIGNAL_VIRTUAL_ADDRESS_CHANGE event.
@retval  FALSE The system has not finished executing the EVT_SIGNAL_VIRTUAL_ADDRESS_CHANGE event.

**/
BOOLEAN
EfiGoneVirtual (
  VOID
  );

// gPhysicalRT
extern EFI_RUNTIME_SERVICES *gPhysicalRT;

#endif // ifndef _EFI_RUNTIME_LIB_H_
