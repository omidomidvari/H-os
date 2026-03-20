#include <efi.h>
#include <efilib.h>

/**
 * loader.c
 * This function locates 'kernel.bin' (the compiled version of kernel.c),
 * loads it into memory, and executes it.
 */
EFI_STATUS start_kernel_loader(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    EFI_STATUS Status;
    EFI_LOADED_IMAGE *LoadedImage;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *FileSystem;
    EFI_FILE_PROTOCOL *Root;
    EFI_FILE_PROTOCOL *KernelFile;
    void *KernelBuffer = NULL;
    UINTN FileSize = 0x200000; // Allocate 2MB for the kernel

    // 1. Identify the boot device
    Status = SystemTable->BootServices->HandleProtocol(
        ImageHandle, &LoadedImageProtocol, (void**)&LoadedImage);
    if (EFI_ERROR(Status)) return Status;

    // 2. Access the File System of the boot device
    Status = SystemTable->BootServices->HandleProtocol(
        LoadedImage->DeviceHandle, &FileSystemProtocol, (void**)&FileSystem);
    if (EFI_ERROR(Status)) return Status;

    // 3. Open the Root directory of the FAT32 volume
    Status = FileSystem->OpenVolume(FileSystem, &Root);
    if (EFI_ERROR(Status)) return Status;

    // 4. Open "kernel.bin"
    // Note: This assumes you compiled kernel.c into kernel.bin
    Status = Root->Open(Root, &KernelFile, L"\\kernel.bin", EFI_FILE_MODE_READ, 0);
    if (EFI_ERROR(Status)) {
        Print(L"\n[Loader] Error: kernel.bin not found at drive root.");
        return Status;
    }

    // 5. Allocate memory pool for the kernel code
    Status = SystemTable->BootServices->AllocatePool(EfiLoaderData, FileSize, &KernelBuffer);
    if (EFI_ERROR(Status)) return Status;

    // 6. Read the file into the allocated memory
    Status = KernelFile->Read(KernelFile, &FileSize, KernelBuffer);
    if (EFI_ERROR(Status)) return Status;

    KernelFile->Close(KernelFile);
    Root->Close(Root);

    Print(L"\n[Loader] Kernel loaded. Jumping now...");

    // 7. Transfer Control to the Kernel
    // Cast the memory address to a function pointer and call it
    void (*KernelEntry)() = (void (*)())KernelBuffer;
    KernelEntry();

    // The kernel should never return here.
    return EFI_SUCCESS;
}