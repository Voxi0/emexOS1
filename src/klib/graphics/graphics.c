#define SSFN_CONSOLEBITMAP_TRUECOLOR
#include "graphics.h"
#include <kernel/module/module.h>
#include <klib/debug/serial.h>

extern ssfn_font_t *_binary_fonts_test_sfn_start;

//donnot put static before the uints!
u32 *framebuffer = NULL;
u32 fb_width = 0;
u32 fb_height = 0;
u32 fb_pitch = 0;

font_t font = {
    .src = &_binary_fonts_test_sfn_start,
    .width = 12,
    .height = 24,
};

void graphics_init(struct limine_framebuffer *fb)
{
    framebuffer = (u32 *)fb->address;
    fb_width = fb->width;
    fb_height = fb->height;
    fb_pitch = fb->pitch;

    ssfn_src = font.src;
    ssfn_dst.ptr = (u8 *)framebuffer;
    ssfn_dst.w = fb_width;
    ssfn_dst.h = fb_height;
    ssfn_dst.p = fb_pitch;
    ssfn_dst.x = ssfn_dst.y = 0;

    print("Welcome to doccrOS \n", GFX_WHITE);
    print("v0.0.1 (alpha)\n", GFX_WHITE);

    print("Graphics\n", GFX_WHITE);
    char res_buf[128]; //donot set this to 64
    str_copy(res_buf, "\nFramebuffer ->\n");
    str_append(res_buf, " \t Width: ");
    str_append_uint(res_buf, fb_width);
    str_append(res_buf, "\n");
    str_append(res_buf, " \t Height: ");
    str_append_uint(res_buf, fb_height);
    str_append(res_buf, "\n");
    str_append(res_buf, " \t Pitch: ");
    str_append_uint(res_buf, fb_pitch);
    str_append(res_buf, "\n");
    str_append(res_buf, " \t adress: ");
    str_append_uint(res_buf, (u64) framebuffer);
    str_append(res_buf, "\n");
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
            framebuffer[y * pitch_dwords + x] = CONSOLESCREEN_BG_COLOR;
        }
    }
}

void putpixel(u32 x, u32 y, u32 color)
{
    if (x < fb_width && y < fb_height) {
        framebuffer[y * (fb_pitch / 4) + x] = color;
    }
}

u32 get_fb_width(void){
    return fb_width;
}

u32 get_fb_height(void){
    return fb_height;
}

u32* get_framebuffer(void){
    return framebuffer;
}

u32 get_fb_pitch(void){
    return fb_pitch;
}

/*void reset_cursor(void)
{
    cursor_x = 10;
    cursor_y = 10;
    }*/
