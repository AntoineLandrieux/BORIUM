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
 * @author Antoine LANDRIEUX
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

        CPUTS("Except: ", 0xC);
        CPUTS(Exceptions[error], 0xC);
        CPUTS("\n        \"", 0xC);
        CPUTS(string, 0xC);
        CPUTS("\"\n         ^~~~\n", 0xC);
        CPUTS("        At file: ", 0xC);
        CPUTS(file.file, 0xC);
        CPUTC(':', 0xC);
        CPUTS(ln, 0xC);
        CPUTC(':', 0xC);
        CPUTS(col, 0xC);

        PUTC('\n');
    }

    // set error at level 1
    errorlevel = 1;

    // Store the error in the `__ERROR__` variable
    MemSet(MemGet(MEMORY, "__ERROR__"), Exceptions[error]);

    return NULL;
}
