#ifndef INCLUDE_DEVFS_H_
#define INCLUDE_DEVFS_H_

typedef struct devfs_device {
        void (*open)();
} dev_t;

int devfs_init();
int devfs_alloc_inode();
int devfs_destroy_inode();
int devfs_delete_inode();
int devfs_write_inode();

int devfs_create_device(char *, dev_t *);
int devfs_remove_device();

#endif // INCLUDE_DEVFS_H_
