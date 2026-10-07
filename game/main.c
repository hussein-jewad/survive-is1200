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
bool isPaused = false;
RoomId currentRoomId = OUTSIDE;
GameState gameState;

typedef enum {
    APP_START,
    APP_GAME,
    APP_TERMINAL,
    APP_END
} AppMode;

static void resetGame(struct player *player) { // husien
    player->x = 160;
    player->y = 120;
    player->left = 1;
    currentRoomId = OUTSIDE;
    for (int i = 0; i < QUEST_COUNT; i++) {
        gameState.quests[i] = NOT_STARTED;
    }
    for (int i = 0; i < ITEM_COUNT; i++) {
        gameState.items[i] = false;
        gameState.worldItems[i] = false;
    }
    gameState.taOnBalcony = false;
    gameState.computerRoomHintShown = false;
    gameState.terminalRequested = false;
    gameState.doorLockedMessageActive = false;

    static const char welcomeMessage[] =
        "WELCOME TO P1. GO TO LECTURE ON THE RIGHT.";
    size_t i = 0;
    while (i < sizeof(gameState.message) - 1 && welcomeMessage[i] != '\0') {
        gameState.message[i] = welcomeMessage[i];
        i++;
    }
    gameState.message[i] = '\0';
}

static void drawBlackScreen(const char *message) { // husien
    clearScreen(black);
    drawTextBox(message);
}

static void drawCharacters(RoomId roomId) { // weilong
    if (roomId == LECTURE) {
        fillRectColor((MAP_WIDTH - 2) * MAP_TILE_SIZE, (MAP_HEIGHT / 2) * MAP_TILE_SIZE, MAP_TILE_SIZE, MAP_TILE_SIZE, professor);
    }
    if (roomId == HALLD) {
        fillRectColor(6 * MAP_TILE_SIZE, 5 * MAP_TILE_SIZE, MAP_TILE_SIZE, MAP_TILE_SIZE, ballknowledge);
    }
    if (roomId == COMPUTERROOM && gameState.quests[QUEST_FINDKEYCARD] == NOT_STARTED) { 
        fillRectColor(8 * MAP_TILE_SIZE, 2 * MAP_TILE_SIZE + MAP_TILE_SIZE / 2, MAP_TILE_SIZE, MAP_TILE_SIZE, TA);
    }
    if (roomId == BALCONY && gameState.taOnBalcony) {
        fillRectColor(8 * MAP_TILE_SIZE, 7 * MAP_TILE_SIZE,
        MAP_TILE_SIZE, MAP_TILE_SIZE, TA);
    }
    if (roomId == LUNCHROOM) {
        fillRectColor(8 * MAP_TILE_SIZE + MAP_TILE_SIZE / 2, 5 * MAP_TILE_SIZE + MAP_TILE_SIZE / 2, MAP_TILE_SIZE, MAP_TILE_SIZE, mrarmy);
    }
}

void handle_interrupt (unsigned cause) { // Husien
    if (cause == 16) {
        frameReady = true;
        timer[0] = 0;
        return;
    }
    if (cause == 17) {
        *switchEdge = 0x000;
        isPaused = !isPaused;
        return;
    }
}

int main(void) { // Husien and Weilong
    struct player myPlayer;
    AppMode mode = APP_START;
    int terminalStage = 0;
    const char *terminalMessage = "SET SWITCHES TO 64 THEN PRESS BTN";
    resetGame(&myPlayer);
    vgaInit();
    intInit();

    while(1) {
        if (mode == APP_START) {
            drawBlackScreen("PRESS BUTTON TO START");
        } else if (mode == APP_TERMINAL) {
            drawBlackScreen(terminalMessage);
        } else if (mode == APP_END) {
            drawBlackScreen("ACCESSED GRADES. PRESS BTN TO CHANGE TO A AND FINISH.");

        } else if (isPaused) {
            fillRectColor(screenWidth / 2, screenHeight / 2, screenWidth, screenHeight, PAUSED);
            drawTextBox("GAME PAUSED. DOWN, UP, RIGHT, LEFT, TO MOVE");
        }
        else {
            const RoomDefinition *currentRoom = getRoomDefinition(currentRoomId);
            if (currentRoom == NULL) {
                currentRoomId = OUTSIDE;
                currentRoom = getRoomDefinition(currentRoomId);
            }

            drawMap(currentRoom->map);
            drawQuestMarkers(currentRoomId, &gameState);
            drawCharacters(currentRoomId);
            drawPlayer(myPlayer);
            drawTextBox(gameState.message);
        }

        while(frameReady == false) {

        }
        frameReady = false;
        const bool buttonPressed = get_btn_pressed();
        
        if (!isPaused) {
            switch (mode) {
        case APP_START:
            if (buttonPressed) {
                resetGame(&myPlayer);
                mode = APP_GAME;
            }
            break;
        case APP_GAME:
            moveCheck(&myPlayer, &currentRoomId, &gameState, buttonPressed);
            if (gameState.terminalRequested) {
                gameState.terminalRequested = false;
                terminalStage = 0;
                terminalMessage = "SET SWITCHES TO 64 THEN PRESS BTN";
                mode = APP_TERMINAL;
            }
            break;
        case APP_TERMINAL:
            if (buttonPressed) {
                const int code = get_sw();
                if (terminalStage == 0 && code == 64) {
                    terminalStage = 1;
                    terminalMessage = "CONVERT 167 TO BINARY UNLOCK GRADE.";
                } else if (terminalStage == 1 && code == 167) {
                    mode = APP_END;
                } else if (terminalStage == 0) {
                    terminalMessage = "WRONG CODE.";
                } else {
                    terminalMessage = "WRONG. CONVERT 167 TO BINARY.";
                }
            }
            break;
        case APP_END:
            if (buttonPressed) {
                resetGame(&myPlayer);
                mode = APP_GAME;
            }
            break;
        }
        }

        bufferSwap();
    }

    return 0;
}