#include "vga.h"
#include "graphics.h"
#include <stdint.h>
// uint8_t so we dont have to deal with negative wrap arounds for the colors
#define vgaBuffer ((volatile uint32_t*) 0x04000100) // VGA register
#define vgaBackbuffer ((volatile uint32_t*) 0x04000104) //  register that selects the active back buffer
#define vgaStatusReg ((volatile uint32_t*) 0x0400010C) // status register for the swap operation.
#define frameA 0x08000000 // First framebuffer address.
#define frameB (frameA + screenWidth * screenHeight) // Second framebuffer address.

// all of this was written by husien

static uint32_t drawAddr = frameB; // we start by drawing to frameB

static volatile uint8_t *backBuffer(void) { // get the inactive buffer
    return (volatile uint8_t*) (uintptr_t) drawAddr; // drawAddr is a uint32, we cast it to a uintptr and then a pointer to volatile uint8 which matches our colors
}

void vgaInit(void) {  
    clearScreen(black); // clear frameb
    bufferSwap(); // swap to a 
    clearScreen(black); // clear a
}

void bufferSwap(void) {
    *vgaBackbuffer = drawAddr; // set backbuffer to our inactive one (drawAddr)
    *vgaBuffer = 1; // swap
    while(((*vgaStatusReg) & 1) == 1); // wait until the swap is complete
    if (drawAddr == frameA) drawAddr = frameB; // if we are on a then go to b
    else drawAddr = frameA; // if not a then we showing be rn so switch back to a
}

void drawPixel(int x, int y, uint8_t color) {
    if (x >= 0 && x < screenWidth && y >= 0 && y < screenHeight) { // safety check
        backBuffer()[(y * screenWidth) + x] = color;
    }
}

void clearScreen(uint8_t color) {
    volatile uint8_t *pixel = backBuffer(); // take current buffer
    for (int i = 0; i < screenWidth * screenHeight; i++) { // put them all to color
        pixel[i] = color;
    }    
}

void drawRect(int x, int y, int width, int height, uint8_t color) { 
        for(int i = x; i < (width + x); i++) { // top row
            drawPixel(i, y, color);
        }
        for(int i = x; i < (width + x); i++) { // bottom row
            drawPixel(i, (y + height - 1), color);
        }

        for(int i = (y + 1); i < (height - 1 + y); i++) { // left column
            drawPixel(x, i, color);
        }
        for(int i = (y + 1); i < (height - 1 + y); i++) { // right column
            drawPixel((x + width - 1), i, color);
        }
}

void fillRect(int x, int y, int width, int height, uint8_t color) { 
    for(int i = y; i < (height + y); i++) { // loop through each row
        for(int j = x; j < (width + x); j++) { // loop across each column
            drawPixel(j, i, color); // set every pixel to color
        }
    }
}

void fillRectColor(int x, int y, int width, int height, const uint8_t* colorArray) { // same as above but just takes an array to set each specific color instead
    for(int i = (y - (height / 2) ); i < ( (height / 2) + y); i++) { 
        for(int j = (x - width / 2); j < ( (width / 2) + x); j++) { 
            uint8_t color = *colorArray++; 
            if (color != transparent) drawPixel(j, i, color);
        }
    }
}