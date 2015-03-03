#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#define ASCII_BACKSPACE 0x8
#define UNKNOWN_SYMBOL 0x2A

void init_keyboard();
char scancode_toSymbol(unsigned char scanCode);
void key_pressed(unsigned char scanCode);

#endif