#ifndef PLAYER_MOVEMENT_H
#define PLAYER_MOVEMENT_H

#include "game_state.h"

bool playerHitsWall(const struct player *player, RoomId currentRoomId);
bool tryMovePlayer(struct player *player, int dx, int dy, RoomId *currentRoomId);
void moveCheck(struct player *player, RoomId *currentRoomId, GameState *gameState,
               bool buttonPressed);

#endif
