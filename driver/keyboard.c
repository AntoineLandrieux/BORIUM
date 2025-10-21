#include <DRIVER/keyboard.h>
#include <DRIVER/video.h>

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
 * <https://github.com/AntoineLandrieux/x86driver/>
 *
 */

// Stores the current keyboard layout.
static KEYBOARD_LAYOUT KEYBOARD = QWERTY;

// Keyboard layout (Keymaps)
static const char KEYBOARDS[][58] = {
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

// Keyboard layout (Shifted Keymap)
static const char SHIFTED_KEYBOARDS[][58] = {
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
 * @brief Sets the current keyboard layout (QWERTY or AZERTY).
 *
 * @param keyboard
 */
void KEYBOARD_INIT(KEYBOARD_LAYOUT keyboard)
{
    KEYBOARD = keyboard;
}

/**
 * @brief Keycode to ASCII Conversion
 *
 * @param keycode
 * @param shift_pressed
 * @return char
 */
static inline char ascii_char(unsigned char keycode, unsigned char shift_pressed)
{
    // Converts a keycode to its ASCII character,
    // considering the current layout and shift state.
    if (keycode >= sizeof(KEYBOARDS[KEYBOARD]))
        return 0;
    return shift_pressed ? SHIFTED_KEYBOARDS[KEYBOARD][keycode] : KEYBOARDS[KEYBOARD][keycode];
}

/**
 * @brief Returns true if the keycode is a shift key press.
 *
 * @param keycode
 * @return unsigned char
 */
static inline unsigned char is_shift_key(unsigned char keycode)
{
    return keycode == 0x2A || keycode == 0x36;
}

/**
 * @brief Returns true if the keycode is a shift key release.
 *
 * @param keycode
 * @return unsigned char
 */
static inline unsigned char is_shift_release(unsigned char keycode)
{
    return keycode == 0xAA || keycode == 0xB6;
}

/**
 * @brief Single Character Input
 *
 * @return char
 */
char GETC(void)
{
    static unsigned char shifted = 0;

    char character = 0;

    while (!character)
    {
        // Waits for a key press,
        unsigned char keycode = INB(KEYBOARD_PORT);

        // handles shift state,
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

        // and returns the corresponding ASCII character
        character = ascii_char(keycode, shifted);

        // Wait for key release
        while (INB(KEYBOARD_PORT) == keycode)
            /* pass */;
    }

    return character;
}

/**
 * @brief String Input
 *
 * @param dest
 * @param size
 */
void GETS(char *dest, long unsigned int size)
{
    if (size <= 0)
        return;

    // Length
    unsigned long tlen = 0;

    while (1)
    {
        char c = GETC();

        // Handles backspace
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

        // Echoes each character to the screen as it is typed
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
