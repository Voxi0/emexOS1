#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <types.h>
#include <limine/limine.h>
#include <theme/doccr.h>
#include <theme/stdclrs.h>
#include <klib/string/print.h>
#include <klib/string/string.h>

// Framebuffer
extern u32 *framebuffer;
extern u32 fb_width;
extern u32 fb_height;
extern u32 fb_pitch;

// Font
extern u8 font_width;
extern u8 font_height;
extern u8 font_scale;

// Basic
void graphics_init(struct limine_framebuffer *fb);
void reset_cursor(void);
void clear(u32 color);
void scroll_up(u32 lines);
void putc(char c);

// Draw stuff
void putpixel(u32 x, u32 y, u32 color);
void draw_rect(u32 x, u32 y, u32 width, u32 height, u32 color);
void draw_circle(u32 cx, u32 cy, u32 radius, u32 color);
void draw_line(u32 x0, u32 y0, u32 x1, u32 y1, u32 color);

// Getters
u32* get_framebuffer(void);
u32 get_fb_width(void);
u32 get_fb_height(void);
u32 get_fb_pitch(void);
u32 get_fg_color(void);
u32 get_bg_color(void);
int get_cursor_x(void);
int get_cursor_y(void);

// Setters
void set_font_scale(u8 scale);
void set_fg_color(u32 color);
void set_bg_color(u32 color);
void set_cursor_pos(int x, int y);
void set_cursor_x(int pos);
void set_cursor_y(int pos);

#endif
