#include "video.h"

#include <types.h>

/*
 * video.c

 * Antoine LANDRIEUX
 * BORIUM <https://github.com/AntoineLandrieux/BORIUM>
 *
 * MIT License
*/

static uint8 screen_color = 0x0F;
static uint16 VGA_POINTER = 0;

void CPUTC(const char _Char, const unsigned char _Color)
{
    char *VIDEO = (char *)VGA_ADDRESS;

    if (VGA_POINTER >= (SCREEN * 2))
        SCREEN_SCROLL();

    switch (_Char)
    {
    case '\0':
        return;

    case '\n':
        VGA_POINTER += (SCREEN_WIDTH * 2);
        __attribute__((fallthrough));
    case '\r':
        VGA_POINTER -= (VGA_POINTER % (SCREEN_WIDTH * 2));
        return;

    case '\t':
        /* TODO: fix '\t' case */
        for (int i = 0; i < TAB_SIZE; i++)
            CPUTC(' ', _Color);
        return;

    default:
        VIDEO[(volatile uint16)VGA_POINTER++] = _Char;
        VIDEO[(volatile uint16)VGA_POINTER++] = _Color;
        return;
    }
}

void CPUTS(const char *_String, const unsigned char _Color)
{
    for (; *_String; (volatile char *)_String++)
        CPUTC(*_String, _Color);
}

void PUTC(const char _Char)
{
    CPUTC(_Char, screen_color);
}

void PUTS(const char *_String)
{
    CPUTS(_String, screen_color);
}

void CURSOR_MOVE_LEFT(unsigned int _Move)
{
    VGA_POINTER -= (_Move * 2);
}

void CURSOR_MOVE_RIGHT(unsigned int _Move)
{
    VGA_POINTER += (_Move * 2);
}

void SCREEN_SCROLL()
{
    char *VIDEO = (char *)VGA_ADDRESS;
    VGA_POINTER = ((SCREEN * 2) - (SCREEN_WIDTH * 2));

    for (uint16 i = 0; i < VGA_POINTER; i++)
    {
        VIDEO[i] = VIDEO[i + (SCREEN_WIDTH * 2)];
        VIDEO[i + (SCREEN_WIDTH * 2)] = 0;
    }
}

void SCREEN_COLOR(const unsigned char _Color)
{
    char *VIDEO = (char *)VGA_ADDRESS;
    screen_color = _Color;

    for (uint16 i = 1; i < (SCREEN * 2); i += 2)
        VIDEO[i] = screen_color;
}

void SCREEN_CLEAR()
{
    char *VIDEO = (char *)VGA_ADDRESS;
    VGA_POINTER = 0;

    for (uint16 i = 0; i < (SCREEN * 2); i += 2)
    {
        VIDEO[i] = 0;
        VIDEO[i + 1] = screen_color;
    }
}

void SCREEN_INIT()
{
    screen_color = 0xF;
    SCREEN_CLEAR();
}
