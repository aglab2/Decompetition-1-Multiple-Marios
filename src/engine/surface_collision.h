#ifndef SURFACE_COLLISION_H
#define SURFACE_COLLISION_H

#include <PR/ultratypes.h>

#include "types.h"

#include "config/config_world.h"

// The y coord is moved upward by this amount when finding floors.
// Vanilla value is 78.
#define FIND_FLOOR_BUFFER 78

#define CELL_HEIGHT_LIMIT            20000
#define FLOOR_LOWER_LIMIT           -11000
#define FLOOR_LOWER_LIMIT_MISC      (FLOOR_LOWER_LIMIT + 1000)

#define is_outside_level_bounds(xPos, zPos) \
    (((xPos) <= -LEVEL_BOUNDARY_MAX) ||     \
     ((xPos) >=  LEVEL_BOUNDARY_MAX) ||     \
     ((zPos) <= -LEVEL_BOUNDARY_MAX) ||     \
     ((zPos) >=  LEVEL_BOUNDARY_MAX))

#define get_surface_height_at_location(xPos, zPos, surf) (-(((xPos) * (surf)->normal.x) + ((zPos) * (surf)->normal.z) + (surf)->originOffset) / (surf)->normal.y)

#define SURFACE_YAW(s) (atan2s(((s)->normal.z), ((s)->normal.x)))

enum RaycastFlags {
    RAYCAST_FIND_FLOOR = (1 << 0),
    RAYCAST_FIND_WALL  = (1 << 1),
    RAYCAST_FIND_CEIL  = (1 << 2),
    RAYCAST_FIND_WATER = (1 << 3),
    RAYCAST_FIND_ALL   = (0xFFFFFFFF)
};

struct WallCollisionData {
    /*0x00*/ f32 x, y, z;
    /*0x0C*/ f32 offsetY;
    /*0x10*/ f32 radius;
    /*0x14*/ u8 filler[2];
    /*0x16*/ s16 numWalls;
    /*0x18*/ struct Surface *walls[MAX_REFERENCED_WALLS];
};

s32 f32_find_wall_collision(f32 *xPtr, f32 *yPtr, f32 *zPtr, f32 offsetY, f32 radius);
s32 find_wall_collisions(struct WallCollisionData *colData);
void resolve_and_return_wall_collisions(Vec3f pos, f32 offset, f32 radius, struct WallCollisionData *collisionData);
f32 find_ceil(f32 posX, f32 posY, f32 posZ, struct Surface **pceil);

extern s32 check_within_ceil_triangle_bounds(s32 x, s32 z, struct Surface *surf);
// Finds the ceiling from a vec3f and a minimum height (with 3 unit vertical buffer).
ALWAYS_INLINE f32 find_mario_ceil(struct MarioState* m, Vec3f pos, f32 height, struct Surface **ceil) {
    f32 x = pos[0];
    f32 y = MAX(height, pos[1]) + 3.0f;
    f32 z = pos[2];
    struct Surface *cache = m->ceil;

    if (cache && check_within_ceil_triangle_bounds(x, z, cache)) {
        *ceil = cache;
        return get_surface_height_at_location(x, z, cache);
    }

    return find_ceil(x, y, z, ceil);
}

f32 find_floor_height(f32 x, f32 y, f32 z);
f32 find_floor(f32 xPos, f32 yPos, f32 zPos, struct Surface **pfloor);
f32 find_room_floor(f32 x, f32 y, f32 z, struct Surface **pfloor);
s32 get_room_at_pos(f32 x, f32 y, f32 z);
static inline s32 find_water_level_and_floor(s32, s32, s32, struct Surface **) {
    return FLOOR_LOWER_LIMIT;
}
static inline s32 find_water_level(s32, s32) { // TODO: Allow y pos
    return FLOOR_LOWER_LIMIT;
}
static inline s32 find_poison_gas_level(s32, s32) {
    return FLOOR_LOWER_LIMIT;
}

#ifdef VANILLA_DEBUG
void debug_surface_list_info(f32 xPos, f32 zPos);
#endif

#endif // SURFACE_COLLISION_H
