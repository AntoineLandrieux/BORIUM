#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__ 0x1

/**
 *
 *  _____  _____ _____ _____ _   _ __  __
 * | ___ \|  _  | ___ \_   _| | | |  \/  |
 * | |_/ /| | | | |_/ / | | | | | | .  . |
 * | ___ \| | | |    /  | | | | | | |\/| |
 * | |_/ /\ \_/ / |\ \ _| |_| |_| | |  | |
 * \____/  \___/\_| \_|\___/ \___/\_|  |_/
 *
 * Antoine LANDRIEUX (MIT License) <keyboard.h>
 * <https://github.com/AntoineLandrieux/BORIUM/>
 *
 */

#define KEYBOARD_PORT 0x60

/**
 * @brief Keyboard layout selector
 */
typedef enum keyboard_layout
{

    QUERTY,
    AZERTY,

    MAX_KEYBOARD_LAYOUT

} KEYBOARD_LAYOUT;

/**
 * @brief Initialize keyboard
 *
 * @param _Keyboard
 */
void KEYBOARD_INIT(KEYBOARD_LAYOUT _Keyboard);

/**
 * @brief Get char
 *
 * @return char
 */
char GETC();

/**
 * @brief Get string
 *
 * @param dest
 * @param size
 */
void GETS(char *dest, long unsigned int size);

#endif /* __KEYBOARD_H__ */