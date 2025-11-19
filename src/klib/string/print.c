#include "print.h"
#include <klib/graphics/graphics.h>
#include <klib/debug/serial.h>

#include <kernel/console/graph/dos.h>
#include <ssfn.h>

void putc(char c) {
    // u32 char_width = 8 * font_scale;
    // u32 char_height = 8 * font_scale;
    // u32 char_spacing = char_width;
    // u32 line_height = char_height + 2 * font_scale;
    switch(c) {
        // Newline
        case '\n':
          ssfn_dst.x = 0;
          ssfn_dst.y += font_height;
          console_window_check_scroll();
          break;
        default:
          if(ssfn_dst.x + font_width >= (int)fb_width) {
              ssfn_dst.x = 0;
              ssfn_dst.y += font_height;
          }
          console_window_check_scroll();
          ssfn_putc(c);
          break;
    };
}

void string(const char *str, u32 color)
{
    // prints everything from the os terminal to the host-terminal
    for (size_t i = 0; str[i]; i++) {
        set_fg_color(color);
        putc(str[i]);
    }
    printf("%s", str);
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
}

void reset_cursor(void)
{
    set_cursor_pos(0, 0);
}
