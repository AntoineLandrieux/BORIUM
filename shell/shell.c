#include "../driver/x86/io/keyboard.h"
#include "../driver/x86/io/video.h"

#include "../kernel/kernel.h"

#include "shell.h"

#include <types.h>

/*
 * shell.c

 * Antoine LANDRIEUX
 * BORIUM <https://github.com/AntoineLandrieux/BORIUM>
 *
 * MIT License
*/

void help()
{
    CPUTS("help  : show all commands | reload   : reset your computer\n", 0x7);
    CPUTS("echo  : print message     | license  : show license\n", 0x7);
    CPUTS("clear : clear screen      | shutdown : turn off your computer\n", 0x7);
}

boolean strcmp(char *_Dest, const char *_Source, uint64 _Size)
{
    for (uint64 i = 0; i < _Size; i++)
        if (_Dest[i] != _Source[i])
            return False;
    return True;
}

// TODO: See notes.txt
void ParseShell(char *_Code)
{
    if (!*_Code)
        return;

    if (strcmp(_Code, "help", 4))
        help();

    else if (strcmp(_Code, "echo", 4))
        for (uint8 i = 5; _Code[i]; i++)
            PUTC(_Code[i]);

    else if (strcmp(_Code, "clear", 5))
        SCREEN_CLEAR();

    else if (strcmp(_Code, "reload", 6))
        return start();

    else if (strcmp(_Code, "license", 7))
        CPUTS("Antoine LANDRIEUX (MIT license)\n", 0x7);

    else if (strcmp(_Code, "shutdown", 8))
        _Running = False;

    else
    {
        CPUTS("Unknown command:\nAt: ", 0xC0);
        CPUTS(_Code, 0xC0);
    }
}

void shell()
{
    while (_Running)
    {
        char code[25] = {
            0, 0, 0, 0, 0,
            0, 0, 0, 0, 0,
            0, 0, 0, 0, 0,
            0, 0, 0, 0, 0,
            0, 0, 0, 0, 0
        };

        PUTC('\n');
        CPUTS(_User, 0xC);
        CPUTC('@', 0xE);
        CPUTS("Borium", 0xA);
        PUTS("$ ");

        GETS(code, sizeof(code));
        ParseShell(code);
    }
}
