#ifndef ROOMSH
#define ROOMSH

#include "graphics.h"

typedef enum {
	NORTH,
	SOUTH,
	EAST,
	WEST,
	DIR_COUNT // 4
} Direction; // indexes the exits array in RoomDefinition

typedef enum {
	OUTSIDE, 
	HALLD, 
	LECTURE,
    STAIRS,
	TRASH,
	HALLU,
	TAROOM,
	PROFESSOR,
	BALCONY,
	HALLU2,
	COMPUTERROOM,
	LUNCHROOM,
	COMPUTER,
	ROOM_COUNT // the number of our rooms
} RoomId;

typedef enum {
    ITEM_CROWBAR,
    ITEM_KEYCARD,
    ITEM_COUNT
} ItemId;

typedef enum {
    DOOR_OPEN,
    DOOR_REQUIRES_RELIC,
    DOOR_REQUIRES_QUEST,
    DOOR_REQUIRES_CODE,
    DOOR_REQUIRES_ITEM
} DoorRequirement;

typedef struct { // graph data structure
	bool connected;
	RoomId destination;
	int doorTileX; // door of the current room
	int doorTileY; 
	int spawnPixelX; // spawn in the NEXT room
	int spawnPixelY;
    DoorRequirement requirement;
	int QuestId; // optional quest requirement for the door, if applicable
	int passcode; // optional passcode for the door, if applicable
    int itemId; // optional item requirement for the door
} RoomExit;

typedef struct {
	const char *name;
	const uint8_t (*map)[MAP_WIDTH]; // piskel map data for the room
	RoomExit exits[DIR_COUNT]; // array of exits, indexed by Direction
} RoomDefinition;

extern const char *const dirNames[DIR_COUNT];
extern const RoomDefinition roomDefinitions[ROOM_COUNT];

const RoomDefinition *getRoomDefinition(RoomId id);
bool validateRoomDefinitions(void);

#endif
