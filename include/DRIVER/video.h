#ifndef __VIDEO_H__
#define __VIDEO_H__ 0x1

/**
 *
 *  _____  _____ _____ _____ _   _ __  __
 * | ___ \|  _  | ___ \_   _| | | |  \/  |
 * | |_/ /| | | | |_/ / | | | | | | .  . |
 * | ___ \| | | |    /  | | | | | | |\/| |
 * | |_/ /\ \_/ / |\ \ _| |_| |_| | |  | |
 * \____/  \___/\_| \_|\___/ \___/\_|  |_/
 *
 * Antoine LANDRIEUX (MIT License) <video.h>
 * <https://github.com/AntoineLandrieux/BORIUM/>
 *
 */

/* ADDRESSES */

#define VGA_DRAW_ADDRESS 0xA0000
#define VGA_TEXT_ADDRESS 0xB8000

/* DRAW */

#define SCREEN_DRAW_WIDTH 320
#define SCREEN_DRAW_HEIGHT 200

#define SCREEN_DRAW (SCREEN_DRAW_HEIGHT * SCREEN_DRAW_WIDTH)

/* TEXT */

#define SCREEN_TEXT_WIDTH 80
#define SCREEN_TEXT_HEIGHT 25

#define SCREEN_TEXT (SCREEN_TEXT_HEIGHT * SCREEN_TEXT_WIDTH)

/**
 * @brief Outb
 *
 * @param port
 * @param value
 */
void outb(unsigned short port, unsigned char value);

/**
 * @brief If move cursor
 *
 * @param move_cursor
 */
void UPDATE_AND_MOVE_CURSOR(unsigned char move_cursor);

/**
 * @brief Enable text blinking
 *
 */
void TEXT_BLINKING(unsigned char enable);

/**
 * @brief Place a pixel to the screen
 *
 * @param x
 * @param y
 * @param color
 */
void PUT_PIXEL(unsigned short x, unsigned short y, unsigned char color);

/**
 * @brief Fill rectangle to the screen
 *
 * @param x
 * @param y
 * @param w
 * @param h
 * @param color
 */
void FILL_RECT(unsigned short x, unsigned short y, unsigned short w, unsigned short h, unsigned char color);

/**
 * @brief Writes single character to stream output at current position (colored)
 *
 * @param character
 * @param color
 */
void CPUTC(const char character, const unsigned char color);

/**
 * @brief Writes strings to stream output at current position (colored)
 *
 * @param string
 * @param color
 */
void CPUTS(const char *string, const unsigned char color);

/**
 * @brief Writes single character to stream output at current position
 *
 * @param character
 */
void PUTC(const char character);

/**
 * @brief Writes strings to stream output at current position
 *
 * @param string
 */
void PUTS(const char *string);

/**
 * @brief Get cursor location
 *
 * @return unsigned short
 */
unsigned short GET_CURSOR();

/**
 * @brief Set cursor location
 *
 * @param cursor
 */
void SET_CURSOR(unsigned short cursor);

/**
 * @brief Set local console color
 *
 * @param color
 */
void SET_LOCAL_COLOR(unsigned char color);

/**
 * @brief Set global console color
 *
 * @param color
 */
void SET_GLOBAL_COLOR(unsigned char color);

/**
 * @brief Get global console color
 *
 * @param color
 */
unsigned char GET_GLOBAL_COLOR(void);

/**
 * @brief Clear screen
 *
 */
void SCREEN_CLEAR();

#endif /* __VIDEO_H__ */