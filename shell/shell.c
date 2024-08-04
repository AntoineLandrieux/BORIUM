#include "../driver/x86/io/keyboard.h"
#include "../driver/x86/io/video.h"

#include "../kernel/kernel.h"

#include <types.h>

/*
 * shell.c

 * Antoine LANDRIEUX
 * BORIUM <https://github.com/AntoineLandrieux/BORIUM>
 *
 * MIT License
*/

boolean strcmp(char *_Dest, const char *_Source, uint64 _Size)
{
    for (uint64 i = 0; i < _Size; i++)
        if (_Dest[i] != _Source[i])
            return False;
    return True;
}

// TODO: See notes.txt
void ParseShellCode(char *_Code)
{
    if (strcmp(_Code, "help", 4))
    {
        PUTS("help     : show all commands\n");
        PUTS("echo     : print message\n");
        PUTS("reload   : reset your computer\n");
        PUTS("license  : show license\n");
        PUTS("shutdown : turn off your computer");
    }

    else if (strcmp(_Code, "echo", 4))
        for (uint8 i = 5; _Code[i]; i++)
            PUTC(_Code[i]);

    else if (strcmp(_Code, "reload", 6))
        return start();

    else if (strcmp(_Code, "license", 6))
        PUTS("Antoine LANDRIEUX (MIT license)");

    else if (strcmp(_Code, "shutdown", 8))
        _Running = False;

    else
    {
        CPUTS("Unknown command:\nAt: ", 0xC),
            CPUTS(_Code, 0xC);
    }
}

void shell()
{
    while (_Running)
    {
        char code[20] = {
            0, 0, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0
        };

        PUTS("\n Borium@");
        PUTS(_User);
        PUTS("$ ");
        GETS(code, sizeof(code));
        ParseShellCode(code);
        PUTC('\n');
    }
}
