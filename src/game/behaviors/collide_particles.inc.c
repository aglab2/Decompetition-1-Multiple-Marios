// collide_particles.inc.c

static s16 sTinyTriMovementParams[] = { 0xD000, 0,      0x3000, 0,      0xDE67, 0x2199,
                                        0x2199, 0x2199, 0xDE67, 0xDE67, 0x2199, 0xDE67 };

static s16 sTinyStarMovementParams[] = { 0xE000, 0,      0,      0,      0x2000, 0,      0xE99A,
                                         0x1666, 0x1666, 0x1666, 0xE99A, 0xE99A, 0x1666, 0xE99A };

void bhv_punch_tiny_triangle_loop(void) {
    if (o->oTimer == 0) {
        s16 yaw = o->oMoveAngleYaw;
        o->oCollisionParticleScale = 1.28f;
        cur_obj_set_pos_relative(o->parentObj, 0.0f, 60.0f, 100.0f);
        o->oMoveAngleYaw = yaw;
    }

    cur_obj_move_using_fvel_and_gravity();
    o->oAnimState = 5;
    cur_obj_scale(o->oCollisionParticleScale);
    o->oCollisionParticleScale -= 0.2f;

    if (gDebugInfo[DEBUG_PAGE_EFFECTINFO][0] + 6 < o->oTimer) {
        obj_mark_for_deletion(o);
    }
}

void bhv_punch_tiny_triangle_init(void) {
    s32 i;
    struct Object *triangle;

    for (i = 0; i < 6; i++) {
        triangle = spawn_object(o, MODEL_DIRT_ANIMATION, bhvPunchTinyTriangle);
        triangle->oMoveAngleYaw = o->parentObj->oMoveAngleYaw + sTinyTriMovementParams[2 * i] + 0x8000;
        triangle->oVelY = sins(sTinyTriMovementParams[2 * i + 1]) * 25.0f;
        triangle->oForwardVel = coss(sTinyTriMovementParams[2 * i + 1]) * 25.0f;
    }
}

void bhv_wall_tiny_star_particle_loop(void) {
    if (o->oTimer == 0) {
        s16 yaw = o->oMoveAngleYaw;
        o->oCollisionParticleScale = 0.28f;
        cur_obj_set_pos_relative(o->parentObj, 0.0f, 30.0f, 110.0f);
        o->oMoveAngleYaw = yaw;
    }

    cur_obj_move_using_fvel_and_gravity();
    o->oAnimState = 4;
    cur_obj_scale(o->oCollisionParticleScale);
    o->oCollisionParticleScale -= 0.015f;
}

void bhv_tiny_star_particles_init(void) {
    s32 i;
    struct Object *particle;

    for (i = 0; i < 7; i++) {
        particle = spawn_object(o, MODEL_CARTOON_STAR, bhvWallTinyStarParticle);
        particle->oMoveAngleYaw = o->parentObj->oMoveAngleYaw + sTinyStarMovementParams[2 * i] + 0x8000;
        particle->oVelY = sins(sTinyStarMovementParams[2 * i + 1]) * 25.0f;
        particle->oForwardVel = coss(sTinyStarMovementParams[2 * i + 1]) * 25.0f;
    }
}

void bhv_pound_tiny_star_particle_loop(void) {
    if (o->oTimer == 0) {
        o->oCollisionParticleScale = 0.28f;
        o->oForwardVel = 25.0f;
        o->oPosY -= 20.0f;
        o->oVelY = 14.0f;
    }

    cur_obj_move_using_fvel_and_gravity();
    o->oAnimState = 4;
    cur_obj_scale(o->oCollisionParticleScale);
    o->oCollisionParticleScale -= 0.015f;
}

void bhv_pound_tiny_star_particle_init(void) {
    s32 i;
    s32 amt = 8;

    struct Object *particle;
    for (i = 0; i < amt; i++) {
        particle = spawn_object(o, MODEL_CARTOON_STAR, bhvPoundTinyStarParticle);
        particle->oMoveAngleYaw = (i * 65536) / amt;
    }
}
