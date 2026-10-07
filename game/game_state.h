#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <stdbool.h>
#include <stddef.h>

#include "rooms.h"

#define PLAYER_WIDTH 32
#define PLAYER_HEIGHT 32

typedef enum {
    QUEST_MARCO,
    QUEST_BALLKNOWLEDGE,
    QUEST_FINDTOOL,
    QUEST_FINDCODE,
    QUEST_FINDKEYCARD,
    QUEST_COUNT
} QuestId;

typedef enum {
    NOT_STARTED,
    S1 = 1,
    S2,
    S3,
    S4,
    S5,
    S6,
    S7,
    S8,
    S9,
    S10,
    EXTENDED
} QuestStatus;

typedef struct {
    QuestStatus quests[QUEST_COUNT];
    char message[64];
    bool items[ITEM_COUNT];
    bool worldItems[ITEM_COUNT];
    bool taOnBalcony;
    bool computerRoomHintShown;
    bool terminalRequested;
    bool doorLockedMessageActive;
} GameState;

static inline void gameStateSetMessage(GameState *gameState, const char *message) { // weilong
    size_t index = 0;
    while (index < sizeof(gameState->message) - 1 && message[index] != '\0') {
        gameState->message[index] = message[index];
        index++;
    }
    gameState->message[index] = '\0';
}

#endif
