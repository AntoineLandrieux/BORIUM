#include "keyboard.h"
#include "video.h"

#include <types.h>

/*
 * keyboard.c

 * Antoine LANDRIEUX
 * BORIUM <https://github.com/AntoineLandrieux/BORIUM>
 *
 * MIT License
*/

static KEYBOARD_INPUT keyboard = QUERTY;

static const char KEYBOARDS[][58] = {
    /* QWERTY */
    {
        0, '~', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '+', 0,
        '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '{', '}', '\n',
        0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ':', '"', 0,
        0, 0, 'z', 'x', 'c', 'v', 'b', 'n', 'm', '<', '>', '?', 0, 0, 0, ' '
    },

    /* AZERTY */
    {
        0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 0, '+', 0,
        '\t', 'a', 'z', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '^', '$', '\n',
        0, 'q', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'm', '%', 0,
        0, '*', 'w', 'x', 'c', 'v', 'b', 'n', ',', ';', ':', '!', 0, 0, 0, ' '
    }
};

void KEYBOARD_INIT(KEYBOARD_INPUT _Keyboard)
{
    keyboard = _Keyboard;
}

void sleep(unsigned int _Timer)
{
    while (1)
    {
        __asm__ __volatile__("nop");
        _Timer--;
        if (_Timer <= 0)
            break;
    }
}

unsigned char inb(unsigned short _Port)
{
    unsigned char data;
    __asm__ __volatile__("inb %1, %0" : "=a"(data) : "Nd"(_Port));
    return data;
}

char ascii_char(unsigned char _KeyCode)
{
    if (_KeyCode <= sizeof(KEYBOARDS[keyboard]))
        return KEYBOARDS[keyboard][_KeyCode];
    return 0;
}

char GETC()
{
    char c = 0;
    while (!c)
        c = ascii_char(inb(KEYBOARD_PORT));
    return c;
}

void GETS(char *_Dest, unsigned int _Size)
{
    if (_Size <= 0)
        return;

    unsigned int n = 0;
    char c = 0;

    while (1)
    {
        sleep(0x2FFFFF0);
        c = inb(KEYBOARD_PORT);

        if (c == KEY_BACKSPACE && n > 0)
        {
            n--;
            _Dest[n] = ' ';
            CURSOR_MOVE_LEFT(1);
            PUTC(' ');
            CURSOR_MOVE_LEFT(1);
        }

        c = ascii_char(c);

        if (!c || n > _Size)
            continue;

        if (c == '\n')
            break;

        _Dest[n] = c;
        PUTC(c);
        n++;
    }

    PUTC(c);
}
