/*
 *
 *		main.c
 *		内核主入口
 *
 *		2024/6/23 By MicroFish
 *		基于 GPL-3.0 开源协议
 *		Copyright © 2020 ViudiraTech，保留所有权利。
 *
 */

#include "idt.h"
#include "gdt.h"
#include "console.h"
#include "debug.h"
#include "printk.h"
#include "memory.h"
#include "keyboard.h"
#include "uinxed.h"
#include "pci.h"
#include "serial.h"
#include "block.h"
#include "vfs.h"
#include "timer.h"
#include "beep.h"
#include "cpu.h"
#include "vgafont.h"
#include "multiboot.h"
#include "task.h"
#include "sched.h"

extern uint32_t end;
uint32_t placement_address = (uint32_t) & end;

void shell(void); // 声明shell程序入口

/* 内核shell进程 */
int kthread_shell(void *arg)
{
	shell();
}

/* 内核入口 */
void kernel_init(multiboot_t *glb_mboot_ptr)
{
	console_to_serial(1);		// 输出内核启动日志到串口
	console_clear();			// 清屏

	/* 检测内存是否达到最低要求 */
	if ((glb_mboot_ptr->mem_upper + glb_mboot_ptr->mem_lower) / 1024 + 1 < 16) {
		panic("OUT_OF_MEMORY");
	}
	printk("Uinxed-Kernel "KERNL_VERS"(build-%d)\n", KERNL_BUID);		// 打印内核信息
	printk(PROJK_COPY"\n");												// 打印版权信息
	printk("This version compiles at "BUILD_DATE" "BUILD_TIME"\n\n");	// 打印编译日期时间

	printk("Initializing operating system kernel components.\n");		// 提示用户正在初始化内核

	init_gdt();					// 初始化gdt
	init_idt();					// 初始化idt
	ISR_registe_Handle();		// 注册ISR处理
	init_page();				// 初始化内存分页
	init_pci();					// 初始化PCI设备
	init_serial();				// 初始化计算机串口
	init_keyboard();			// 初始化键盘驱动
	init_sched();				// 初始化多任务
	block_init();				// 初始化块设备
	init_vfs();					// 初始化虚拟文件系统

	init_timer(1);				// 初始化定时器
	init_pit();					// 初始化PIT

	enable_intr();				// 开启中断

	init_kfont();				// 设置内核字体

	system_beep(1000);			// 初始化完毕后蜂鸣
	sleep(10);
	system_beep(0);

	console_write_newline();	// 打印一个空行，和上面的信息保持隔离
	print_cpu_info();			// 打印当前CPU的信息

	console_to_serial(0);		// 停止输出内核启动日志到串口
	kernel_thread(kthread_shell, NULL);
}
