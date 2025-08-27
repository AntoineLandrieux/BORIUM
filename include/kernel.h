#ifndef __KERNEL_H__
#define __KERNEL_H__ 0x1

/**
 *
 *  _____  _____ _____ _____ _   _ __  __
 * | ___ \|  _  | ___ \_   _| | | |  \/  |
 * | |_/ /| | | | |_/ / | | | | | | .  . |
 * | ___ \| | | |    /  | | | | | | |\/| |
 * | |_/ /\ \_/ / |\ \ _| |_| |_| | |  | |
 * \____/  \___/\_| \_|\___/ \___/\_|  |_/
 *
 * Antoine LANDRIEUX (MIT License) <kernel.h>
 * <https://github.com/AntoineLandrieux/BORIUM/>
 *
 */

// Kernel version
#define BORIUM_KERNEL_VERSION "Rv1.1.0"

// Kernel version major
#define BORIUM_KERNEL_MAJOR 1
// Kernel version minor
#define BORIUM_KERNEL_MINOR 1
// Kernel version patch
#define BORIUM_KERNEL_PATCH 0

// Is the kernel running?
extern unsigned char running;

// Username
extern char USERNAME[20];

/**
 * @brief Sleep miliseconds
 * 
 * @param ms 
 */
void SLEEP(unsigned int ms);

/**
 * @brief Text editor
 * 
 */
void EDITOR(void);

/**
 * @brief Setup the kernel
 *
 */
void SETUP();

/**
 * @brief Start kernel
 * 
 */
void start();

#endif /* __KERNEL_H__ */