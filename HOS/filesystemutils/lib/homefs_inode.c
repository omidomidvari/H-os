// homefs_inode.c

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

typedef struct Inode {
    int id;
    time_t creation_time;
    time_t modification_time;
    // Other inode attributes can be added here
} Inode;

// Function to allocate an inode
Inode* allocate_inode(int id) {
    Inode* new_inode = (Inode*)malloc(sizeof(Inode));
    if (!new_inode) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }
    new_inode->id = id;
    new_inode->creation_time = time(NULL);
    new_inode->modification_time = new_inode->creation_time;
    return new_inode;
}

// Function to deallocate an inode
void deallocate_inode(Inode* inode) {
    if (inode) {
        free(inode);
    }
}

// Function to update the modification time of an inode
void update_modification_time(Inode* inode) {
    if (inode) {
        inode->modification_time = time(NULL);
    }
}

// Example usage of the functions
int main() {
    Inode* inode1 = allocate_inode(1);
    if (inode1) {
        printf("Allocated inode with id: %d\n", inode1->id);
        printf("Creation time: %s", ctime(&inode1->creation_time));
        update_modification_time(inode1);
        printf("Updated modification time: %s", ctime(&inode1->modification_time));
        deallocate_inode(inode1);
    }
    return 0;
}