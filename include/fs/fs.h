#ifndef INCLUDE_FS_H_
#define INCLUDE_FS_H_

#include "stddef.h"
#include "stdint.h"
#include "fs/superblock.h"
#include "intrusive_list.h"
#include "fs/file.h"

#define FILENAME_MAX_LENGTH 255

struct filesystem;
struct vfsmount;

typedef struct dentry dentry;

struct vfsmount {
        dentry *mnt_root;
        sb_t *mnt_sb;
        int mnt_flags;
        struct vfsmount *mnt_parent;
        ilist_node_t mnt_mounts;
};

struct filesystem {
        const char *name;
        int fs_flags;
};

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
