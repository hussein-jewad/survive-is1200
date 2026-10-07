#include "quest_logic.h"

#include "graphics.h"
#include "vga.h"

#include <stddef.h>

#define MAX_QUEST_STAGES 10
#define MARCO_X ((MAP_WIDTH - 2) * MAP_TILE_SIZE)
#define MARCO_Y ((MAP_HEIGHT / 2) * MAP_TILE_SIZE)
#define BALLKNOWLEDGE_X (6 * MAP_TILE_SIZE)
#define BALLKNOWLEDGE_Y (5 * MAP_TILE_SIZE)
#define CROWBAR_X ((MAP_WIDTH - 4) * MAP_TILE_SIZE + MAP_TILE_SIZE / 2)
#define CROWBAR_Y (3 * MAP_TILE_SIZE + MAP_TILE_SIZE / 2)
#define CODE_X (5 * MAP_TILE_SIZE + MAP_TILE_SIZE / 2)
#define CODE_Y (4 * MAP_TILE_SIZE + MAP_TILE_SIZE / 2)
#define COMPUTER_TA_X (8 * MAP_TILE_SIZE)
#define COMPUTER_TA_Y (2 * MAP_TILE_SIZE + MAP_TILE_SIZE / 2)
#define BALCONY_TA_X (8 * MAP_TILE_SIZE)
#define BALCONY_TA_Y (7 * MAP_TILE_SIZE)
#define PROFESSOR_COMPUTER_X ((MAP_WIDTH / 2) * MAP_TILE_SIZE)
#define PROFESSOR_COMPUTER_Y (MAP_TILE_SIZE / 2)
#define PROFESSOR_NPC_X (8 * MAP_TILE_SIZE + MAP_TILE_SIZE / 2)
#define PROFESSOR_NPC_Y (7 * MAP_TILE_SIZE + MAP_TILE_SIZE / 2)
#define KEYCARD_X BALCONY_TA_X
#define KEYCARD_Y (BALCONY_TA_Y + MAP_TILE_SIZE)
#define MRARMY_X (8 * MAP_TILE_SIZE + MAP_TILE_SIZE / 2)
#define MRARMY_Y (5 * MAP_TILE_SIZE + MAP_TILE_SIZE / 2)
#define INTERACTION_RADIUS 20

typedef struct {
    RoomId room; // Room where the quest NPC can be found.
    int npcX; // NPC's x-coordinate in pixels.
    int npcY; // NPC's y-coordinate in pixels.
    const char *const *stageMessages; // Dialogue displayed as the quest advances.
    size_t stageCount; // Number of progress stages in this quest.
    int prerequisiteQuest; // Quest that must be complete first, or -1 if there is none.
} QuestDefinition;

typedef struct {
    RoomId room; // Room containing the collectible.
    int x; // Collectible's x-coordinate in pixels.
    int y; // Collectible's y-coordinate in pixels.
    ItemId item; // ID used for the inventory and world-item state.
    QuestId quest; // Quest updated when the item is collected.
    const char *pickupMessage; // Message shown after collection.
    QuestStatus requiredQuestStage; // Quest stage required to collect the item.
    QuestStatus nextQuestStage; // Quest stage set after collection.
} QuestItemDefinition;

static const char *const marcoStageMessages[] = {
    "Dear all welcome to IS1200",
    "In this course you will learn everything",
    "We will have 67 assignments, 21 labs and 1 exam",
    "make sure to study everyday this course is cooked",
    "I will tell you more in this introductionary lecture",
    "I aint doing alldat, i gotta cheat"
};

static const char *const ballKnowledgeStageMessages[] = {
    "FIND A CROWBAR IN TRASH. USE IT ON THE TA ROOM."
};

static const QuestDefinition questDefinitions[QUEST_COUNT] = {
    [QUEST_MARCO] = {
        .room = LECTURE, // Marco's dialogue advances in the lecture room.
        .npcX = MARCO_X,
        .npcY = MARCO_Y,
        .stageMessages = marcoStageMessages,
        .stageCount = sizeof(marcoStageMessages) / sizeof(marcoStageMessages[0]),
        .prerequisiteQuest = -1 // This quest has no prerequisite.
    },
    [QUEST_BALLKNOWLEDGE] = {
        .room = HALLD, // Ball Knowledge's dialogue is in the lower hall.
        .npcX = BALLKNOWLEDGE_X,
        .npcY = BALLKNOWLEDGE_Y,
        .stageMessages = ballKnowledgeStageMessages,
        .stageCount = sizeof(ballKnowledgeStageMessages) /
                      sizeof(ballKnowledgeStageMessages[0]),
        .prerequisiteQuest = QUEST_MARCO // Marco's quest must be complete first.
    },
    [QUEST_FINDTOOL] = {
        .room = ROOM_COUNT, // This quest advances through item pickup, not an NPC.
        .stageCount = 2, // The stages cover receiving the objective and finding the crowbar.
        .prerequisiteQuest = QUEST_BALLKNOWLEDGE // Ball Knowledge's objective unlocks it.
    },
    [QUEST_FINDCODE] = {
        .stageCount = 1, // One interaction completes this quest.
        .prerequisiteQuest = QUEST_FINDTOOL // The player must find the crowbar first.
    },
    [QUEST_FINDKEYCARD] = {
        .stageCount = 3, // Tracks meeting the TA, revealing the card, and collecting it.
        .prerequisiteQuest = QUEST_FINDCODE // The player must find the code first.
    }
};

static const QuestItemDefinition questItems[] = {
    {
        .room = TRASH, // The crowbar is placed in the trash room.
        .x = CROWBAR_X,
        .y = CROWBAR_Y,
        .item = ITEM_CROWBAR,
        .quest = QUEST_FINDTOOL,
        .pickupMessage = "You have found the crowbar",
        .requiredQuestStage = S1, // The player must have been given the crowbar objective.
        .nextQuestStage = S2 // Collecting it advances the tool quest.
    },
    {
        .room = BALCONY, // The keycard is placed on the balcony.
        .x = KEYCARD_X,
        .y = KEYCARD_Y,
        .item = ITEM_KEYCARD,
        .quest = QUEST_FINDKEYCARD,
        .pickupMessage = "You picked up the keycard.",
        .requiredQuestStage = S2, // The TA must first leave the keycard behind.
        .nextQuestStage = S3 // Collecting it advances the keycard quest.
    }
};

static bool isNearPosition(const struct player *player, int x, int y) { // husien
    const int deltaX = player->x - x; // x distance to our position
    const int deltaY = player->y - y; // y distance to our position
    return deltaX * deltaX + deltaY * deltaY <= INTERACTION_RADIUS * INTERACTION_RADIUS; // distance formula tweeked to avoid square roots
}

bool questIsComplete(const GameState *gameState, QuestId questId) { // weilong
    if (gameState == NULL || (unsigned int) questId >= QUEST_COUNT) return false; // Invalid state or ID cannot represent a completed quest.

    const QuestDefinition *definition = &questDefinitions[questId]; // Read the quest's configured number of stages.
    const int status = gameState->quests[questId]; // Read the player's current progress.
    if (status == EXTENDED) return true; // EXTENDED is also treated as a completed status.
    return definition->stageCount > 0 && definition->stageCount <= MAX_QUEST_STAGES &&
           status >= (int) definition->stageCount && status <= S10; // Completion means progress reached the final valid stage.
}

static bool advanceQuest(GameState *gameState, QuestId questId) { // weilong
    if (gameState == NULL || (unsigned int) questId >= QUEST_COUNT) return false; // Reject missing game state or an invalid quest ID.

    const QuestDefinition *definition = &questDefinitions[questId]; // Get this quest's messages and stage count.
    if (definition->stageMessages == NULL || definition->stageCount == 0 ||
        definition->stageCount > MAX_QUEST_STAGES) { // A quest needs a valid message list and stage count to advance.
        return false;
    }

    const int currentStage = gameState->quests[questId]; // Read the current progress value.
    if (currentStage < NOT_STARTED || currentStage >= (int) definition->stageCount) {
        return false; // Reject invalid progress and quests that are already complete.
    }

    const size_t nextStage = (size_t) currentStage + 1; // Move forward by one stage.
    gameState->quests[questId] = (QuestStatus) nextStage; // Save the updated quest progress.
    gameStateSetMessage(gameState, definition->stageMessages[nextStage - 1]); // Display the dialogue for the new stage.
    return true; // Report that progress was successfully advanced.
}

bool handleWorldInteraction(const struct player *player, RoomId currentRoomId, GameState *gameState) { // weilong
    if (player == NULL || gameState == NULL) return false; // Interaction checks need valid player and game state pointers.

    if (currentRoomId == LUNCHROOM && // Handle the lunchroom NPC before checking quest data.
        isNearPosition(player, MRARMY_X, MRARMY_Y)) {
        gameStateSetMessage(gameState, "X"); // Show this NPC's current response.
        return true; // An interaction was handled.
    }

    if (currentRoomId == LECTURE && // Once his quest is complete, Marco only displays this response.
        isNearPosition(player, MARCO_X, MARCO_Y) &&
        questIsComplete(gameState, QUEST_MARCO)) {
        gameStateSetMessage(gameState, "I AM PRESENTING MY LECTURE. PLEASE DONT DISTURB.");
        return true; // Stop after handling the interaction.
    }

    if (currentRoomId == HALLD && // Check whether Ball Knowledge can give the crowbar objective.
        isNearPosition(player, BALLKNOWLEDGE_X, BALLKNOWLEDGE_Y)) {
        if (!questIsComplete(gameState, QUEST_MARCO)) {
            gameStateSetMessage(gameState, "I AM BUSY RIGHT NOW."); // The prerequisite quest is unfinished.
            return true; // The NPC interaction was handled.
        }
        if (gameState->quests[QUEST_BALLKNOWLEDGE] >= S1) {
            gameStateSetMessage(gameState, "I ALREADY TOLD YOU ABOUT THE CROWBAR."); // Do not give the objective again.
            return true; // The NPC interaction was handled.
        }
    }

    // Find a configured item that is available and close enough to collect.
    for (size_t itemIndex = 0;
         itemIndex < sizeof(questItems) / sizeof(questItems[0]);
         itemIndex++) {
        const QuestItemDefinition *item = &questItems[itemIndex]; // Get the item configuration currently being checked.
        if (currentRoomId != item->room ||
            gameState->quests[item->quest] != item->requiredQuestStage ||
            !gameState->worldItems[item->item] ||
            gameState->items[item->item] ||
            !isNearPosition(player, item->x, item->y)) { // Skip items in another room, unavailable, already owned, or too far away.
            continue;
        }

        gameState->items[item->item] = true; // Put the item in the player's inventory.
        gameState->worldItems[item->item] = false; // Remove its pickup from the room.
        gameState->quests[item->quest] = item->nextQuestStage; // Advance the associated quest.
        gameStateSetMessage(gameState, item->pickupMessage); // Tell the player what they collected.
        return true; // Handle only one world interaction per call.
    }

    if (currentRoomId == TAROOM && gameState->items[ITEM_CROWBAR] && // The code is available after obtaining the crowbar.
        !questIsComplete(gameState, QUEST_FINDCODE) &&
        isNearPosition(player, CODE_X, CODE_Y)) {
        gameState->quests[QUEST_FINDCODE] = S1; // Mark the one-stage code quest complete.
        gameStateSetMessage(gameState, "CODE 64. NOTE IT DOWN."); // Reveal the code to the player.
        return true; // Interaction handled.
    }

    if (currentRoomId == COMPUTERROOM && // Talking to the TA after finding the code sends them to the balcony.
        questIsComplete(gameState, QUEST_FINDCODE) &&
        gameState->quests[QUEST_FINDKEYCARD] == NOT_STARTED &&
        isNearPosition(player, COMPUTER_TA_X, COMPUTER_TA_Y)) {
        gameState->quests[QUEST_FINDKEYCARD] = S1; // Record that the TA has been spoken to.
        gameState->taOnBalcony = true; // Make the TA available on the balcony.
        gameStateSetMessage(gameState, "I WILL TAKE A BREAK ON THE BALCONY."); // Give the player the next location.
        return true; // Interaction handled.
    }

    if (currentRoomId == PROFESSOR && // Terminal access requires the code quest and the keycard.
        questIsComplete(gameState, QUEST_FINDCODE) &&
        gameState->items[ITEM_KEYCARD] &&
        isNearPosition(player, PROFESSOR_COMPUTER_X, PROFESSOR_COMPUTER_Y)) {
        gameState->terminalRequested = true; // Ask the main loop to open the terminal.
        return true; // Interaction handled.
    }

    if (currentRoomId == PROFESSOR && // Approaching the professor displays a response without advancing a quest.
        isNearPosition(player, PROFESSOR_NPC_X, PROFESSOR_NPC_Y)) {
        gameStateSetMessage(gameState,
                            "I AM PRESENTING MY LECTURE. PLEASE DONT DISTURB.");
        return true; // Interaction handled.
    }

    if (currentRoomId == BALCONY && gameState->taOnBalcony && // The TA leaves the keycard behind when approached.
        isNearPosition(player, BALCONY_TA_X, BALCONY_TA_Y)) {
        gameState->taOnBalcony = false; // Remove the TA from the balcony.
        gameState->quests[QUEST_FINDKEYCARD] = S2; // Advance the keycard quest to the pickup stage.
        gameState->worldItems[ITEM_KEYCARD] = true; // Place the keycard in the room.
        gameStateSetMessage(
            gameState, "THE TA WAS CALLED AWAY. A KEYCARD IS LEFT BEHIND."); // Explain where the keycard came from.
        return true; // Interaction handled.
    }

    for (int questIndex = 0; questIndex < QUEST_COUNT; questIndex++) { // Try configured NPC quests for this room.
        const QuestDefinition *definition = &questDefinitions[questIndex]; // Read the current quest's NPC configuration.
        if (definition->stageMessages == NULL || currentRoomId != definition->room || !isNearPosition(player, definition->npcX, definition->npcY)) { // Skip quests with no NPC here or outside interaction range.
            continue;
        }
        if (definition->prerequisiteQuest >= 0 &&
            !questIsComplete(gameState, (QuestId) definition->prerequisiteQuest)) { // Do not advance until its prerequisite is complete.
            continue;
        }
        if (!advanceQuest(gameState, (QuestId) questIndex)) return false; // Stop if this quest cannot advance.
        if (questIndex == QUEST_BALLKNOWLEDGE &&
            gameState->quests[QUEST_FINDTOOL] == NOT_STARTED) {
            gameState->quests[QUEST_FINDTOOL] = S1; // Start the crowbar quest after giving its objective.
            gameState->worldItems[ITEM_CROWBAR] = true; // Make the crowbar available in the trash room.
        }
        return true; // One NPC quest interaction was handled.
    }

    return false; // Nothing interactable matched the player's room and position.
}

void drawQuestMarkers(RoomId currentRoomId, const GameState *gameState) { // husien
    if (gameState == NULL) return; // Quest markers require valid game state.

    if (currentRoomId == TAROOM && gameState->items[ITEM_CROWBAR] && // Mark the code location until its quest is complete.
        !questIsComplete(gameState, QUEST_FINDCODE)) {
        fillRect(CODE_X - 2, CODE_Y - 2, 5, 5, white); // Draw a small white marker at the code location.
    }
    if (currentRoomId == BALCONY && gameState->worldItems[ITEM_KEYCARD] && // Mark the keycard while it remains uncollected.
        !gameState->items[ITEM_KEYCARD]) {
        fillRect(KEYCARD_X - 5, KEYCARD_Y - 5, 10, 10, blue); // Draw a blue marker at the keycard location.
    }
}
