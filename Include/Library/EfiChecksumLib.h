#ifndef _EFI_CHECKSUM_LIB_H_
#define _EFI_CHECKSUM_LIB_H_

// CALCULATE_EFI_TABLE_HEADER_CRC32
#define CALCULATE_EFI_TABLE_HEADER_CRC32(Header) (CalculateCrc32 ((VOID *)&(Header), (UINTN)(Header).HeaderSize, &(Header).CRC32))

// CalculateCrc32
EFI_STATUS
CalculateCrc32 (
	IN     VOID    *Data,
	IN     UINTN   DataSize,
	   OUT UINT32  *Crc32
	);

#endif // ifndef _EFI_CHECKSUM_LIB_H_
