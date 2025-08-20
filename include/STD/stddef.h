#ifndef __STDDEF_H__
#define __STDDEF_H__ 0x1

/**
 *
 *  _____  _____ _____ _____ _   _ __  __
 * | ___ \|  _  | ___ \_   _| | | |  \/  |
 * | |_/ /| | | | |_/ / | | | | | | .  . |
 * | ___ \| | | |    /  | | | | | | |\/| |
 * | |_/ /\ \_/ / |\ \ _| |_| |_| | |  | |
 * \____/  \___/\_| \_|\___/ \___/\_|  |_/
 *
 * Antoine LANDRIEUX (MIT License) <stddef.h>
 * <https://github.com/AntoineLandrieux/BORIUM/>
 *
 */

#define NULL ((void *)(0x00))

#ifdef __SIZE_TYPE__
#undef __SIZE_TYPE__
#endif /* __SIZE_TYPE__ */

#define __SIZE_TYPE__ long unsigned int
typedef __SIZE_TYPE__ size_t;

#endif /* __STDDEF_H__ */