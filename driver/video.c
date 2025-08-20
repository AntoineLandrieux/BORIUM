#include <DRIVER/video.h>

#include <STD/stdint.h>

/**
 *
 *  _____  _____ _____ _____ _   _ __  __
 * | ___ \|  _  | ___ \_   _| | | |  \/  |
 * | |_/ /| | | | |_/ / | | | | | | .  . |
 * | ___ \| | | |    /  | | | | | | |\/| |
 * | |_/ /\ \_/ / |\ \ _| |_| |_| | |  | |
 * \____/  \___/\_| \_|\___/ \___/\_|  |_/
 *
 * Antoine LANDRIEUX (MIT License) <video.c>
 * <https://github.com/AntoineLandrieux/BORIUM/>
 *
 */

// Video memory address pointer
static uint16_t VGA_POINTER = 0;

/**
 * @brief Place a pixel to the screen
 * 
 * @param x 
 * @param y 
 * @param color 
 */
void PUT_PIXEL(unsigned short x, unsigned short y, unsigned char color)
{
    if (x >= SCREEN_DRAW_WIDTH || y >= SCREEN_DRAW_HEIGHT)
        return;
    ((unsigned char *)VGA_DRAW_ADDRESS)[y * SCREEN_DRAW_WIDTH + x] = color;
}

/**
 * @brief Fill rectangle to the screen
 * 
 * @param x 
 * @param y 
 * @param w 
 * @param h 
 * @param color 
 */
void FILL_RECT(unsigned short x, unsigned short y, unsigned short w, unsigned short h, unsigned char color)
{
    for (unsigned short dy = 0; dy < h; dy++)
        for (unsigned short dx = 0; dx < w; dx++)
            PUT_PIXEL(x + dx, y + dy, color);
}

/**
 * @brief Scroll the screen
 *
 */
static void SCREEN_TEXT_SCROLL()
{
    char *VIDEO = (char *)VGA_TEXT_ADDRESS;
    VGA_POINTER = ((SCREEN_TEXT * 2) - (SCREEN_TEXT_WIDTH * 2));

    for (unsigned short i = 0; i < VGA_POINTER; i++)
    {
        VIDEO[i] = VIDEO[i + (SCREEN_TEXT_WIDTH * 2)];
        VIDEO[i + (SCREEN_TEXT_WIDTH * 2)] = 0;
    }
}

/**
 * @brief Writes single character to stream output at current position (colored)
 *
 * @param character
 * @param color
 */
void CPUTC(const char character, const unsigned char color)
{
    char *VIDEO = (char *)VGA_TEXT_ADDRESS;

    if (VGA_POINTER >= (SCREEN_TEXT * 2))
        SCREEN_TEXT_SCROLL();

    switch (character)
    {
    case '\a':
        SCREEN_CLEAR();
        break;

    case '\b':
        VGA_POINTER -= 2;
        VIDEO[VGA_POINTER] = ' ';
        break;

    case '\n':
        VGA_POINTER += (SCREEN_TEXT_WIDTH * 2);
    case '\r':
        VGA_POINTER -= (VGA_POINTER % (SCREEN_TEXT_WIDTH * 2));
    case '\0':
        break;

    default:
        VIDEO[(volatile unsigned short)VGA_POINTER++] = character;
        VIDEO[(volatile unsigned short)VGA_POINTER++] = color;
        break;
    }
}

/**
 * @brief Writes strings to stream output at current position (colored)
 *
 * @param string
 * @param color
 */
void CPUTS(const char *string, const unsigned char color)
{
    for (; *string; string++)
        CPUTC(*string, color);
}

/**
 * @brief Writes single character to stream output at current position
 *
 * @param character
 */
void PUTC(const char character)
{
    CPUTC(character, 0xF);
}

/**
 * @brief Writes strings to stream output at current position
 *
 * @param string
 */
void PUTS(const char *string)
{
    if (!string)
        return PUTS("(null)");
    CPUTS(string, 0xF);
}

/**
 * @brief Clear screen
 *
 */
void SCREEN_CLEAR()
{
    FILL_RECT(0, 0, SCREEN_DRAW_WIDTH, SCREEN_DRAW_HEIGHT, 0x00);

    char *VIDEO = (char *)VGA_TEXT_ADDRESS;
    VGA_POINTER = 0;

    for (unsigned short i = 0; i < (SCREEN_TEXT * 2); i++)
        VIDEO[i] = 0;
}
