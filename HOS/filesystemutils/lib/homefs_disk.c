// homefs_disk.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DISK_SIZE 1024 // Size of the disk in blocks
#define BLOCK_SIZE 64   // Block size in bytes

// Structure to represent a disk block
typedef struct {
    char data[BLOCK_SIZE];
    int allocated;
} Block;

// Simulated disk using an array of blocks
Block disk[DISK_SIZE];

// Function to initialize the disk
void initialize_disk() {
    for (int i = 0; i < DISK_SIZE; i++) {
        disk[i].allocated = 0;
    }
}

// Function to read a block from the disk
int block_read(int block_num, char *buffer) {
    if (block_num < 0 || block_num >= DISK_SIZE || !disk[block_num].allocated) {
        return -1; // Error: invalid block
    }
    memcpy(buffer, disk[block_num].data, BLOCK_SIZE);
    return 0; // Success
}

// Function to write a block to the disk
int block_write(int block_num, const char *data) {
    if (block_num < 0 || block_num >= DISK_SIZE) {
        return -1; // Error: invalid block
    }
    memcpy(disk[block_num].data, data, BLOCK_SIZE);
    disk[block_num].allocated = 1;
    return 0; // Success
}

// Function to allocate a block
int block_alloc() {
    for (int i = 0; i < DISK_SIZE; i++) {
        if (!disk[i].allocated) {
            disk[i].allocated = 1;
            return i; // Return the index of allocated block
        }
    }
    return -1; // Error: no free blocks
}

// Function to deallocate a block
int block_dealloc(int block_num) {
    if (block_num < 0 || block_num >= DISK_SIZE) {
        return -1; // Error: invalid block
    }
    disk[block_num].allocated = 0;
    return 0; // Success
}

// Function to flush metadata (just an example implementation)
void flush_metadata() {
    // In a real implementation, write metadata to a persistent store
    printf("Flushing metadata...\n");
}

// Main function for demonstration
int main() {
    initialize_disk(); // Initialize the disk
    // Example usage
    int block_num = block_alloc(); // Allocate a block
    char data[BLOCK_SIZE] = "Hello, World!";
    block_write(block_num, data); // Write data to the block
    char buffer[BLOCK_SIZE];
    block_read(block_num, buffer); // Read data back
    printf("Read from block %d: %s\n", block_num, buffer);
    flush_metadata(); // Flush the metadata
    block_dealloc(block_num); // Deallocate the block
    return 0;
}