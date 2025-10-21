#ifndef __VIDEO_H__
#define __VIDEO_H__ 0x1

#include "io.h"

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
 * <https://github.com/AntoineLandrieux/x86driver/>
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
 *
 * Cursor Management
 *
 */

/**
 * @brief Enables or disables automatic cursor movement
 *
 * @param enable
 */
void UPDATE_AND_MOVE_CURSOR(unsigned char enable);

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
void PUT_PIXEL(unsigned short x, unsigned short y, unsigned char color);

/**
 * @brief Fills a rectangle in graphics mode with the specified color
 *
 * @param x
 * @param y
 * @param w
 * @param h
 * @param color
 */
void FILL_RECT(unsigned short x, unsigned short y, unsigned short w, unsigned short h, unsigned char color);

/**
 *
 * Text Output Functions
 *
 */

/**
 * @brief Clears the graphics and text screen
 *
 */
void SCREEN_CLEAR();

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
 * @brief Registers a callback function to be called on screen updates
 *
 * @param callback
 */
void REGISTER_SCREEN_UPDATE_CALLBACK(void (*callback)(void));

/**
 *
 * Color Control
 *
 */

/**
 * @brief Enables or disables text blinking by setting the BLINK mask
 *
 */
void TEXT_BLINKING(unsigned char enable);

/**
 * @brief Sets the global color
 *
 * @param color
 */
void SET_GLOBAL_COLOR(unsigned char color);

/**
 * @brief Returns the current global color.
 *
 * @param color
 */
unsigned char GET_GLOBAL_COLOR(void);

#endif /* __VIDEO_H__ */