#include "vga.h"
#include "timer.h"
#include "other.h"
#include <stdint.h>

void handleInterrupt (unsigned _irq)
{ }


int main(void) {
    intInit();
    clearScreen(black);
    for(int i = 0; i < 320; i++) {
        drawPixel(0, i, white);
        drawPixel(i, 0, white);
    }
    bufferSwap();
    while(1) {
        for (int i = 0; i < screenHeight; i++) {
            for (int j = 0; j < screenWidth; j++) {
                if(get_btn == 0) (i, j, red);
                else (i, j, blue);
            }
        }
    }
}