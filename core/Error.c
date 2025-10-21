#include <STD/stdlib.h>
#include <STD/stdarg.h>

#include <DRIVER/keyboard.h>
#include <DRIVER/video.h>

/**
 *  _____  _____  ___  ______ _____
 * /  ___||  _  |/ _ \ | ___ \  ___|
 * \ `--. | | | / /_\ \| |_/ / |__
 *  `--. \| | | |  _  ||    /|  __|
 * /\__/ /\ \_/ / | | || |\ \| |___
 * \____/  \___/\_| |_/\_| \_\____/
 *
 * Antoine LANDRIEUX (MIT License) <Error.c>
 * <https://github.com/AntoineLandrieux/SOARE/>
 *
 */

#include <SOARE/SOARE.h>

/* Enable/disable error display */
static unsigned char enable = 1;

/* Error level */
static char errorlevel = EXIT_SUCCESS;

/* Exceptions */
static char *Exceptions[] = {

    "InterpreterError",
    "FileError",
    "CharacterError",
    "SyntaxError",
    "ValueError",
    "UnexpectedNear",
    "UndefinedReference",
    "ObjectIsNotCallable",
    "VariableDefinedAsFunction",
    "MathError",
    "InvalidEscapeSequence",
    "IndexOutOfRange",
    "DivideByZero",
    "RaiseException"

};

/**
 * @brief Error display
 *
 * @return unsigned char
 */
unsigned char AsIgnoredException(void)
{
    return !enable;
}

/**
 * @brief Enable/disable error display
 *
 * @param ignore
 */
void IgnoreException(unsigned char ignore)
{
    enable = !ignore;
}

/**
 * @brief Clears errors
 *
 */
void ClearException(void)
{
    errorlevel = EXIT_SUCCESS;
}

/**
 * @brief Returns the error level
 *
 * @return char
 */
char ErrorLevel(void)
{
    return errorlevel;
}

/**
 * @brief Create a new error, and display it
 *
 * @param error
 * @param string
 * @param file
 * @return void* (always returns NULL)
 */
void *LeaveException(SoareExceptions error, char *string, Document file)
{
    // If the errors are disabled, nothing is displayed
    if (enable)
    {
        char col[200], ln[200];

        itoa(ln, sizeof(ln), (int)file.ln);
        itoa(col, sizeof(col), (int)file.col);

        unsigned char color = GET_GLOBAL_COLOR();
        SET_GLOBAL_COLOR(0x4);

        soare_write("\aExcept: ");
        soare_write(Exceptions[error]);
        soare_write("\n        \"");
        soare_write(string);
        soare_write("\"\n         ^~~~\n");
        soare_write("        At file: ");
        soare_write(file.file);
        soare_write(":");
        soare_write(ln);
        soare_write(":");
        soare_write(col);
        soare_write("\n");

        SET_GLOBAL_COLOR(color);
    }

    // Set error at level EXIT_FAILURE (1)
    errorlevel = EXIT_FAILURE;
    return NULL;
}
