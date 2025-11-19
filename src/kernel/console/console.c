#include "console.h"
#include "functions.h"

static char input_buffer[MAX_INPUT_LEN];
static int input_pos = 0;

//----------------------------------
// ! IMPORTANT FOR NEW COMMANDS !
static int cmd_count = 11;

static console_cmd_t commands[MAX_CMDS] = {
    CMDENTRY(cmd_echo, "echo", "prints text to console", "echo [text]"),
    CMDENTRY(cmd_clear, "clear", "clears the screen", "clear [color]"),
    CMDENTRY(cmd_help, "help", "displays all available commands", "help [command]"),
    CMDENTRY(cmd_fsize, "scale", "change screen size", "scale [2-4]"),
    CMDENTRY(cmd_modules, "modules", "list loaded modules", "modules"),
    CMDENTRY(cmd_meminfo, "meminfo", "displays memory infos", "meminfo"),
    //CMDENTRY(cmd_memtest, "memtest", "Memory test suite", "memtest"),
    CMDENTRY(cmd_sysinfo, "dofetch", "displays doccrOS fetch", "dofetch"),
    CMDENTRY(cmd_cal, "calendar", "displays current date & time", "calendar"),
    CMDENTRY(cmd_date, "date", "displays current date", "date"),
    CMDENTRY(cmd_uptime, "uptime", "System uptime", "uptime"),
    CMDENTRY(cmd_time, "time", "displays current time", "time"),
};

//----------------------------------

//module---------------------------
static int console_module_init(void) {
    // console already initialized in main
    return 0;
}

static void console_module_fini(void) {
    //
}

driver_module console_module = (driver_module) {
    .name = "console",
    .mount = "/dev/console",
    .version = VERSION_NUM(0, 1, 1, 0),
    .init = console_module_init,
    .fini = console_module_fini,
    .open = NULL, // later for fs
    .read = NULL, // later for fs
    .write = NULL, // later for fs
};

//---------------------------------

void console_init(void)
{
    input_pos = 0;
    input_buffer[0] = '\0';

    clear(CONSOLESCREEN_BG_COLOR);
    //reset_cursor();
    /*
    //module_register_driver(&console_module);

    if (get_cursor_x() == 0 && get_cursor_y() == 0) {
        clear(CONSOLESCREEN_COLOR);
        reset_cursor();
    }
    */

    set_cursor_pos(0, 0);
    banner_init();
    console_window_init();
    shell_print_prompt();
}

void console_run(void)
{
    // main console loop will be called from keyboard handler
    // this function exists for future expansion like syscalls maybe for a app like a code editor which needs a console
    // actually idk why i created this xd
}

void console_handle_key(char c)
{
    if (c == '\n') {
        // execute command when enter
        putc('\n');

        if (input_pos > 0) {
            input_buffer[input_pos] = '\0';

            // check for && and use chained execution
            int has_chain = 0;
            for (int i = 0; i < input_pos - 1; i++) {
                if (input_buffer[i] == '&' && input_buffer[i+1] == '&') {
                    has_chain = 1;
                    break;
                }
            }

            if (has_chain) parse_and_execute_chained(input_buffer);
            else console_execute(input_buffer);

            input_pos = 0;
            input_buffer[0] = '\0';
        }

        shell_print_prompt();
        return;
    }

    if (c == '\r') {
        putc('\n');
        input_buffer[input_pos++] = '\n';
        return;
    }

    if (c == '\b') {
        if (input_pos > 0) {
            input_pos--;
            input_buffer[input_pos] = '\0';

            // just move the cursor back then print space, draw rext, and move back again
            if (get_cursor_x() >= font_width) {
                set_cursor_x(get_cursor_x() - font_width);
                set_fg_color(GFX_WHITE);
                putc(' ');
                set_cursor_x(get_cursor_x() - font_width);
                draw_rect(get_cursor_x(), get_cursor_y(), font_width, font_height, get_bg_color());
            }
        }
        return;
    }

    console_window_check_scroll();

    // add character to buffer
    if (input_pos < MAX_INPUT_LEN - 1) {
        input_buffer[input_pos++] = c;
        input_buffer[input_pos] = '\0';
        set_fg_color(GFX_WHITE);
        putc(c);
    }
}

void console_execute(const char *input)
{
    // skip leading spaces
    while (*input == ' ') input++;
    if (*input == '\0') return;

    // find command name end
    const char *end = input;
    while (*end && *end != ' ') end++;

    // extract it
    char cmd_name[64];
    int len = end - input;
    if (len >= 64) len = 63;

    for (int i = 0; i < len; i++) {
        cmd_name[i] = input[i];
    }
    cmd_name[len] = '\0';

    // find arguments
    const char *args = end;
    while (*args == ' ') args++;

    console_cmd_t *cmd = console_find_cmd(cmd_name);
    if (cmd) {
        cmd->func(args);

        banner_force_update();
    } else {
        print("> Unknown command, Type 'help' for available commands...", GFX_RED);
    }
}

console_cmd_t* console_find_cmd(const char *name)
{
    for (int i = 0; i < cmd_count; i++) {
        // simple string comparison
        const char *a = name;
        const char *b = commands[i].name;
        int match = 1;

        while (*a && *b) {
            if (*a != *b) {
                match = 0;
                break;
            }
            a++;
            b++;
        }

        if (match && *a == '\0' && *b == '\0') {
            return &commands[i];
        }
    }
    return NULL;
}
