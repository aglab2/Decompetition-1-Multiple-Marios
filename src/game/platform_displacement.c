#include <PR/ultratypes.h>

#include "engine/math_util.h"
#include "engine/surface_collision.h"
#include "level_update.h"
#include "object_fields.h"
#include "object_helpers.h"
#include "object_list_processor.h"
#include "platform_displacement.h"
#include "types.h"
#include "sm64.h"
#include "behavior_data.h"
#include "game_init.h"
#include "mario_coop.h"

#include "config.h"

struct Object *gMarioPlatform[COOP_MARIO_STATES_MAX];
static struct PlatformDisplacementInfo sMarioDisplacementInfo[COOP_MARIO_STATES_MAX];
static Vec3f sMarioAmountDisplaced[COOP_MARIO_STATES_MAX];

/**
 * Determine if Mario is standing on a platform object, meaning that he is
 * within 4 units of the floor. Set his referenced platform object accordingly.
 */
extern s32 check_within_floor_triangle_bounds(s32 x, s32 z, struct Surface *surf);
void update_mario_platform(void) {
    struct Surface *floor;
    f32 marioX, marioY, marioZ;
    f32 floorHeight;
    u32 awayFromFloor;

    //! If Mario moves onto a rotating platform in a PU, the find_floor call
    //  will detect the platform and he will end up receiving a large amount
    //  of displacement since he is considered to be far from the platform's
    //  axis of rotation.

    for (int i = 0; i < COOP_MARIO_STATES_MAX; i++) {

        if (gMarioStates[i].marioObj == NULL) {
            continue;
        }

        struct Object * marioobj = gMarioStates[i].marioObj;
        marioX = marioobj->oPosX;
        marioY = marioobj->oPosY;
        marioZ = marioobj->oPosZ;
        struct Surface *cache = gMarioStates[i].floor;
        if (cache && check_within_floor_triangle_bounds(marioX, marioZ, cache)) {
            floor = cache;
            floorHeight = get_surface_height_at_location(marioX, marioZ, cache);
        } else {
            floorHeight = find_floor(marioX, marioY, marioZ, &floor);
            gMarioStates[i].floor = floor;
        }

        awayFromFloor =  absf(marioY - floorHeight) >= 4.0f;

        if (awayFromFloor) {
            gMarioPlatform[i] = NULL;
            marioobj->platform = NULL;
        } else {
            if (floor != NULL && floor->object != NULL) {
                gMarioPlatform[i] = floor->object;
                marioobj->platform = floor->object;
                // If this is the first frame of Mario standing on the platform,
                // then initialise his platform displacement info struct
                if ((floor->object != sMarioDisplacementInfo[i].prevPlatform) || (gGlobalTimer != sMarioDisplacementInfo[i].prevTimer)) {
                    update_platform_displacement_info(&sMarioDisplacementInfo[i], gMarioStates[i].pos, gMarioStates[i].faceAngle[1], floor->object);
                }
            } else {
                gMarioPlatform[i] = NULL;
                marioobj->platform = NULL;
            }
        }
    }
}

/**
 * Set the values in the platform displacement struct for use next frame
 */
void update_platform_displacement_info(struct PlatformDisplacementInfo *displaceInfo, Vec3f pos, s16 yaw, struct Object *platform) {
    Vec3f scaledPos, yawVec, localPos;

    // Avoid a crash if the platform unloaded its collision while stood on or is static
    if (platform->header.gfx.throwMatrix == NULL) return;

    // Update position
    vec3_diff(localPos, pos, (*platform->header.gfx.throwMatrix)[3]);
    linear_mtxf_transpose_mul_vec3(*platform->header.gfx.throwMatrix, scaledPos, localPos);
    vec3_quot(displaceInfo->prevTransformedPos, scaledPos, platform->header.gfx.scale);
    vec3_copy(displaceInfo->prevPos, pos);

    // Update yaw
    vec3_set(yawVec, sins(yaw), 0, coss(yaw));
    linear_mtxf_transpose_mul_vec3(*platform->header.gfx.throwMatrix, displaceInfo->prevTransformedYawVec, yawVec);
    displaceInfo->prevYaw = yaw;

    // Update platform and timer
    displaceInfo->prevPlatform = platform;
    displaceInfo->prevTimer = gGlobalTimer;
}

/**
 * Apply one frame of platform displacement to Mario or an object using the given
 * platform.
 */
void apply_platform_displacement(struct PlatformDisplacementInfo *displaceInfo, Vec3f pos, s16 *yaw, struct Object *platform, struct MarioState * m) {
    Vec3f posDifference;
    Vec3f yawVec;
    Vec3f scaledPos;
    // Determine how much Mario turned on his own since last frame
    s16 yawDifference = *yaw - displaceInfo->prevYaw;

    // Avoid a crash if the platform unloaded its collision while stood on or is static
    if (platform->header.gfx.throwMatrix == NULL) return;

    // Determine how far Mario moved on his own since last frame
    vec3_diff(posDifference, pos, displaceInfo->prevPos);

    if ((platform == displaceInfo->prevPlatform) && (gGlobalTimer == displaceInfo->prevTimer + 1)) {
        // For certain objects, only use velocity for displacement rather than the transform
        // E.g. TTC treadmills
        if (platform->oFlags & OBJ_FLAG_NO_AUTO_DISPLACEMENT) {
            pos[0] += platform->oVelX;
            pos[1] += platform->oVelY;
            pos[2] += platform->oVelZ;
        } else {
            // Transform from relative positions to world positions
            vec3_prod(scaledPos, displaceInfo->prevTransformedPos, platform->header.gfx.scale);
            linear_mtxf_mul_vec3(*platform->header.gfx.throwMatrix, pos, scaledPos);
            vec3_add(pos, (*platform->header.gfx.throwMatrix)[3]);

            // Add on how much Mario moved in the previous frame
            vec3_add(pos, posDifference);

            // Calculate new yaw
            linear_mtxf_mul_vec3(*platform->header.gfx.throwMatrix, yawVec, displaceInfo->prevTransformedYawVec);
            *yaw = atan2s(yawVec[2], yawVec[0]) + yawDifference;
        }
    }

    Vec3f oldPos;
    vec3_sum(oldPos, displaceInfo->prevPos, posDifference);
    update_platform_displacement_info(displaceInfo, pos, *yaw, platform);

    // If the object is Mario, set inertia
    if (m != NULL) {
        vec3_diff(sMarioAmountDisplaced[m->playerID], pos, oldPos);
    }
}

// Doesn't change in the code, set this to FALSE if you don't want inertia
u8 gDoInertia = TRUE;

static u8 sShouldApplyInertia[COOP_MARIO_STATES_MAX];
static u8 sInertiaFirstFrame[COOP_MARIO_STATES_MAX];

/**
 * Apply inertia based on Mario's last platform.
 */
static void apply_mario_inertia(struct MarioState * m) {
    int marioIndex = m->playerID;
#ifdef MARIO_INERTIA_UPWARD
    // On the first frame of leaving the ground, boost Mario's y velocity
    if (sInertiaFirstFrame[marioIndex]) {
        if (sMarioAmountDisplaced[marioIndex][1] > 0.0f) {
            m->vel[1] += sMarioAmountDisplaced[marioIndex][1];
        }
    }
#endif

#ifdef MARIO_INERTIA_LATERAL
    // Apply sideways inertia
    m->pos[0] += sMarioAmountDisplaced[marioIndex][0];
    m->pos[2] += sMarioAmountDisplaced[marioIndex][2];

    // Drag
    sMarioAmountDisplaced[marioIndex][0] *= 0.97f;
    sMarioAmountDisplaced[marioIndex][2] *= 0.97f;
#endif

    // Stop applying inertia once Mario has landed, or when ground pounding
    if (!(m->action & ACT_FLAG_AIR) || (m->action == ACT_GROUND_POUND)) {
        sShouldApplyInertia[marioIndex] = FALSE;
    }
}

/**
 * Apply platform displacement or inertia if required.
 */
void apply_mario_platform_displacement(void) {
    struct Object *platform;

    for (int i = 0; i < COOP_MARIO_STATES_MAX; i++) {
        platform = gMarioPlatform[i];
        if (!(gTimeStopState & TIME_STOP_ACTIVE) && gMarioObject != NULL) {
            if (platform != NULL) {
                apply_platform_displacement(&sMarioDisplacementInfo[i], gMarioStates[i].pos, &gMarioStates[i].faceAngle[1], platform, &gMarioStates[i]);
                sShouldApplyInertia[i] = TRUE;
                sInertiaFirstFrame[i] = TRUE;
            } else if (sShouldApplyInertia[i] && gDoInertia) {
                apply_mario_inertia(&gMarioStates[i]);
                sInertiaFirstFrame[i] = FALSE;
            }
        }
    }
}

/**
 * Set Mario's platform to NULL.
 */
void clear_mario_platform(void) {
    for (int i = 0; i < COOP_MARIO_STATES_MAX; i++) {
        gMarioPlatform[i] = NULL;
    }
}
