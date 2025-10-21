#include <DRIVER/video.h>
#include <DRIVER/speaker.h>

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
 * <https://github.com/AntoineLandrieux/x86driver/>
 *
 */

// Tracks the current position in video memory for text output
static unsigned short VGA_POINTER = 0;

// Stores the current global text color
static unsigned char GLOBAL_COLOR = 0x0F;

// Indicates if the cursor should be updated after output
static unsigned char MOVE_CURSOR = 0x1;

// Controls text blinking (attribute mask)
static unsigned char BLINK = 0x7F;

/**
 *
 * Cursor Management
 *
 */

/**
 * @brief Updates the hardware VGA cursor position based on VGA_POINTER
 *
 * @param row
 * @param col
 */
static void update_cursor_location(void)
{
    if (!MOVE_CURSOR)
        return;

    unsigned short position = (unsigned short)(VGA_POINTER / 2);

    OUTB(0x3D4, 0x0F);
    OUTB(0x3D5, (unsigned char)(position & 0xFF));

    OUTB(0x3D4, 0x0E);
    OUTB(0x3D5, (unsigned char)((position >> 8) & 0xFF));
}

/**
 * @brief Enables or disables automatic cursor movement
 *
 * @param enable
 */
void UPDATE_AND_MOVE_CURSOR(unsigned char enable)
{
    MOVE_CURSOR = enable;
}

/**
 * @brief Get cursor location
 *
 * @return unsigned short
 */
unsigned short GET_CURSOR()
{
    return (unsigned short)(VGA_POINTER / 2);
}

/**
 * @brief Set cursor location
 *
 * @param cursor
 */
void SET_CURSOR(unsigned short cursor)
{
    VGA_POINTER = cursor * 2;
    update_cursor_location();
}

/**
 *
 * Graphics Output
 *
 */

/**
 * @brief Draws a pixel at (x, y) in graphics mode with the specified color
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
 * @brief Fills a rectangle in graphics mode with the specified color
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
 *
 * Text Output Functions
 *
 */

/**
 * @brief Clears the graphics and text screen
 *
 */
void SCREEN_CLEAR()
{
    // Clears the graphics
    FILL_RECT(0, 0, SCREEN_DRAW_WIDTH, SCREEN_DRAW_HEIGHT, 0x00);

    // Clears the text screen
    char *VIDEO = (char *)VGA_TEXT_ADDRESS;
    VGA_POINTER = 0;

    // Fills text attributes with the global color
    for (unsigned short i = 0; i < (SCREEN_TEXT * 2); i++)
        VIDEO[i] = i % 2 ? GLOBAL_COLOR & BLINK : 0;

    // Resets the cursor
    update_cursor_location();
}

/**
 * @brief Scrolls the text screen up by one line when the end of the screen is reached.
 *
 */
static void SCREEN_TEXT_SCROLL()
{
    char *VIDEO = (char *)VGA_TEXT_ADDRESS;
    VGA_POINTER = ((SCREEN_TEXT * 2) - (SCREEN_TEXT_WIDTH * 2));

    for (unsigned short i = 0; i < VGA_POINTER; i++)
    {
        VIDEO[i] = VIDEO[i + (SCREEN_TEXT_WIDTH * 2)];
        VIDEO[i + (SCREEN_TEXT_WIDTH * 2)] = i % 2 ? GLOBAL_COLOR & BLINK : 0;
    }

    update_cursor_location();
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
        TERMINAL_BEEP();
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
        VIDEO[(volatile unsigned short)VGA_POINTER++] = color & BLINK;
        break;
    }

    update_cursor_location();
}

/**
 * @brief Writes strings to stream output at current position (colored)
 *
 * @param string
 * @param color
 */
void CPUTS(const char *string, const unsigned char color)
{
    if (!string)
        return CPUTS("(null)", color);
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
    CPUTC(character, GLOBAL_COLOR);
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
    CPUTS(string, GLOBAL_COLOR);
}

/**
 *
 * Color Control
 *
 */

/**
 * @brief Enables or disables text blinking by setting the BLINK mask
 *
 */
void TEXT_BLINKING(unsigned char enable)
{
    BLINK = enable ? 0xFF : 0x7F;
}

/**
 * @brief Sets the global color
 *
 * @param color
 */
void SET_GLOBAL_COLOR(unsigned char color)
{
    GLOBAL_COLOR = color;
}

/**
 * @brief Returns the current global color.
 *
 * @param color
 */
unsigned char GET_GLOBAL_COLOR(void)
{
    return GLOBAL_COLOR;
}
