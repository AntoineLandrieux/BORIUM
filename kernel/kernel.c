#include <DRIVER/keyboard.h>
#include <DRIVER/speaker.h>
#include <DRIVER/video.h>

#include <STD/stdlib.h>
#include <STD/stdint.h>

#include <SOARE/SOARE.h>

/**
 *
 *  _____  _____ _____ _____ _   _ __  __
 * | ___ \|  _  | ___ \_   _| | | |  \/  |
 * | |_/ /| | | | |_/ / | | | | | | .  . |
 * | ___ \| | | |    /  | | | | | | |\/| |
 * | |_/ /\ \_/ / |\ \ _| |_| |_| | |  | |
 * \____/  \___/\_| \_|\___/ \___/\_|  |_/
 *
 * Antoine LANDRIEUX (MIT License) <kernel.c>
 * <https://github.com/AntoineLandrieux/BORIUM/>
 *
 */

#include <kernel.h>

// Indicates if the kernel main loop is running.
unsigned char running = 0;

// Stores the current username (max 20 chars).
char USERNAME[20] = {0};

#define SCANCODE_ENTER 0x1C
#define SCANCODE_UP 0x48
#define SCANCODE_DOWN 0x50

/**
 * @brief Executes a shell command using the SOARE interpreter.
 *
 * @param command
 * @return int
 */
int system(char *command)
{
    // Returns errorlevel
    return Execute("shell", command);
}

/**
 * @brief Displays a formatted line number with inverted color attributes.
 *
 * @param line
 */
static void LINE_NUMBER(uint16_t line)
{
    // Buffer to hold the ASCII representation of the line number
    // (max 2 digits + null)
    char lines[3] = {0, 0, 0};

    // Swap foreground and background color bits
    unsigned char color = GET_GLOBAL_COLOR();
    color = (color >> 4) | (color << 4);

    // Limit the line number to two digits (0–99)
    line %= 100;

    // Print leading zero for single-digit numbers
    // (e.g., "03")
    if (line < 10)
        CPUTC('0', color);

    // Convert int to string
    itoa(lines, sizeof(lines), line);

    // Print the line number string with the inverted color
    CPUTS(lines, color);
    CPUTC('.', color);
    PUTC(' ');
}

/**
 * @brief Implements a simple line-based text editor
 *
 */
void EDITOR(void)
{
#define __MAX_LINE_EDITOR__ 100
    char file[__MAX_LINE_EDITOR__ * __SOARE_MAX_INPUT__] = {0};
    char user[__SOARE_MAX_INPUT__] = {0};

    // Displays instructions
    PUTS(
        //
        "\n"
        " EDITOR MODE\n"
        " \t TYPE '?exit' or '?cancel' to quit\n"
        " \t TYPE '?run' or '?commit' to execute\n"
        "\n"
        //
    );

    // Reads up to 100 lines from the user.
    for (unsigned short i = 0; i < __MAX_LINE_EDITOR__; i++)
    {
        LINE_NUMBER(i + 1);

        GETS(user, sizeof(user));
        strcat(file, user);

        // Special commands:
        //  - ?exit, ?cancel to quit;
        //  - ?run, ?commit to execute.

        if (strstr(user, "?run") || strstr(user, "?commit"))
            break;

        if (strstr(user, "?exit") || strstr(user, "?cancel"))
            return;
    }

    PUTC('\n');
    // Executes the collected text via the SOARE interpreter.
    Execute("editor", file);
}

/**
 * @brief Allows the user to select the keyboard layout (QUERTY/AZERTY) using arrow keys and Enter.
 *
 */
static void KEYBOARD_SELECTOR(void)
{
    CPUTS("\n KEYBOARD SELECT   \n", 0x1F);

    TEXT_BLINKING(1);
    CPUTS(" USE ARROW KEY AND PRESS ENTER TO SELECT \n\n", 0xF4);
    TEXT_BLINKING(0);

    uint8_t selected = QUERTY;
    UPDATE_AND_MOVE_CURSOR(0);
    uint16_t cursor = GET_CURSOR();

    while (INB(KEYBOARD_PORT) == SCANCODE_ENTER)
        /* pass */;

    while (1)
    {
        PUTS("      \t 1) ");
        CPUTS(" QUERTY \n", selected == QUERTY ? 0x9F : 0xF1);
        PUTS("      \t 2) ");
        CPUTS(" AZERTY \n", selected == AZERTY ? 0x9F : 0xF1);

        switch (INB(KEYBOARD_PORT))
        {
        case SCANCODE_UP:
            selected = QUERTY;
            break;

        case SCANCODE_DOWN:
            selected = AZERTY;
            break;

        case SCANCODE_ENTER:

            while (INB(KEYBOARD_PORT) == SCANCODE_ENTER)
                /* Wait for release */;

            PUTS("\n Keyboard (");
            PUTC('0' + selected + 1);
            PUTS(") selected !\n");

            UPDATE_AND_MOVE_CURSOR(1);

            return KEYBOARD_INIT(selected);

        default:
            break;
        }

        SET_CURSOR(cursor);
    }

    UPDATE_AND_MOVE_CURSOR(1);
}

/**
 * @brief Select username (purely decorative)
 *
 */
static inline void USER_SELECTOR(void)
{
    CPUTS("\n USER SELECT       \n\n", 0x1F);

    PUTS(" Enter a username : ");
    GETS(USERNAME, sizeof(USERNAME));
}

/**
 * @brief Asks for confirmation (Y/N)
 *
 * @return uint8_t
 */
static inline uint8_t CONFIRM(void)
{
    CPUTS("\n CONFIRM          \n\n", 0x1F);
    CPUTS(" All Ok ? [Y/N] ", 0xF0);

    while (1)
    {
        switch (GETC())
        {
        // Yes
        case 'y':
        case 'Y':
            return 1;

        // No
        case 'n':
        case 'N':
            return 0;

        default:
            break;
        }
    }
}

/**
 * @brief Shell for the interpreter
 *
 */
static void shell()
{

    // Displays welcome and license information.
    PUTS(
        //
        "\n"
        "The programs included with the BORIUM system are free software;\n"
        "the exact distribution terms for each program are described in the\n"
        "<https://github.com/AntoineLandrieux/BORIUM>\n"
        "Type 'license', 'help' or see SOARE documentation\n"
        //
    );

    // While running, prompts the user for input, displays the username,
    // and executes commands using the SOARE interpreter.
    while (running)
    {
        char input[80] = {0};

        unsigned char color = (GET_GLOBAL_COLOR() >> 4) | (GET_GLOBAL_COLOR() << 4);

        CPUTS("\n ", color);
        CPUTS(USERNAME, color);
        CPUTS(" >>> ", color);
        PUTC(' ');

        GETS(input, sizeof(input));

        system(input);
    }
}

/**
 * @brief Startup sound
 *
 */
static inline void STARTUP_SOUND(void)
{
    PLAY_NOTE(NOTE_C, 2, 500);
    PLAY_NOTE(NOTE_D, 2, 500);
    PLAY_NOTE(NOTE_E, 2, 500);
    PLAY_NOTE(NOTE_D, 2, 500);
}

/**
 * @brief Startup screen
 *
 */
static inline void STARTUP_SCREEN(void)
{
    // Clears the screen and sets default color.
    SET_GLOBAL_COLOR(0xF0);
    SCREEN_CLEAR();

    SET_CURSOR(SCREEN_TEXT_WIDTH * 2);

    PUTS(
        //
        "       _____  _____ _____ _____ _   _ __  __ \n"
        "      | ___ \\|  _  | ___ \\_   _| | | |  \\/  |\n"
        "      | |_/ /| | | | |_/ / | | | | | | .  . |\n"
        "      | ___ \\| | | |    /  | | | | | | |\\/| |\n"
        "      | |_/ /\\ \\_/ / |\\ \\ _| |_| |_| | |  | |\n"
        "      \\____/  \\___/\\_| \\_|\\___/ \\___/\\_|  |_/\n"
        "       BORIUM KERNEL (MIT LICENSE)"
        //
    );

    // Sets running to true
    running = 1;

    SET_CURSOR(SCREEN_TEXT_WIDTH * 24);
    CPUTS("[-] Booting.....", 0x0F);
    STARTUP_SOUND();
    SET_CURSOR(SCREEN_TEXT_WIDTH * 24);
    CPUTS("[-] Load setup.....", 0x0F);
    SLEEP(500);
}

/**
 * @brief Setup the kernel
 *
 */
void SETUP(void)
{
    // Clears the screen and sets default color.
    SET_GLOBAL_COLOR(0xF0);
    SCREEN_CLEAR();

    CPUTS("\a\n BORIUM SETUP ", 0xF1);
    PUTS("\n =================================\n");

    // Prompts for keyboard layout, username, and theme.
    KEYBOARD_SELECTOR();
    USER_SELECTOR();

    if (!CONFIRM())
        return SETUP();

    SET_GLOBAL_COLOR(0x0F);
    SCREEN_CLEAR();
}

/**
 * @brief Start the kernel
 *
 */
void start(void)
{
    // Statup screen
    STARTUP_SCREEN();
    // Runs setup,
    SETUP();
    // And enters the shell loop.
    shell();
}
