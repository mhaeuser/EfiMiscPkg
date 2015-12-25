/** @file
  Copyright (C) 2015 CupertinoNet.  All rights reserved.<BR>

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

#ifndef EFI_IMAGE_LIB_H_
#define EFI_IMAGE_LIB_H_

// LoadImage
/** Loads an EFI image into memory.
 
  @param[in]  BootPolicy         If TRUE, indicates that the request originates from the boot
                                 manager, and that the boot manager is attempting to load
                                 FilePath as a boot selection.  Ignored if SourceBuffer is
                                 not NULL.
  @param[in]  ParentImageHandle  The caller's image handle.
  @param[in]  DevicePath         The DeviceHandle specific file path from which the image is
                                 loaded.
  @param[in]  SourceBuffer       If not NULL, a pointer to the memory location containing a copy
                                 of the image to be loaded.
  @param[in]  SourceSize         The size in bytes of SourceBuffer.  Ignored if SourceBuffer is NULL.
  @param[out] ImageHandle        The pointer to the returned image handle that is created when the
                                 image is successfully loaded.
 
  @retval EFI_SUCCESS             Image was loaded into memory correctly.
  @retval EFI_NOT_FOUND           Both SourceBuffer and DevicePath are NULL.
  @retval EFI_INVALID_PARAMETER   One or more parametes are invalid.
  @retval EFI_UNSUPPORTED         The image type is not supported.
  @retval EFI_OUT_OF_RESOURCES    Image was not loaded due to insufficient resources.
  @retval EFI_LOAD_ERROR          Image was not loaded because the image format was corrupt or not
                                  understood.
  @retval EFI_DEVICE_ERROR        Image was not loaded because the device returned a read error.
  @retval EFI_ACCESS_DENIED       Image was not loaded because the platform policy prohibits the
                                  image from being loaded.  NULL is returned in *ImageHandle.
  @retval EFI_SECURITY_VIOLATION  Image was loaded and an ImageHandle was created with a
                                  valid EFI_LOADED_IMAGE_PROTOCOL.  However, the current
                                  platform policy specifies that the image should not be started.
**/
EFI_STATUS
LoadImage (
  IN  BOOLEAN                   BootPolicy,
  IN  EFI_HANDLE                ParentImageHandle,
  IN  EFI_DEVICE_PATH_PROTOCOL  *DevicePath,
  IN  VOID                      *SourceBuffer, OPTIONAL
  IN  UINTN                     SourceSize,
  OUT EFI_HANDLE                *ImageHandle
  );

// StartImage
/** Transfers control to a loaded image's entry point.

  @param[in]  ImageHandle   Handle of image to be started.
  @param[out] ExitDataSize  The pointer to the size, in bytes, of ExitData.
  @param[out] ExitData      The pointer to a pointer to a data buffer that includes a Null-terminated
                            string, optionally followed by additional binary data.

  @retval EFI_INVALID_PARAMETER   ImageHandle is either an invalid image handle or the image
                                  has already been initialized with StartImage.
  @retval EFI_SECURITY_VIOLATION  The current platform policy specifies that the image should not be started.
  @return                         Exit code from image.
  **/
EFI_STATUS
StartImage (
  IN  EFI_HANDLE  ImageHandle,
  OUT UINTN       *ExitDataSize,
  OUT CHAR16      **ExitData OPTIONAL
  );

// Exit
/** Terminates a loaded EFI image and returns control to boot services.

  @param[in] ImageHandle   Handle that identifies the image.  This parameter is passed to the
                           image on entry.
  @param[in] ExitStatus    The image's exit code.
  @param[in] ExitDataSize  The size, in bytes, of ExitData.  Ignored if ExitStatus is EFI_SUCCESS.
  @param[in] ExitData      The pointer to a data buffer that includes a Null-terminated string,
                           optionally followed by additional binary data.  The string is a
                           description that the caller may use to further indicate the reason
                           for the image's exit.  ExitData is only valid if ExitStatus
                           is something other than EFI_SUCCESS.  The ExitData buffer
                           must be allocated by calling AllocatePool().

  @retval EFI_SUCCESS            The image specified by ImageHandle was unloaded.
  @retval EFI_INVALID_PARAMETER  The image specified by ImageHandle has been loaded and
                                 started with LoadImage() and StartImage(), but the
                                 image is not the currently executing image.
**/
EFI_STATUS
Exit (
  IN EFI_HANDLE  ImageHandle,
  IN EFI_STATUS  ExitStatus,
  IN UINTN       ExitDataSize,
  IN CHAR16      *ExitData OPTIONAL
  );

// UnloadImage
/** Unloads an image.

  @param[in] ImageHandle  Handle that identifies the image to be unloaded.

  @retval EFI_SUCCESS            The image has been unloaded.
  @retval EFI_INVALID_PARAMETER  ImageHandle is not a valid image handle.
**/
EFI_STATUS
UnloadImage (
  IN EFI_HANDLE  ImageHandle
  );

// ExitBootServices
/** Terminates all boot services.

  @param[in] ImageHandle  Handle that identifies the exiting image.
  @param[in] MapKey       Key to the latest memory map.

  @retval EFI_SUCCESS            Boot services have been terminated.
  @retval EFI_INVALID_PARAMETER  MapKey is incorrect.
**/
EFI_STATUS
ExitBootServices (
  IN EFI_HANDLE  ImageHandle,
  IN UINTN       MapKey
  );

#endif // EFI_IMAGE_LIB_H_
