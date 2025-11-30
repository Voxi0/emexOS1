#include "print.h"
#include <klib/graphics/graphics.h>
#include <klib/debug/serial.h>
#include <kernel/console/graph/dos.h>
#include <fonts/font_8x8.h>

static void putchar_at(char c, u32 x, u32 y, u32 color) {
    const u8 *glyph = font_8x8[(u8)c];
    for (int dy = 0; dy < 8; dy++) {
        u8 row = glyph[dy];
        for (int dx = 0; dx < 8; dx++) {
            if (row & (1 << (7 - dx))) {
                // Draw scaled pixel
                for (u32 sy = 0; sy < font.scale; sy++) {
                    for (u32 sx = 0; sx < font.scale; sx++) {
                        putpixel(x + dx * font.scale + sx, y + dy * font.scale + sy, color);
                    }
                }
            }
        }
    }
}

void putchar(char c, u32 color)
{
    // Force cursor to correct pixel cell
    ssfn_dst.x = (ssfn_dst.x / font.width) * font.width;
    ssfn_dst.y = (ssfn_dst.y / font.height) * font.height;

    // Handle special characters and rendering normal characters
    switch(c) {
        case '\n':
            ssfn_dst.x = 0;
            ssfn_dst.y += font.height;
            console_window_check_scroll();
            return;
        default:
            if(ssfn_dst.x + font.width >= fb_width) {
                ssfn_dst.x = 0;
                ssfn_dst.y += font.height;
                console_window_check_scroll();
            }
            
            ssfn_dst.fg = color;
            ssfn_putc(c);
            break;
    }
}

void string(const char *str, u32 color)
{
    for (size_t i = 0; str[i]; i++) putchar(str[i], color);
    printf("%s", str); // prints everything from the os terminal to the host-terminal
}

void IntToString(int value, char *buffer)
{
    char temp[11];
    int i = 0;
    int isNegative = 0;

    if (value < 0)
    {
        isNegative = 1;
        value = -value;
    }

    do
    {
        temp[i++] = (value % 10) + '0';
        value /= 10;
    } while (value > 0);

    if (isNegative)
    {
        temp[i++] = '-';
    }

    temp[i] = '\0';

    for (int j = 0; j < i; j++)
    {
        buffer[j] = temp[i - j - 1];
    }

    buffer[i] = '\0';
}

void printInt(int value, u32 color)
{
    char buffer[12];
    IntToString(value, buffer);
    string(buffer, color);
}

void print(const char *str, u32 color)
{
    string(str, color);
    //putchar('\n', color);
}

void reset_cursor(void)
{
    ssfn_dst.x = ssfn_dst.y = 0;
}
