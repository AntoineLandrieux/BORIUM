#include <DRIVER/io.h>

/**
 *
 *  _____  _____ _____ _____ _   _ __  __
 * | ___ \|  _  | ___ \_   _| | | |  \/  |
 * | |_/ /| | | | |_/ / | | | | | | .  . |
 * | ___ \| | | |    /  | | | | | | |\/| |
 * | |_/ /\ \_/ / |\ \ _| |_| |_| | |  | |
 * \____/  \___/\_| \_|\___/ \___/\_|  |_/
 *
 * Antoine LANDRIEUX (MIT License) <io.c>
 * <https://github.com/AntoineLandrieux/BORIUM/>
 *
 */

/**
 * @brief Low-Level Port Input
 *
 * @param port
 * @return unsigned char
 */
unsigned char INB(unsigned short port)
{
    // Reads a byte from the specified hardware port
    // (used to get keyboard scancodes)
    unsigned char data;
    __asm__ volatile("inb %1, %0" : "=a"(data) : "Nd"(port));
    return data;
}

/**
 * @brief Low-Level Output
 *
 * @param port
 * @param value
 */
void OUTB(unsigned short port, unsigned char value)
{
    // Writes a byte to a hardware port
    // (used for VGA register access)
    __asm__ volatile("outb %0, %1" : : "a"(value), "Nd"(port));
}

/**
 * @brief Busy-wait loop to simulate a sleep/delay for a given number of milliseconds.
 *
 *
 * @param ms
 */
void SLEEP(unsigned int ms)
{
    volatile unsigned long count = 0;
    // NOTE: Arbitrary value
    unsigned long limit = ms * 500000;

    while (count < limit)
    {
        __asm__ volatile("nop");
        count++;
    }
}
