
#include <STD/stdlib.h>
#include <STD/stdint.h>

#define __MEMORY_POOL_SIZE__ 0x80000

static char memory[__MEMORY_POOL_SIZE__];
static char *mfree = memory;

/**
 * @brief Memory allocation
 *
 * @param size
 * @return void*
 */
void *malloc(size_t size)
{
    if ((mfree + size) > (memory + __MEMORY_POOL_SIZE__))
        return NULL;

    void *alloc = mfree;
    mfree += size;
    return alloc;
}

/**
 * @brief Free allocated memory
 *
 * @param ptr
 */
void free(void *ptr)
{
    if (ptr)
        __asm__("nop");
    return;
}

/**
 * @brief Free allocated memory
 *
 */
void free_all()
{
    for (size_t i = 0; i < sizeof(memory); i++)
        memory[i] = 0;
    mfree = memory;
}

/**
 * @brief String duplicate
 *
 * @param string
 * @return char*
 */
char *strdup(char *string)
{
    if (!string)
        return NULL;
    char *result = (char *)malloc(strlen(string) + 1);
    strcat(result, string);
    return result;
}

/**
 * @brief Copy a block of memory from a location to another
 *
 * @param dest
 * @param src
 * @param n
 * @return void*
 */
void *memmove(void *destination, const void *source, size_t size)
{
    unsigned char *d = (unsigned char *)destination;
    const unsigned char *s = (const unsigned char *)source;

    if (d == s || size == 0)
        return destination;

    if (d < s)
        for (size_t i = 0; i < size; i++)
            d[i] = s[i];
    else
        for (size_t i = size; i > 0; i--)
            d[i - 1] = s[i - 1];

    return destination;
}

/**
 * @brief Compare string
 *
 * @param str1
 * @param str2
 * @return unsigned char
 */
unsigned char strcmp(char *str1, char *str2)
{
    for (unsigned long long i = 0; str1[i] || str2[i]; i++)
        if (str1[i] != str2[i])
            return 1;
    return 0;
}

/**
 * @brief Search character from string
 *
 * @param string
 * @param character
 * @return unsigned char
 */
char *strchr(const char *string, int character)
{
    for (; *string; string++)
        if (*string == (char)character)
            return (char *)string;
    return !character ? (char *)string : NULL;
}

/**
 * @brief Search string from string
 *
 * @param haystack
 * @param needle
 * @return char*
 */
char *strstr(const char *haystack, const char *needle)
{
    if (!*needle)
        return (char *)haystack;

    for (; *haystack; haystack++)
    {
        const char *h = haystack;
        const char *n = needle;

        while (*h && *n && *h == *n)
        {
            h++;
            n++;
        }

        if (!*n)
            return (char *)haystack;
    }

    return NULL;
}

/**
 * @brief String length
 *
 * @param string
 * @return long long
 */
long long strlen(const char *string)
{
    for (long long i = 0; 1; i++)
        if (!string[i])
            return i;
    return 0;
}

/**
 * @brief Hex to int
 *
 * @param str
 * @return int
 */
int htoi(const char *str)
{
    int result = 0;
    int i = 2 * (str[0] == '0' && (str[1] == 'x' || str[1] == 'X'));

    while (str[i])
    {
        char c = str[i];
        int value;

        if (c >= '0' && c <= '9')
            value = c - '0';
        else if (c >= 'a' && c <= 'f')
            value = c - 'a' + 10;
        else if (c >= 'A' && c <= 'F')
            value = c - 'A' + 10;
        else
            break;

        result = (result << 4) | value;
        i++;
    }

    return result;
}

/**
 * @brief String to int
 *
 * @param string
 * @return int
 */
int atoi(const char *string)
{
    int result = 0;
    int sign = 1;

    if (*string == '-' || *string == '+')
    {
        sign = *string == '-' ? -1 : 1;
        string++;
    }

    for (; *string; string++)
        if (*string >= '0' && *string <= '9')
            result = result * 10 + (*string - '0');
        else
            break;

    return sign * result;
}

/**
 * @brief Int to string
 *
 * @param buff
 * @param size
 * @param value
 * @return char*
 */
char *itoa(char *buff, int size, int value)
{
    int i = 0;
    int sign = 0;

    if (!value)
    {
        buff[i++] = '0';
        buff[i] = 0;
        return buff;
    }

    if (value < 0)
    {
        sign = 1;
        value = -value;
    }

    for (; value > 0 && i < size - 1; value /= 10)
        buff[i++] = '0' + (value % 10);

    if (sign && i < size - 1)
        buff[i++] = '-';

    buff[i] = 0;

    int start = 0;
    int end = i - 1;

    while (start < end)
    {
        char temp = buff[start];
        buff[start] = buff[end];
        buff[end] = temp;
        start++;
        end--;
    }

    return buff;
}

/**
 * @brief Append string
 *
 * @param dest
 * @param string
 */
void strcat(char *dest, char *string)
{
    unsigned long long i = 0;
    for (; dest[i]; i++)
        ;
    for (; *string; (volatile char *)string++)
    {
        dest[i] = *string;
        i++;
    }
    dest[i] = 0;
}

/**
 * @brief Copy string
 *
 * @param dest
 * @param string
 */
void strcpy(char *dest, char *string)
{
    unsigned long long i = 0;
    for (; string[i]; i++)
        dest[i] = string[i];
    dest[i] = 0;
}
