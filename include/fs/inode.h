#ifndef INCLUDE_INODE_H_
#define INCLUDE_INODE_H_

#include "fs/fs.h"
#include "fs/superblock.h"
// #include "list.h"
#include "lock.h"
#include "stdint.h"
#include "time.h"

typedef struct inode {
        // list_t *i_dentry;
        uint32_t i_nlink;
        uint64_t i_count;
        uid_t i_uid;
        gid_t i_gid;
        struct timespec i_atime;
        struct timespec i_mtime;
        struct timespec i_ctime;
        spinlock_t i_lock;
        sb_t *superblock;
        long i_size;
} inode;

typedef struct dentry {
        uint8_t d_name[FILENAME_MAX_LENGTH];
        inode *d_inode;
        dentry *parent;
        uint64_t d_count;
        sb_t *d_sb;
} dentry;

#endif // INCLUDE_INODE_H_
