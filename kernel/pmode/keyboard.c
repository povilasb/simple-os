#include "keyboard.h"
#include "../stdlib/stdio.h"

unsigned char symbolTable[256];
unsigned char keyboardBuffer[256];
unsigned char keyboardBufferPos = 0;

void init_keyboard()
{
    int i;
    
    for (i = 0; i < sizeof(symbolTable); i++)
        symbolTable[i] = UNKNOWN_SYMBOL;
        
    symbolTable[0x2] = '1'; symbolTable[0x3] = '2'; symbolTable[0x4] = '3';
    symbolTable[0x5] = '4'; symbolTable[0x6] = '5'; symbolTable[0x7] = '6';
    symbolTable[0x8] = '7'; symbolTable[0x9] = '8'; symbolTable[0xA] = '9';
    symbolTable[0xB] = '0'; symbolTable[0xC] = '-'; symbolTable[0xD] = '=';
    symbolTable[0xE] = ASCII_BACKSPACE; symbolTable[0x10] = 'q'; symbolTable[0x11] = 'w';
    symbolTable[0x12] = 'e'; symbolTable[0x13] = 'r'; symbolTable[0x14] = 't';
    symbolTable[0x15] = 'y'; symbolTable[0x16] = 'u'; symbolTable[0x17] = 'i';
    symbolTable[0x18] = 'o'; symbolTable[0x19] = 'p'; symbolTable[0x1A] = '[';
    symbolTable[0x1B] = ']'; symbolTable[0x1C] = '\n'; symbolTable[0x1E] = 'a';
    symbolTable[0x1F] = 's'; symbolTable[0x20] = 'd'; symbolTable[0x21] = 'f';
    symbolTable[0x22] = 'g'; symbolTable[0x23] = 'h'; symbolTable[0x24] = 'j';
    symbolTable[0x25] = 'k'; symbolTable[0x26] = 'l'; symbolTable[0x27] = ';';
    symbolTable[0x28] = '\''; symbolTable[0x29] = '~'; symbolTable[0x2C] = 'z';
    symbolTable[0x2D] = 'x'; symbolTable[0x2E] = 'c'; symbolTable[0x2F] = 'v';
    symbolTable[0x30] = 'b'; symbolTable[0x31] = 'n'; symbolTable[0x32] = 'm';
    symbolTable[0x33] = ','; symbolTable[0x34] = '.'; symbolTable[0x35] = '/';
    symbolTable[0x39] = ' ';
}

char scancode_toSymbol(unsigned char scanCode)
{
    return symbolTable[scanCode];
}

void key_pressed(unsigned char scanCode)
{
    char symbol;
    
    symbol = scancode_toSymbol(scanCode);
    kprintf("%c", symbol);
    keyboardBuffer[keyboardBufferPos] = symbol;
    keyboardBufferPos++;
    
    //create input line resource
    if (symbol == '\n')
    {
        keyboardBuffer[keyboardBufferPos - 1] = 0;
        keyboard_createResource(keyboardBuffer);
        keyboardBufferPos = 0;
    }
}