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

char QUERTY[] = {
    0, 0,
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 0, 0, 0, '\t',
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', 0, 0, 0xA, 0,
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 0, 0, 0, 0, 0,
    'z', 'x', 'c', 'v', 'b', 'n', 'm', 0, 0, 0, 0, 0, 0, ' '};

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
    if (_KeyCode >= KEY_1 && _KeyCode <= sizeof(QUERTY))
        return QUERTY[_KeyCode];
    return 0;
}

void GETS(char *_Dest, unsigned int _Size)
{
    if (_Size <= 0)
        return;

    unsigned int n = 0;
    char c = 0;

    while (1)
    {
        c = inb(KEYBOARD_PORT);

        if (c == KEY_ENTER)
            break;

        if (c == KEY_BACKSPACE && n > 0)
        {
            n--;
            _Dest[n] = ' ';
            CURSOR_MOVE_LEFT(1);
            PUTC(' ');
            CURSOR_MOVE_LEFT(1);
        }

        c = ascii_char(c);

        if (c && n < _Size)
        {
            _Dest[n] = c;
            PUTC(c);
            n++;
        }

        c = 0;
        sleep(0x2FFFFF0);
    }

    sleep(0x2FFFFF0);
    PUTC('\n');
}
