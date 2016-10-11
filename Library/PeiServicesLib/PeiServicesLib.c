/** @file
  Copyright (C) 2016, CupertinoNet.  All rights reserved.<BR>

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

#include <PiPei.h>

#include <Library/DebugLib.h>
#include <Library/PeiServicesTablePointerLib.h>

// PPI Functions

// PeiInstallPpi
/** This service is the first one provided by the PEI Foundation.  This
    function installs an interface in the PEI PPI database by GUID.  The
    purpose of the service is to publish an interface that other parties can
    use to call additional PEIMs.

  @param[in] PpiList  A pointer to the list of interfaces that the caller shall
                      install.

  @retval EFI_SUCCESS            The interface was successfully installed.
  @retval EFI_INVALID_PARAMETER  The PpiList pointer is NULL or Any of the PEI
                                 PPI descriptors in the list do not have the
                                 EFI_PEI_PPI_DESCRIPTOR_PPI bit set in the
                                 Flags field.
  @retval EFI_OUT_OF_RESOURCES   There is no additional space in the PPI
                                 database.
**/
EFI_STATUS
PeiInstallPpi (
  IN CONST EFI_PEI_PPI_DESCRIPTOR  *PpiList
  )
{
  EFI_STATUS             Status;

  UINTN                  Index;
  CONST EFI_PEI_SERVICES **PeiServices;

  ASSERT (PpiList != NULL);

  for (Index = 0;
        (PpiList[Index].Flags & EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST) == 0;
          ++Index) {
    ASSERT ((PpiList[Index].Flags & EFI_PEI_PPI_DESCRIPTOR_PPI) != 0);
  }

  PeiServices = GetPeiServicesTablePointer ();
  Status      = (*PeiServices)->InstallPpi (PeiServices, PpiList);

  ASSERT_EFI_ERROR (Status);

  return Status;
}

// PeiReInstallPpi
/** This function reinstalls an interface in the PEI PPI database by GUID.  The
    purpose of the service is to publish an interface that other parties can
    use to replace a same-named interface in the protocol database with a
    different interface.

  @param[in] OldPpi  A pointer to the former PPI in the database.
  @param[in] NewPpi  A pointer to the new interfaces that the caller shall
                     install.

  @retval EFI_SUCCESS            The interface was successfully installed.
  @retval EFI_INVALID_PARAMETER  The PpiList pointer is NULL or Any of the PEI
                                 PPI descriptors in the list do not have the
                                 EFI_PEI_PPI_DESCRIPTOR_PPI bit set in the
                                 Flags field.
  @retval EFI_OUT_OF_RESOURCES   There is no additional space in the PPI
                                 database.
  @retval EFI_NOT_FOUND          The PPI for which the reinstallation was
                                 requested has not been installed.
**/
EFI_STATUS
PeiReInstallPpi (
  IN CONST EFI_PEI_PPI_DESCRIPTOR  *OldPpi,
  IN CONST EFI_PEI_PPI_DESCRIPTOR  *NewPpi
  )
{
  EFI_STATUS             Status;

  UINTN                  Index;
  UINTN                  Index2;
  CONST EFI_PEI_SERVICES **PeiServices;

  ASSERT (OldPpi != NULL);

  for (Index = 0;
        (OldPpi[Index].Flags & EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST) == 0;
          ++Index) {
  ASSERT ((OldPpi[Index].Flags & EFI_PEI_PPI_DESCRIPTOR_PPI) != 0);
    }

  for (Index2 = 0;
        (OldPpi[Index2].Flags & EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST) == 0;
          ++Index2) {
    ASSERT ((OldPpi[Index2].Flags & EFI_PEI_PPI_DESCRIPTOR_PPI) != 0);
  }

  ASSERT (Index == Index2);

  PeiServices = GetPeiServicesTablePointer ();
  Status      = (*PeiServices)->ReInstallPpi (PeiServices, OldPpi, NewPpi);

  ASSERT_EFI_ERROR (Status);

  return Status;
}

// PeiLocatePpi
/** This function locates an interface in the PEI PPI database by GUID. 

  @param[in]      Guid           A pointer to the GUID whose corresponding
                                 interface needs to be found.
  @param[in]      Instance       The N-th instance of the interface that is
                                 required.
  @param[in, out] PpiDescriptor  A pointer to instance of the
                                 EFI_PEI_PPI_DESCRIPTOR.
  @param[in, out] Ppi            A pointer to the instance of the interface.

  @retval EFI_SUCCESS    The interface was successfully returned.
  @retval EFI_NOT_FOUND  The PPI descriptor is not found in the database.
**/
EFI_STATUS
PeiLocatePpi (
  IN     CONST EFI_GUID          *Guid,
  IN     UINTN                   Instance,
  IN OUT EFI_PEI_PPI_DESCRIPTOR  **PpiDescriptor, OPTIONAL
  IN OUT VOID                    **Ppi
  )
{
  EFI_STATUS             Status;

  CONST EFI_PEI_SERVICES **PeiServices;

  ASSERT (Guid != NULL);
  ASSERT (Ppi != NULL);

  PeiServices = GetPeiServicesTablePointer ();
  Status      = (*PeiServices)->LocatePpi (
                                  PeiServices,
                                  Guid,
                                  Instance,
                                  PpiDescriptor,
                                  Ppi
                                  );

  if (Status != EFI_NOT_FOUND) {
    ASSERT_EFI_ERROR (Status);
  }

  return Status;
}

// PeiNotifyPpi
/** This function installs a notification service to be called back when a
    given interface is installed or reinstalled.  The purpose of the service
    is to publish an interface that other parties can use to call additional
    PPIs that may materialize later.

  @param[in] NotifyList   A pointer to the list of notification interfaces that
                          the caller shall install.

  @retval EFI_SUCCESS            The interface was successfully installed.
  @retval EFI_INVALID_PARAMETER  The PpiList pointer is NULL, or any of the PEI
                                 PPI descriptors in the list do not have the
                                 EFI_PEI_PPI_DESCRIPTOR_PPI bit set in the
                                 Flags field.
  @retval EFI_OUT_OF_RESOURCES   There is no additional space in the PPI
                                 database.
**/
EFI_STATUS
PeiNotifyPpi (
  IN CONST EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyList
  )
{
  EFI_STATUS             Status;

  UINTN                  Index;
  CONST EFI_PEI_SERVICES **PeiServices;

  ASSERT (NotifyList != NULL);

  for (Index = 0;
        (NotifyList[Index].Flags & EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST) == 0;
          ++Index) {
    ASSERT ((NotifyList[Index].Flags & EFI_PEI_PPI_DESCRIPTOR_PPI) != 0);
  }

  PeiServices = GetPeiServicesTablePointer ();
  Status      = (*PeiServices)->NotifyPpi (PeiServices, NotifyList);

  ASSERT_EFI_ERROR (Status);

  return Status;
}

// Boot Mode Functions

// PeiGetBootMode
/** This function returns the present value of the boot mode.

  @param[out] BootMode     A pointer to contain the value of the boot mode.

  @retval EFI_SUCCESS  The boot mode returned successfully.
**/
EFI_STATUS
PeiGetBootMode (
  OUT EFI_BOOT_MODE  *BootMode
  )
{
  EFI_STATUS             Status;

  CONST EFI_PEI_SERVICES **PeiServices;

  ASSERT (BootMode != NULL);

  PeiServices = GetPeiServicesTablePointer ();
  Status      = (*PeiServices)->GetBootMode (PeiServices, BootMode);

  ASSERT_EFI_ERROR (Status);

  return Status;
}

// PeiSetBootMode
/** This function sets the value of the boot mode.

  @param[in] BootMode     The value of the boot mode to set.

  @retval EFI_SUCCESS  The boot mode returned successfully.
**/
EFI_STATUS
PeiSetBootMode (
  IN EFI_BOOT_MODE  BootMode
  )
{
  EFI_STATUS             Status;

  CONST EFI_PEI_SERVICES **PeiServices;

  PeiServices = GetPeiServicesTablePointer ();
  Status      = (*PeiServices)->SetBootMode (PeiServices, BootMode);

  ASSERT_EFI_ERROR (Status);

  return Status;
}

// HOB Functions

// PeiGetHobList
/** This function returns the pointer to the list of Hand-Off Blocks (HOBs) in
    memory.

  @param[out] HobList      A pointer to the list of HOBs that the PEI
                           Foundation will initialize

  @retval EFI_SUCCESS            The list was successfully returned.
  @retval EFI_NOT_AVAILABLE_YET  The HOB list is not yet published.
**/
EFI_STATUS
PeiGetHobList (
  OUT VOID  **HobList
  )
{
  EFI_STATUS             Status;

  CONST EFI_PEI_SERVICES **PeiServices;

  ASSERT (HobList != NULL);

  PeiServices = GetPeiServicesTablePointer ();
  Status      = (*PeiServices)->GetHobList (PeiServices, HobList);

  return Status;
}

// PeiCreateHob
/** This service, published by the PEI Foundation, abstracts the creation of a
    Hand-Off Block's (HOB's) headers.

  @param[in]      Type    The type of HOB to be installed.
  @param[in]      Length  The length of the HOB to be added.
  @param[in, out] Hob     The address of a pointer that will contain the HOB
                          header.

  @retval EFI_SUCCESS           The HOB was successfully created.
  @retval EFI_OUT_OF_RESOURCES  There is no additional space for HOB creation.
**/
EFI_STATUS
PeiCreateHob (
  IN     UINT16  Type,
  IN     UINT16  Length,
  IN OUT VOID    **Hob
  )
{
  EFI_STATUS             Status;

  CONST EFI_PEI_SERVICES **PeiServices;

  ASSERT (Length > 0);
  ASSERT (Hob != NULL);

  PeiServices = GetPeiServicesTablePointer ();
  Status      = (*PeiServices)->CreateHob (PeiServices, Type, Length, Hob);

  ASSERT_EFI_ERROR (Status);

  return Status;
}

// Firmware Volume Functions

// FfsFindNextVolume
/** The purpose of the service is to abstract the capability of the PEI
    Foundation to discover instances of firmware volumes in the system.

  This service enables PEIMs to discover additional firmware volumes.  The PEI
  Foundation uses this service to abstract the locations and formats of various
  firmware volumes.  These volumes include the Boot Firmware Volume and any
  other volumes exposed by EFI_PEI_FV_PPI.  The service returns a volume handle
  of type EFI_PEI_FV_HANDLE, which must be unique within the system.

  @param[in]  Instance      This instance of the firmware volume to find.
                            The value 0 is the Boot Firmware Volume (BFV).
  @param[out] VolumeHandle  On exit, points to the next volumn handle or NULL
                            if it does not exist.

  @retval EFI_SUCCESS            The volume was found.
  @retval EFI_NOT_FOUND          The volume was not found.
  @retval EFI_INVALID_PARAMETER  VolumeHandle is NULL.
**/
EFI_STATUS
FfsFindNextVolume (
  IN  UINTN              Instance,
  OUT EFI_PEI_FV_HANDLE  *VolumeHandle
  )
{
  EFI_STATUS             Status;

  CONST EFI_PEI_SERVICES **PeiServices;

  ASSERT (VolumeHandle != NULL);

  PeiServices = GetPeiServicesTablePointer ();
  Status      = (*PeiServices)->FfsFindNextVolume (
                                  PeiServices,
                                  Instance,
                                  VolumeHandle
                                  );

  if (Status != EFI_NOT_FOUND) {
    ASSERT_EFI_ERROR (Status);
  }

  return Status;
}

// PeiFfsFindNextFile
/** Searches for the next matching file in the firmware volume.

  This service enables PEIMs to discover firmware files within a specified
  volume.  To find the first instance of a firmware file, pass a FileHandle
  value of NULL into the service.  The service returns a file handle of type
  EFI_PEI_FILE_HANDLE, which must be unique within the system.

  @param[in]     SearchType  A filter to find files only of this type.
  @param[in]     FvHandle    Handle of firmware volume in which to search.
  @param[in,out] FileHandle  On entry, points to the current handle from which
                             to begin searching or NULL to start at the
                             beginning of the firmware volume.  On exit,
                             points the file handle of the next file in the
                             volume or NULL if there are no more files.

  @retval EFI_SUCCESS    The file was found.
  @retval EFI_NOT_FOUND  The file was not found.
  @retval EFI_NOT_FOUND  The header checksum was not zero.
**/
EFI_STATUS
PeiFfsFindNextFile (
  IN     EFI_FV_FILETYPE          SearchType,
  IN     CONST EFI_PEI_FV_HANDLE  FvHandle,
  IN OUT EFI_PEI_FILE_HANDLE      *FileHandle
  )
{
  EFI_STATUS             Status;

  CONST EFI_PEI_SERVICES **PeiServices;

  ASSERT (FvHandle != NULL);
  ASSERT (FileHandle != NULL);

  PeiServices = GetPeiServicesTablePointer ();
  Status      = (*PeiServices)->FfsFindNextFile (
                                  PeiServices,
                                  SearchType,
                                  FvHandle,
                                  FileHandle
                                  );

  ASSERT_EFI_ERROR (Status);

  return Status;
}

// PeiFfsFindSectionData
/** Searches for the next matching section within the specified file.

  This service enables PEI modules to discover the first section of a given
  type within a valid file.  This service will search within encapsulation
  sections (compression and GUIDed) as well.  It will search inside of a GUIDed
  section or a compressed section, but may not, for example, search a GUIDed
  section inside a GUIDes section.  This service will not search within
  compression sections or GUIDed sections that require extraction if memory is
  not present.

  @param[in]  SectionType  The value of the section type to find.
  @param[in]  FileHandle   Handle of the firmware file to search.
  @param[out] SectionData  A pointer to the discovered section, if successful.

  @retval EFI_SUCCESS    The section was found.
  @retval EFI_NOT_FOUND  The section was not found.
**/
EFI_STATUS
PeiFfsFindSectionData (
  IN  EFI_SECTION_TYPE     SectionType,
  IN  EFI_PEI_FILE_HANDLE  FileHandle,
  OUT VOID                 **SectionData
  )
{
  EFI_STATUS             Status;

  CONST EFI_PEI_SERVICES **PeiServices;

  ASSERT (FileHandle != NULL);
  ASSERT (SectionData != NULL);

  PeiServices = GetPeiServicesTablePointer ();
  Status      = (*PeiServices)->FfsFindSectionData (
                                  PeiServices,
                                  SectionType,
                                  FileHandle,
                                  SectionData
                                  );

  ASSERT_EFI_ERROR (Status);

  return Status;
}

// PEI Memory Functions

/** This function registers the found memory configuration with the PEI
    Foundation.

  @param[in] MemoryBegin   The value of a region of installed memory.
  @param[in] MemoryLength  The corresponding length of a region of installed
                           memory.

  @retval EFI_SUCCESS            The region was successfully installed in a
                                 HOB.
  @retval EFI_INVALID_PARAMETER  MemoryBegin and MemoryLength are illegal for
                                 this system.
  @retval EFI_OUT_OF_RESOURCES   There is no additional space for HOB creation.
**/
EFI_STATUS
PeiInstallPeiMemory (
  IN EFI_PHYSICAL_ADDRESS  MemoryBegin,
  IN UINT64                MemoryLength
  )
{
  EFI_STATUS             Status;

  CONST EFI_PEI_SERVICES **PeiServices;

  ASSERT (MemoryBegin > 0);
  ASSERT (MemoryLength > 0);
  ASSERT ((MemoryLength - 1) <= (MAX_ADDRESS - MemoryBegin));

  PeiServices = GetPeiServicesTablePointer ();
  Status      = (*PeiServices)->InstallPeiMemory (
                                  PeiServices,
                                  MemoryBegin,
                                  MemoryLength
                                  );

  ASSERT_EFI_ERROR (Status);

  return Status;
}

// PeiAllocatePages
/** The purpose of the service is to publish an interface that allows PEIMs to
    allocate memory ranges that are managed by the PEI Foundation.

  @param[in]  MemoryType   The type of memory to allocate.
  @param[in]  Pages        The number of contiguous 4 KB pages to allocate.
  @param[out] Memory       A pointer to a physical address.  On output, the
                           address is set to the base of the page range that
                           was allocated.

  @retval EFI_SUCCESS            The memory range was successfully allocated.
  @retval EFI_OUT_OF_RESOURCES   The pages could not be allocated.
  @retval EFI_INVALID_PARAMETER  The type is not equal to EfiLoaderCode,
                                 EfiLoaderData, EfiRuntimeServicesCode,
                                 EfiRuntimeServicesData, EfiBootServicesCode,
                                 EfiBootServicesData, EfiACPIReclaimMemory,
                                 EfiReservedMemoryType, or EfiACPIMemoryNVS.
**/
EFI_STATUS
PeiAllocatePages (
  IN  EFI_MEMORY_TYPE       MemoryType,
  IN  UINTN                 Pages,
  OUT EFI_PHYSICAL_ADDRESS  *Memory
  )
{
  EFI_STATUS             Status;

  CONST EFI_PEI_SERVICES **PeiServices;

  ASSERT ((MemoryType == EfiLoaderCode)
       || (MemoryType == EfiLoaderData)
       || (MemoryType == EfiRuntimeServicesCode)
       || (MemoryType == EfiRuntimeServicesData)
       || (MemoryType == EfiBootServicesCode)
       || (MemoryType == EfiBootServicesData)
       || (MemoryType == EfiACPIReclaimMemory)
       || (MemoryType == EfiReservedMemoryType)
       || (MemoryType == EfiACPIMemoryNVS));

  ASSERT (Pages > 0);
  ASSERT (Memory != NULL);

  PeiServices = GetPeiServicesTablePointer ();
  Status      = (*PeiServices)->AllocatePages (
                                  PeiServices,
                                  MemoryType,
                                  Pages,
                                  Memory
                                  );

  ASSERT_EFI_ERROR (Status);

  return Status;
}

// PeiAllocatePool
/** The purpose of this service is to publish an interface that allows PEIMs to
    allocate memory ranges that are managed by the PEI Foundation.

  @param[in]  Size         The number of bytes to allocate from the pool.
  @param[out] Buffer       If the call succeeds, a pointer to a pointer to the
                           allocated buffer; undefined otherwise.

  @retval EFI_SUCCESS           The allocation was successful.
  @retval EFI_OUT_OF_RESOURCES  There is not enough heap to allocate the
                                requested size.
**/
EFI_STATUS
PeiAllocatePool (
  IN  UINTN  Size,
  OUT VOID   **Buffer
  )
{
  EFI_STATUS             Status;

  CONST EFI_PEI_SERVICES **PeiServices;

  ASSERT (Size > 0);
  ASSERT (Buffer != NULL);

  PeiServices = GetPeiServicesTablePointer ();
  Status      = (*PeiServices)->AllocatePool (PeiServices, Size, Buffer);

  ASSERT_EFI_ERROR (Status);

  return Status;
}

// PeiCopyMem
/** This service copies the contents of one buffer to another buffer.

  @param[in] Destination  The pointer to the destination buffer of the memory
                          copy.
  @param[in] Source       The pointer to the source buffer of the memory copy.
  @param[in] Length       The number of bytes to copy from Source to
                          Destination.
**/
VOID
PeiCopyMem (
  IN VOID   *Destination,
  IN VOID   *Source,
  IN UINTN  Length
  )
{
  CONST EFI_PEI_SERVICES **PeiServices;

  ASSERT (Destination != NULL);
  ASSERT (Source != NULL);
  ASSERT (Length > 0);

  PeiServices = GetPeiServicesTablePointer ();

  (*PeiServices)->CopyMem (Destination, Source, Length);
}

// PeiSetMem
/** The service fills a buffer with a specified value.

  @param[in] Buffer  The pointer to the buffer to fill.
  @param[in] Size    The number of bytes in Buffer to fill.
  @param[in] Value   The value to fill Buffer with.
**/
VOID
PeiSetMem (
  IN VOID   *Buffer,
  IN UINTN  Size,
  IN UINT8  Value
  )
{
  CONST EFI_PEI_SERVICES **PeiServices;

  ASSERT (Buffer != NULL);
  ASSERT (Size > 0);
  ASSERT ((Size - 1) <= (MAX_ADDRESS - (UINTN)Buffer));

  PeiServices = GetPeiServicesTablePointer ();
  
  (*PeiServices)->SetMem (Buffer, Size, Value);
}

// Status Code

/** This service publishes an interface that allows PEIMs to report status
    codes.

  ReportStatusCode() is called by PEIMs that wish to report status information
  on their progress.  The principal use model is for a PEIM to emit one of the
  standard 32-bit error codes.  This will allow a platform owner to ascertain
  the state of the system, especially under conditions where the full consoles
  might not have been installed.

  @param[in] Type         Indicates the type of status code being reported.
  @param[in] Value        Describes the current status of a hardware or
                          software entity. This includes information about the
                          class and subclass that is used to classify the
                          entity as well as an operation.  For progress codes,
                          the operation is the current activity.  For error
                          codes, it is the exception.  For debug codes, it is
                          not defined at this time.
  @param[in] Instance     The enumeration of a hardware or software entity
                          within the system. A system may contain multiple
                          entities that match a class/subclass
                          pairing. The instance differentiates between them.
                          An instance of 0 indicates that instance information
                          is unavailable, not meaningful, or not relevant.
                          Valid instance numbers start with 1.
  @param[in] CallerId     This optional parameter may be used to identify the
                          caller.
                          This parameter allows the status code driver to apply
                          different rules to different callers.
  @param[in] Data         This optional parameter may be used to pass
                          additional data.

  @retval EFI_SUCCESS            The function completed successfully.
  @retval EFI_NOT_AVAILABLE_YET  No progress code provider has installed an
                                 interface in the system.
**/
EFI_STATUS
PeiReportStatusCode (
  IN EFI_STATUS_CODE_TYPE        Type,
  IN EFI_STATUS_CODE_VALUE       Value,
  IN UINT32                      Instance,
  IN CONST EFI_GUID              *CallerId, OPTIONAL
  IN CONST EFI_STATUS_CODE_DATA  *Data OPTIONAL
  )
{
  EFI_STATUS             Status;

  CONST EFI_PEI_SERVICES **PeiServices;

  PeiServices = GetPeiServicesTablePointer ();
  Status      = (*PeiServices)->ReportStatusCode (
                                  PeiServices,
                                  Type,
                                  Value,
                                  Instance,
                                  CallerId,
                                  Data
                                  );

  return Status;
}

// Reset

// PeiResetSystem
/** Resets the entire platform.

  This service resets the entire platform, including all processors and
  devices, and reboots the system.  This service will never return
  EFI_SUCCESS.

  @retval EFI_NOT_AVAILABLE_YET  The service has not been installed yet.
**/
EFI_STATUS
PeiResetSystem (
  VOID
  )
{
  CONST EFI_PEI_SERVICES **PeiServices;

  PeiServices = GetPeiServicesTablePointer ();
  
  return(*PeiServices)->ResetSystem (PeiServices);
}

// Future Installed Services

// FfsFindFileByName
/** Find a file within a volume by its name.

  This service searches for files with a specific name, within
  either the specified firmware volume or all firmware volumes.
  The service returns a file handle of type EFI_PEI_FILE_HANDLE,
  which must be unique within the system.

  @param FileName       A pointer to the name of the file to
                        find within the firmware volume.
  @param VolumeHandle   The firmware volume to search.
  @param FileHandle     Upon exit, points to the found file's
                        handle or NULL if it could not be found.

  @retval EFI_SUCCESS             The file was found.
  @retval EFI_NOT_FOUND           The file was not found.
  @retval EFI_INVALID_PARAMETER   VolumeHandle or FileHandle or
                                  FileName was NULL.
**/
EFI_STATUS
FfsFindFileByName (
  IN  CONST  EFI_GUID      *FileName,
  IN  EFI_PEI_FV_HANDLE    VolumeHandle,
  OUT EFI_PEI_FILE_HANDLE  *FileHandle
  )
{
  EFI_STATUS             Status;

  CONST EFI_PEI_SERVICES **PeiServices;

  ASSERT (FileName != NULL);
  ASSERT (VolumeHandle != NULL);
  ASSERT (FileHandle != NULL);

  PeiServices = GetPeiServicesTablePointer ();
  Status      = (*PeiServices)->FfsFindFileByName (
                                  FileName,
                                  VolumeHandle,
                                  FileHandle
                                  );

  if (Status != EFI_NOT_FOUND) {
    ASSERT_EFI_ERROR (Status);
  }

  return Status;
}

// PeiFfsGetFileInfo
/** Returns information about a specific file.

  This function returns information about a specific file, including its file
  name, type, attributes, starting address and size.  If the firmware volume is
  not memory mapped, then the Buffer member will be NULL.

  @param[in]  FileHandle  The handle of the file.
  @param[out] FileInfo    Upon exit, points to the file's information.

  @retval EFI_SUCCESS            File information was returned.
  @retval EFI_INVALID_PARAMETER  FileHandle does not represent a valid file.
  @retval EFI_INVALID_PARAMETER  FileInfo is NULL.
**/
EFI_STATUS
PeiFfsGetFileInfo (
  IN  EFI_PEI_FILE_HANDLE  FileHandle,
  OUT EFI_FV_FILE_INFO     *FileInfo
  )
{
  EFI_STATUS             Status;

  CONST EFI_PEI_SERVICES **PeiServices;

  ASSERT (FileHandle != NULL);
  ASSERT (FileInfo != NULL);

  PeiServices = GetPeiServicesTablePointer ();
  Status      = (*PeiServices)->FfsGetFileInfo (FileHandle, FileInfo);

  ASSERT_EFI_ERROR (Status);

  return Status;
}

// PeiFfsGetVolumeInfo
/** Returns information about the specified volume.

  This function returns information about a specific firmware volume, including
  its name, type, attributes, starting address and size.

  @param[in]  VolumeHandle  Handle of the volume.
  @param[out] VolumeInfo    Upon exit, points to the volume's information.

  @retval EFI_SUCCESS            The volume information returned.
  @retval EFI_INVALID_PARAMETER  If VolumeHandle does not represent a valid
                                 volume.
  @retval EFI_INVALID_PARAMETER  If VolumeHandle is NULL.
  @retval EFI_INVALID_PARAMETER  The volume designated by the VolumeHandle is
                                 not available.
**/
EFI_STATUS
PeiFfsGetVolumeInfo (
  IN  EFI_PEI_FV_HANDLE  VolumeHandle,
  OUT EFI_FV_INFO        *VolumeInfo
  )
{
  EFI_STATUS             Status;

  CONST EFI_PEI_SERVICES **PeiServices;

  ASSERT (VolumeHandle != NULL);
  ASSERT (VolumeInfo != NULL);

  PeiServices = GetPeiServicesTablePointer ();
  Status      = (*PeiServices)->FfsGetVolumeInfo (VolumeHandle, VolumeInfo);

  ASSERT_EFI_ERROR (Status);

  return Status;
}

// PeiRegisterForShadow
/** Register a PEIM so that it will be shadowed and called again.

  This service registers a file handle so that after memory is available, the
  PEIM will be re-loaded into permanent memory and re-initialized.  The PEIM
  registered this way will always be initialized twice.  The first time, this
  function call will return EFI_SUCCESS.  The second time, this function call
  will return EFI_ALREADY_STARTED.  Depending on the order in which PEIMs are
  dispatched, the PEIM making this call may be initialized after permanent
  memory is installed, even the first time.

  @param[in] FileHandle  PEIM's file handle. Must be the currently executing
                         PEIM.
  
  @retval EFI_SUCCESS          The PEIM was successfully registered for
                               shadowing.
  @retval EFI_ALREADY_STARTED  The PEIM was previously registered for
                               shadowing.
  @retval EFI_NOT_FOUND        The FileHandle does not refer to a valid file
                               handle.
**/
EFI_STATUS
PeiRegisterForShadow (
  IN EFI_PEI_FILE_HANDLE  FileHandle
  )
{
  EFI_STATUS             Status;

  CONST EFI_PEI_SERVICES **PeiServices;

  ASSERT (FileHandle != NULL);

  PeiServices = GetPeiServicesTablePointer ();
  Status      = (*PeiServices)->RegisterForShadow (FileHandle);

  ASSERT_EFI_ERROR (Status);

  return Status;
}

// PeiFfsFindSectionData3
/** Searches for the next matching section within the specified file.

  This service enables PEI modules to discover the section of a given type
  within a valid file.  This service will search within encapsulation sections
  (compression and GUIDed) as well.  It will search inside of a GUIDed section
  or a compressed section, but may not, for example, search a GUIDed section
  inside a GUIDes section.  This service will not search within compression
  sections or GUIDed sections that require extraction if memory is not present.

  @param[in]  SectionType           The value of the section type to find.
  @param[in]  SectionInstance       Section instance to find.
  @param[in]  FileHandle            Handle of the firmware file to search.
  @param[out] SectionData           A pointer to the discovered section, if
                                    successful.
  @param[out] AuthenticationStatus  A pointer to the authentication status for
                                    this section.

  @retval EFI_SUCCESS      The section was found.
  @retval EFI_NOT_FOUND    The section was not found.
**/
EFI_STATUS
PeiFfsFindSectionData3 (
  IN  EFI_SECTION_TYPE     SectionType,
  IN  UINTN                SectionInstance,
  IN  EFI_PEI_FILE_HANDLE  FileHandle,
  OUT VOID                 **SectionData,
  OUT UINT32               *AuthenticationStatus
  )
{
  EFI_STATUS             Status;

  CONST EFI_PEI_SERVICES **PeiServices;

  ASSERT (FileHandle != NULL);
  ASSERT (SectionData != NULL);
  ASSERT (AuthenticationStatus != NULL);

  PeiServices = GetPeiServicesTablePointer ();
  Status      = (*PeiServices)->FindSectionData3 (
                                  PeiServices,
                                  SectionType,
                                  SectionInstance,
                                  FileHandle,
                                  SectionData,
                                  AuthenticationStatus
                                  );

  return Status;
}

// PeiFfsGetFileInfo2
/** Returns information about a specific file.

  This function returns information about a specific file, including its file
  name, type, attributes, starting address, size and authentication status.  If
  the firmware volume is not memory mapped, then the Buffer member will be
  NULL.

  @param[in]  FileHandle  The handle of the file.
  @param[out] FileInfo    Upon exit, points to the file's information.

  @retval EFI_SUCCESS            File information was returned.
  @retval EFI_INVALID_PARAMETER  FileHandle does not represent a valid file.
  @retval EFI_INVALID_PARAMETER  FileInfo is NULL.
**/
EFI_STATUS
PeiFfsGetFileInfo2 (
  IN  EFI_PEI_FILE_HANDLE  FileHandle,
  OUT EFI_FV_FILE_INFO2    *FileInfo
  )
{
  EFI_STATUS             Status;

  CONST EFI_PEI_SERVICES **PeiServices;

  ASSERT (FileHandle != NULL);
  ASSERT (FileInfo != NULL);

  PeiServices = GetPeiServicesTablePointer ();
  Status      = (*PeiServices)->FfsGetFileInfo2 (FileHandle, FileInfo);

  ASSERT_EFI_ERROR (Status);

  return Status;
}

// ResetSystem2
/** Resets the entire platform.

  @param[in] ResetType    The type of reset to perform.
  @param[in] ResetStatus  The status code for the reset.
  @param[in] DataSize     The size, in bytes, of WatchdogData.
  @param[in] ResetData    For a ResetType of EfiResetCold, EfiResetWarm, or
                          EfiResetShutdown the data buffer starts with a
                          Null-terminated string, optionally followed by
                          additional binary data. The string is a description
                          that the caller may use to further indicate the
                          reason for the system reset.  ResetData is only valid
                          if ResetStatus is something other than EFI_SUCCESS
                          unless the ResetType is EfiResetPlatformSpecific
                          where a minimum amount of ResetData is always
                          required.
**/
VOID
PeiResetSystem2 (
  IN EFI_RESET_TYPE  ResetType,
  IN EFI_STATUS      ResetStatus,
  IN UINTN           DataSize,
  IN VOID            *ResetData OPTIONAL
  )
{
  CONST EFI_PEI_SERVICES **PeiServices;

  ASSERT ((ResetType >= EfiResetCold)
       && (ResetType <= EfiResetPlatformSpecific));
  ASSERT (((((ResetType == EfiResetPlatformSpecific)
          || (ResetStatus != EFI_SUCCESS)) ? 1 : 0)
            ^ ((ResetData == NULL) ? 1 : 0)) != 0);

  ASSERT ((((DataSize > 0) ? 1 : 0) ^ ((ResetData == NULL) ? 1 : 0)) != 0);

  PeiServices = GetPeiServicesTablePointer ();
  
  (*PeiServices)->ResetSystem2 (ResetType, ResetStatus, DataSize, ResetData);
}
