#include "player_movement.h"

#include "other.h"
#include "quest_logic.h"
#include "room_transitions.h"
#include "vga.h"

// all of this was written by husien
bool playerHitsWall(const struct player *player, RoomId currentRoomId) {
    const int left = player->x - 5; // x is the players center predefined, these lines draw a hitbox
    const int right = player->x + 5;
    const int top = player->y - 5;
    const int bottom = player->y + 5;

    const RoomDefinition *currentRoom = getRoomDefinition(currentRoomId); // get our room
    if (currentRoom == NULL) return true; // safety check
    if (left < 0 || right >= screenWidth || top < 5 || bottom >= 215) { // no movement outside the screen (215 cuz of text bar)
        return true;
    }

    const int firstColumn = left / MAP_TILE_SIZE; // get tiles
    const int lastColumn = right / MAP_TILE_SIZE;
    const int firstRow = top / MAP_TILE_SIZE;
    const int lastRow = bottom / MAP_TILE_SIZE;

    for (int row = firstRow; row <= lastRow; row++) { // loop through every tile we touch
        for (int column = firstColumn; column <= lastColumn; column++) {
            if (row >= MAP_HEIGHT || column >= MAP_WIDTH) {
                return true;
            }

            const uint8_t tile = currentRoom->map[row][column];
            if (tile >= MAP_TILE_COUNT || tileDefinitions[tile].blocksMovement) { // check if we can move through it
                return true;
            }
        }
    }
    return false;
}

bool tryMovePlayer(struct player *player, int dx, int dy, RoomId *currentRoomId) { 
    if (player == NULL || currentRoomId == NULL) return false; // safety check

    struct player nextPlayer = *player; // nextPlayer to actually test if its a legal move or not
    nextPlayer.x += dx;
    nextPlayer.y += dy;
    if (playerHitsWall(&nextPlayer, *currentRoomId)) {
        return false;
    }

    *player = nextPlayer; // if legal set player to nextPlayer
    return true;
}

void moveCheck(struct player *player, RoomId *currentRoomId, GameState *gameState, bool buttonPressed) {
    if (player == NULL || currentRoomId == NULL || gameState == NULL) return;
    int sw = get_sw();
    if (sw & (1 << 0)) {
        player->left = 1;
        tryMovePlayer(player, -4, 0, currentRoomId);
    }
    if (sw & (1 << 1)) {
        player->left = 0;
        tryMovePlayer(player, 4, 0, currentRoomId);
    }
    if (sw & (1 << 2)) tryMovePlayer(player, 0, -4, currentRoomId);
    if (sw & (1 << 3)) tryMovePlayer(player, 0, 4, currentRoomId);
    bool interactionHandled = buttonPressed &&
        handleWorldInteraction(player, *currentRoomId, gameState);
    if (!interactionHandled) {
        checkRoomTransition(player, currentRoomId, gameState, buttonPressed);
    }
}