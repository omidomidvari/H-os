#include <efi.h>
#include <efilib.h>

EFI_STATUS
EFIAPI
efi_main (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    InitializeLib(ImageHandle, SystemTable);
    
    EFI_LOADED_IMAGE *LoadedImage;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *FileSystem;
    EFI_FILE_PROTOCOL *Root;
    EFI_FILE_PROTOCOL *TargetFile;
    EFI_STATUS Status;

    // 1. Clear Screen & Print Message
    uefi_call_wrapper(ST->ConOut->ClearScreen, 1, ST->ConOut);
    uefi_call_wrapper(ST->ConOut->SetCursorPosition, 3, ST->ConOut, 0, 24);
    Print(L"(c-left) 2026 homemovie");

    // 2. Wait 1 second (1,000,000 microseconds)
    uefi_call_wrapper(BS->Stall, 1, 1000000);

    // 3. Get the handle for where this .efi file is located
    Status = uefi_call_wrapper(BS->HandleProtocol, 3, 
                               ImageHandle, &LoadedImageProtocol, (void**)&LoadedImage);
    if (EFI_ERROR(Status)) return Status;

    // 4. Open the File System on that device
    Status = uefi_call_wrapper(BS->HandleProtocol, 3, 
                               LoadedImage->DeviceHandle, &FileSystemProtocol, (void**)&FileSystem);
    if (EFI_ERROR(Status)) return Status;

    Status = uefi_call_wrapper(FileSystem->OpenVolume, 2, FileSystem, &Root);
    if (EFI_ERROR(Status)) return Status;

    // 5. Load "loader.c"
    Status = uefi_call_wrapper(Root->Open, 5, Root, &TargetFile, L"\\loader.c", EFI_FILE_MODE_READ, 0);
    
    if (EFI_ERROR(Status)) {
        Print(L"\nError: Could not find loader.c\n");
        return Status;
    }

    Print(L"\nFile loader.c found. Executing...");

    return EFI_SUCCESS;
}