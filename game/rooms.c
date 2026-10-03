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


static const uint8_t starter_map[MAP_HEIGHT][MAP_WIDTH] = {
	[0] = { [0 ... (MAP_WIDTH / 2 - 1)] = MAP_WALL,
		[MAP_WIDTH / 2] = MAP_VERTICALDOOR,
		[MAP_WIDTH / 2 + 1 ... (MAP_WIDTH - 1)] = MAP_WALL },
	[1 ... (MAP_HEIGHT - 1)] = { [0 ... (MAP_WIDTH - 1)] = MAP_GRASS }
};



static const uint8_t halld[MAP_HEIGHT][MAP_WIDTH] = {
	[0] = { [0 ... (MAP_WIDTH / 2) - 1] = MAP_WALL, [MAP_WIDTH / 2] = MAP_VERTICALDOOR, [MAP_WIDTH / 2 + 1 ... (MAP_WIDTH - 1)] = MAP_WALL },
	[1] = {[0] = MAP_WALL, [MAP_WIDTH-1] = MAP_WALL},
	[2] = { [0] = MAP_WALL, [4] = MAP_BENCH, [11] = MAP_BENCH, [15] = MAP_WALL },
	[3 ... ((MAP_HEIGHT/2) - 2)] = {[0] = MAP_WALL, [MAP_WIDTH-1] = MAP_WALL},
	[(MAP_HEIGHT/2) - 1] = {[0] = MAP_WESTDOOR, [MAP_WIDTH - 1] = MAP_EASTDOOR},
	[(MAP_HEIGHT/2) ... MAP_HEIGHT - 5] = {[0]= MAP_WALL, [MAP_WIDTH - 1]= MAP_WALL},
	[MAP_HEIGHT - 4] = { [0] = MAP_WALL, [4] = MAP_TABLE, [11] = MAP_TABLE, [15] = MAP_WALL},
	[MAP_HEIGHT - 3] = {[0]= MAP_WALL, [MAP_WIDTH - 1]= MAP_WALL},
	[MAP_HEIGHT -2] = {[0 ... (MAP_WIDTH /2) -1] = MAP_WALL, [MAP_WIDTH / 2] = MAP_VERTICALDOOR, [MAP_WIDTH / 2 + 1 ... (MAP_WIDTH - 1)] = MAP_WALL}
};

static const uint8_t lecture[MAP_HEIGHT][MAP_WIDTH] = {
	[(MAP_HEIGHT / 2) -1] = { [0] = MAP_WESTDOOR }
};

static const uint8_t stairs[MAP_HEIGHT][MAP_WIDTH] = {
	[(MAP_HEIGHT / 2)-1] = { [MAP_WIDTH - 1] = MAP_EASTDOOR, [0] = MAP_WESTDOOR }
};

static const uint8_t trash[MAP_HEIGHT][MAP_WIDTH] = {
	[10] = { [MAP_WIDTH / 2] = MAP_VERTICALDOOR }
};

static const uint8_t hallu[MAP_HEIGHT][MAP_WIDTH] = {
	[0] = { [0 ... (MAP_WIDTH / 2) - 1] = MAP_WALL, [MAP_WIDTH / 2] = MAP_VERTICALDOOR, [MAP_WIDTH / 2 + 1 ... MAP_WIDTH - 1] = MAP_WALL },
	[1] = { [0] = MAP_WALL, [MAP_WIDTH - 1] = MAP_WALL },
	[2] = { [0] = MAP_WALL, [MAP_WIDTH - 1] = MAP_WALL },
	[3] = { [0] = MAP_WALL, [4] = MAP_BENCH, [11] = MAP_BENCH, [MAP_WIDTH - 1] = MAP_WALL },
	[4] = { [0] = MAP_WALL, [MAP_WIDTH - 1] = MAP_WALL },
	[5] = { [0] = MAP_WESTDOOR, [MAP_WIDTH - 1] = MAP_EASTDOOR },
	[6] = { [0] = MAP_WALL, [MAP_WIDTH - 1] = MAP_WALL },
	[7] = { [0] = MAP_WALL, [MAP_WIDTH - 1] = MAP_WALL },
	[8] = { [0] = MAP_WALL, [4] = MAP_TABLE, [11] = MAP_TABLE, [MAP_WIDTH - 1] = MAP_WALL },
	[9] = { [0] = MAP_WALL, [MAP_WIDTH - 1] = MAP_WALL },
	[10] = { [0 ... (MAP_WIDTH / 2) - 1] = MAP_WALL, [MAP_WIDTH / 2] = MAP_VERTICALDOOR, [MAP_WIDTH / 2 + 1 ... MAP_WIDTH - 1] = MAP_WALL },
	[11] = { [0] = MAP_WALL, [MAP_WIDTH - 1] = MAP_WALL }
};

static const uint8_t balcony[MAP_HEIGHT][MAP_WIDTH] = {
	[0] = { [0 ... (MAP_WIDTH / 2) - 1] = MAP_WALL, [MAP_WIDTH / 2] = MAP_VERTICALDOOR, [MAP_WIDTH / 2 + 1 ... MAP_WIDTH - 1] = MAP_WALL },
	[1] = { [0] = MAP_WALL, [MAP_WIDTH - 1] = MAP_WALL },
	[2] = { [0] = MAP_WALL, [MAP_WIDTH - 1] = MAP_WALL },
	[3] = { [0] = MAP_WALL, [4] = MAP_WALL, [11] = MAP_WALL, [MAP_WIDTH - 1] = MAP_WALL },
	[4] = { [0] = MAP_WALL, [4] = MAP_WALL, [11] = MAP_WALL, [MAP_WIDTH - 1] = MAP_WALL },
	[5] = { [0] = MAP_WALL, [4] = MAP_WALL, [11] = MAP_WALL, [MAP_WIDTH - 1] = MAP_WALL },
	[6] = { [0] = MAP_WALL, [4] = MAP_WALL, [11] = MAP_WALL, [MAP_WIDTH - 1] = MAP_WALL },
	[7] = { [0] = MAP_WALL, [MAP_WIDTH - 1] = MAP_WALL },
	[8 ... 10] = { [0 ... MAP_WIDTH - 1] = MAP_GRASS },
	[11] = { [0 ... MAP_WIDTH - 1] = MAP_GRASS }
};

static const uint8_t taroom[MAP_HEIGHT][MAP_WIDTH] = {
	[10] = { [MAP_WIDTH / 2] = MAP_VERTICALDOOR },
	[0] = { [MAP_WIDTH / 2] = MAP_VERTICALDOOR }
};

static const uint8_t professor[MAP_HEIGHT][MAP_WIDTH] = {
	[10] = { [MAP_WIDTH / 2] = MAP_VERTICALDOOR },
	[0] = { [MAP_WIDTH / 2] = MAP_VERTICALDOOR }
};

static const uint8_t professorcomputer[MAP_HEIGHT][MAP_WIDTH] = {
	[10] = { [MAP_WIDTH / 2] = MAP_VERTICALDOOR }
};

static const uint8_t hallu2[MAP_HEIGHT][MAP_WIDTH] = {
	[0] = { [0 ... (MAP_WIDTH / 2) - 1] = MAP_WALL, [MAP_WIDTH / 2] = MAP_VERTICALDOOR, [MAP_WIDTH / 2 + 1 ... MAP_WIDTH - 1] = MAP_WALL },
	[1] = { [0] = MAP_WALL, [MAP_WIDTH - 1] = MAP_WALL },
	[2] = { [0] = MAP_WALL, [4] = MAP_BENCH, [11] = MAP_BENCH, [MAP_WIDTH - 1] = MAP_WALL },
	[3] = { [0] = MAP_WALL, [MAP_WIDTH - 1] = MAP_WALL },
	[4] = { [0] = MAP_WALL, [MAP_WIDTH - 1] = MAP_WALL },
	[5] = { [0] = MAP_WALL, [MAP_WIDTH - 1] = MAP_EASTDOOR },
	[6] = { [0] = MAP_WALL, [MAP_WIDTH - 1] = MAP_WALL },
	[7] = { [0] = MAP_WALL, [MAP_WIDTH - 1] = MAP_WALL },
	[8] = { [0] = MAP_WALL, [MAP_WIDTH - 1] = MAP_WALL },
	[9] = { [0] = MAP_WALL, [4] = MAP_TABLE, [11] = MAP_TABLE, [MAP_WIDTH - 1] = MAP_WALL },
	[10] = { [0 ... (MAP_WIDTH / 2) - 1] = MAP_WALL, [MAP_WIDTH / 2] = MAP_VERTICALDOOR, [MAP_WIDTH / 2 + 1 ... MAP_WIDTH - 1] = MAP_WALL },
	[11] = { [0] = MAP_WALL, [MAP_WIDTH - 1] = MAP_WALL }
};

#undef HALL_TOP_ROW
#undef HALL_SIDE_ROW
#undef HALL_EAST_DOOR_ROW
#undef HALL_BOTTOM_ROW

static const uint8_t computerroom[MAP_HEIGHT][MAP_WIDTH] = {
	[0] = { [0 ... (MAP_WIDTH / 2) - 1] = MAP_WALL, [MAP_WIDTH / 2] = MAP_WALL, [MAP_WIDTH / 2 + 1 ... MAP_WIDTH - 1] = MAP_WALL },
	[1] = { [0] = MAP_WALL, [MAP_WIDTH - 1] = MAP_WALL },
	[2] = { [0] = MAP_WALL, [3] = MAP_COMPUTER, [6] = MAP_COMPUTER, [9] = MAP_COMPUTER, [12] = MAP_COMPUTER, [MAP_WIDTH - 1] = MAP_WALL },
	[3] = { [0] = MAP_WALL, [3] = MAP_COMPUTER, [6] = MAP_COMPUTER, [9] = MAP_COMPUTER, [12] = MAP_COMPUTER, [MAP_WIDTH - 1] = MAP_WALL },
	[4] = { [0] = MAP_WALL, [3] = MAP_COMPUTER, [6] = MAP_COMPUTER, [9] = MAP_COMPUTER, [12] = MAP_COMPUTER, [MAP_WIDTH - 1] = MAP_WALL },
	[5] = { [0] = MAP_WALL, [3] = MAP_COMPUTER, [6] = MAP_COMPUTER, [9] = MAP_COMPUTER, [12] = MAP_COMPUTER, [MAP_WIDTH - 1] = MAP_WALL },
	[6] = { [0] = MAP_WALL, [3] = MAP_COMPUTER, [6] = MAP_COMPUTER, [9] = MAP_COMPUTER, [12] = MAP_COMPUTER, [MAP_WIDTH - 1] = MAP_WALL },
	[7] = { [0] = MAP_WALL, [3] = MAP_COMPUTER, [6] = MAP_COMPUTER, [9] = MAP_COMPUTER, [12] = MAP_COMPUTER, [MAP_WIDTH - 1] = MAP_WALL },
	[8] = { [0] = MAP_WALL, [3] = MAP_COMPUTER, [6] = MAP_COMPUTER, [9] = MAP_COMPUTER, [12] = MAP_COMPUTER, [MAP_WIDTH - 1] = MAP_WALL },
	[9] = { [0] = MAP_WALL, [MAP_WIDTH - 1] = MAP_WALL },
	[10] = { [0 ... (MAP_WIDTH / 2) - 1] = MAP_WALL, [MAP_WIDTH / 2] = MAP_VERTICALDOOR, [MAP_WIDTH / 2 + 1 ... MAP_WIDTH - 1] = MAP_WALL },
	[11] = { [0] = MAP_WALL, [MAP_WIDTH - 1] = MAP_WALL }
};

static const uint8_t lunchroom[MAP_HEIGHT][MAP_WIDTH] = {
	[0] = { [0 ... (MAP_WIDTH / 2) - 1] = MAP_WALL, [MAP_WIDTH / 2] = MAP_VERTICALDOOR, [MAP_WIDTH / 2 + 1 ... MAP_WIDTH - 1] = MAP_WALL },
	[1] = { [0] = MAP_WALL, [MAP_WIDTH - 1] = MAP_WALL },
	[2] = { [0] = MAP_WALL, [2] = MAP_TABLE, [5] = MAP_TABLE, [11] = MAP_TABLE, [14] = MAP_TABLE, [MAP_WIDTH - 1] = MAP_WALL },
	[3] = { [0] = MAP_WALL, [MAP_WIDTH - 1] = MAP_WALL },
	[4] = { [0] = MAP_WALL, [2] = MAP_TABLE, [5] = MAP_TABLE, [11] = MAP_TABLE, [14] = MAP_TABLE, [MAP_WIDTH - 1] = MAP_WALL },
	[5] = { [0] = MAP_WALL, [MAP_WIDTH - 1] = MAP_WALL },
	[6] = { [0] = MAP_WALL, [2] = MAP_TABLE, [5] = MAP_TABLE, [11] = MAP_TABLE, [14] = MAP_TABLE, [MAP_WIDTH - 1] = MAP_WALL },
	[7] = { [0] = MAP_WALL, [MAP_WIDTH - 1] = MAP_WALL },
	[8] = { [0] = MAP_WALL, [2] = MAP_TABLE, [5] = MAP_TABLE, [11] = MAP_TABLE, [14] = MAP_TABLE, [MAP_WIDTH - 1] = MAP_WALL },
	[9] = { [0] = MAP_WALL, [MAP_WIDTH - 1] = MAP_WALL },
	[10] = { [0] = MAP_WALL, [MAP_WIDTH - 1] = MAP_WALL },
	[11] = { [0] = MAP_WALL, [MAP_WIDTH - 1] = MAP_WALL }
};
const RoomDefinition roomDefinitions[ROOM_COUNT] = { // initialize each room
	[OUTSIDE] = {
		.name = "Outside",
		.map = starter_map,
		.exits = {
			[NORTH] = { //DEFINEDNORTH
				.connected = true,
				.destination = HALLD,
				.doorTileX = MAP_WIDTH / 2,
				.doorTileY = 0,
				.spawnPixelX = (MAP_WIDTH / 2) * MAP_TILE_SIZE,
				.spawnPixelY = (MAP_HEIGHT - 3) * MAP_TILE_SIZE,
				.requirement = DOOR_OPEN,
				.QuestId= 0
			}
		}
	},
	[HALLD] = {
		.name = "Hall Floor 1",
		.map = halld,
		.exits = {
			[SOUTH] = { //DEFINEDSOUTH
				.connected = true,
				.destination = OUTSIDE,
				.doorTileX = MAP_WIDTH / 2,
				.doorTileY = 10,
				.spawnPixelX = (MAP_WIDTH / 2) * MAP_TILE_SIZE,
				.spawnPixelY = 2 * MAP_TILE_SIZE,
				.requirement = DOOR_OPEN
			},
			[EAST] = { //DEFINEDEAST
				.connected = true,
				.destination = LECTURE,
				.doorTileX = MAP_WIDTH - 1,
				.doorTileY = (MAP_HEIGHT / 2) -1,
				.spawnPixelX = 2 * MAP_TILE_SIZE,
				.spawnPixelY = (MAP_HEIGHT / 2) * MAP_TILE_SIZE,
				.requirement = DOOR_OPEN
			},
			[WEST] = {
				.connected = true,
				.destination = STAIRS,
				.doorTileX = 0,
				.doorTileY = (MAP_HEIGHT / 2) -1,
				.spawnPixelX = (MAP_WIDTH - 3) * MAP_TILE_SIZE,
				.spawnPixelY = (MAP_HEIGHT / 2) * MAP_TILE_SIZE,
				.requirement = DOOR_OPEN
			},
			[NORTH] = {
				.connected = true,
				.destination = TRASH,
				.doorTileX = MAP_WIDTH / 2,
				.doorTileY = 0,
				.spawnPixelX = (MAP_WIDTH / 2) * MAP_TILE_SIZE,
				.spawnPixelY = (MAP_HEIGHT - 3) * MAP_TILE_SIZE,
				.requirement = DOOR_OPEN,
			}
		}
	},
	[LECTURE] = { 
		.name = "Lecture Hall E1",
		.map = lecture,
		.exits = {
			[WEST] = { //DEFINEDWEST
				.connected = true,
				.destination = HALLD,
				.doorTileX = 0,
				.doorTileY = (MAP_HEIGHT / 2) -1,
				.spawnPixelX = (MAP_WIDTH - 3) * MAP_TILE_SIZE,
				.spawnPixelY = (MAP_HEIGHT / 2) * MAP_TILE_SIZE,
				.requirement = DOOR_OPEN
			}
		}
	},
	[STAIRS] = {
		.name = "Stairway",
		.map = stairs,
		.exits = {
			[EAST] = {
				.connected = true,
				.destination = HALLD,
				.doorTileX = MAP_WIDTH - 1,
				.doorTileY = (MAP_HEIGHT / 2) -1,
				.spawnPixelX = 2 * MAP_TILE_SIZE,
				.spawnPixelY = (MAP_HEIGHT / 2) * MAP_TILE_SIZE,
				.requirement = DOOR_OPEN
			},
			[WEST] = { 
				.connected = true,
				.destination = HALLU,
				.doorTileX = 0,
				.doorTileY = (MAP_HEIGHT / 2) -1,
				.spawnPixelX = (MAP_WIDTH - 3) * MAP_TILE_SIZE,
				.spawnPixelY = (MAP_HEIGHT / 2) * MAP_TILE_SIZE,
				.requirement = DOOR_OPEN
			}
		}
	},
	[TRASH] = {
		.name = "E-House basement garbage room",
		.map = trash,
		.exits = {
			[SOUTH] = {
				.connected = true,
				.destination = HALLD,
				.doorTileX = MAP_WIDTH / 2,
				.doorTileY = 10,
				.spawnPixelX = (MAP_WIDTH / 2) * MAP_TILE_SIZE,
				.spawnPixelY = 2 * MAP_TILE_SIZE,
				.requirement = DOOR_OPEN
			}
		}
	},
	[HALLU] = {
		.name = "Hall Floor 2",
		.map = hallu,
		.exits = {
			[SOUTH] = { 
				.connected = true,
				.destination = BALCONY,
				.doorTileX = MAP_WIDTH / 2,
				.doorTileY = 10,
				.spawnPixelX = (MAP_WIDTH / 2) * MAP_TILE_SIZE,
				.spawnPixelY = 2 * MAP_TILE_SIZE,
				.requirement = DOOR_OPEN
			},
			[EAST] = { 
				.connected = true,
				.destination = STAIRS,
				.doorTileX = MAP_WIDTH - 1,
				.doorTileY = (MAP_HEIGHT / 2) -1,
				.spawnPixelX = 2 * MAP_TILE_SIZE,
				.spawnPixelY = (MAP_HEIGHT / 2) * MAP_TILE_SIZE,
				.requirement = DOOR_OPEN
			},
			[WEST] = {
				.connected = true,
				.destination = HALLU2,
				.doorTileX = 0,
				.doorTileY = (MAP_HEIGHT / 2) -1,
				.spawnPixelX = (MAP_WIDTH - 3) * MAP_TILE_SIZE,
				.spawnPixelY = (MAP_HEIGHT / 2) * MAP_TILE_SIZE,
				.requirement = DOOR_OPEN
			},
			[NORTH] = {
				.connected = true,
				.destination = TAROOM,
				.doorTileX = MAP_WIDTH / 2,
				.doorTileY = 0,
				.spawnPixelX = (MAP_WIDTH / 2) * MAP_TILE_SIZE,
				.spawnPixelY = (MAP_HEIGHT - 3) * MAP_TILE_SIZE,
				.requirement = DOOR_OPEN,
			}
		}
	},
	[BALCONY] = {
		.name = "The Balcony",
		.map = balcony,
		.exits = {
			[NORTH] = { 
				.connected = true,
				.destination = HALLU,
				.doorTileX = MAP_WIDTH / 2,
				.doorTileY = 0,
				.spawnPixelX = (MAP_WIDTH / 2) * MAP_TILE_SIZE,
				.spawnPixelY = (MAP_HEIGHT - 3) * MAP_TILE_SIZE,
				.requirement = DOOR_OPEN,
			}
		}
	},
	[TAROOM] = {
		.name = "Teacher Assistants office",
		.map = taroom,
		.exits = {
			[NORTH] = { 
				.connected = true,
				.destination = PROFESSOR,
				.doorTileX = MAP_WIDTH / 2,
				.doorTileY = 0,
				.spawnPixelX = (MAP_WIDTH / 2) * MAP_TILE_SIZE,
				.spawnPixelY = (MAP_HEIGHT - 3) * MAP_TILE_SIZE,
				.requirement = DOOR_OPEN,
				.QuestId= 0
			},
			[SOUTH] = { 
				.connected = true,
				.destination = HALLU,
				.doorTileX = MAP_WIDTH / 2,
				.doorTileY = 10,
				.spawnPixelX = (MAP_WIDTH / 2) * MAP_TILE_SIZE,
				.spawnPixelY = 2 * MAP_TILE_SIZE,
				.requirement = DOOR_OPEN
			},
		}
	},
	[PROFESSOR] = {
		.name = "The Professors Office",
		.map = professor,
		.exits = {
			[SOUTH] = {
				.connected = true,
				.destination = TAROOM,
				.doorTileX = MAP_WIDTH / 2,
				.doorTileY = 10,
				.spawnPixelX = (MAP_WIDTH / 2) * MAP_TILE_SIZE,
				.spawnPixelY = 2 * MAP_TILE_SIZE,
				.requirement = DOOR_OPEN
			},
			[NORTH] = { 
				.connected = true,
				.destination = COMPUTER,
				.doorTileX = MAP_WIDTH / 2,
				.doorTileY = 0,
				.spawnPixelX = (MAP_WIDTH / 2) * MAP_TILE_SIZE,
				.spawnPixelY = (MAP_HEIGHT - 3) * MAP_TILE_SIZE,
				.requirement = DOOR_OPEN,
				.QuestId= 0
			},
		}
	},
	[HALLU2] = {
		.name = "Second Hallway Floor 2",
		.map = hallu2,
		.exits = {
			[EAST] = {
				.connected = true,
				.destination = HALLU,
				.doorTileX = MAP_WIDTH - 1,
				.doorTileY = (MAP_HEIGHT / 2) -1,
				.spawnPixelX = 2 * MAP_TILE_SIZE,
				.spawnPixelY = (MAP_HEIGHT / 2) * MAP_TILE_SIZE,
				.requirement = DOOR_OPEN
			},
			[NORTH] = { 
				.connected = true,
				.destination = COMPUTERROOM,
				.doorTileX = MAP_WIDTH / 2,
				.doorTileY = 0,
				.spawnPixelX = (MAP_WIDTH / 2) * MAP_TILE_SIZE,
				.spawnPixelY = (MAP_HEIGHT - 3) * MAP_TILE_SIZE,
				.requirement = DOOR_OPEN,
			},
			[SOUTH] = { 
				.connected = true,
				.destination = LUNCHROOM,
				.doorTileX = MAP_WIDTH / 2,
				.doorTileY = 10,
				.spawnPixelX = (MAP_WIDTH / 2) * MAP_TILE_SIZE,
				.spawnPixelY = 2 * MAP_TILE_SIZE,
				.requirement = DOOR_OPEN
			}
			
		}
	},
	[COMPUTERROOM] = {
		.name = "Computer Room for labs",
		.map = computerroom,
		.exits = {
			[SOUTH] = {
				.connected = true,
				.destination = HALLU2,
				.doorTileX = MAP_WIDTH / 2,
				.doorTileY = 10,
				.spawnPixelX = (MAP_WIDTH / 2) * MAP_TILE_SIZE,
				.spawnPixelY = 2 * MAP_TILE_SIZE,
				.requirement = DOOR_OPEN
			}
		}
	},
	[LUNCHROOM] = {
		.name = "The students lunchroom",
		.map = lunchroom,
		.exits = {
			[NORTH] = { 
				.connected = true,
				.destination = HALLU2,
				.doorTileX = MAP_WIDTH / 2,
				.doorTileY = 0,
				.spawnPixelX = (MAP_WIDTH / 2) * MAP_TILE_SIZE,
				.spawnPixelY = (MAP_HEIGHT - 3) * MAP_TILE_SIZE,
				.requirement = DOOR_OPEN,
			}
		}
	},
	[COMPUTER] = {
		.name = "PROFFESSORS COMPUTER",
		.map = professorcomputer,
		.exits = {
			[SOUTH] = {
				.connected = true,
				.destination = PROFESSOR,
				.doorTileX = MAP_WIDTH / 2,
				.doorTileY = 10,
				.spawnPixelX = (MAP_WIDTH / 2) * MAP_TILE_SIZE,
				.spawnPixelY = 2 * MAP_TILE_SIZE,
				.requirement = DOOR_OPEN
			}
		}
	},
};

const RoomDefinition *getRoomDefinition(RoomId id) { // 
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
			const uint8_t expectedDoor = direction == EAST ? MAP_EASTDOOR :
				(direction == WEST ? MAP_WESTDOOR : MAP_VERTICALDOOR);
			if (room->map[exit->doorTileY][exit->doorTileX] != expectedDoor) return false;

			const RoomDefinition *destination = &roomDefinitions[exit->destination];
			if (exit->spawnPixelX < 0 || exit->spawnPixelX >= MAP_WIDTH * MAP_TILE_SIZE ||
				exit->spawnPixelY < 0 || exit->spawnPixelY >= MAP_HEIGHT * MAP_TILE_SIZE) {
				return false;
			}
			const uint8_t spawnTile = destination->map[exit->spawnPixelY / MAP_TILE_SIZE]
				[exit->spawnPixelX / MAP_TILE_SIZE];
			if (spawnTile >= MAP_TILE_COUNT || spawnTile == MAP_WALL) return false;

			Direction opposite = oppositeDirection((Direction) direction);
			const RoomExit *returnExit = &destination->exits[opposite];
			if (!returnExit->connected || returnExit->destination != (RoomId) roomId) return false;
		}
	}

	return true;
}