/*
 *
 *		lib_os_terminal.lib.h
 *		lib_os_terminal库头文件
 *
 *		2024/10/20 By MicroFish
 *		基于 GPL-3.0 开源协议
 *		Copyright © 2020 ViudiraTech，保留最终解释权。
 *
 */

#ifndef INCLUDE_LIB_OS_TERMINAL_LIB_H_
#define INCLUDE_LIB_OS_TERMINAL_LIB_H_

#include "types.h"

typedef struct TerminalPalette {
	uint32_t foreground;
	uint32_t background;
	uint32_t ansi_colors[16];
} TerminalPalette;

extern void terminal_destroy(void);
extern void terminal_flush(void);
extern void terminal_set_auto_flush(unsigned int auto_flush);
extern void terminal_advance_state(const char *s);
extern void terminal_advance_state_single(char c);
extern const char *terminal_handle_keyboard(unsigned char scancode);
extern void terminal_set_color_scheme(unsigned int palette_index);
extern void terminal_set_custom_color_scheme(struct TerminalPalette palette);
extern bool terminal_init(unsigned int width, unsigned int height,
                          uint32_t *screen, uint32_t (*malloc)(uint32_t), void (*free)(void*));

#endif // INCLUDE_LIB_OS_TERMINAL_LIB_H_