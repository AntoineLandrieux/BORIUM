#ifndef __VIDEO_H__
#define __VIDEO_H__ 0x1

/*
 * video.h

 * Antoine LANDRIEUX
 * BORIUM <https://github.com/AntoineLandrieux/BORIUM>
 *
 * MIT License
*/

#define VGA_ADDRESS 0xB8000

#define SCREEN_HEIGHT 25
#define SCREEN_WIDTH 80

#define SCREEN (SCREEN_HEIGHT * SCREEN_WIDTH * 2)

#define TAB_SIZE 4

/**
 * @brief cputc writes single character to stream output at current position
 * @author Antoine LANDRIEUX
 * 
 * @param _Char 1 byte
 * @param _Color 1 byte (4 bits Foreground - 4 bits Background)
 */
void CPUTC(const char _Char, const unsigned char _Color);

/**
 * @brief cputs writes strings to stream output at current position
 * @author Antoine LANDRIEUX
 * 
 * @param _String <pointer>
 * @param _Color 1 byte (4 bits Background - 4 bits Foreground)
 */
void CPUTS(const char *_String, const unsigned char _Color);

/**
 * @brief cputc writes single character to stream output at current position
 * @author Antoine LANDRIEUX
 * 
 * @param _Char 1 byte
 */
void PUTC(const char _Char);

/**
 * @brief cputs writes strings to stream output at current position
 * @author Antoine LANDRIEUX
 * 
 * @param _String <pointer>
 */
void PUTS(const char *_String);

/**
 * @brief Move cursor
 * @author Antoine LANDRIEUX
 * 
 * @param _Move
 */
void CURSOR_MOVE_LEFT(unsigned int _Move);

/**
 * @brief Screen scroll
 * 
 */
void SCREEN_SCROLL();

/**
 * @brief Initialize screen color
 * @author Antoine LANDRIEUX
 * 
 * @param _Color 1 byte (4 bits Background - 4 bits Foreground)
 */
void SCREEN_COLOR(const unsigned char _Color);

/**
 * @brief Initialize screen
 * @author Antoine LANDRIEUX
 * 
 */
void SCREEN_INIT();

#endif /* __VIDEO_H__ */