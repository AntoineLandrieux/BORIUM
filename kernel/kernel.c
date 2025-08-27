#include <DRIVER/keyboard.h>
#include <DRIVER/video.h>

#include <STD/stdlib.h>
#include <STD/stdint.h>

#include <SOARE/SOARE.h>

char USERNAME[20] = {0};

int system(char *command)
{
    int errorlevel = Execute("shell", command);
    free_all();
    return errorlevel;
}

/**
 *
 *  _____  _____ _____ _____ _   _ __  __
 * | ___ \|  _  | ___ \_   _| | | |  \/  |
 * | |_/ /| | | | |_/ / | | | | | | .  . |
 * | ___ \| | | |    /  | | | | | | |\/| |
 * | |_/ /\ \_/ / |\ \ _| |_| |_| | |  | |
 * \____/  \___/\_| \_|\___/ \___/\_|  |_/
 *
 * Antoine LANDRIEUX (MIT License) <kernel.c>
 * <https://github.com/AntoineLandrieux/BORIUM/>
 *
 */

#include "kernel.h"

// Is the kernel running?
unsigned char running = 0;

#define SCANCODE_ENTER 0x1C
#define SCANCODE_LEFT 0x4B
#define SCANCODE_RIGHT 0x4D
#define SCANCODE_UP 0x48
#define SCANCODE_DOWN 0x50

/**
 * @brief Sleep miliseconds
 *
 * @param ms
 */
void SLEEP(unsigned int ms)
{
    volatile unsigned long count = 0;
    unsigned long limit = ms * 500000;

    while (count < limit)
    {
        __asm__ __volatile__("nop");
        count++;
    }
}

/**
 * @brief Text editor
 *
 */
void EDITOR(void)
{
#define __MAX_LINE_EDITOR__ 100
    char file[__MAX_LINE_EDITOR__ * __SOARE_MAX_INPUT__] = {0};
    char user[__SOARE_MAX_INPUT__] = {0};

    PUTS(
        //
        "\n"
        " EDITOR MODE\n"
        " \t TYPE '?exit' or '?cancel' to quit\n"
        " \t TYPE '?run' or '?commit' to execute\n"
        "\n"
        //
    );

    unsigned char color = (GET_GLOBAL_COLOR() >> 4) | (GET_GLOBAL_COLOR() << 4);

    for (unsigned short i = 0; i < __MAX_LINE_EDITOR__; i++)
    {
        char lines[3] = {0, 0, 0};

        if ((i + 1) < 10)
            CPUTC('0', color);

        itoa(lines, sizeof(lines), (i + 1));
        CPUTS(lines, color);
        CPUTC('.', color);
        PUTC(' ');

        GETS(user, sizeof(user));
        strcat(file, user);

        if (strstr(user, "?run") || strstr(user, "?commit"))
            break;

        if (strstr(user, "?exit") || strstr(user, "?cancel"))
            return;
    }

    PUTC('\n');
    Execute("editor", file);
    free_all();
}

static void USAGE_SELECTOR(void)
{
    TEXT_BLINKING(1);
    CPUTS(" USE ARROW KEY AND PRESS ENTER TO SELECT \n\n", 0xFC);
    TEXT_BLINKING(0);
}

/**
 * @brief Select the keyboard layout
 *
 */
static void KEYBOARD_SELECTOR(void)
{
    CPUTS("\n KEYBOARD SELECT   \n", 0x1F);

    USAGE_SELECTOR();

    uint8_t selected = QUERTY;
    UPDATE_AND_MOVE_CURSOR(0);
    uint16_t cursor = GET_CURSOR();

    while (inb(KEYBOARD_PORT) == SCANCODE_ENTER)
        /* pass */;

    while (1)
    {
        PUTS("      \t 1) ");
        CPUTS(" QUERTY \n", selected == QUERTY ? 0x9F : 0xF1);
        PUTS("      \t 2) ");
        CPUTS(" AZERTY \n", selected == AZERTY ? 0x9F : 0xF1);

        switch (inb(KEYBOARD_PORT))
        {
        case SCANCODE_UP:
        case SCANCODE_LEFT:
            selected = QUERTY;
            break;

        case SCANCODE_DOWN:
        case SCANCODE_RIGHT:
            selected = AZERTY;
            break;

        case SCANCODE_ENTER:

            while (inb(KEYBOARD_PORT) == SCANCODE_ENTER)
                /* Wait for release */;

            PUTS(" Keyboard (");
            PUTC('0' + selected + 1);
            PUTS(") selected !\n");

            UPDATE_AND_MOVE_CURSOR(1);

            return KEYBOARD_INIT(selected);

        default:
            break;
        }

        SET_CURSOR(cursor);
    }

    UPDATE_AND_MOVE_CURSOR(1);
}

/**
 * @brief Select username
 *
 */
static void USER_SELECTOR(void)
{
    CPUTS("\n USER SELECT       \n\n", 0x1F);

    PUTS(" Enter a username : ");
    GETS(USERNAME, sizeof(USERNAME));
}

/**
 * @brief Select theme
 *
 */
static unsigned char THEME_SELECTOR(void)
{
    CPUTS("\n THEME SELECT      \n", 0x1F);

    USAGE_SELECTOR();

    int8_t selected = 0;
    UPDATE_AND_MOVE_CURSOR(0);
    uint16_t cursor = GET_CURSOR();

    while (inb(KEYBOARD_PORT) == SCANCODE_ENTER)
        /* pass */;

    while (1)
    {
        PUTS("      \t 1) ");
        CPUTS(" LIGHT      ", selected == 0 ? 0x8F : 0xF1);
        PUTS("      \t 2) ");
        CPUTS(" DARK       \n", selected == 1 ? 0x0F : 0xF1);
        PUTS("      \t 3) ");
        CPUTS(" GIRLY      ", selected == 2 ? 0xDF : 0xF1);
        PUTS("      \t 4) ");
        CPUTS(" POWERSHELL \n", selected == 3 ? 0x1F : 0xF1);

        switch (inb(KEYBOARD_PORT))
        {
        case SCANCODE_UP:
            if (selected == 2 || selected == 3)
                selected -= 2;
            break;

        case SCANCODE_DOWN:
            if (selected == 0 || selected == 1)
                selected += 2;
            break;

        case SCANCODE_RIGHT:
            if (selected == 0 || selected == 2)
                selected += 1;
            break;

        case SCANCODE_LEFT:
            if (selected == 1 || selected == 3)
                selected -= 1;
            break;

        case SCANCODE_ENTER:

            while (inb(KEYBOARD_PORT) == SCANCODE_ENTER)
                /* Wait for release */;

            PUTS(" Theme (");
            PUTC('0' + selected + 1);
            PUTS(") selected !\n");

            UPDATE_AND_MOVE_CURSOR(1);

            switch (selected)
            {
            case 0:
                return 0xF0;

            case 1:
                return 0x0F;

            case 2:
                return 0xDF;

            case 3:
                return 0x1F;

            default:
                break;
            }

        default:
            break;
        }

        SET_CURSOR(cursor);
    }

    UPDATE_AND_MOVE_CURSOR(1);
    return 0x0F;
}

/**
 * @brief Shell for the interpreter
 *
 */
static void shell()
{

    PUTS(
        //
        "\n"
        "The programs included with the BORIUM system are free software;\n"
        "the exact distribution terms for each program are described in the\n"
        "<https://github.com/AntoineLandrieux/BORIUM>\n"
        "Type 'license', 'help' or see SOARE documentation\n"
        //
    );

    while (running)
    {
        char input[80] = {0};

        unsigned char color = (GET_GLOBAL_COLOR() >> 4) | (GET_GLOBAL_COLOR() << 4);

        CPUTS("\n ", color);
        CPUTS(USERNAME, color);
        CPUTS(" >>> ", color);
        PUTC(' ');

        GETS(input, sizeof(input));

        system(input);
    }
}

/**
 * @brief Setup the kernel
 *
 */
void SETUP()
{
    SET_GLOBAL_COLOR(0xF0);
    SCREEN_CLEAR();

    CPUTS("\n BORIUM SETUP ", 0xF1);
    PUTS("\n =================================\n");

    KEYBOARD_SELECTOR();
    USER_SELECTOR();
    unsigned char theme = THEME_SELECTOR();
    CPUTS("\n All Ok ? [Y/N] ", 0xF0);

    while (1)
    {
        switch (GETC())
        {
        case 'y':
        case 'Y':
            SET_GLOBAL_COLOR(theme);
            return SCREEN_CLEAR();

        case 'n':
        case 'N':
            return SETUP();

        default:
            break;
        }
    }
}

/**
 * @brief Start the kernel
 *
 */
void start()
{
    SCREEN_CLEAR();

    running = 1;

    SETUP();
    shell();
}
