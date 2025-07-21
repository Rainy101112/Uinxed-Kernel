#include "fs/fs.h"
#include "fs/inode.h"
#include "fs/superblock.h"

int devfs_create_device(char *, void (*)(void *));
int devfs_remove_device();

// dev_read dev_write dev_open dev_close dev_lock dev_unlock
