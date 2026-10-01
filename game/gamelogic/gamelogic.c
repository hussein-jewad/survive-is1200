#include "gamelogic.h"
#include "graphics.h"
#include "vga.h"
#include "other.h"
#include <stdbool.h>

bool playerHitsWall(const struct player *player) {
    const int left = player->x - (PLAYER_WIDTH - 10) / 2;
    const int right = player->x + ( (PLAYER_WIDTH - 10) / 2) - 1;
    const int top = player->y - (PLAYER_HEIGHT - 10) / 2;
    const int bottom = player->y + ((PLAYER_HEIGHT - 10) / 2) - 1;

    if (left < 0 || right >= screenWidth || top < 0 ||
        bottom >= screenHeight - MAP_TILE_SIZE) {
        return true;
    }

    const int firstColumn = left / MAP_TILE_SIZE;
    const int lastColumn = right / MAP_TILE_SIZE;
    const int firstRow = top / MAP_TILE_SIZE;
    const int lastRow = bottom / MAP_TILE_SIZE;

    for (int row = firstRow; row <= lastRow; row++) {
        for (int column = firstColumn; column <= lastColumn; column++) {
            if (row >= MAP_HEIGHT || column >= MAP_WIDTH ||
                tileDefinitions[starter_map[row][column]].blocksMovement) {
                return true;
            }
        }
    }
    return false;
}

bool tryMovePlayer(struct player *player, int dx, int dy) {
    struct player nextPlayer = {player->x + dx, player->y + dy};
    if (playerHitsWall(&nextPlayer)) {
        return false;
    }

    *player = nextPlayer;
    return true;
}

void moveCheck(struct player *player) {
    int switches = get_sw();
    bool hitWall = false;
    if ((switches & (1 << 0)) && !tryMovePlayer(player, -4, 0)) hitWall = true;
    if ((switches & (1 << 1)) && !tryMovePlayer(player, 4, 0)) hitWall = true;
    if ((switches & (1 << 2)) && !tryMovePlayer(player, 0, -4)) hitWall = true;
    if ((switches & (1 << 3)) && !tryMovePlayer(player, 0, 4)) hitWall = true;
    if (hitWall) drawTextBox("YOU HIT A WALL!");
}
