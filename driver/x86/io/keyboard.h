#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__ 0x1

/*
 * keyboard.h

 * Antoine LANDRIEUX
 * BORIUM <https://github.com/AntoineLandrieux/BORIUM>
 *
 * MIT License
*/

#define KEYBOARD_PORT 0x60

#define KEY_A 0x1E
#define KEY_B 0x30
#define KEY_C 0x2E
#define KEY_D 0x20
#define KEY_E 0x12
#define KEY_F 0x21
#define KEY_G 0x22
#define KEY_H 0x23
#define KEY_I 0x17
#define KEY_J 0x24
#define KEY_K 0x25
#define KEY_L 0x26
#define KEY_M 0x32
#define KEY_N 0x31
#define KEY_O 0x18
#define KEY_P 0x19
#define KEY_Q 0x10
#define KEY_R 0x13
#define KEY_S 0x1F
#define KEY_T 0x14
#define KEY_U 0x16
#define KEY_V 0x2F
#define KEY_W 0x11
#define KEY_X 0x2D
#define KEY_Y 0x15
#define KEY_Z 0x2C

#define KEY_1 0x02
#define KEY_2 0x03
#define KEY_3 0x04
#define KEY_4 0x05
#define KEY_5 0x06
#define KEY_6 0x07
#define KEY_7 0x08
#define KEY_8 0x09
#define KEY_9 0x0A
#define KEY_0 0x0B

#define KEY_BACKSPACE 0x0E
#define KEY_TAB 0x0F
#define KEY_ENTER 0x1C
#define KEY_SPACE 0x39

/**
 * @brief inb function
 * @author Antoine LANDRIEUX
 * 
 * @param _Port 
 * @return unsigned char 
 */
unsigned char inb(unsigned short _Port);

/**
 * @brief Wait a moment
 * @author Antoine LANDRIEUX
 * 
 * @param _Timer 
 */
void sleep(unsigned int _Timer);

/**
 * @brief Convert inb to ascii char (QUERTY mode)
 * @author Antoine LANDRIEUX
 * 
 * @param _KeyCode 
 * @return char 
 */
char ascii_char(unsigned char _KeyCode);

/**
 * @brief Get string
 * @author Antoine LANDRIEUX
 * 
 * @param _Dest
 * @param _Size
 */
void GETS(char *_Dest, unsigned int _Size);

#endif /* __KEYBOARD_H__ */