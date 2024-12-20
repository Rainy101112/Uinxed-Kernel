/*
 *
 *		syscall.c
 *		系统调用
 *
 *		2024/12/15 By MicroFish
 *		基于 GPL-3.0 开源协议
 *		Copyright © 2020 ViudiraTech，保留最终解释权。
 *
 */

#include "syscall.h"
#include "printk.h"
#include "common.h"
#include "sched.h"
#include "types.h"
#include "task.h"

typedef enum oflags {
	O_RDONLY,
	O_WRONLY,
	O_RDWR,
	O_CREAT = 4
} oflags_t;

/* POSIX规范-打开文件 */
static uint32_t syscall_posix_open(uint32_t ebx,uint32_t ecx,uint32_t edx,uint32_t esi,uint32_t edi)
{
	enable_intr();
	char *name = (char *)ebx;
	if (ecx & O_CREAT) {
		int status = vfs_mkfile(name);
		if (status == -1)goto error;
	}
	vfs_node_t r = vfs_open(name);
	if (r == 0) goto error;
	for (int i = 3; i < 255; i++) {
		if (get_current_proc()->file_table[i] == 0) {
			cfile_t file = kmalloc(sizeof(cfile_t));
			file->handle = r;
			file->pos = 0;
			file->flags = ecx;
			get_current_proc()->file_table[i] = file;
			return i;
		}
	}
	error:
	return -1;
}

/* POSIX规范-关闭文件 */
static uint32_t syscall_posix_close(uint32_t ebx,uint32_t ecx,uint32_t edx,uint32_t esi,uint32_t edi)
{
	for (int i = 3; i < 255; i++) {
		if (i == ebx) {
			cfile_t file = get_current_proc()->file_table[i];
			if (file == 0)return -1;
			vfs_close(file->handle);
			kfree(file);
			return (uint32_t)(get_current_proc()->file_table[i] = 0);
		}
	}
	return -1;
}

/* POSIX规范-读取文件 */
static uint32_t syscall_posix_read(uint32_t ebx,uint32_t ecx,uint32_t edx,uint32_t esi,uint32_t edi)
{
	enable_intr();
	if (ebx < 0 || ebx == 1 || ebx == 2)return -1;
	cfile_t file = get_current_proc()->file_table[ebx];
	if (file == 0)return -1;
	if (file->flags == O_WRONLY)return -1;
	char* buffer = kmalloc(file->handle->size);
	if (vfs_read(file->handle, buffer, 0, file->handle->size) == -1)return -1;
	int ret = 0;
	disable_intr();
	char *filebuf = (char *)buffer;
	char *retbuf = (char *)ecx;

	if(edx > file->handle->size){
		memcpy((uint8_t *)retbuf, (const uint8_t *)filebuf, file->handle->size);
		ret = file->pos += file->handle->size;
	}else{
		memcpy((uint8_t *)retbuf, (const uint8_t *)filebuf, edx);
		ret = file->pos += edx;
	}
	kfree(buffer);
	enable_intr();
	return ret;
}

/* POSIX规范-文件大小 */
static uint32_t syscall_posix_sizex(uint32_t ebx,uint32_t ecx,uint32_t edx,uint32_t esi,uint32_t edi)
{
	if (ebx < 0 || ebx == 1 || ebx == 2)return -1;
	cfile_t file = get_current_proc()->file_table[ebx];
	if (file == 0)return -1;
	return file->handle->size;
}

/* 发送格式化输出到标准输出 */
static uint32_t syscall_printf(uint32_t ebx, uint32_t ecx, uint32_t edx, uint32_t esi, uint32_t edi)
{
	printk((const char *)ebx);
	return 0;
}

/* 发送字符到标准输出 */
static uint32_t syscall_putc(uint32_t ebx,uint32_t ecx,uint32_t edx,uint32_t esi,uint32_t edi)
{
	putchar(ebx);
	return 0;
}

/* 系统调用表 */
syscall_t syscall_handlers[MAX_SYSCALLS] = {
	[1] = syscall_posix_open,
	[2] = syscall_posix_close,
	[3] = syscall_posix_read,
	[4] = syscall_posix_sizex,
	[5] = syscall_printf,
	[6] = syscall_putc
};

/* 系统调用处理 */
unsigned int syscall(void)
{
	volatile unsigned int eax, ebx, ecx, edx, esi, edi;
	asm("mov %%eax, %0\n\t" : "=r"(eax));
	asm("mov %%ebx, %0\n\t" : "=r"(ebx));
	asm("mov %%ecx, %0\n\t" : "=r"(ecx));
	asm("mov %%edx, %0\n\t" : "=r"(edx));
	asm("mov %%esi, %0\n\t" : "=r"(esi));
	asm("mov %%edi, %0\n\t" : "=r"(edi));
	if (0 <= eax && eax < MAX_SYSCALLS && syscall_handlers[eax] != 0) {
		eax = ((syscall_t)syscall_handlers[eax])(ebx, ecx, edx, esi, edi);
	} else {
		eax = -1;
	}
	return eax;
}

/* 初始化系统调用 */
void syscall_init(void)
{
	print_busy("Setting up system calls...\r");
	idt_use_reg(0x80, (uint32_t)asm_syscall_handler);
	print_succ("The system call setup is complete.\n");
}
