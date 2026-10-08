#ifndef vgaH

#define vgaH
#define black 0x00
#define white 0xFF
#define red   0xE0
#define green 0x1C
#define blue  0x03
#define transparent 0x67 // Reserved pixel value; do not use for a visible color.

#define screenWidth 320
#define screenHeight 240

#include <stdint.h>

void vgaInit(void);
void bufferSwap(void);
void drawPixel(int x, int y, uint8_t color);
void clearScreen(uint8_t color);
void drawRect(int x, int y, int width, int height, uint8_t color);
void fillRect(int x, int y, int width, int height, uint8_t color);
void fillRectColor(int x, int y, int width, int height, const uint8_t* colorArray);

#endif