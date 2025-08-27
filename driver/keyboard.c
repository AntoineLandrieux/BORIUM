#include <DRIVER/keyboard.h>
#include <DRIVER/video.h>

#include <STD/stddef.h>
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
 * Antoine LANDRIEUX (MIT License) <keyboard.c>
 * <https://github.com/AntoineLandrieux/BORIUM/>
 *
 */

static KEYBOARD_LAYOUT keyboard = QUERTY;

// Keyboard layout
static const char KEYBOARDS[MAX_KEYBOARD_LAYOUT][58] = {
    /* QWERTY */
    {
        //
        0, '`', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '=', '\b',
        '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '{', '}', '\n',
        0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ':', '"', 0,
        0, 0, 'z', 'x', 'c', 'v', 'b', 'n', 'm', '<', '>', '?', 0, 0, 0, ' '
        //
    },
    /* AZERTY */
    {
        //
        0, 0, '&', '<', '"', '\'', '(', '-', '>', '_', ']', '@', ')', '=', '\b',
        '\t', 'a', 'z', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '^', '$', '\n',
        0, 'q', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'm', '[', 0,
        0, '*', 'w', 'x', 'c', 'v', 'b', 'n', ',', ';', ':', '!', 0, 0, 0, ' '
        //
    }
    //
};

// Keyboard layout (shifted)
static const char SHIFTED_KEYBOARDS[MAX_KEYBOARD_LAYOUT][58] = {
    /* QWERTY */
    {
        //
        0, '~', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '+', '\b',
        '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', '\n',
        0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '\'', 0,
        0, '\\', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.', '/', 0, 0, 0, ' '
        //
    },
    /* AZERTY */
    {
        //
        0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 0, '+', '\b',
        '\t', 'A', 'Z', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', 0, '*', '\n',
        0, 'Q', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 'M', '%', 0,
        0, '\\', 'W', 'X', 'C', 'V', 'B', 'N', '?', '.', '/', 0, 0, 0, 0, ' '
        //
    }
    //
};

/**
 * @brief Input byte
 * 
 * @param port 
 * @return unsigned char 
 */
unsigned char inb(unsigned short port)
{
    unsigned char data;
    __asm__ __volatile__("inb %1, %0" : "=a"(data) : "Nd"(port));
    return data;
}

/**
 * @brief Initialize keyboard
 *
 * @param _Keyboard
 */
void KEYBOARD_INIT(KEYBOARD_LAYOUT _Keyboard)
{
    keyboard = _Keyboard;
}

/**
 * @brief Get ascii char
 * 
 * @param keycode 
 * @param shift_pressed 
 * @return char 
 */
static inline char ascii_char(uint8_t keycode, uint8_t shift_pressed)
{
    if (keycode >= sizeof(KEYBOARDS[keyboard]))
        return 0;
    return shift_pressed ? SHIFTED_KEYBOARDS[keyboard][keycode] : KEYBOARDS[keyboard][keycode];
}

/**
 * @brief Is shift key ?
 * 
 * @param keycode 
 * @return uint8_t 
 */
static inline uint8_t is_shift_key(uint8_t keycode)
{
    return keycode == 0x2A || keycode == 0x36;
}

/**
 * @brief Is shift key release ?
 * 
 * @param keycode 
 * @return uint8_t 
 */
static inline uint8_t is_shift_release(uint8_t keycode)
{
    return keycode == 0xAA || keycode == 0xB6;
}

/**
 * @brief Get char
 *
 * @return char
 */
char GETC()
{
    static uint8_t shifted = 0;

    char character = 0;

    while (!character)
    {
        uint8_t keycode = inb(KEYBOARD_PORT);

        if (is_shift_key(keycode))
        {
            shifted = 1;
            continue;
        }

        if (is_shift_release(keycode))
        {
            shifted = 0;
            continue;
        }

        character = ascii_char(keycode, shifted);

        // Wait for key release
        while (inb(KEYBOARD_PORT) == keycode)
            /* pass */;
    }

    return character;
}

/**
 * @brief Get string
 *
 * @param dest
 * @param size
 */
void GETS(char *dest, long unsigned int size)
{
    if (size <= 0)
        return;

    // Length
    size_t tlen = 0;

    while (1)
    {
        char c = GETC();

        // Backspace
        if (c == '\b')
        {
            if (tlen)
            {
                tlen--;
                dest--;
                PUTC(c);
            }
            *dest = 0;
            continue;
        }
        
        // Enter key
        if (c == '\n')
            break;

        // Prevent overflow
        if (tlen >= size)
            continue;

        // Print character
        PUTC(c);

        // Update dest
        *dest = c;

        // Increment tlen and dest pointer
        tlen++;
        dest++;
    }

    *dest = 0;
    PUTC('\n');
}
