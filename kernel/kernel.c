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

/**
 * @brief Executes a shell command using the SOARE interpreter.
 *
 * @param command
 * @return int
 */
int system(char *command)
{
    free(Execute("system", command));
    // Returns errorlevel
    return ErrorLevel();
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

    SCREEN_CLEAR();

    // Displays instructions
    PUTS(
        //
        "\n"
        " EDITOR MODE\n"
        " \t TYPE '?exit' TO QUIT\n"
        " \t TYPE '?run' TO EXECUTE\n"
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

        if (strstr(user, "?run"))
            break;

        if (strstr(user, "?exit"))
            return;
    }

    PUTC('\n');
    // Executes the collected text via the SOARE interpreter.
    free(Execute("editor", file));
}

/**
 * @brief Allows the user to select the keyboard layout (QUERTY/AZERTY) using arrow keys and Enter.
 *
 */
static void KEYBOARD_SELECTOR(void)
{
#define SCANCODE_ENTER 0x1C
#define SCANCODE_UP 0x48
#define SCANCODE_DOWN 0x50
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
 * @brief Startup sound
 *
 */
static inline void STARTUP_SOUND(void)
{
    PLAY_NOTE(NOTE_E, 2, 500);
    PLAY_NOTE(NOTE_E, 3, 500);
    PLAY_NOTE(NOTE_E, 4, 500);
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

    PUTS(
        //
        "\n\n"
        "\n  BB##"
        "\n  BB####BBB                               II"
        "\n  BB####BBB                     RRRRRR    II"
        "\n  BB   ##BB       OOO          RRRRRRR#"
        "\n  BB####BBB    OOOOOO##        RR######                        MMMMMMMMMM"
        "\n BBB####BBB    OOOO #####      RR##                UU    ##   MMMMMMMMMMMMM"
        "\n BBB########  OOO##   #####    RR##      II#      UUU#   ##   MMM#MMMM#MMMMM"
        "\n BB#####  ### OO##      ###OO  RR##      II#      UU##   ##   MMM# MM####MMM"
        "\n BB##     ### ##         ##OO  RRR#      II#      UU##   ##   MMM# MM  ##MM"
        "\n BB##      ## ##          #OO  RRR##     II#      UU##   ##   MMM# ##  ##MM"
        "\n BBB#      ## ###        ##OO   RR##     II#      UU##  ###   MMM# ##  ##M"
        "\n BBB#    #### ###      ###OOO   RR##    III##    #UUU####UUU  MMM#     ##M"
        "\n BBB########   ##########OOOO   RR##    III##### #UUUUUUUUUU  MM##     ##M"
        "\n BBB######      ###OOOOOOOOO            IIIIIIII   UUUUUUUU     ##     ##M"
        "\n BB#               OOOOOOO               IIIIIII      UUUU             ##M"
        "\n                                                                         M"
        "\n\n"
        //
    );

    // Sets running to true
    running = 1;

    SET_CURSOR(1920);
    CPUTS("[-] Booting.....", 0x0F);
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
 * @brief Shell for the interpreter
 *
 */
static void shell()
{

    soare_kill();
    soare_init();

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
        char input[__SOARE_MAX_INPUT__] = {0};

        unsigned char color = (GET_GLOBAL_COLOR() >> 4) | (GET_GLOBAL_COLOR() << 4);

        CPUTS("\n ", color);
        CPUTS(USERNAME, color);
        CPUTS(" >>> ", color);
        PUTC(' ');

        GETS(input, sizeof(input));

        char *result = Execute("shell", input);
        if (result)
            CPUTS(result, 0xA);
        free(result);
    }

    soare_kill();
}

/**
 * @brief Start the kernel
 *
 */
void start(void)
{
    // Statup screen
    STARTUP_SCREEN();
    STARTUP_SOUND();
    // Runs setup
    SETUP();
    // Initializes SOARE kernel functions
    INIT_SOARE_KERNEL();
    // And enters the shell loop
    shell();
}
