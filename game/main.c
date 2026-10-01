#include "vga.h"
#include "timer.h"
#include "other.h"
#include "graphics.h"
#include "gamelogic.h"
#include <stdint.h>
#include <stdbool.h>

volatile bool frameReady = false;

void handle_interrupt (unsigned _irq) {
    frameReady = true;
    timer[0] = 0;
    return;
}

int main(void) {
    struct player myPlayer = {160, 120};

    vgaInit();
    intInit();
    while(1) {
        drawMap(starter_map);
        drawPlayer(myPlayer, new_piskel_data);
        drawTextBox("WELCOME TO THE GRASSLAND!");

        while(frameReady == false) {

        }
        frameReady = false;
        moveCheck(&myPlayer);
        

        bufferSwap();
    }

    return 0;
}
