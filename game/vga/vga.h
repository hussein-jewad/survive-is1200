#ifndef vgaH

#define vgaH
#define black 0x00
#define white 0xFF
#define red   0xE0
#define green 0x1C
#define blue  0x03

#define screenWidth 320
#define screenHeight 240

#include <stdint.h>

void bufferSwap(void);
void drawPixel(int x, int y, uint8_t color);
void clearScreen(uint8_t color);

#endif
