#ifndef __STDLIB_H__
#define __STDLIB_H__ 0x1

#include "stddef.h"

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

/**
 * @brief System commands
 *
 * @param command
 * @return int
 */
int system(char *command);

/**
 * @brief Memory allocation
 *
 * @param size
 * @return void*
 */
void *malloc(size_t size);

/**
 * @brief Free allocated memory
 *
 * @param ptr
 */
void free(void *ptr);

/**
 * @brief Copy a block of memory from a location to another
 *
 * @param dest
 * @param src
 * @param n
 * @return void*
 */
void *memmove(void *destination, const void *source, size_t size);

/**
 * @brief String duplicate
 *
 * @param string
 * @return char*
 */
char *strdup(const char *string);

/**
 * @brief Compare string
 *
 * @param str1
 * @param str2
 * @return unsigned char
 */
unsigned char strcmp(char *str1, char *str2);

/**
 * @brief Search character from string
 *
 * @param string
 * @param character
 * @return unsigned char
 */
char *strchr(const char *string, int character);

/**
 * @brief Search string from string
 *
 * @param haystack
 * @param needle
 * @return char*
 */
char *strstr(const char *haystack, const char *needle);

/**
 * @brief String length
 *
 * @param string
 * @return long long
 */
long long strlen(const char *string);

/**
 * @brief Hex to int
 *
 * @param str
 * @return int
 */
int htoi(const char *str);

/**
 * @brief String to int
 *
 * @param string
 * @return int
 */
int atoi(const char *string);

/**
 * @brief Int to string
 *
 * @param buff
 * @param size
 * @param value
 * @return char*
 */
char *itoa(char *buff, int size, int value);

/**
 * @brief Append string
 *
 * @param dest
 * @param string
 */
void strcat(char *dest, const char *string);

/**
 * @brief Copy string
 *
 * @param dest
 * @param string
 */
void strcpy(char *dest, const char *string);

#endif /* __STDLIB_H__ */