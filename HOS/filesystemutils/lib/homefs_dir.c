# Home Filesystem Utilities

This file contains directory operations including mkdir, rmdir, readdir, and path utilities.

## Includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

## Functions

### mkdir_custom

```c
int mkdir_custom(const char *path) {
    return mkdir(path, 0755);
}
```

### rmdir_custom

```c
int rmdir_custom(const char *path) {
    return rmdir(path);
}
```

### readdir_custom

```c
void readdir_custom(const char *path) {
    struct dirent *entry;
    DIR *dp = opendir(path);
    if (dp == NULL) {
        perror("opendir");
        return;
    }
    while ((entry = readdir(dp))) {
        printf("%s\n", entry->d_name);
    }
    closedir(dp);
}
```

### path_utilities

```c
char* get_filename(const char *filepath) {
    char *filename = strrchr(filepath, '/');
    return filename ? filename + 1 : (char*)filepath;
}
```

```c
char* get_directory(const char *filepath) {
    char *last_slash = strrchr(filepath, '/');
    if (last_slash) {
        size_t length = last_slash - filepath;
        char *directory = malloc(length + 1);
        strncpy(directory, filepath, length);
        directory[length] = '\0';
        return directory;
    }
    return NULL;
}
}
