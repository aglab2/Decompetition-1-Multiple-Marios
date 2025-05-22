#include "types.h"

#ifndef MARIO_COOP_H
#define MARIO_COOP_H

/*
Check behaviors/coop_objects.inc.c for documentation on custom objects.
You can also check behaviors/seesaw_platform.inc.c to play with new seesaw dynamics based on real world physics.
*/

enum {
    COOP_CM_TAKE_TURNS, // Controllable by player when set as main Mario
    COOP_CM_ALL_ACTIVE, // Controllable by player at all times
    COOP_CM_NPC, // Acts on its own, has a programmable brain
    COOP_CM_CUSTOM // For custom programmed behavior, does nothing by default
};

#define COOP_DEBUG_MARIO_CONTROL_MODE COOP_CM_TAKE_TURNS // Default mode for debug Marios spawned with D-Down
#define COOP_MARIO_HITBOX_SIZE 80.0f // For Mario vs. Mario collision, is a sphere at the origin point

#define COOP_MARIO_STATES_MAX 35 // Maximum amount of Marios allowed to be spawned at once
/*
If the emulator crashes from spawning too many Marios, try increasing GFX_POOL_SIZE in config_graphics.h
If the game freezes with music still playing from spawning too many Marios, try increasing OBJECT_POOL_CAPACITY in object_list_processor.h
*/

// Comment out to disable
#define COOP_MAIN_MARIO_MUST_LIVE // If defined, game over when your initial Mario dies
#define COOP_DEBUG_SPAWN_MARIO_WITH_DDOWN // If defined, spawn Mario when pressing D-Pad down
#define COOP_SNAPPY_SWAP_CAMERA // If defined, camera will swap instantly instead of gradually

#define IS_CONTROLLABLE(controlMode) (controlMode == COOP_CM_TAKE_TURNS || controlMode == COOP_CM_ALL_ACTIVE)

extern int gCoopActiveMarios;
extern int gCoopActiveControllableMarios;
extern int gCoopActiveMarioIndex;

struct MarioState * coop_spawn_mario_with_id(Vec3f pos, int marioID, int control_mode);
struct MarioState * coop_spawn_mario(Vec3f pos, int control_mode);
void coop_give_control_to_next(void);
int coop_delete_mario(struct MarioState * m);

void coop_npc_behavior(struct MarioState * m);

void coop_reset_state(void);
void coop_mario_collision(struct MarioState * m);

#endif