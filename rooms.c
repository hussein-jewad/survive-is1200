#include "rooms.h"

#include <stddef.h>

const char *const dirNames[DIR_COUNT] = { "North", "South", "East", "West" };

static Direction oppositeDirection(Direction direction) {
	switch (direction) {
	case NORTH:
		return SOUTH;
	case SOUTH:
		return NORTH;
	case EAST:
		return WEST;
	case WEST:
		return EAST;
	default:
		return DIR_COUNT;
	}
}

static const uint8_t cave_map[MAP_HEIGHT][MAP_WIDTH] = {
	[6] = { [0] = MAP_DOOR, [8] = MAP_WALL },
	[0] = { [MAP_WIDTH / 2] = MAP_DOOR }
};

static const uint8_t starter_map[MAP_HEIGHT][MAP_WIDTH] = {
    [3] = { [4] = MAP_WALL},
	[6] = { [MAP_WIDTH - 1] = MAP_DOOR }
};

static const uint8_t new_map[MAP_HEIGHT][MAP_WIDTH] = {
	[MAP_HEIGHT - 2] = { [MAP_WIDTH / 2] = MAP_DOOR }
};

const RoomDefinition roomDefinitions[ROOM_COUNT] = { // initialize each room
	[ROOM_GRASSLAND] = {
		.name = "Grassland",
		.map = starter_map,
		.exits = {
			[EAST] = {
				.connected = true,
				.destination = ROOM_CAVE,
				.doorTileX = MAP_WIDTH - 1,
				.doorTileY = 6,
				.spawnPixelX = 60,
				.spawnPixelY = 120,
				.requirement = DOOR_REQUIRES_QUEST,
				.QuestId = 0
			}
		}
	},
	[ROOM_CAVE] = {
		.name = "Cave",
		.map = cave_map,
		.exits = {
			[WEST] = {
				.connected = true,
				.destination = ROOM_GRASSLAND,
				.doorTileX = 0,
				.doorTileY = 6,
				.spawnPixelX = 280,
				.spawnPixelY = 120,
				.requirement = DOOR_OPEN
			},
			[NORTH] = {
				.connected = true,
				.destination = ROOM_3,
				.doorTileX = MAP_WIDTH / 2,
				.doorTileY = 0,
				.spawnPixelX = (MAP_WIDTH / 2) * 20,
				.spawnPixelY = 180,
				.requirement = DOOR_OPEN
			}
		}
	},
	[ROOM_3] = {
		.name = "new room",
		.map = new_map,
		.exits = {
			[SOUTH] = {
				.connected = true,
				.destination = ROOM_CAVE,
				.doorTileX = MAP_WIDTH / 2,
				.doorTileY = MAP_HEIGHT - 2,
				.spawnPixelX = (MAP_WIDTH / 2) * 20,
				.spawnPixelY = 20,
				.requirement = DOOR_REQUIRES_CODE,
				.passcode = 32 // example passcode for the door
			}
		}
	}
};

const RoomDefinition *getRoomDefinition(RoomId id) {
	if ((unsigned int) id >= ROOM_COUNT) return NULL;
	return &roomDefinitions[id];
}

bool validateRoomDefinitions(void) {
	for (int roomId = 0; roomId < ROOM_COUNT; roomId++) {
		const RoomDefinition *room = &roomDefinitions[roomId];
		if (room->name == NULL || room->map == NULL) return false;

		for (int row = 0; row < MAP_HEIGHT; row++) {
			for (int column = 0; column < MAP_WIDTH; column++) {
				if (room->map[row][column] >= MAP_TILE_COUNT) return false;
			}
		}

		for (int direction = 0; direction < DIR_COUNT; direction++) {
			const RoomExit *exit = &room->exits[direction];
			if (!exit->connected) continue;
			if ((unsigned int) exit->destination >= ROOM_COUNT) return false;
			if ((unsigned int) exit->requirement > DOOR_REQUIRES_CODE) return false;
			if (exit->doorTileX < 0 || exit->doorTileX >= MAP_WIDTH ||
				exit->doorTileY < 0 || exit->doorTileY >= MAP_HEIGHT) {
				return false;
			}
			if (room->map[exit->doorTileY][exit->doorTileX] != MAP_DOOR) return false;

			const RoomDefinition *destination = &roomDefinitions[exit->destination];
			if (exit->spawnPixelX < 0 || exit->spawnPixelX >= MAP_WIDTH * MAP_TILE_SIZE ||
				exit->spawnPixelY < 0 || exit->spawnPixelY >= MAP_HEIGHT * MAP_TILE_SIZE) {
				return false;
			}
			const uint8_t spawnTile = destination->map[exit->spawnPixelY / MAP_TILE_SIZE]
				[exit->spawnPixelX / MAP_TILE_SIZE];
			if (tileDefinitions[spawnTile].blocksMovement) return false;

			Direction opposite = oppositeDirection((Direction) direction);
			const RoomExit *returnExit = &destination->exits[opposite];
			if (!returnExit->connected || returnExit->destination != (RoomId) roomId) return false;
		}
	}

	return true;
}