#include "room_transitions.h"

#include "other.h"
#include "quest_logic.h"
#include "vga.h"

#include <stddef.h>

#define DOOR_INTERACTION_RADIUS (MAP_TILE_SIZE + MAP_TILE_SIZE / 2) // Maximum distance from a door center at which it can be used.

static void drawEnterPrompt(const char *roomName) {
    static const char prefix[] = "ENTER "; // Text shown before the destination room name.
    char message[64]; // Buffer for the complete prompt.
    size_t length = 0; // Next position to fill in the message buffer.

    for (size_t i = 0; prefix[i] != '\0'; i++) {
        message[length++] = prefix[i]; // Copy the "ENTER " prefix into the message.
    }
    while (roomName != NULL && *roomName != '\0' && length < sizeof(message) - 2) {
        message[length++] = *roomName++; // Append the room name, leaving space for '?' and '\0'.
    }
    message[length++] = '?'; // End the prompt with a question mark.
    message[length] = '\0'; // Terminate the string.
    drawTextBox(message); // Display the prompt.
}

static bool doorRequirementMet(const RoomExit *exit, const GameState *gameState) {
    if (exit == NULL || gameState == NULL) return false; // safety check

    switch (exit->requirement) {
    case DOOR_OPEN:
        return true; // This exit has no lock requirement.
    case DOOR_REQUIRES_RELIC:
        if (exit->QuestId < 0 || exit->QuestId >= QUEST_COUNT) return false; // Reject an invalid quest ID.
        return questHasRelic(gameState, (QuestId) exit->QuestId); // Allow passage if that quest's relic was obtained.
    case DOOR_REQUIRES_QUEST:
        if (exit->QuestId < 0 || exit->QuestId >= QUEST_COUNT) return false; // Reject an invalid quest ID.
        return questIsComplete(gameState, (QuestId) exit->QuestId); // Allow passage when the specified quest is complete.
    case DOOR_REQUIRES_ITEM:
        return exit->itemId >= 0 && exit->itemId < ITEM_COUNT && // Check the item ID is valid before indexing the inventory.
               gameState->items[exit->itemId];
    case DOOR_REQUIRES_CODE:
        return get_sw() == exit->passcode; // Compare the hardware switch value with the door's passcode.
    default:
        return false; // Unknown requirement types do not grant access.
    }
}

static const char *doorLockMessage(const RoomExit *exit) {
    switch (exit->requirement) {
    case DOOR_REQUIRES_RELIC:
        return "YOU NEED THE RELIC TO PASS!"; // Explain that the relic is missing.
    case DOOR_REQUIRES_QUEST:
        return "COMPLETE THE QUEST FIRST!"; // Explain that the required quest is incomplete.
    case DOOR_REQUIRES_CODE:
        return "ENTER THE CODE TO PASS!"; // Explain that the code is needed.
    case DOOR_REQUIRES_ITEM:
        return exit->itemId == ITEM_KEYCARD ? // Select the message for the item required by this door.
            "YOU NEED THE KEYCARD TO ENTER!" :
            "YOU NEED THE CROWBAR TO BREAK IN!";
    default:
        return "THE DOOR IS LOCKED!"; // Generic message for any other locked door.
    }
}

bool checkRoomTransition(struct player *player, RoomId *currentRoomId,
                         GameState *gameState, bool buttonPressed) {
    if (player == NULL || currentRoomId == NULL || gameState == NULL || // Reject missing state and negative positions.
        player->x < 0 || player->y < 0) {
        return false;
    }

    const RoomDefinition *currentRoom = getRoomDefinition(*currentRoomId); // Look up this room's map and exits using its ID.
    if (currentRoom == NULL) return false; // Stop if the current room ID is invalid.

    const int tileX = player->x / MAP_TILE_SIZE; // Convert the player's pixel x-coordinate to a map column.
    const int tileY = player->y / MAP_TILE_SIZE; // Convert the player's pixel y-coordinate to a map row.
    if (tileX >= MAP_WIDTH || tileY >= MAP_HEIGHT) return false; // Ignore positions outside the room map.

    for (int direction = 0; direction < DIR_COUNT; direction++) {
        const RoomExit *exit = &currentRoom->exits[direction]; // Get the exit for this direction.
        const int doorCenterX = exit->doorTileX * MAP_TILE_SIZE + MAP_TILE_SIZE / 2; // Convert the door's map column to its pixel center.
        const int doorCenterY = exit->doorTileY * MAP_TILE_SIZE + MAP_TILE_SIZE / 2; // Convert the door's map row to its pixel center.
        const int deltaX = player->x - doorCenterX; // Horizontal distance from the player to the door.
        const int deltaY = player->y - doorCenterY; // Vertical distance from the player to the door.
        if (!exit->connected || deltaX * deltaX + deltaY * deltaY > DOOR_INTERACTION_RADIUS * DOOR_INTERACTION_RADIUS) { // Skip missing exits and doors outside the interaction radius. 
            continue;
        }

        const RoomDefinition *destination = getRoomDefinition(exit->destination); // Look up the room this exit leads to.
        if (destination == NULL) return false; // Stop if the exit points to an invalid room.

        drawEnterPrompt(destination->name); // Show the player which room they can enter.
        if (!buttonPressed) return false; // Wait for the interaction button before changing rooms.

        if (!doorRequirementMet(exit, gameState)) { // Check any quest, relic, item, or code lock attached to this exit.
            gameStateSetMessage(gameState, doorLockMessage(exit)); // Store the appropriate locked-door message.
            drawTextBox(gameState->message); // Show why passage was denied.
            return false; // Stay in the current room.
        }

        *currentRoomId = exit->destination; // Update the current room to the exit's destination.
        player->x = exit->spawnPixelX; // Place the player at the destination room's configured x-coordinate.
        player->y = exit->spawnPixelY; // Place the player at the destination room's configured y-coordinate.
        if (exit->destination == COMPUTERROOM && // Show a one-time hint when entering after completing the code quest.
            questIsComplete(gameState, QUEST_FINDCODE) &&
            !gameState->computerRoomHintShown) {
            gameState->computerRoomHintShown = true; // Prevent this hint from appearing again.
            gameStateSetMessage(
                gameState, "KEYCARD FOR PROF ROOM. FIND A TA IN COMPUTER ROOM."); // Tell the player where to find the keycard.
        }
        return true; // The room transition succeeded.
    }

    return false; // No nearby connected exit was used.
}