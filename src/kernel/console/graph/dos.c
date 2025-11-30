#include "dos.h"
#include "uno.h"
#include <klib/graphics/graphics.h>

void console_window_init(void) {
    ssfn_dst.x = CONSOLE_PADDING_X;
    ssfn_dst.y = banner_get_height();
}

void console_window_clear(u32 color)
{
    u32 fb_w = get_fb_width();
    u32 fb_h = get_fb_height();
    u32 banner_h = banner_get_height();

    // clear below banner
    draw_rect(0, banner_h, fb_w, fb_h - banner_h, color);
    //TODO:
    // implement real dirty rectangles/ screen clear

    // redraw banner
    banner_draw();

    // reset cursor to console start
    ssfn_dst.x = CONSOLE_PADDING_X;
    ssfn_dst.y = banner_h;
}

u32 console_window_get_start_y(void) {
    return banner_get_height();
}

u32 console_window_get_max_y(void) {
    return get_fb_height();
}

void console_window_check_scroll(void)
{
    u32 fb_h = get_fb_height();
    u32 banner_h = banner_get_height();

    // check if cursor is at bottom
    if (ssfn_dst.y + font.height >= fb_h) {
        // scroll only console area
        u32 fb_w = get_fb_width();
        u32 pitch_dwords = get_fb_pitch() / 4;
        u32 *fb = get_framebuffer();

        // move content up
        for (u32 y = banner_h + font.height; y < fb_h; y++) {
            for (u32 x = 0; x < fb_w; x++) {
                fb[(y - font.height) * pitch_dwords + x] = fb[y * pitch_dwords + x];
            }
        }

        // cls top line
        for (u32 y = fb_h - font.height; y < fb_h; y++) {
            for (u32 x = 0; x < fb_w; x++) {
                fb[y * pitch_dwords + x] = CONSOLESCREEN_BG_COLOR;
            }
        }

        //adjust cursor
        ssfn_dst.y = fb_h - font.height - 10;
        if (ssfn_dst.y < banner_h) {
            ssfn_dst.y = banner_h;
        }
    }
}

void console_window_update_layout(void)
{
    //after fsize change
    // adjust cursor
    u32 banner_h = banner_get_height();

    if (ssfn_dst.y < banner_h) {
        ssfn_dst.y = banner_h;
    }
}
