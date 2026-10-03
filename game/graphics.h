#ifndef GRAPHICSH
#define GRAPHICSH
#include <stdbool.h>
#include <stdint.h>

#define MAP_WIDTH 16
#define MAP_HEIGHT 12
#define MAP_TILE_SIZE 20

enum mapTile {
        MAP_FLOOR,
        MAP_WALL,
        MAP_GRASS,
        MAP_BENCH, 
        MAP_TABLE, 
        MAP_COMPUTER,
        MAP_PROFCOMP,
        MAP_WESTDOOR,
        MAP_VERTICALDOOR,
        MAP_EASTDOOR,
        MAP_TILE_COUNT
};

struct tileDefinition {
        const uint8_t *pixels;
        bool blocksMovement;
};

extern const struct tileDefinition tileDefinitions[MAP_TILE_COUNT];

struct player {
        int x;
        int y;
        bool left;
};

uint8_t rgbFix(uint32_t piskel);
void drawMap(const uint8_t map[MAP_HEIGHT][MAP_WIDTH]);
void drawPlayer(struct player myPlayer);
void drawTextBox(const char *text);

void scroll(int direction, int speed);
void setCamera(int x, int y);
void centerCamera(int playerx, int playery, int mapWidth, int mapHeight);



#endif 
