#include "graphics.h"
#include "graphicsdata.h"
#include "vga.h"
#include <stdint.h>

#define NEW_PISKEL_FRAME_COUNT 1
#define NEW_PISKEL_FRAME_WIDTH 32
#define NEW_PISKEL_FRAME_HEIGHT 32

uint8_t rgbFix(uint32_t piskel) { // piskel gives us 0xAARRGGBB this function converts to 0bRRRGGGBB
    if(piskel == 0) return 0x67;
    uint8_t result = 0;
    result |= ((piskel >> 16) & 0xE0) ; // take red shift to lsb and everything except top 3 bits 1110 0000
                   //                                                                             rrrg ggbb

    result |= ((piskel >> 11) & 0x1C); //0001 1100
                   //                    rrrg ggbb
    
    result |= ((piskel >> 6) & 0x3); // 0000 0011
                   //                   rrrg ggbb
    return result;
}

const struct tileDefinition tileDefinitions[MAP_TILE_COUNT] = {
    [MAP_GRASS] = {grass, false},
    [MAP_WALL] = {wall, true},
    [MAP_DOOR] = {door, false}
};

static void drawTile(uint8_t tile, int column, int row) {
    if (tile >= MAP_TILE_COUNT) return;

    int tileCenterX = column * MAP_TILE_SIZE + MAP_TILE_SIZE / 2;
    int tileCenterY = row * MAP_TILE_SIZE + MAP_TILE_SIZE / 2;
    fillRectColor(tileCenterX, tileCenterY, MAP_TILE_SIZE, MAP_TILE_SIZE, tileDefinitions[tile].pixels);
}

void drawMap(const uint8_t map[MAP_HEIGHT][MAP_WIDTH]) {
    clearScreen(black);
    for (int row = 0; row < MAP_HEIGHT; row++) {
        for (int column = 0; column < MAP_WIDTH; column++) {
            drawTile(map[row][column], column, row);
        }
    }
}

void drawPlayer(struct player myPlayer, const uint32_t* piskelArray) {
    int x = myPlayer.x;
    int y = myPlayer.y;
    fillRectColor(x, y, NEW_PISKEL_FRAME_WIDTH, NEW_PISKEL_FRAME_HEIGHT, piskelArray);
}

static const uint8_t *fontGlyph(char character) {
    static const uint8_t blank[7] = {0};
    static const uint8_t period[7] = {0, 0, 0, 0, 0, 0x0C, 0x0C};
    static const uint8_t comma[7] = {0, 0, 0, 0, 0x0C, 0x0C, 0x08};
    static const uint8_t exclamation[7] = {0x04, 0x04, 0x04, 0x04, 0x04, 0, 0x04};
    static const uint8_t question[7] = {0x0E, 0x11, 0x01, 0x02, 0x04, 0, 0x04};
    static const uint8_t colon[7] = {0, 0x0C, 0x0C, 0, 0x0C, 0x0C, 0};
    static const uint8_t hyphen[7] = {0, 0, 0, 0x1F, 0, 0, 0};

    if (character >= 'a' && character <= 'z') {
        character -= 'a' - 'A';
    }
    if (character >= '0' && character <= '9') {
        return font5x7[character - '0'];
    }
    if (character >= 'A' && character <= 'Z') {
        return font5x7[10 + character - 'A'];
    }

    switch (character) {
    case ' ':
        return blank;
    case '.':
        return period;
    case ',':
        return comma;
    case '!':
        return exclamation;
    case '?':
        return question;
    case ':':
        return colon;
    case '-':
        return hyphen;
    default:
        return question;
    }
}

void drawTextBox(const char *text) {
    int boxTop = screenHeight - MAP_TILE_SIZE;
    int textX = 5;
    int textY = boxTop + (MAP_TILE_SIZE - 7) / 2;

    fillRect(0, boxTop, screenWidth, MAP_TILE_SIZE, black);
    fillRect(0, boxTop, screenWidth, 1, white);

    while (text != 0 && *text != '\0' && textX + 5 <= screenWidth - 4) {
        const uint8_t *glyph = fontGlyph(*text++);
        for (int row = 0; row < 7; row++) {
            for (int column = 0; column < 5; column++) {
                if (glyph[row] & (1u << (4 - column))) {
                    drawPixel(textX + column, textY + row, white);
                }
            }
        }
        textX += 6;
    }
}