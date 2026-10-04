#ifndef GAMELOGICH
#define GAMELOGICH

#include <stdbool.h>
#define PLAYER_WIDTH 32
#define PLAYER_HEIGHT 32

#include "rooms.h"

struct player;

typedef enum {
    QUEST_RELIC,
    QUEST_MRINDEK,
    QUEST_COUNT
} QuestId;

typedef enum {
    NOT_STARTED,
    ACTIVE,
    READY,
    COMPLETED,
	EXTENDED
} QuestStatus;

typedef struct {
	QuestStatus quests[QUEST_COUNT];
	char message[64];
} GameState;


bool playerHitsWall(const struct player *player, RoomId currentRoomId);
bool checkRoomTransition(struct player *player, RoomId *currentRoomId, GameState *gameState);
bool tryMovePlayer(struct player *player, int dx, int dy, RoomId *currentRoomId);
bool handleWorldInteraction(const struct player *player, RoomId currentRoomId,
							GameState *gameState);
void moveCheck(struct player *player, RoomId *currentRoomId, GameState *gameState);

#endif