#ifndef HOMEFS_H
#define HOMEFS_H

#include <stdint.h>
#include <stddef.h>

/* =====================================================
 * HOMEFS - Custom Filesystem for H-OS
 * 
 * A simple, educational filesystem with:
 * - Superblock: metadata about the filesystem
 * - Inode Table: file metadata and block pointers
 * - Directory Entries: file names and inode references
 * - Data Blocks: actual file content
 * ===================================================== */

/* Constants */
#define HOMEFS_BLOCK_SIZE        4096
#define HOMEFS_INODE_SIZE        256
#define HOMEFS_MAX_FILENAME      255
#define HOMEFS_MAX_BLOCKS        65536
#define HOMEFS_MAX_INODES        8192
#define HOMEFS_MAX_FILE_SIZE     (1024 * 1024 * 100)
#define HOMEFS_MAGIC             0x484F4D45
#define HOMEFS_VERSION           1

/* File type constants */
#define HOMEFS_TYPE_FILE         0x1
#define HOMEFS_TYPE_DIR          0x2
#define HOMEFS_TYPE_SYMLINK      0x4

/* File permissions */
#define HOMEFS_PERM_OWNER_READ   0400
#define HOMEFS_PERM_OWNER_WRITE  0200
#define HOMEFS_PERM_OWNER_EXEC   0100
#define HOMEFS_PERM_GROUP_READ   0040
#define HOMEFS_PERM_GROUP_WRITE  0020
#define HOMEFS_PERM_GROUP_EXEC   0010
#define HOMEFS_PERM_OTHER_READ   0004
#define HOMEFS_PERM_OTHER_WRITE  0002
#define HOMEFS_PERM_OTHER_EXEC   0001

/* On-Disk Structures */

typedef struct {
    uint32_t magic;
    uint32_t version;
    uint32_t block_size;
    uint32_t total_blocks;
    uint32_t total_inodes;
    uint32_t free_blocks;
    uint32_t free_inodes;
    uint32_t created_timestamp;
    uint32_t mounted_count;
    uint32_t last_check;
    uint32_t inode_table_block;
    uint32_t block_bitmap_block;
    uint32_t inode_bitmap_block;
    uint32_t data_block_start;
    char volume_name[64];
    char creation_os[32];
    uint8_t reserved[256];
} homefs_superblock_t;

typedef struct {
    uint32_t inode_number;
    uint16_t file_type;
    uint16_t permissions;
    uint32_t owner_uid;
    uint32_t owner_gid;
    uint32_t file_size;
    uint32_t created_time;
    uint32_t modified_time;
    uint32_t accessed_time;
    uint32_t link_count;
    uint32_t block_pointers[12];
    uint32_t indirect_block;
    uint32_t double_indirect;
    uint32_t triple_indirect;
    uint8_t reserved[156];
} homefs_inode_t;

typedef struct {
    uint32_t inode_number;
    uint16_t name_length;
    uint8_t file_type;
    char filename[256];
} homefs_dir_entry_t;

/* In-Memory Structures */

typedef struct {
    homefs_inode_t disk_inode;
    uint32_t ref_count;
    int dirty;
    void *cached_blocks[12];
} homefs_inode_mem_t;

typedef struct {
    homefs_superblock_t superblock;
    uint8_t *block_bitmap;
    uint8_t *inode_bitmap;
    homefs_inode_mem_t *inode_cache;
    uint32_t total_blocks;
    uint32_t total_inodes;
    int mounted;
} homefs_fs_t;

typedef struct {
    homefs_inode_mem_t *inode;
    uint32_t offset;
    int mode;
} homefs_file_t;

/* Function Prototypes */

int homefs_init(void);
int homefs_mount(uint32_t device_block);
int homefs_unmount(void);
int homefs_format(uint32_t total_blocks, uint32_t total_inodes);

homefs_file_t *homefs_open(const char *path, int flags);
int homefs_close(homefs_file_t *file);
int homefs_read(homefs_file_t *file, void *buf, size_t count);
int homefs_write(homefs_file_t *file, const void *buf, size_t count);
int homefs_seek(homefs_file_t *file, int offset, int whence);
int homefs_unlink(const char *path);

int homefs_mkdir(const char *path);
int homefs_rmdir(const char *path);
homefs_dir_entry_t *homefs_readdir(const char *path, uint32_t *entry_count);

homefs_inode_mem_t *homefs_inode_get(uint32_t inode_num);
void homefs_inode_put(homefs_inode_mem_t *inode);
int homefs_inode_alloc(uint16_t type);
int homefs_inode_free(uint32_t inode_num);

uint32_t homefs_block_alloc(void);
int homefs_block_free(uint32_t block_num);
int homefs_block_read(uint32_t block_num, void *buf);
int homefs_block_write(uint32_t block_num, const void *buf);

int homefs_get_inode_by_path(const char *path, homefs_inode_mem_t **out);
int homefs_path_to_parent_and_name(const char *path, char *parent, char *name);
void homefs_print_superblock(void);

#endif