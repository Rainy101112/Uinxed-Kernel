#ifndef INCLUDE_SUPERBLOCK_H_
#define INCLUDE_SUPERBLOCK_H_

#include "fs/fs.h"

struct filesystem;
typedef struct filesystem filesystem_t;

struct superblock;

typedef struct superblock sb_t;

struct superblock_operations {
        struct inode *(*alloc_inode)(struct superblock *);
        void (*destroy_inode)(struct inode *);
        int (*write_inode)(struct inode *, int);
        void (*delete_inode)(struct inode *);
        void (*mount)(sb_t *, char *);
        void (*umount)(sb_t *);
};

typedef struct superblock_operations sb_op_t;

struct superblock {
        filesystem_t *fs_type;
        sb_op_t *sb_op;
};

#endif // INCLUDE_SUPERBLOCK_H_
