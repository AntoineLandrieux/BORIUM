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

char *_User;
unsigned char _Running = False;

/**
 * @brief Connect to your session
 * @author Antoine LANDRIEUX
 * TODO: Create a multi-user system
 *
 * @return boolean
 */
static boolean LOGIN()
{
    char user[] =  {
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0
    };

    PUTS("\nlogin: ");
    GETS(user, sizeof(user));

    CPUTS("You have been logged in as ", 0xA);
    CPUTS(user, 0xA);
    PUTC('\n');

    _User = &0[user];
    return True;
}

static void KEYBOARD_SELECTOR()
{
    PUTS("\n KEYBOARD\n\n ");
 
    CPUTS("\t1) QWERTY\t2) AZERTY\t\n", 0xF0);
    CPUTS("\n Tap the keyboard number on your keyboard to activate it\n It doesn't work with numpad!\n", 0xC);

    while (1)
    {
        switch (GETC())
        {
        case '1':
            return KEYBOARD_INIT(QUERTY);
        
        case '2':
            return KEYBOARD_INIT(AZERTY);
        
        default:
            break;
        }
    }
}

static void setup()
{
    PUTS("\n\tBORIUM [Antoine LANDRIEUX MIT license]\n");
    KEYBOARD_SELECTOR();
    while (!LOGIN()) {;}
}

void start()
{
    SCREEN_CLEAR();
    _Running = True;

    setup();
    shell();
}
