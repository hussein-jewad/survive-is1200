#include "vga.h"
#include "timer.h"
#include "other.h"
#include <stdint.h>
#include <stdbool.h>

volatile int buttonState;
volatile bool frameReady = false;

void handle_interrupt (unsigned _irq) {
    buttonState = get_btn();
    frameReady = true;
    timer[0] = 0;
    return;
}


int main(void) {
    vgaInit();
    intInit();
    while(1) {
        int myColor;
        if(buttonState == 0) myColor = red;
        else myColor = blue;
        for (int i = 0; i < screenHeight; i++) {
            for (int j = 0; j < screenWidth; j++) {
                drawPixel(j, i, myColor);
            }
        }

        while(frameReady == false) {

        }
        frameReady = false;
        bufferSwap();
    }
}
