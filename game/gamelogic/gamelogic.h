#ifndef GAMELOGICH
#define GAMELOGICH

#include <stdbool.h>
#define PLAYER_WIDTH 32
#define PLAYER_HEIGHT 32

struct player;

bool playerHitsWall(const struct player *player);
bool tryMovePlayer(struct player *player, int dx, int dy);
void moveCheck(struct player *player);

#endif
