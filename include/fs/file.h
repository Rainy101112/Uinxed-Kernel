#ifndef INCLUDE_FILE_H_
#define INCLUDE_FILE_H_

#include "stddef.h"
#include "stdint.h"
#include "fs/fs.h"
#include "fs/inode.h"
#include "spin_lock.h"
#include "intrusive_list.h"
#include "atomic.h"

#define O_ACCMODE 00000003
#define O_RDONLY  00000000
#define O_WRONLY  00000001
#define O_RDWR    00000002


#define S_IRUSR 0400
#define S_IWUSR 0200
#define S_IXUSR 0100

#define S_IRWXU (S_IRUSR | S_IWUSR | S_IXUSR)

#define S_IRWXUGO (S_IRWXU | S_IRWXG | S_IRWXO)

struct path;
struct file;
struct file_operations;

struct path {
	struct vfsmount *mnt;
	struct dentry *dentry;
};

typedef struct path path;

struct file_operations {
	uint64_t (*lseek)(struct file *, uint64_t, int);
	long (*read)(struct file *, char *, size_t, uint64_t);
	long (*write)(struct file *, const char *, size_t, uint64_t);
};

typedef struct file_operations f_op_t;

struct file {
	inode *f_inode;
	atomic_t f_count;
	const f_op_t *f_op;
	uint32_t f_flags;
	uint32_t f_mode;
	uint64_t f_pos;
	uid_t f_uid;
	gid_t f_gid;
	uint64_t f_version;
	void *f_security;
	void *private_date;
	ilist_node_t f_ep_links;
	spinlock_t f_ep_lock;
};

typedef struct file file;

#endif // INCLUDE_FILE_H_
