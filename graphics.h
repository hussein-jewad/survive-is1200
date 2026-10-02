#ifndef GRAPHICSH
#define GRAPHICSH
#include <stdbool.h>
#include <stdint.h>

#define MAP_WIDTH 16
#define MAP_HEIGHT 12
#define MAP_TILE_SIZE 20

enum mapTile {
        MAP_GRASS,
        MAP_WALL,
        MAP_DOOR,
        MAP_TILE_COUNT
};

struct tileDefinition {
        const uint32_t *pixels;
        bool blocksMovement;
};

extern const struct tileDefinition tileDefinitions[MAP_TILE_COUNT];

struct player {
        int x;
        int y;
};

uint8_t rgbFix(uint32_t piskel);
void drawMap(const uint8_t map[MAP_HEIGHT][MAP_WIDTH]);
void drawPlayer(struct player myPlayer, const uint32_t* piskelArray);
void drawTextBox(const char *text);

void scroll(int direction, int speed);
void setCamera(int x, int y);
void centerCamera(int playerx, int playery, int mapWidth, int mapHeight);



#endif 
