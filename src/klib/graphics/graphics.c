#include "graphics.h"
#include <kernel/module/module.h>
#include <fonts/font_8x8.h>

#define SSFN_CONSOLEBITMAP_TRUECOLOR
#include <ssfn.h>
extern u8 _binary_fonts_unifont_sfn_start;

//donnot put static before the uints!
u32 *framebuffer = NULL;
u32 fb_width = 0;
u32 fb_height = 0;
u32 fb_pitch = 0;
u8 font_scale = 1;
u8 font_width = 8;
u8 font_height = 16;

void graphics_init(struct limine_framebuffer *fb)
{
    framebuffer = (u32 *)fb->address;
    fb_width = fb->width;
    fb_height = fb->height;
    fb_pitch = fb->pitch;

    ssfn_src = (ssfn_font_t *)&_binary_fonts_unifont_sfn_start;
    ssfn_dst.ptr = (u8 *)fb->address;
    ssfn_dst.w = fb_width;
    ssfn_dst.h = fb_height;
    ssfn_dst.p = fb_pitch;
    ssfn_dst.x = ssfn_dst.y = 0;
    ssfn_dst.bg = GFX_BG;

    print("Welcome to doccrOS \n", GFX_WHITE);
    print("v0.0.1 (alpha)\n", GFX_WHITE);

    print("Graphics\n", GFX_WHITE);
    char res_buf[64];
    str_copy(res_buf, "Framebuffer ");
    str_append_uint(res_buf, fb_width);
    str_append(res_buf, "x");
    str_append_uint(res_buf, fb_height);
    print(res_buf, GFX_WHITE);
}

void clear(u32 color)
{
    draw_rect(0, 0, fb_width, fb_height, color);
    reset_cursor();
}

void scroll_up(u32 lines)
{
    u32 pixels_to_scroll = lines;
    u32 pitch_dwords = fb_pitch / 4;

    // Move framebuffer content up
    for (u32 y = pixels_to_scroll; y < fb_height; y++) {
        for (u32 x = 0; x < fb_width; x++) {
            framebuffer[(y - pixels_to_scroll) * pitch_dwords + x] =
                framebuffer[y * pitch_dwords + x];
        }
    }

    // Clear bottom lines
    for (u32 y = fb_height - pixels_to_scroll; y < fb_height; y++) {
        for (u32 x = 0; x < fb_width; x++) {
            framebuffer[y * pitch_dwords + x] = ssfn_dst.bg;
        }
    }
}

// Getters
u32 *get_framebuffer(void) {return framebuffer;}
u32 get_fb_width(void) {return fb_width;}
u32 get_fb_height(void) {return fb_height;}
u32 get_fb_pitch(void) {return fb_pitch;}
u32 get_fg_color(void) {return ssfn_dst.fg;}
u32 get_bg_color(void) {return ssfn_dst.bg;}
int get_cursor_x(void) {return ssfn_dst.x;}
int get_cursor_y(void) {return ssfn_dst.y;}

// Setters
void set_fg_color(u32 color) {ssfn_dst.fg = color;}
void set_bg_color(u32 color) {ssfn_dst.bg = color;}
void set_cursor_x(int pos) {ssfn_dst.x = pos;}
void set_cursor_y(int pos) {ssfn_dst.y = pos;}
void set_cursor_pos(int x, int y) {
    ssfn_dst.x = x;
    ssfn_dst.y = y;
}
void set_font_scale(u8 scale) {
    if(scale >= 1 && scale <= 4) font_scale = scale;
}
