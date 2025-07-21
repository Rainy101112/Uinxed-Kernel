#ifndef INCLUDE_SUPERBLOCK_H_
#define INCLUDE_SUPERBLOCK_H_

#include "fs/fs.h"

typedef struct superblock_operations {
        struct inode *(*alloc_inode)(struct superblock *);
        void (*destroy_inode)(struct inode *);
        int (*write_inode)(struct inode *, int);
        void (*delete_inode)(struct inode *);
        void (*mount)(struct superblock *, char *);
        void (*umount)(struct superblock *);
} sb_op_t;

typedef struct superblock {
        filesystem_t *fs_type;
        sb_op_t *sb_op;
} sb_t;

#endif // INCLUDE_SUPERBLOCK_H_
