#include <DRIVER/keyboard.h>
#include <DRIVER/speaker.h>
#include <DRIVER/video.h>

#include <STD/stdlib.h>

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
 * Antoine LANDRIEUX (MIT License) <soare_kernel.c>
 * <https://github.com/AntoineLandrieux/BORIUM/>
 *
 */

#include <kernel.h>

/**
 * @brief Show help information
 *
 */
void kw_help(void)
{
    PUTS(
        //
        "\n"
        " [ HELP - BORIUM / SOARE KERNEL ===== \n"
        " \t clear              <keyword>  Clear screen \n"
        " \t editor             <keyword>  Text editor \n"
        " \t help               <keyword>  Show commands \n"
        " \t license            <keyword>  Show license \n"
        " \t pause              <keyword>  Interrupts the execution \n"
        " \t setup              <keyword>  Change BORIUM settings \n"
        " \t chr(ascii_code)    <function> Character from ASCII code \n"
        " \t color(vga_color)   <function> Text color \n"
        " \t cursor(x; y)       <function> Set cursor location \n"
        " \t eval(code)         <function> Execute SOARE code \n"
        " \t getc()             <function> Get char \n"
        " \t input(...)         <function> Write text and ask for user input \n"
        " \t keydown(scancode)  <function> Check if a key is pressed \n"
        " \t ord(character)     <function> ASCII code from character \n"
        " \t play_note(freq; t) <function> Play frequency (freq) for a while (t) \n"
        " \t sleep(time)        <function> Pause for a while \n"
        " \t system(cmd)        <function> Execute shell code \n"
        " \t werr(...)          <function> Write text (error) \n"
        " \t write(...)         <function> Write text \n"
        "\n"
        //
    );
}

/**
 * @brief Show license information
 *
 */
void kw_license(void)
{
    PUTS(
        //
        "\n"
        " BORIUM (MIT LICENSE) \n"
        "  <https://github.com/AntoineLandrieux/BORIUM> \n"
        "  <https://github.com/AntoineLandrieux/BORIUM/blob/main/LICENSE> \n"
        "  <https://github.com/AntoineLandrieux/BORIUM/blob/main/AUTHORS> \n"
        " SOARE (MIT LICENSE) \n"
        "  <https://github.com/AntoineLandrieux/SOARE> \n"
        "  <https://github.com/AntoineLandrieux/SOARE/blob/main/LICENSE> \n"
        "  <https://github.com/AntoineLandrieux/SOARE/blob/main/AUTHORS> \n"
        " x86driver (MIT LICENSE) \n"
        "  <https://github.com/AntoineLandrieux/x86driver> \n"
        "  <https://github.com/AntoineLandrieux/x86driver/blob/main/LICENSE> \n"
        "  <https://github.com/AntoineLandrieux/x86driver/blob/main/AUTHORS> \n"
        "\n"
        //
    );
}

/**
 * @brief Pause execution until a key is pressed
 *
 */
void kw_pause(void)
{
    PUTS("\nPress any key to continue...\n");
    GETC();
}

/**
 * ======================
 * FUNCTIONS
 * =====================
 */

/**
 * @brief Get character from ASCII code
 *
 * @param args
 * @return char*
 */
char *fn_chr(soare_arguments_list args)
{
    char *arg = soare_getarg(args, 0);

    if (!args)
        return LeaveException(UndefinedReference, "ascii_code", EmptyDocument());

    char result[2] = {(char)atoi(arg), 0};
    free(arg);

    return strdup(result);
}

/**
 * @brief Set text color
 *
 * @param args
 * @return char*
 */
char *fn_color(soare_arguments_list args)
{
    char *arg = soare_getarg(args, 0);

    if (!args)
        return LeaveException(UndefinedReference, "vga_color", EmptyDocument());

    unsigned char color = (unsigned char)atoi(arg);
    SET_GLOBAL_COLOR(color);
    free(arg);

    return NULL;
}

/**
 * @brief Set cursor location
 *
 * @param args
 * @return char*
 */
char *fn_cursor(soare_arguments_list args)
{
    char *arg_x = soare_getarg(args, 0);
    char *arg_y = soare_getarg(args, 1);

    if (!args || !arg_y)
        return LeaveException(UndefinedReference, "x; y", EmptyDocument());

    unsigned short x = (unsigned short)atoi(arg_x);
    unsigned short y = (unsigned short)atoi(arg_y);

    free(arg_x);
    free(arg_y);

    SET_CURSOR(y * SCREEN_TEXT_WIDTH + x);

    return NULL;
}

/**
 * @brief Evaluate SOARE code (system too)
 *
 * @param args
 * @return char*
 */
char *fn_eval(soare_arguments_list args)
{
    char *arg = soare_getarg(args, 0);

    if (!args)
        return LeaveException(UndefinedReference, "code", EmptyDocument());

    char *result = Execute("eval", arg);
    free(arg);
    return result;
}

/**
 * @brief Get char from keyboard
 *
 * @param args
 * @return char*
 */
char *fn_getc(soare_arguments_list args)
{
    char result[2] = {GETC(), 0};
    return strdup(result);
}

/**
 * @brief Write text
 *
 * @param args
 * @return char*
 */
char *fn_write(soare_arguments_list args)
{
    char *value = NULL;

    // Loop through all arguments and print them
    for (int i = 0; 1; i++)
    {
        if (!(value = soare_getarg(args, i)))
            break;

        PUTS(value);
        free(value);
    }

    return NULL;
}

/**
 * @brief Input text from user
 *
 * @param args
 * @return char*
 */
char *fn_input(soare_arguments_list args)
{
    fn_write(args);

    char input[__SOARE_MAX_INPUT__] = {0};
    GETS(input, sizeof(input));

    return strdup(input);
}

/**
 * @brief Check if a key is pressed
 *
 * @param args
 * @return char*
 */
char *fn_keydown(soare_arguments_list args)
{
    char *arg = soare_getarg(args, 0);

    if (!args)
        return LeaveException(UndefinedReference, "scancode", EmptyDocument());

    unsigned char scancode = (unsigned char)atoi(arg);
    free(arg);

    // Check if the specified key is pressed
    if (INB(KEYBOARD_PORT) == scancode)
        return strdup("1");

    return strdup("0");
}

/**
 * @brief Get ASCII code from character
 *
 * @param args
 * @return char*
 */
char *fn_ord(soare_arguments_list args)
{
    char *arg = soare_getarg(args, 0);

    if (!args)
        return LeaveException(UndefinedReference, "character", EmptyDocument());

    int ascii_code = (int)arg[0];
    free(arg);

    char result[4] = {0};
    itoa(result, sizeof(result), ascii_code);

    return strdup(result);
}

/**
 * @brief Play frequency for a while
 *
 * @param args
 * @return char*
 */
char *fn_play_note(soare_arguments_list args)
{
    char *arg_note = soare_getarg(args, 0);
    char *arg_time = soare_getarg(args, 1);

    if (!arg_note || !arg_time)
        return LeaveException(UndefinedReference, "note; time", EmptyDocument());

    unsigned int note = (unsigned int)atoi(arg_note);
    unsigned int time = (unsigned int)atoi(arg_time);

    free(arg_note);
    free(arg_time);

    PLAY_FREQUENCY(note);
    SLEEP(time);
    DISABLE_SPEAKER();

    return NULL;
}

/**
 * @brief Sleep for a while
 *
 * @param args
 * @return char*
 */
char *fn_sleep(soare_arguments_list args)
{
    char *arg = soare_getarg(args, 0);

    if (!args)
        return LeaveException(UndefinedReference, "time", EmptyDocument());

    unsigned int time = (unsigned int)atoi(arg);
    free(arg);

    SLEEP(time);

    return NULL;
}

/**
 * @brief Write text (error)
 *
 * @param args
 * @return char*
 */
char *fn_werr(soare_arguments_list args)
{
    unsigned char color = GET_GLOBAL_COLOR();

    SET_GLOBAL_COLOR(0x4);
    fn_write(args);
    SET_GLOBAL_COLOR(color);

    return NULL;
}

/**
 * @brief Initialize SOARE kernel functions
 *
 */
void INIT_SOARE_KERNEL(void)
{
    soare_addkeyword("clear", SCREEN_CLEAR);
    soare_addkeyword("editor", EDITOR);
    soare_addkeyword("help", kw_help);
    soare_addkeyword("license", kw_license);
    soare_addkeyword("pause", kw_pause);
    soare_addkeyword("setup", SETUP);

    soare_addfunction("chr", fn_chr);
    soare_addfunction("color", fn_color);
    soare_addfunction("cursor", fn_cursor);
    soare_addfunction("eval", fn_eval);
    soare_addfunction("getc", fn_getc);
    soare_addfunction("input", fn_input);
    soare_addfunction("keydown", fn_keydown);
    soare_addfunction("ord", fn_ord);
    soare_addfunction("play_note", fn_play_note);
    soare_addfunction("sleep", fn_sleep);
    soare_addfunction("system", fn_eval);
    soare_addfunction("werr", fn_werr);
    soare_addfunction("write", fn_write);
}
