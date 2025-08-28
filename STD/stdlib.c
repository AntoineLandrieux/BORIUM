
#include <STD/stdlib.h>
#include <STD/stdint.h>

#define MEMORY_POOL_SIZE 0x90000

/**
 * @brief Memory block
 *
 */
typedef struct mem_block
{

    size_t size;
    int32_t free;

    struct mem_block *next;

} mem_block_t;

/* Memory pool */
static char memory[MEMORY_POOL_SIZE];

/* Head */
static mem_block_t *head = NULL;

#define ALIGN4(x) (((x) + 3) & ~3)

#define BLOCK_SIZE sizeof(mem_block_t)

/**
 * @brief Memory allocation
 *
 * @param size
 * @return void*
 */
void *malloc(size_t size)
{
    size = ALIGN4(size);

    if (!head)
    {
        head = (mem_block_t *)memory;
        head->size = MEMORY_POOL_SIZE - BLOCK_SIZE;
        head->free = 1;
        head->next = NULL;
    }

    mem_block_t *curr = head;

    while (curr)
    {
        if (curr->free && curr->size >= size)
        {
            if (curr->size >= size + BLOCK_SIZE + 4)
            {
                mem_block_t *new_block = (mem_block_t *)((char *)curr + BLOCK_SIZE + size);

                new_block->size = curr->size - size - BLOCK_SIZE;
                new_block->free = 1;
                new_block->next = curr->next;

                curr->size = size;
                curr->next = new_block;
            }

            curr->free = 0;
            return (char *)curr + BLOCK_SIZE;
        }

        curr = curr->next;
    }

    return NULL;
}

/**
 * @brief Free allocated memory
 *
 * @param ptr
 */
void free(void *ptr)
{
    if (!ptr)
        return;

    mem_block_t *block = (mem_block_t *)((char *)ptr - BLOCK_SIZE);
    block->free = 1;

    while (block->next && block->next->free)
    {
        block->size += BLOCK_SIZE + block->next->size;
        block->next = block->next->next;
    }

    mem_block_t *curr = head;

    while (curr && curr->next)
    {
        if (curr->next == block && curr->free)
        {
            curr->size += BLOCK_SIZE + block->size;
            curr->next = block->next;
            break;
        }
        curr = curr->next;
    }
}

/**
 * @brief String duplicate
 *
 * @param string
 * @return char*
 */
char *strdup(const char *string)
{
    if (!string)
        return NULL;
    char *result = (char *)malloc(strlen(string) + 1);
    if (result)
        strcpy(result, string);
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
void strcat(char *dest, const char *string)
{
    while (*dest)
        dest++;
    while (*string)
        *dest++ = *string++;
    *dest = 0;
}

/**
 * @brief Copy string
 *
 * @param dest
 * @param string
 */
void strcpy(char *dest, const char *string)
{
    unsigned long long i = 0;
    for (; string[i]; i++)
        dest[i] = string[i];
    dest[i] = 0;
}
