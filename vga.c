#include "vga.h"
#include "graphics.h"
#include <stdint.h>
// uint8_t so we dont have to deal with negative wrap arounds for the colors
#define vgaBuffer ((volatile uint32_t*) 0x04000100) 
#define vgaBackbuffer ((volatile uint32_t*) 0x04000104)
#define vgaStatusReg ((volatile uint32_t*) 0x0400010C)

#define frameA 0x08000000
#define frameB (frameA + screenWidth * screenHeight)

static uint32_t drawAddr = frameB;

static volatile uint8_t *backBuffer(void) {
    return (volatile uint8_t*) (uintptr_t) drawAddr;
}

void vgaInit(void) {
    drawAddr = frameB;
    clearScreen(black);
    bufferSwap();
    *vgaBackbuffer = frameB;
    clearScreen(black);
}

void bufferSwap(void) {
    *vgaBackbuffer = drawAddr;
    *vgaBuffer = 1; // we are going to swap registers
    while(((*vgaStatusReg) & 1) == 1);
    if (drawAddr == frameA) drawAddr = frameB;
    else drawAddr = frameA;
}

void drawPixel(int x, int y, uint8_t color) {
    if (x >= 0 && x < screenWidth && y >= 0 && y < screenHeight) {
    backBuffer()[(y * screenWidth) + x] = color; // y * screenwidth to get to the correct row, then add x to get to the correct pixel in that row
    }
}

void clearScreen(uint8_t color) {
    volatile uint8_t *pixel = backBuffer();
    for (int i = 0; i < screenWidth * screenHeight; i++) {
        pixel[i] = color;
    }    
}

void drawRect(int x, int y, int width, int height, uint8_t color) {
        for(int i = x; i < (width + x); i++) { // draw the top row
            drawPixel(i, y, color);
        }
        for(int i = x; i < (width + x); i++) { // draw the bottom row
            drawPixel(i, (y + height - 1), color);
        }

        for(int i = (y + 1); i < (height - 1 + y); i++) { // draw the left pillar
            drawPixel(x, i, color);
        }
        for(int i = (y + 1); i < (height - 1 + y); i++) { // draw the right pillar
            drawPixel((x + width - 1), i, color);
        }
}

void fillRect(int x, int y, int width, int height, uint8_t color) {
    for(int i = y; i < (height + y); i++) { // loop until every row is drawn giving us the full rectangle
        for(int j = x; j < (width + x); j++) { // draw a full row
            drawPixel(j, i, color); 
        }
    }
}

void fillRectColor(int x, int y, int width, int height, const uint32_t* piskelArray) {
    for(int i = (y - (height / 2) ); i < ( (height / 2) + y); i++) {
        for(int j = (x - width / 2); j < ( (width / 2) + x); j++) {
            uint8_t color = rgbFix(*piskelArray);
            if (! (color == 0x67)) drawPixel(j, i, color);
            piskelArray++;
        }
    }
}
