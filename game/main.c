#include "vga.h"
#include "timer.h"
#include "other.h"
#include "graphics.h"
#include "graphicsdata.h"
#include "gamelogic.h"
#include "rooms.h"
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

volatile bool frameReady = false;

RoomId currentRoomId = OUTSIDE;
GameState gameState = { .message = "KILL YOURSELF"};

void handle_interrupt (unsigned _irq) {
    frameReady = true;
    timer[0] = 0;
    return;
}

int main(void) {
    struct player myPlayer = { .x = 160, .y = 120, .left = 1 };
    vgaInit();
    intInit();

    while(1) {
        const RoomDefinition *currentRoom = getRoomDefinition(currentRoomId);
        if (currentRoom == NULL) {
            currentRoomId = OUTSIDE;
            currentRoom = getRoomDefinition(currentRoomId);
        }

        drawMap(currentRoom->map);
        /*if (currentRoomId == ROOM_GRASSLAND && (gameState.quests[QUEST_RELIC]) != COMPLETED) {
            fillRect(74, 114, 12, 12, red);
        }
        if (currentRoomId == ROOM_CAVE && (gameState.quests[QUEST_RELIC]) < READY) {
            fillRect(154, 74, 12, 12, white);
        }
        if (currentRoomId == ROOM_3 && (gameState.quests[QUEST_MRINDEK]) != EXTENDED) {
            fillRect(34, 154, 12, 12, green);
        }*/

        drawPlayer(myPlayer);
        drawTextBox(gameState.message);

        while(frameReady == false) {

        }
        frameReady = false;
        moveCheck(&myPlayer, &currentRoomId, &gameState);
        

        bufferSwap();
    }

    return 0;
}