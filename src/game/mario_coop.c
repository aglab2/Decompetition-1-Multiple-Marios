#include "types.h"
#include "mario_coop.h"
#include "level_update.h"
#include "object_helpers.h"
#include "model_ids.h"
#include "mario.h"
#include "object_list_processor.h"
#include "behavior_data.h"
#include "engine/math_util.h"
#include "game_init.h"
#include "camera.h"
/*

mario_coop.c

This file is responsible for handling multiple active Marios at once.
Check mario_coop.h for configuration.

*/

int gCoopActiveMarios = 1;
int gCoopActiveControllableMarios = 1;
int gCoopActiveMarioIndex = 0;

/* 
Spawns a Mario at the specified position with a specific player ID.
Returns the MarioState of the spawned Mario, NULL if the ID is already used.
*/
struct MarioState * coop_spawn_mario_with_id(Vec3f pos, int marioID, int control_mode) {
    if (gMarioStates[marioID].marioObj != NULL) {return NULL;} // Slot already used

    gCoopActiveMarios++;
    if (IS_CONTROLLABLE(control_mode)) {
        gCoopActiveControllableMarios --;
    }

    init_mario_from_save_file(&gMarioStates[marioID],marioID);
    gMarioStates[marioID].marioObj = spawn_object(gMarioState->marioObj, MODEL_MARIO, bhvMario);
    gMarioStates[marioID].marioObj->oPlayerID = marioID;
    gMarioStates[marioID].marioObj->oFlags |= OBJ_FLAG_IS_A_MARIO;
    init_mario(&gMarioStates[marioID]);
    gMarioStates[marioID].controlMode = control_mode;
    vec3f_copy(gMarioStates[marioID].pos, pos);
    vec3f_copy(gMarioStates[marioID].kartHome, pos);
    return &gMarioStates[marioID];
}

/*
Spawns a Mario at the specified position using any empty ID.
Returns the MarioState of the spawned Mario, NULL if Mario count is maxed out.
*/
struct MarioState * coop_spawn_mario(Vec3f pos, int control_mode) {
    for (int i = 0; i < COOP_MARIO_STATES_MAX; i ++) {
        // Search for a uninitialized mario
        if (gMarioStates[i].marioObj == NULL) {
            return coop_spawn_mario_with_id(pos,i,control_mode);
        }
    }

    return NULL; // Returns NULL if Marios are maxed out
}

/*
Sets the next Mario with control mode COOP_CM_TAKE_TURNS in line as the primary controller.
*/
void coop_give_control_to_next(void) {
    do {
        gCoopActiveMarioIndex++;
        if (gCoopActiveMarioIndex >= COOP_MARIO_STATES_MAX) {
            gCoopActiveMarioIndex = 0;
        }
    } while(gMarioStates[gCoopActiveMarioIndex].marioObj == NULL || !IS_CONTROLLABLE(gMarioStates[gCoopActiveMarioIndex].controlMode));

    gMarioState=&gMarioStates[gCoopActiveMarioIndex];
    gMarioObject=gMarioStates[gCoopActiveMarioIndex].marioObj;

    #ifdef COOP_SNAPPY_SWAP_CAMERA
        reset_camera(gCurrentArea->camera);
    #endif
}


/*
Deletes the Mario
*/
extern s16 kart_angle(int kartId);
int coop_delete_mario(struct MarioState * m) {
    m->pos[0] = m->kartSafePos[0];
    m->pos[1] = m->kartSafePos[1] + 300.f;
    m->pos[2] = m->kartSafePos[2];
    m->vel[0] = 0.f;
    m->vel[1] = 0.f;
    m->vel[2] = 0.f;
    m->forwardVel = 0.f;
    m->faceAngle[0] = 0;
    m->faceAngle[1] = m->kartSafeAngle + 0x8000 + kart_angle(m->kartId) / 2;
    m->faceAngle[2] = 0;
    if (m == gMarioState)
        reset_camera(gCurrentArea->camera);

    return TRUE;
}

extern void coop_npc_behavior(struct MarioState * m);

// Don't call this function yourself, used for level transitions
void coop_reset_state(void) {
    gCoopActiveMarios = 1;
    gCoopActiveControllableMarios = 1;
    gCoopActiveMarioIndex = 0;
    gMarioState = &gMarioStates[0];
    for (int i = 0; i < COOP_MARIO_STATES_MAX; i ++) {
        gMarioStates[i].marioObj = NULL;
        gMarioStates[i].isDead = FALSE;
    }
}

// Don't call this function yourself, used for Mario touching other Marios O////O 
void coop_mario_collision(struct MarioState * m) {
    for (int i = 0; i < COOP_MARIO_STATES_MAX; i ++) {
        if (&gMarioStates[i] == m || &gMarioStates[i].marioObj == NULL) {continue;}

        Vec3f diff;
        vec3_diff(diff, gMarioStates[i].pos, m->pos);
        f32 distSquared = sqr(diff[0]) + sqr(diff[1]) + sqr(diff[2]);

        if (distSquared < sqr(COOP_MARIO_HITBOX_SIZE)) {
            f32 pressure = sqrtf(sqr(COOP_MARIO_HITBOX_SIZE)-distSquared)/4.f;

            vec3f_normalize(diff);
            vec3_scale_dest(diff,diff,-pressure);

            vec3f_sum(m->pos,m->pos,diff);
            vec3_scale_dest(diff,diff,-1.0f);
            vec3f_sum(gMarioStates[i].pos,gMarioStates[i].pos,diff);
        }
    }
}

void coop_mario_pin()
{
    for (int i = 0; i < COOP_MARIO_STATES_MAX; i ++) {
        if (&gMarioStates[i] == gMarioState || &gMarioStates[i].marioObj == NULL) {continue;}
        vec3_copy(gMarioStates[i].pos, gMarioStates[i].kartHome);
    }
}
