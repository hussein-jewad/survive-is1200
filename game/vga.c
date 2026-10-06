#include "vga.h"
#include "graphics.h"
#include <stdint.h>
// uint8_t so we dont have to deal with negative wrap arounds for the colors
#define vgaBuffer ((volatile uint32_t*) 0x04000100) // Memory-mapped VGA register.
#define vgaBackbuffer ((volatile uint32_t*) 0x04000104) // Memory-mapped register that selects the active back buffer.
#define vgaStatusReg ((volatile uint32_t*) 0x0400010C) // Memory-mapped status register for the swap operation.
#define frameA 0x08000000 // First framebuffer address.
#define frameB (frameA + screenWidth * screenHeight) // Second framebuffer address.

// all of this was written by husien

static uint32_t drawAddr = frameB; // Tracks which framebuffer is currently being drawn to.

static volatile uint8_t *backBuffer(void) {
    return (volatile uint8_t*) (uintptr_t) drawAddr; // Return the current back buffer pointer.
}

void vgaInit(void) {
    drawAddr = frameB; // Start drawing to frameB.
    clearScreen(black); // Fill the drawing buffer with black.
    bufferSwap(); // Present the black frame.
    *vgaBackbuffer = frameB; // Select frameB as the hardware back buffer.
    clearScreen(black); // Clear the new drawing buffer.
}

void bufferSwap(void) {
    *vgaBackbuffer = drawAddr; // Tell the hardware which buffer to display.
    *vgaBuffer = 1; // Trigger the swap.
    while(((*vgaStatusReg) & 1) == 1); // Wait until the swap is complete.
    if (drawAddr == frameA) drawAddr = frameB; // Toggle drawing back to frameB.
    else drawAddr = frameA; // Toggle drawing back to frameA.
}

void drawPixel(int x, int y, uint8_t color) {
    if (x >= 0 && x < screenWidth && y >= 0 && y < screenHeight) { // safety check
        backBuffer()[(y * screenWidth) + x] = color; // Write the pixel into the current framebuffer.
    }
}

void clearScreen(uint8_t color) {
    volatile uint8_t *pixel = backBuffer(); // Get the current drawing buffer.
    for (int i = 0; i < screenWidth * screenHeight; i++) { // Fill all pixels with the same color.
        pixel[i] = color;
    }    
}

void drawRect(int x, int y, int width, int height, uint8_t color) { 
        for(int i = x; i < (width + x); i++) { // Draw the top row.
            drawPixel(i, y, color);
        }
        for(int i = x; i < (width + x); i++) { // Draw the bottom row.
            drawPixel(i, (y + height - 1), color);
        }

        for(int i = (y + 1); i < (height - 1 + y); i++) { // Draw the left pillar.
            drawPixel(x, i, color);
        }
        for(int i = (y + 1); i < (height - 1 + y); i++) { // Draw the right pillar.
            drawPixel((x + width - 1), i, color);
        }
}

void fillRect(int x, int y, int width, int height, uint8_t color) { 
    for(int i = y; i < (height + y); i++) { // Loop through each row.
        for(int j = x; j < (width + x); j++) { // Loop across each column.
            drawPixel(j, i, color); // Paint each pixel in the filled rectangle.
        }
    }
}

void fillRectColor(int x, int y, int width, int height, const uint8_t* colorArray) {
    for(int i = (y - (height / 2) ); i < ( (height / 2) + y); i++) { // Loop through the rows of the centered region.
        for(int j = (x - width / 2); j < ( (width / 2) + x); j++) { // Loop through the columns of the centered region.
            uint8_t color = *colorArray++; // Read the next color from the bitmap array.
            if (color != transparent) drawPixel(j, i, color); // Skip transparent pixels.
        }
    }
}