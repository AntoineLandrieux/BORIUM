#ifndef __SHELL_H__
#define __SHELL_H__ 0x1

/*
 * shell.h

 * Antoine LANDRIEUX
 * BORIUM <https://github.com/AntoineLandrieux/BORIUM>
 *
 * MIT License
*/

struct TOKENS
{
    char _Value[25];
    struct TOKENS *_Next;
};

/**
 * @brief Initialize shell
 * @author Antoine LANDRIEUX
 * 
 */
void shell();

#endif /* __SHELL_H__ */