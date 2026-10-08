#ifndef QUEST_LOGIC_H
#define QUEST_LOGIC_H

#include "game_state.h"

bool questIsComplete(const GameState *gameState, QuestId questId);
bool handleWorldInteraction(const struct player *player, RoomId currentRoomId,
                            GameState *gameState);
void drawQuestMarkers(RoomId currentRoomId, const GameState *gameState);

#endif
