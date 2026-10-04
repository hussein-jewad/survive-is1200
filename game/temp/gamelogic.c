#include "gamelogic.h"
#include "graphics.h"
#include "vga.h"
#include "other.h"
#include <stdbool.h>
#include <stddef.h>

#define NPC_X 80
#define NPC_Y 120
#define RELIC_X 160
#define RELIC_Y 80
#define MRINDEK_X 40
#define MRINDEK_Y 160
#define INTERACTION_RADIUS 20

static bool isNearPosition(const struct player *player, int x, int y) {
    const int deltaX = player->x - x;
    const int deltaY = player->y - y;
    return deltaX * deltaX + deltaY * deltaY <=
           INTERACTION_RADIUS * INTERACTION_RADIUS;
}

static void setGameMessage(GameState *gameState, const char *message) {
    size_t index = 0;
    while (index < sizeof(gameState->message) - 1 && message[index] != '\0') {
        gameState->message[index] = message[index];
        index++;
    }
    gameState->message[index] = '\0';
}

static void drawEnterPrompt(const char *roomName) {
    static const char prefix[] = "ENTER ";
    char message[64];
    size_t length = 0;

    for (size_t i = 0; prefix[i] != '\0'; i++) {
        message[length++] = prefix[i];
    }
    while (roomName != NULL && *roomName != '\0' && length < sizeof(message) - 2) {
        message[length++] = *roomName++;
    }
    message[length++] = '?';
    message[length] = '\0';
    drawTextBox(message);
}

static bool doorRequirementMet(const RoomExit *exit, const GameState *gameState) {
    switch (exit->requirement) {
    case DOOR_OPEN:
        return true;
    case DOOR_REQUIRES_RELIC:
        return gameState->quests[exit->QuestId];
    case DOOR_REQUIRES_QUEST:
        return gameState->quests[exit->QuestId];
    case DOOR_REQUIRES_CODE:
        if (get_sw() ==  exit->passcode) {
            return true;
        } else {
            return false;
        }
    default:
        return false;
    }
}

static const char *doorLockMessage(const RoomExit *exit) {
    switch (exit->requirement) {
    case DOOR_REQUIRES_RELIC:
        return "YOU NEED THE RELIC TO PASS!";
    case DOOR_REQUIRES_QUEST:
        return "TALK TO THE NPC FIRST!";
    case DOOR_REQUIRES_CODE:
        return "ENTER THE CODE TO PASS!";
    default:
        return "THE DOOR IS LOCKED!";
    }
}

bool playerHitsWall(const struct player *player, RoomId currentRoomId) {
    const int left = player->x - (20 - 16) / 2;
    const int right = player->x + ( (20 - 16) / 2) - 1;
    const int top = player->y - (20 - 14) / 2;
    const int bottom = player->y + ((20 - 14) / 2) - 1;

    const RoomDefinition *currentRoom = getRoomDefinition(currentRoomId);
    if (currentRoom == NULL) return true;
    if (left < 0 || right >= screenWidth || top < 5 || bottom >= 215) {
        return true;
    }

    const int firstColumn = left / MAP_TILE_SIZE;
    const int lastColumn = right / MAP_TILE_SIZE;
    const int firstRow = top / MAP_TILE_SIZE;
    const int lastRow = bottom / MAP_TILE_SIZE;

    for (int row = firstRow; row <= lastRow; row++) {
        for (int column = firstColumn; column <= lastColumn; column++) {
            if (row >= MAP_HEIGHT || column >= MAP_WIDTH) {
                return true;
            }

            const uint8_t tile = currentRoom->map[row][column];
            if (tile >= MAP_TILE_COUNT || tileDefinitions[tile].blocksMovement) {
                return true;
            }
        }
    }
    return false;
}

bool checkRoomTransition(struct player *player, RoomId *currentRoomId, GameState *gameState) {
    if (player == NULL || currentRoomId == NULL || gameState == NULL ||
        player->x < 0 || player->y < 0) {
        return false;
    }

    const RoomDefinition *currentRoom = getRoomDefinition(*currentRoomId);
    if (currentRoom == NULL) return false;

    const int tileX = player->x / MAP_TILE_SIZE;
    const int tileY = player->y / MAP_TILE_SIZE;
    if (tileX >= MAP_WIDTH || tileY >= MAP_HEIGHT) return false;

    for (int direction = 0; direction < DIR_COUNT; direction++) {
        const RoomExit *exit = &currentRoom->exits[direction];
        if (!exit->connected || tileX != exit->doorTileX || tileY != exit->doorTileY) {
            continue;
        }

        const RoomDefinition *destination = getRoomDefinition(exit->destination);
        if (destination == NULL) return false;

        drawEnterPrompt(destination->name);
        if (get_btn() != 1) return false;

        if (!doorRequirementMet(exit, gameState)) {
            setGameMessage(gameState, doorLockMessage(exit));
            drawTextBox(gameState->message);
            return false;
        }

        *currentRoomId = exit->destination;
        player->x = exit->spawnPixelX;
        player->y = exit->spawnPixelY;
        return true;
    }

    return false;
}

bool tryMovePlayer(struct player *player, int dx, int dy, RoomId *currentRoomId) {
    if (player == NULL || currentRoomId == NULL) return false;

    struct player nextPlayer = *player;
    nextPlayer.x += dx;
    nextPlayer.y += dy;
    if (playerHitsWall(&nextPlayer, *currentRoomId)) {
        return false;
    }

    *player = nextPlayer;
    return true;
}

bool handleWorldInteraction(const struct player *player, RoomId currentRoomId,
                            GameState *gameState) {
    if (player == NULL || gameState == NULL) return false;

    /*if (currentRoomId == ROOM_GRASSLAND && (gameState->quests[QUEST_RELIC]) != COMPLETED &&
        isNearPosition(player, NPC_X, NPC_Y)) {
        if (gameState->quests[QUEST_RELIC] == READY) {
            gameState->quests[QUEST_RELIC] = COMPLETED;
            setGameMessage(gameState, "QUEST COMPLETE! THANK YOU!");
        } else {
            gameState->quests[QUEST_RELIC] = ACTIVE;
            setGameMessage(gameState, "FIND THE RELIC IN THE CAVE!");
        }
        return true;
    }

    if (currentRoomId == ROOM_CAVE && (gameState->quests[QUEST_RELIC]) <= ACTIVE &&
        isNearPosition(player, RELIC_X, RELIC_Y)) {
        if (gameState->quests[QUEST_RELIC] == ACTIVE) {
            gameState->quests[QUEST_RELIC] = READY;
            setGameMessage(gameState, "RELIC FOUND! RETURN TO THE NPC!");
        } else {
            setGameMessage(gameState, "YOU SHOULD TALK TO THE NPC FIRST.");
        }
        return true;
    }

    if (currentRoomId == ROOM_3 && (gameState->quests[QUEST_MRINDEK]) != EXTENDED &&
        isNearPosition(player, MRINDEK_X, MRINDEK_Y)) {
        switch (gameState->quests[QUEST_MRINDEK]) {
        case COMPLETED:
            gameState->quests[QUEST_MRINDEK] = EXTENDED;
            setGameMessage(gameState, "im going now bye see you later");
            return true;
        case READY:
            gameState->quests[QUEST_MRINDEK] = COMPLETED;
            setGameMessage(gameState, "so lock in okay?");
            return true;
        case ACTIVE:
            gameState->quests[QUEST_MRINDEK] = READY;
            setGameMessage(gameState, "This course is cooked");
            return true;
        case NOT_STARTED:
            gameState->quests[QUEST_MRINDEK] = ACTIVE;
            setGameMessage(gameState, "Hi, I am Mrindek. listen up");
            return true;
        default:
            return false;
        }
        
    }*/

    return false;
}

void moveCheck(struct player *player, RoomId *currentRoomId, GameState *gameState) {
    if (player == NULL || currentRoomId == NULL || gameState == NULL) return;
    int switches = get_sw();
    bool hitWall = false;
    if (switches & (1 << 0)) {
        player->left = 1;
        if (!tryMovePlayer(player, -4, 0, currentRoomId)) hitWall = true;
    } 
    if (switches & (1 << 1)) {
        player->left = 0;
        if (!tryMovePlayer(player, 4, 0, currentRoomId)) hitWall = true;
    }
    if ((switches & (1 << 2)) && !tryMovePlayer(player, 0, -4, currentRoomId)) hitWall = true;
    if ((switches & (1 << 3)) && !tryMovePlayer(player, 0, 4, currentRoomId)) hitWall = true;
    bool buttonPressed = get_btn() == 1;
    bool interactionHandled = buttonPressed &&
        handleWorldInteraction(player, *currentRoomId, gameState);
    if (!interactionHandled) checkRoomTransition(player, currentRoomId, gameState);
}