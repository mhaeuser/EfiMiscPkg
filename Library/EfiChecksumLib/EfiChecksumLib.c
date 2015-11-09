// 18/07/2015

#include <Uefi.h>

#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>

#include <Library/EfiChecksumLib/EfiChecksumLib.h>

// CalculateCrc32
EFI_STATUS
CalculateCrc32 (
	IN     VOID    *Data,
	IN     UINTN   DataSize,
	   OUT UINT32  *Crc32
	)
{
	EFI_STATUS Status;

  ASSERT (!EfiAtRuntime ());
  ASSERT (Data != NULL);
  ASSERT (DataSize != 0);
  ASSERT (Crc32 != NULL);

	*Crc32 = 0;
	Status = gBS->CalculateCrc32 (Data, DataSize, Crc32);

	ASSERT_EFI_ERROR (Status);

	return Status;
}
