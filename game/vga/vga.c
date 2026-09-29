#include "vga.h"
#include <stdint.h>
// uint8_t so we dont have to deal with negative wrap arounds for the colors
#define vgaBuffer ((volatile uint32_t*) 0x04000100) 
#define vgaFrontbuffer ((volatile uint32_t*) 0x08000000)
#define vgaBackbuffer ((volatile uint32_t*) 0x08012C00)
#define vgaStatusReg ((volatile uint32_t*) 0x0400010C)

void bufferSwap(void) {
    *vgaBuffer = 1; // we are going to swap registers
    while(((*vgaStatusReg) & 1) == 1);
}

void drawPixel(int x, int y, uint8_t color) {
    if (x >= 0 && x < screenWidth && y >= 0 && y < screenHeight) {
    volatile uint8_t *pixel = (volatile uint8_t*) *vgaBackbuffer;
    pixel[(y * screenWidth) + x] = color; // y * screenwidth to get to the correct row, then add x to get to the correct pixel in that row
    }
}

void clearScreen(uint8_t color) {
    volatile uint8_t *pixel = (volatile uint8_t*) *vgaBackbuffer;
    for (int i = 0; i < (320*240); i++) {
        pixel[i] = color;
    }    
}
