#ifndef INCLUDE_FS_H_
#define INCLUDE_FS_H_

#include "stdint.h"

#define FILENAME_MAX_LENGTH 255

typedef uint32_t uid_t;
typedef uint32_t gid_t;

typedef struct filesystem {
        const char *name;
        int fs_flags;
} filesystem_t;

int vfs_register(sb_t *);
int vfs_unregister(sb_t *);
int vfs_mount(sb_t *, char *);
int vfs_umount(sb_t *);
int vfs_open(struct file *);
int vfs_close(struct file *);
struct file *vfs_creat(const char *);
int vfs_write(struct file *, const char *, size_t, long *);
int vfs_read(struct file *, void *, size_t, size_t);
int vfs_unlink(struct file *);
int vfs_mkdir();
int vfs_rmdir();

#endif // INCLUDE_FS_H_
