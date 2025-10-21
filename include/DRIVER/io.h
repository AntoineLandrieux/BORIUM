#ifndef __IO_H__
#define __IO_H__ 0x1

/* #pragma once */

/**
 *
 *  _____  _____ _____ _____ _   _ __  __
 * | ___ \|  _  | ___ \_   _| | | |  \/  |
 * | |_/ /| | | | |_/ / | | | | | | .  . |
 * | ___ \| | | |    /  | | | | | | |\/| |
 * | |_/ /\ \_/ / |\ \ _| |_| |_| | |  | |
 * \____/  \___/\_| \_|\___/ \___/\_|  |_/
 *
 * Antoine LANDRIEUX (MIT License) <io.h>
 * <https://github.com/AntoineLandrieux/BORIUM/>
 * <https://github.com/AntoineLandrieux/x86driver/>
 *
 */

/**
 * @brief Low-Level Port Input
 *
 * @param port
 * @return unsigned char
 */
unsigned char INB(unsigned short port);

/**
 * @brief Low-Level Output
 *
 * @param port
 * @param value
 */
void OUTB(unsigned short port, unsigned char value);

/**
 * @brief Busy-wait loop to simulate a sleep/delay for a given number of milliseconds.
 *
 * @param ms (milliseconds)
 */
void SLEEP(unsigned int ms);

#endif /* __IO_H__ */