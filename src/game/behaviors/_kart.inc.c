#include "game/mario_coop.h"

extern const u8* bpe_pairs[];
extern const u8* bpe_relations[];

extern const u8 uRoute101Track[];
extern const u8 uRoute280Track[];
extern const u8 uExpertTrack[];
extern const u8 uStandardTrack[];
extern const u8 sBeginnerTrack[];

extern const int sRouteSizes[];

#define RACERS_COUNT 35
// #define DEBUG_DISABLE_FUZZ
// #define DEBUG_LOC_TEST_PARTICLES

void print_defer(s16 x, s16, const char* line, u8 ttl, u8 centered, u8 colored);

#include "_track.inc.c"

static const u8 sBPETrack[] = {
    1, 3,
};

#define oPartIndex oF4
#define oPartNext oObjF8
#define oPartPrev oObjFC

struct SpawnerState
{
    Vec3f pos;
    s16 angle;
};
static struct SpawnerState sSpawnerState;

#define SCALE 0.5f

#define WALK_LIMIT_SAFEGAP 10

extern s16 sSourceWarpNodeId;
s8 gAmountOfParts;
static u16 sWalkLimit = 0;
static u8 sEnableProgress = 1;
static s16 sSilentPeriodId = 0;
int sCompletedTracks = 0;

#define oCtlFinalTime oF4

extern void seq_player_play_sequence(u8 player, u8 seqId, u16 arg2);

struct SpawnResult
{
    struct Object* firstPart;
    struct Object* lastPart;
};

static u8 uRNGScratch[270];

static int can_spawn_enemy_on_part(int partId)
{
    return partId == 34 || partId == 35 || partId == 54 || partId == 53 || partId == 9 || partId == 80 || partId == 7;
}

static int part_refreshing_silent_period(int partId)
{
    return partId != 9 && partId != 80; 
}

static void spawn_enemy(int trackId, int partId, f32 x, f32 y, f32 z, s16 angle)
{
    if (trackId < sSilentPeriodId)
        return;

    if (!can_spawn_enemy_on_part(partId))
        return;

    f32 raiseY = 0.f;
    struct Object* enemy;
    switch (random_u16() % 4)
    {
        case 0:
            enemy = spawn_object(o, MODEL_BLACK_BOBOMB, bhvBobomb);
            break;            
        case 1:
            enemy = spawn_object(o, MODEL_HEAVE_HO, bhvHeaveHo);
            break;
        case 2:
            enemy = spawn_object(o, MODEL_AMP, bhvCirclingAmp);
            raiseY = 70.f;
            break;
        case 3:
            enemy = spawn_object(o, MODEL_RED_FLAME, bhvFlame);
            raiseY = 70.f;
            break;
    }

    f32 xs = random_f32_around_zero(800.f);
    f32 zs = random_f32_around_zero(400.f) - 500.f;

    f32 xRot = xs * coss(angle) 
             + zs * sins(angle);
    f32 zRot = -xs * sins(angle)
              + zs * coss(angle);

    enemy->oPosX = x + xRot;
    enemy->oPosY = y + raiseY;
    enemy->oPosZ = z + zRot;
    enemy->oFaceAngleYaw = angle;

    if (part_refreshing_silent_period(partId))
    {
        sSilentPeriodId = trackId + 5;
    }
}

static struct SpawnResult spawn_track(int idx_shift, const u8* track, int trackSize)
{
    // spawn the track
    struct Object* firstPart = NULL;
    struct Object* prevPart = NULL;
    for (int i = 0; i < trackSize; i++)
    {
        int entry = track[i];

        {
            Collision* partCollision = sCollisionHeaders[entry];
            struct Object* part = spawn_object(o, partCollision ? (0x20 + entry) : 0, bhvPart);

            if (partCollision)
                obj_set_collision_data(part, partCollision);

            part->oFaceAngleYaw = sSpawnerState.angle;
            part->oPosX = sSpawnerState.pos[0];
            part->oPosY = sSpawnerState.pos[1];
            part->oPosZ = sSpawnerState.pos[2];
            part->oPartIndex = idx_shift + i;
            part->oBehParams2ndByte = entry;
            
            obj_scale(part, SCALE);
    
            spawn_enemy(idx_shift + i, entry, sSpawnerState.pos[0], sSpawnerState.pos[1], sSpawnerState.pos[2], sSpawnerState.angle);

            if (i == 0)
                firstPart = part;
            
            if (prevPart)
            {
                prevPart->oPartNext = part;
                part->oPartPrev = prevPart;
            }
            prevPart = part;
        }

        const struct PartConfig* partConfig = &sPartConfigs[entry];

        // We need to rotate the shift around spawner.angle around (0, 0)
        f32 shiftXRotated = partConfig->shift[0] * coss(sSpawnerState.angle) 
                          + partConfig->shift[2] * sins(sSpawnerState.angle);
        f32 shiftZRotated = -partConfig->shift[0] * sins(sSpawnerState.angle)
                          + partConfig->shift[2] * coss(sSpawnerState.angle);

        sSpawnerState.pos[0] += shiftXRotated * SCALE;
        sSpawnerState.pos[1] += partConfig->shift[1] * SCALE;
        sSpawnerState.pos[2] += shiftZRotated * SCALE;
        sSpawnerState.angle += partConfig->turn;
    }

    return (struct SpawnResult) {
        .firstPart = firstPart,
        .lastPart = prevPart,
    };
}

struct WalkResult
{
    Vec3f center;
    int walked;
};

static struct WalkResult walk_track(const u8* track, int trackSize)
{
    f32 minX = 0.f, maxX = 0.f;
    f32 minY = 0.f, maxY = 0.f;
    f32 minZ = 0.f, maxZ = 0.f;

    // Account for walk before entering the track
    if (sSpawnerState.pos[0] < minX) minX = sSpawnerState.pos[0];
    if (sSpawnerState.pos[0] > maxX) maxX = sSpawnerState.pos[0];
    if (sSpawnerState.pos[2] < minZ) minZ = sSpawnerState.pos[2];
    if (sSpawnerState.pos[2] > maxZ) maxZ = sSpawnerState.pos[2];

    for (int i = 0; i < trackSize; i++)
    {
        int entry = track[i];
        const struct PartConfig* partConfig = &sPartConfigs[entry];

        // We need to rotate the shift around spawner.angle around (0, 0)
        f32 shiftXRotated = partConfig->shift[0] * coss(sSpawnerState.angle) 
                          + partConfig->shift[2] * sins(sSpawnerState.angle);
        f32 shiftZRotated = -partConfig->shift[0] * sins(sSpawnerState.angle)
                          + partConfig->shift[2] * coss(sSpawnerState.angle);

        sSpawnerState.pos[0] += shiftXRotated * SCALE;
        sSpawnerState.pos[1] += partConfig->shift[1] * SCALE;
        sSpawnerState.pos[2] += shiftZRotated * SCALE;
        sSpawnerState.angle += partConfig->turn;

        f32 prevMinX = minX, prevMaxX = maxX;
        f32 prevMinY = minY, prevMaxY = maxY;
        f32 prevMinZ = minZ, prevMaxZ = maxZ;

        if (sSpawnerState.pos[0] < minX) minX = sSpawnerState.pos[0];
        if (sSpawnerState.pos[0] > maxX) maxX = sSpawnerState.pos[0];
        if (sSpawnerState.pos[1] < minY) minY = sSpawnerState.pos[1];
        if (sSpawnerState.pos[1] > maxY) maxY = sSpawnerState.pos[1];
        if (sSpawnerState.pos[2] < minZ) minZ = sSpawnerState.pos[2];
        if (sSpawnerState.pos[2] > maxZ) maxZ = sSpawnerState.pos[2];

        if ((maxX - minX > 120000.f) || (maxZ - minZ > 120000.f))
        {
            return (struct WalkResult){ { -(prevMaxX + prevMinX) / 2.f
                                        , -(prevMaxY + prevMinY) / 2.f
                                        , -(prevMaxZ + prevMinZ) / 2.f }, i };
        }
    }

    return (struct WalkResult){ { -(maxX + minX) / 2.f
                                , -(maxY + minY) / 2.f
                                , -(maxZ + minZ) / 2.f }, trackSize };
}

static int sAmountGenerated = 0;
static struct Object* sLastPart = NULL;
static u8 sWantBPE = 0;

static void bpe_gen(void);
static void bpe_feed(void);

void bhv_ctl_init()
{
    o->oCtlFinalTime = 0;
    sEnableProgress = 1;
    gMarioStates->faceAngle[1] = 0x8000;
    sSilentPeriodId = 10;

    const u8* track = NULL;
    int trackSize = 0;
    int* ptrackSize = segmented_to_virtual(sRouteSizes);
    switch (sSourceWarpNodeId)
    {
        case 0x20: 
            track = segmented_to_virtual(sBeginnerTrack);
            trackSize = ptrackSize[0];
            break;
        case 0x21: 
            track = segmented_to_virtual(uStandardTrack);
            trackSize = ptrackSize[1];
            break;
        case 0x22: 
            track = segmented_to_virtual(uExpertTrack);
            trackSize = ptrackSize[2];
            break;
    }

    gAmountOfParts = trackSize;

retry:
    sSpawnerState.pos[0] = 0;
    sSpawnerState.pos[1] = 0;
    sSpawnerState.pos[2] = 0;
    sSpawnerState.angle = 0;

    if (track)
    {
        struct SpawnResult result = spawn_track(0, track, trackSize);
        struct Object* firstPart = result.firstPart;
        struct Object* lastPart = result.lastPart;
        firstPart->oPartPrev = lastPart;
        lastPart->oPartNext = firstPart;
        sWantBPE = 0;
    }
    else
    {
        bpe_gen();

        // This will help the walk so initial platform in sBPETrack is not too far away
        sSpawnerState.pos[2] = -4000.f;
        // reduce walk to 250 because there might be some garbage in the end
        struct WalkResult walk = walk_track(uRNGScratch, 250);

        sSpawnerState.pos[0] = gMarioStates->pos[0] = o->oPosX = walk.center[0];
        sSpawnerState.pos[1] = gMarioStates->pos[1] = o->oPosY = walk.center[1];
        sSpawnerState.pos[2] = gMarioStates->pos[2] = o->oPosZ = walk.center[2];
        sSpawnerState.angle = 0;
        // this is extremely unlikely to happen
        if (walk.walked < 150)
            goto retry;

        sWalkLimit = walk.walked - WALK_LIMIT_SAFEGAP;

        sLastPart = spawn_track(0, sBPETrack, sizeof(sBPETrack)).lastPart;

        gMarioStates->kartProgress = 0.0f;

        sAmountGenerated = 0;
        sWantBPE = 1;
    }

    for (int i = 0; i < RACERS_COUNT - 1; i++)
    {
        // I want an arragemnt that looks like this:        
        /*
        <3     X   X
        <9  X X X X X X
        <14  X X X X X
        <20 X X X X X X
        <25  X X X X X
        <30 X X X X X X
             X X M X X
        */
        // The X's are the spawners and the M is the player
        f32 x;
        f32 y;
        if (i < 2)
        {
            x = 6;
            y = i*4 - 2;
        }
        else if (i < 8)
        {
            int iRelative = i - 2;
            x = 5;
            y = iRelative*2 - 1 - 2*2;
        }
        else if (i < 13)
        {
            int iRelative = i - 8;
            x = 4;
            y = iRelative*2 - 2*2;
        }
        else if (i < 19)
        {
            int iRelative = i - 13;
            x = 3;
            y = iRelative*2 - 1 - 2*2;
        }
        else if (i < 24)
        {
            int iRelative = i - 19;
            x = 2;
            y = iRelative*2 - 2*2;
        }
        else if (i < 30)
        {
            int iRelative = i - 24;
            x = 1;
            y = iRelative*2 - 1 - 2*2;
        }
        else
        {
            int iRelative = i - 30;
            x = 0;
            y = iRelative*2 - 2*2;
            if (y >= 0) y += 2;
        }

        Vec3f pos;
        pos[0] = o->oPosX + y * 80;
        pos[1] = o->oPosY;
        pos[2] = o->oPosZ - x * 250 - 50;
        struct MarioState* racer = coop_spawn_mario(pos, COOP_CM_NPC);
        racer->faceAngle[1] = 0x8000;
    }

    gMarioStates->pos[1] = -50.f;
}

extern void coop_mario_pin();

char timerLine[20];
char placeLine[30];
u8 sPlacement;

static void shuffle_u8(u8 *array, s32 n)
{
    if (n > 1) {
        s32 i;
        for (i = n - 1; i > 0; i--) {
            s32 j = (unsigned int) (random_float() * (i+1));
            int t = array[j];
            array[j] = array[i];
            array[i] = t;
        }
    }
}

static s8 sLastPickedTrack = -1;
static u8 sMusicTracks[] = { 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d };

extern void set_camera_mode_fixed2(struct Camera* c);
void bhv_ctl_loop()
{
    if (sWantBPE)
        bpe_feed();

    if (o->oAction)
    {
        print_defer(20, 220, timerLine, 255, 0, 0);
        print_defer(20, 200, placeLine, 255, 0, 1 == sPlacement);

        if (30 == o->oTimer)
        {
            gMarioStates->usedObj = o;
            int warpId = sPlacement == 1 ? 0xa : 0xf1;
            if (1 == sPlacement)
                sCompletedTracks |= 1 << (sSourceWarpNodeId - 0x20);

            gMarioStates->usedObj->oBehParams = warpId << 16;
            gMarioStates->usedObj->oBehParams2ndByte = warpId;
            level_trigger_warp(gMarioStates, WARP_OP_TELEPORT);
        }
    }
    else
    {
        if (o->oTimer == 30)
        {
            play_sound(SOUND_PEACH_POWER_OF_THE_STARS, gMarioStates->marioObj->header.gfx.cameraToObject);
        }

        if (130 == o->oTimer)
        {            
            switch (sSourceWarpNodeId)
            {
                case 0x20: 
                    seq_player_play_sequence(0, 0x23, 0);
                    break;
                case 0x21: 
                    seq_player_play_sequence(0, 0x24, 0);
                    break;
                case 0x22:
                    break;
            }
        }
        if (10 == o->oTimer)
        {
            if (0x30 == sSourceWarpNodeId)
            {
                if (sLastPickedTrack < 0)
                {
                    shuffle_u8(sMusicTracks + 1, sizeof(sMusicTracks) / sizeof(sMusicTracks[0]) - 1);
                    sLastPickedTrack = 0;
                    seq_player_play_sequence(0, 0x26, 0);
                }
                else
                {
                    if (sLastPickedTrack == sizeof(sMusicTracks) / sizeof(sMusicTracks[0]) - 1)
                    {
                        sLastPickedTrack = 0;
                        shuffle_u8(sMusicTracks, sizeof(sMusicTracks) / sizeof(sMusicTracks[0]));
                    }
                    else
                    {
                        sLastPickedTrack++;
                    }

                    seq_player_play_sequence(0, sMusicTracks[sLastPickedTrack], 0);
                }
            }
        }
        if (110 == o->oTimer)
        {
            if (0x22 == sSourceWarpNodeId)
                seq_player_play_sequence(0, 0x25, 0); 
        }

        if (o->oTimer < 130)
        {
            gMarioStates->pos[0] = o->oPosX;
            gMarioStates->pos[1] = o->oPosY;
            gMarioStates->pos[2] = o->oPosZ - 50.f;
            coop_mario_pin();
        }
        else
        {
            int time = o->oCtlFinalTime ?: o->oTimer - 130;
            sprintf(timerLine, "%d:%02d.%02d", time / 60 / 30, (time / 30) % 60, (int) (3.33333f * (time % 30)));
            print_defer(20, 220, timerLine, 255, 0, 0);
        }

        // print_text_fmt_int(20, 20, "%d", (int) (gMarioStates[0].kartProgress * 1000), 0);
        int placement = 1;
        for (int i = 1; i < RACERS_COUNT; i++)
        {
            if (gMarioStates[i].kartProgress >= gMarioStates[0].kartProgress)
            {
                placement++;
            }
        }

        if (o->oTimer > 130)
        {
            sprintf(placeLine, "%02d of %d", placement, RACERS_COUNT);
            print_defer(20, 200, placeLine, 255, 0, 1 == placement);
        }

        gMarioStates->health = 0x880;
        if (gMarioStates->kartProgress > (sWantBPE ? 1.f : 50.f) && gMarioStates->floor && gMarioStates->floor->object)
        {
            int clear = 0;
            if (sWantBPE)
            {
                clear = gMarioStates->kartProgress > (sWalkLimit - 2*WALK_LIMIT_SAFEGAP);
            }
            else
            {
                clear = 0 == gMarioStates->floor->object->oPartIndex
                     || 1 == gMarioStates->floor->object->oPartIndex
                     || 2 == gMarioStates->floor->object->oPartIndex;
            }

            if (clear)
            {
                sEnableProgress = 0;
                sPlacement = placement;
                o->oCtlFinalTime = o->oTimer - 130;
                o->oAction = 1;
                set_camera_mode_fixed2(gCamera);
                gCamera->cutscene = CUTSCENE_ENDING;
            }
        }
    }

    // print_text_fmt_int(120, 20, "%d", gMarioStates->intendedYaw);
    // print_text_fmt_int(120, 40, "%d", (int) (1000 * gMarioStates->floor->normal.y));

    // print_text_fmt_int(160, 20, "X %d", (int) gMarioStates->pos[0]);
    // print_text_fmt_int(160, 40, "Y %d", (int) gMarioStates->pos[1]);
    // print_text_fmt_int(160, 60, "Z %d", (int) gMarioStates->pos[2]);

    // print_text_fmt_int(20, 180, "P %d", sWalkLimit);
}

void bhv_part_loop()
{
    o->oDrawingDistance = 30000.0f;
    
    if (o->collisionData)
    {
        load_object_static_model();        
        obj_update_gfx_pos_and_angle(o);
    }
}

static void get_loc_fuzzed(Vec3f pos, struct Object* part, f32 fuzz)
{
    struct PartConfig* partConfig = &sPartConfigs[part->oBehParams2ndByte];
    if (partConfig->thin)
    {
        fuzz *= 0.5f;
    }
    if (partConfig->twistLeft || partConfig->twistRight)
    {
        fuzz *= 0.05f;
    }
#ifdef DEBUG_DISABLE_FUZZ
    fuzz = 0.0f;
#endif

    pos[0] = part->oPosX + fuzz * coss(part->oFaceAngleYaw);
    pos[1] = part->oPosY;
    pos[2] = part->oPosZ - fuzz * sins(part->oFaceAngleYaw);
}

#ifdef DEBUG_LOC_TEST_PARTICLES
static void spawn_test_particle(struct Object* part, f32 x, f32 y, f32 z)
{
    struct Object* p = spawn_object(part, MODEL_STAR, bhvTest);
    p->oPosX = x;
    p->oPosY = y + 10.f;
    p->oPosZ = z;
}
#else
static void spawn_test_particle(struct Object* part, f32 x, f32 y, f32 z)
{
    // -- 
}
#endif

void coop_npc_behavior(struct MarioState * m)
{
    if (m->kartRNGTime == (gGlobalTimer % 300))
    {
        m->kartLocFuzz = random_f32_around_zero(900.0f);
    }

    if (m->action != ACT_BURNING_GROUND)
        m->input |= INPUT_NONZERO_ANALOG;
    else
        m->input &= ~INPUT_NONZERO_ANALOG;
    
    m->intendedMag = 25.0f + m->kartVelFuzz;
    if (m->floor && m->floor->object)
    {
        struct Object* currPart = m->floor->object;
        struct Object* nextPart1 = currPart->oPartNext ?: currPart;
        struct PartConfig* currPartConfig = &sPartConfigs[currPart->oBehParams2ndByte];
        // print_text_fmt_int(200, 160, "P %d", currPart->oBehParams2ndByte);
        if (11 == currPart->oBehParams2ndByte)
        {
            s16 angleDiff = ((s16) m->faceAngle[1]) - ((s16) currPart->oFaceAngleYaw);
            s16 mult = angleDiff < 0 ? -1 : 1;
            f32 relProgress = m->kartProgress - (int) m->kartProgress;
            if (relProgress < 0.8f)
            {
                if (m->wall)
                {
                    m->intendedYaw = currPart->oFaceAngleYaw;
                }
                else
                {
                    m->intendedYaw = currPart->oFaceAngleYaw + mult * 0x4000;   
                }
            }
            else
            {
                m->intendedYaw = currPart->oFaceAngleYaw - mult * 0x4000;
            }
        }
        else if (currPartConfig->turn)
        {
            f32 x = currPartConfig->shift[0] > 0 ? (5000.f * SCALE) : (-5000.f * SCALE);
            f32 z = 0;

            f32 xRot = x * coss(currPart->oFaceAngleYaw) 
                     + z * sins(currPart->oFaceAngleYaw);
            f32 zRot = -x * sins(currPart->oFaceAngleYaw)
                      + z * coss(currPart->oFaceAngleYaw);

            Vec3f xyzRot;
            xyzRot[0] = xRot;
            xyzRot[1] = 0;
            xyzRot[2] = zRot;
            Vec3f center;
            vec3_sum(center, &currPart->oPosVec, xyzRot);

            Vec3f diff;
            vec3_diff(diff, m->pos, center);
            spawn_test_particle(currPart, center[0], center[1], center[2]);

            s32 length = sqrtf((diff[0] * diff[0]) + (diff[2] * diff[2]));
            s32 lengthStrengthener = CLAMP(length - 2000, 0, 3000) * 3;
            // print_text_fmt_int(20, 140, "LL %d", length);
            // print_text_fmt_int(20, 120, "LS %d", lengthStrengthener);

            spawn_test_particle(currPart, m->pos[0] + sins(atan2s(diff[2], diff[0])) * 300.f, 0.f, m->pos[2] + coss(atan2s(diff[2], diff[0])) * 300.f);

            int turnSign = currPartConfig->turn > 0 ? 1 : -1;
            m->intendedYaw = atan2s(diff[2], diff[0]) + (0x8000) * turnSign;

            // print_text_fmt_int(20, 20, "TT %d", m->intendedYaw);
            // print_text_fmt_int(20, 40, "Y %d", m->slideYaw);
        }
        else if ((currPartConfig->twistLeft || currPartConfig->twistRight || currPartConfig->twistRightChanging || currPartConfig->twistLeftChanging))
        {
            m->intendedYaw = currPart->oFaceAngleYaw + 0x8000;
            // print_text_fmt_int(20, 120, "TA %x", (u16) m->intendedYaw);

            s16 angleDiff16 = m->intendedYaw - m->slideYaw;
            s32 angleDiff = angleDiff16;
            if (currPartConfig->twistRight)
            {
                if (angleDiff < 0)
                    angleDiff = 0;
            }
            if (currPartConfig->twistLeft)
            {
                if (angleDiff > 0)
                    angleDiff = 0;
            }
            // print_text_fmt_int(20, 140, "DA %d", angleDiff); 
            // print_text_fmt_int(20, 160, "SY %x", (u16) m->slideYaw);

            angleDiff *= 10;
            angleDiff = CLAMP(angleDiff, -0x4000, 0x4000);
            m->intendedYaw += angleDiff;
            
            // print_text_fmt_int(20, 20, "TF %x", (u16) m->intendedYaw);
        }
        else
        {
            if (m->wall)
            {
                m->intendedYaw = atan2s(m->wall->normal.z, m->wall->normal.x);
            }
            else
            {
                Vec3f loc1;
                get_loc_fuzzed(loc1, nextPart1, m->kartLocFuzz);

                struct Object* nextPart2 = nextPart1->oPartNext ?: nextPart1;
                Vec3f loc2;
                get_loc_fuzzed(loc2, nextPart2, m->kartLocFuzz);

                Vec3f nextLoc;
                if (currPartConfig->twistLeft || currPartConfig->twistRight)
                {
                    nextLoc[0] = loc1[0] + 1000.f * coss(currPart->oFaceAngleYaw);
                    nextLoc[1] = loc1[1];
                    nextLoc[2] = loc1[2] + 1000.f * sins(currPart->oFaceAngleYaw);

                    // print_text_fmt_int(20, 40, "A %d", 0);

                    // print_text_fmt_int(120, 140, "N0 %d", (int) nextLoc[0]);
                    // print_text_fmt_int(120, 160, "N1 %d", (int) nextLoc[1]);
                    // print_text_fmt_int(120, 180, "N2 %d", (int) nextLoc[2]);
                }
                else
                {
                    nextLoc[0] = (loc1[0] + loc2[0]) / 2.f;
                    nextLoc[1] = (loc1[1] + loc2[1]) / 2.f;
                    nextLoc[2] = (loc1[2] + loc2[2]) / 2.f;

                    spawn_test_particle(currPart, loc1[0], loc1[1], loc1[2]);
                    spawn_test_particle(currPart, loc2[0], loc2[1], loc2[2]);
                    spawn_test_particle(currPart, nextLoc[0], nextLoc[1], nextLoc[2]);
                }

                s16 forceAngle = 0;
                forceAngle = forceAngle ?: sPartConfigs[currPart ->oBehParams2ndByte].forceAngle;
                forceAngle = forceAngle ?: sPartConfigs[nextPart1->oBehParams2ndByte].forceAngle;

                Vec3f diff;
                vec3_diff(diff, nextLoc, m->pos);
                m->intendedYaw = forceAngle ? forceAngle + currPart->oFaceAngleYaw + 0x8000 : atan2s(diff[2], diff[0]);
                
                // print_text_fmt_int(20, 20, "R %d", m->intendedYaw);
                // print_text_fmt_int(20, 40, "F %x", forceAngle);

                // print_text_fmt_int(20, 100, "X %d", (int) m->slideVelX);
                // print_text_fmt_int(20, 120, "Z %d", (int) m->slideVelZ);
                // print_text_fmt_int(20, 140, "0 %d", (int) m->pos[0]);
                // print_text_fmt_int(20, 160, "1 %d", (int) m->pos[1]);
                // print_text_fmt_int(20, 180, "2 %d", (int) m->pos[2]);
            }
        }
        
        spawn_test_particle(m->marioObj, m->pos[0] + sins(m->intendedYaw) * 300.f, m->pos[1], m->pos[2] + coss(m->intendedYaw) * 300.f);
    }
}

s16 kart_angle(int kartId)
{
    return sPartConfigs[kartId].turn;
}

extern void coop_randomize();
void bhv_kart_show_loop()
{
    coop_randomize();
    if (o->oDistanceToMario < 300.f)
    {
        switch (o->oBehParams2ndByte)
        {
            case 0x20:
                print_defer(160, 20, "Beginner", 255, 1, 0);
                break;
            case 0x21:
                print_defer(160, 20, "Advanced", 255, 1, 0);
                break;
            case 0x22:
                print_defer(160, 20, "Expert", 255, 1, 0);
                break;
            case 0x30:
                print_defer(160, 20, "Personalized", 255, 1, 0);
                break;
        }
    }
}

void bhv_test_loop()
{
    if (o->oTimer)
    {
        o->activeFlags = 0;
    }
}

int kart_deduce_progress(struct MarioState *m, struct Object* part)
{
    if (m == gMarioStates)
    {
        // print_text_fmt_int(20, 20, "P %d", part->oBehParams2ndByte);
    }

    if (!sEnableProgress)
    {
        return 0;
    }

    if (gCurrCourseNum != COURSE_EXAMPLE)
    {
        return 0;
    }

    // truncated progress is the part index
    int curProgressRounded = (int) m->kartProgress;
    int id = part->oPartIndex;

    if (m != gMarioStates)
    {
        if (0 == id || 1 == id || 2 == id)
        {
            if (m->kartProgress > 50.f)
                m->kartProgress = 1000.f;
        }
    }

    if (curProgressRounded != id
     && curProgressRounded != id - 1
     && curProgressRounded != id - 2
     && curProgressRounded != id - 3
     && curProgressRounded != id - 4
     && curProgressRounded != id - 5)
    {
        return 0;
    }

    const struct PartConfig* partConfig = &sPartConfigs[part->oBehParams2ndByte];
    f32 progress = 0.0f;
    // if (0 == partConfig->turn)
    // technically this will work for turns too, but it will be less accurate
    {
        Vec3f diff;
        vec3_diff(diff, m->pos, &part->oPosVec);
        f32 diffUnrotZ = diff[0] * sins(part->oFaceAngleYaw)
                       + diff[2] * coss(part->oFaceAngleYaw);

        f32 divided = diffUnrotZ / (partConfig->shift[2] * SCALE);
        if (m == gMarioStates)
        {
            // print_text_fmt_int(20, 40, "D %d", (int) (divided * 1000));
        }
        progress = CLAMP(divided, 0.f, 0.9999f);
    }

    m->kartProgress = progress + (f32) id;
    return 1;
}

static int bpe_arrlen(const u8* arr)
{
    int len = 0;
    while (arr[len])
    {
        len++;
    }
    return len;
}

static u8 bpe_pick(const u8* arr)
{
    int arrSize = bpe_arrlen(arr);
    if (1 == arrSize)
    {
        return arr[0];
    }

    return arr[random_u32() % arrSize];
}

static void bpe_gen(void)
{
    void** _bpe_pairs = segmented_to_virtual(bpe_pairs);
    void** _bpe_relations = segmented_to_virtual(bpe_relations);

    int amountGenerated = 0;
    u8 lastGenerated = 3;
    while (amountGenerated < 250)
    {
        u8 next = bpe_pick(segmented_to_virtual(_bpe_relations[lastGenerated]));
        lastGenerated = next;
        if (lastGenerated < 100)
        {
            uRNGScratch[amountGenerated] = lastGenerated;
            amountGenerated++;
        }
        else
        {
            const u8* desc = segmented_to_virtual(_bpe_pairs[lastGenerated - 100]);
            int arrlen = bpe_arrlen(desc);
            memcpy(uRNGScratch + amountGenerated, desc, arrlen);
            amountGenerated += arrlen;
        }
    }
}

static void bpe_feed(void)
{
    f32 maxProgress = gMarioStates->kartProgress;
    for (int i = 1; i < RACERS_COUNT; i++)
    {
        if (gMarioStates[i].kartProgress >= maxProgress)
        {
            maxProgress = gMarioStates[i].kartProgress;
        }
    }

    while (maxProgress + 9 > sAmountGenerated)
    {
        if (sAmountGenerated >= sWalkLimit + WALK_LIMIT_SAFEGAP)
        {
            return;
        }

        struct SpawnResult result = spawn_track(sAmountGenerated, uRNGScratch + sAmountGenerated, 1);
        sAmountGenerated++;
        sLastPart->oPartNext = result.firstPart;
        result.firstPart->oPartPrev = sLastPart;
        sLastPart = result.lastPart;
    }
}
