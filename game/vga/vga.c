#include "vga.h"
#include <stdint.h>
// uint8_t so we dont have to deal with negative wrap arounds for the colors
#define vgaBuffer ((volatile uint32_t*) 0x04000100) 
#define vgaFrontbuffer ((volatile uint32_t*) 0x04000100)
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
    for (int i = 0; i < (320*240); i++) {
        pixel[i] = color;
    }    
}
