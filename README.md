# H os
*A simple, well-documented operating system designed for educational purposes.*

## 📖 Overview
**H-OS** is an independent operating system project built to provide a clear, readable example of how low-level systems function. The project is designed with a "learning first" mentality, featuring segmented code and an accompanying book to explain the architecture.

## ✨ Key Features
*   **Custom Bootstrapping**: Utilizes the **Limine** bootloader for a modern, 64-bit entry point.
*   **Hardware Discovery**: Built-in support for **PCI** and **ACPI** enumeration.
*   **Memory Management**: Implements GDT initialization, paging structures, and a heap allocator.
*   **Unique Filesystem**: Features its own filesystem design with custom core utilities.
*   **build it yourself** compile it all youreself!

## 🛠️ Build Requirements
To build H-OS, you will need a Linux environment (Arch Linux or Ubuntu recommended) with the following tools:

*   **Shell**: [Fish shell](https://fishshell.com) (required for build scripts)
*   **Assembler**: [NASM](https://nasm.us)
*   **Compiler**: `x86_64-elf-gcc` (Cross-compiler)
*   **Bootloader**: [Limine](https://limine-bootloader.org)
*   **Image Tools**: `parted`, `mtools`, and `xorriso`
*   **Emulator**: [QEMU](https://www.qemu.org) or also a literal pc

## 🚀 Getting Started

### 1. Installation (Ubuntu)
```bash
sudo apt update && sudo apt install build-essential fish parted mtools nasm xorriso
