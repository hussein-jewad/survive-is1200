#ifndef GRAPHICSDATA
#define GRAPHICSDATA
#include "graphics.h"
#include <stdint.h>

extern const uint8_t font5x7[36][7];
extern const uint32_t player[1024];
extern const uint32_t grass[MAP_TILE_SIZE * MAP_TILE_SIZE];
extern const uint32_t wall[MAP_TILE_SIZE * MAP_TILE_SIZE];
extern const uint32_t door[MAP_TILE_SIZE * MAP_TILE_SIZE];

#endif