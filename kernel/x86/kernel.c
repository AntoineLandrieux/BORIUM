#include "../../driver/x86/io/keyboard.h"
#include "../../driver/x86/io/video.h"
#include "../shell/shell.h"

#include "../kernel.h"

#include <types.h>

/*
 * kernel.c

 * Antoine LANDRIEUX
 * BORIUM <https://github.com/AntoineLandrieux/BORIUM>
 *
 * MIT License
*/

char _User[20] = {0};
unsigned char _Running = False;

#define THEME_DEFAULT 0x0F
#define THEME_LIGHT 0xF3
#define THEME_GIRLY 0xDF
#define THEME_POWERSHELL 0x1F

/**
 * @brief Connect to your session
 * @author Antoine LANDRIEUX
 * TODO: Create a multi-user system
 *
 * @return boolean
 */
static boolean LOGIN()
{
    PUTS("login: ");
    GETS(_User, sizeof(_User));

    CPUTS("You have been logged in as ", 0xA);
    CPUTS(_User, 0xA);
    PUTS("\n\n");

    return True;
}

static void THEME_SELECTOR()
{
    PUTS(" TERMINAL THEME\n\n");

    PUTC('\t');
    CPUTS("\t(0) DEFAULT   \t", 0x8F);
    CPUTS("\t(1) LIGHT     \t\n", THEME_LIGHT);
    PUTC('\t');
    CPUTS("\t(2) GIRLY     \t", THEME_GIRLY);
    CPUTS("\t(3) POWERSHELL\t\n", THEME_POWERSHELL);
    CPUTS("\n Tap the theme number on your keyboard to activate it\n It doesn't work with numpad!\n\n", 0xC);

    while (1)
    {
        switch (inb(KEYBOARD_PORT))
        {
        case KEY_0:
            return;

        case KEY_1:
            return SCREEN_COLOR(THEME_LIGHT);

        case KEY_2:
            return SCREEN_COLOR(THEME_GIRLY);

        case KEY_3:
            return SCREEN_COLOR(THEME_POWERSHELL);

        default:
            break;
        }
    }
}

void start()
{
    _Running = True;
    SCREEN_INIT();

    PUTS("\n\tBORIUM [QWERTY] Antoine LANDRIEUX MIT license\n\n");
    while (!LOGIN()) {;}
    THEME_SELECTOR();

    sleep(0xF222222);
    shell();
    SCREEN_INIT();
}
