#ifndef ROOM_TRANSITIONS_H
#define ROOM_TRANSITIONS_H

#include "game_state.h"

bool checkRoomTransition(struct player *player, RoomId *currentRoomId,
                         GameState *gameState, bool buttonPressed);

#endif
