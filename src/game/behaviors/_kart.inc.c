#include "game/mario_coop.h"

#define RACERS_COUNT 35
// #define DEBUG_DISABLE_FUZZ
// #define DEBUG_LOC_TEST_PARTICLES

void print_defer(s16 x, s16, const char* line, u8 ttl, u8 centered, u8 colored);

extern const Collision p1_collision[];
extern const Collision p2_collision[];
extern const Collision p3_collision[];
extern const Collision p4_collision[];
extern const Collision p5_collision[];
extern const Collision p6_collision[];
extern const Collision p7_collision[];
extern const Collision p8_collision[];
extern const Collision p9_collision[];
extern const Collision p10_collision[];
extern const Collision p11_collision[];
extern const Collision p12_collision[];
extern const Collision p13_collision[];
extern const Collision p14_collision[];
extern const Collision p15_collision[];
extern const Collision p16_collision[];
extern const Collision p17_collision[];
extern const Collision p18_collision[];
extern const Collision p19_collision[];
extern const Collision p20_collision[];
extern const Collision p21_collision[];
extern const Collision p22_collision[];
extern const Collision p23_collision[];
extern const Collision p24_collision[];
extern const Collision p28_collision[];
extern const Collision p29_collision[];
extern const Collision p30_collision[];
extern const Collision p31_collision[];
extern const Collision p32_collision[];
extern const Collision p33_collision[];
extern const Collision p34_collision[];
extern const Collision p35_collision[];
extern const Collision p36_collision[];
extern const Collision p37_collision[];
extern const Collision p38_collision[];
extern const Collision p39_collision[];
extern const Collision p40_collision[];
extern const Collision p41_collision[];
extern const Collision p42_collision[];
extern const Collision p43_collision[];
extern const Collision p44_collision[];
extern const Collision p45_collision[];
extern const Collision p46_collision[];
extern const Collision p47_collision[];
extern const Collision p48_collision[];
extern const Collision p49_collision[];
extern const Collision p50_collision[];
extern const Collision p51_collision[];
extern const Collision p52_collision[];
extern const Collision p53_collision[];
extern const Collision p54_collision[];
extern const Collision p55_collision[];
extern const Collision p56_collision[];
extern const Collision p57_collision[];
extern const Collision p58_collision[];
extern const Collision p59_collision[];
extern const Collision p60_collision[];
extern const Collision p61_collision[];
extern const Collision p62_collision[];
extern const Collision p63_collision[];
extern const Collision p64_collision[];
extern const Collision p65_collision[];
extern const Collision p66_collision[];
extern const Collision p67_collision[];
extern const Collision p68_collision[];
extern const Collision p69_collision[];
extern const Collision p70_collision[];
extern const Collision p71_collision[];
extern const Collision p72_collision[];
extern const Collision p73_collision[];
extern const Collision p74_collision[];
extern const Collision p75_collision[];
extern const Collision p76_collision[];
extern const Collision p77_collision[];
extern const Collision p78_collision[];
extern const Collision p79_collision[];
extern const Collision p80_collision[];
extern const Collision p81_collision[];
extern const Collision p82_collision[];
extern const Collision p83_collision[];
extern const Collision p84_collision[];
extern const Collision p85_collision[];

static const Collision *sCollisionHeaders[] = {
    NULL, // Placeholder for 0
    p1_collision,
    p2_collision,
    p3_collision,
    p4_collision,
    p5_collision,
    p6_collision,
    p7_collision,
    p8_collision,
    p9_collision,
    p10_collision,
    p11_collision,
    p12_collision,
    p13_collision,
    p14_collision,
    p15_collision,
    p16_collision,
    p17_collision,
    p18_collision,
    p19_collision,
    p20_collision,
    p21_collision,
    p22_collision,
    p23_collision,
    p24_collision,
    NULL, // Placeholder for 25
    NULL, // Placeholder for 26
    NULL, // Placeholder for 27
    p28_collision,
    p29_collision,
    p30_collision,
    p31_collision,
    p32_collision,
    p33_collision,
    p34_collision,
    p35_collision,
    p36_collision,
    p37_collision,
    p38_collision,
    p39_collision,
    p40_collision,
    p41_collision,
    p42_collision,
    p43_collision,
    p44_collision,
    p45_collision,
    p46_collision,
    p47_collision,
    p48_collision,
    p49_collision,
    p50_collision,
    p51_collision,
    p52_collision,
    p53_collision,
    p54_collision,
    p55_collision,
    p56_collision,
    p57_collision,
    p58_collision,
    p59_collision,
    p60_collision,
    p61_collision,
    p62_collision,
    p63_collision,
    p64_collision,
    p65_collision,
    p66_collision,
    p67_collision,
    p68_collision,
    p69_collision,
    p70_collision,
    p71_collision,
    p72_collision,
    p73_collision,
    p74_collision,
    p75_collision,
    p76_collision,
    p77_collision,
    p78_collision,
    p79_collision,
    p80_collision,
    p81_collision,
    p82_collision,
    p83_collision,
    p84_collision,
};

struct PartConfig
{
    Vec3s shift;
    s16 turn;
    // instead of range [-1000, 1000] range is [-500, 500]
    u8 thin;
    u8 thinChanging;
    u8 tunnel;
    u8 tunnelChanging;
    u8 twistRight;
    u8 twistRightChanging;
    u8 twistLeft;
    u8 twistLeftChanging;
    s16 forceAngle;
};

#define TURN_LEFT 0x4000
#define TURN_RIGHT -0x4000

static const struct PartConfig sPartConfigs[] = {
    /* 0 */ { },
    /* 1 */ { { 0, 0, -4000 } },
    /* 2 */ { { 0, 0, -4000 } },
    /* 3 */ { { 0, 0, -4000 } },
    /* 4 */ { { 0, 0, -4000 }, .thin = 1 },
    /* 5 */ { { -500, 0, -4000 }, .thinChanging = 1  },
    /* 6 */ { {  500, 0, -4000 }, .thin = 1, .thinChanging = 1 },
    /* 7 */ { { 0, 0, -20000 } },
    /* 8 */ { { 0, 0, -20000 } },
    /* 9 */ { { 0, 0, -16500 } },
    /* 10 */ { { 0, 0, -1000 } },
    /* 11 */ { { 0, 0, -20000 }, }, // scary?
    /* 12 */ { { 0, -500, -2000 } },
    /* 13 */ { { 0, 500, -2000 } },
    /* 14 */ { { 0, -1000, -2000 } },
    /* 15 */ { { 0, 1000, -2000 } },
    /* 16 */ { { 0, -1000, -2000 } },
    /* 17 */ { { 0, -500, -4000 } },
    /* 18 */ { { 0, 500, -4000 } },
    /* 19 */ { { 0, -500, -2000 } },
    /* 20 */ { { 0, -500, -2000 } },
    /* 21 */ { { 0, 500, -2000 } },
    /* 22 */ { { 0, 500, -2000 } },
    /* 23 */ { { 0, -500, -2000 } },
    /* 24 */ { { 0, -500, -2000 } },
    /* 25 */ { { 0, 0, -1000 } }, // raw shifts
    /* 26 */ { { 0, -1000, 0 } },
    /* 27 */ {}, // unused so who the hell knows what this is supposed to be
    /* 28 */ { { 0, 0, -250 } },
    /* 29 */ { { 0, 0, -250 } },
    /* 30 */ { { 0, 0, -250 } },
    /* 31 */ { { 0, 0, -250 } },
    /* 32 */ { { -4000, 0, -4000 }, .turn = TURN_LEFT, },
    /* 33 */ { {  4000, 0, -4000 }, .turn = TURN_RIGHT, },
    /* 34 */ { { -1172, 0, -2828 }, .turn = TURN_LEFT / 2, },
    /* 35 */ { {  1172, 0, -2828 }, .turn = TURN_RIGHT / 2, },
    /* 36 */ { { -4000, 500, -4000 }, .turn = TURN_LEFT, },
    /* 37 */ { {  4000, 500, -4000 }, .turn = TURN_RIGHT, },
    /* 38 */ { { -1172, 500, -2828 }, .turn = TURN_LEFT / 2, },
    /* 39 */ { {  1172, 500, -2828 }, .turn = TURN_RIGHT / 2, },
    /* 40 */ { { -4000, -500, -4000 }, .turn = TURN_LEFT, },
    /* 41 */ { {  4000, -500, -4000 }, .turn = TURN_RIGHT, },
    /* 42 */ { { -1172, -500, -2828 }, .turn = TURN_LEFT / 2, },
    /* 43 */ { {  1172, -500, -2828 }, .turn = TURN_RIGHT / 2, },
    /* 44 */ { { -2000, 0, -2000 }, .turn = TURN_LEFT, },
    /* 45 */ { {  2000, 0, -2000 }, .turn = TURN_RIGHT, },
    /* 46 */ { { -2000, 0, -2000 }, .turn = TURN_LEFT, .forceAngle = TURN_LEFT, },
    /* 47 */ { {  2000, 0, -2000 }, .turn = TURN_RIGHT, .forceAngle = TURN_RIGHT, },
    /* 48 */ { { -3000, 0, -3000 }, .turn = TURN_LEFT, },
    /* 49 */ { {  3000, 0, -3000 }, .turn = TURN_RIGHT, },
    /* 50 */ { { 0, 0, -3000 }, .tunnelChanging = 1, },
    /* 51 */ { { 0, 0, -3000 }, .tunnel = 1, .tunnelChanging = 1, },
    /* 52 */ { { 0, 0, -3000 }, .tunnel = 1, },
    /* 53 */ { { -1172, 0, -2828 }, .tunnel = 1, .turn = TURN_LEFT / 2, },
    /* 54 */ { {  1172, 0, -2828 }, .tunnel = 1, .turn = TURN_RIGHT / 2, },
    /* 55 */ { { 0, 0, -4000 }, .tunnel = 1, .thinChanging = 1, },
    /* 56 */ { { 0, 0, -4000 }, .tunnel = 1, .thin = 1, .thinChanging = 1, },
    /* 57 */ { { 0, 0, -3000 }, .tunnel = 1, .thin = 1, },
    /* 58 */ { { -1172, 0, -2828 }, .tunnel = 1, .turn = TURN_LEFT / 2, },
    /* 59 */ { {  1172, 0, -2828 }, .tunnel = 1, .turn = TURN_RIGHT / 2, },
    /* 60 */ { { 0, 500, -2000 }, .tunnel = 1, },
    /* 61 */ { { 0, 500, -2000 }, .tunnel = 1, },
    /* 62 */ { { 0, 500, -2000 }, .tunnel = 1, },
    /* 63 */ { { 0, -500, -2000 }, .tunnel = 1, },
    /* 64 */ { { 0, -500, -2000 }, .tunnel = 1, },
    /* 65 */ { { 0, -500, -2000 }, .tunnel = 1, },
    /* 66 */ { { 0, 500, -2000 }, .tunnel = 1, .thin = 1, },
    /* 67 */ { { 0, 500, -2000 }, .tunnel = 1, .thin = 1, },
    /* 68 */ { { 0, 500, -2000 }, .tunnel = 1, .thin = 1, },
    /* 69 */ { { 0, -500, -2000 }, .tunnel = 1, .thin = 1, },
    /* 70 */ { { 0, -500, -2000 }, .tunnel = 1, .thin = 1, },
    /* 71 */ { { 0, -500, -2000 }, .tunnel = 1, .thin = 1, },
    /* 72 */ { { 0, 0, -8000 }, .twistRightChanging = 1, },
    /* 73 */ { {  4000, 0, -4000 }, .twistRight = 1, .turn = TURN_RIGHT, },
    /* 74 */ { { 0, 0, -8000 }, .twistRight = 1, .twistRightChanging = 1, },
    /* 75 */ { { 0, 0, -8000 }, .twistLeftChanging = 1, },
    /* 76 */ { { -4000, 0, -4000 }, .twistLeft = 1, .turn = TURN_LEFT, },
    /* 77 */ { { 0, 0, -8000 }, .twistLeft = 1, .twistLeftChanging = 1, },
    /* 78 */ { { 0, 0, -2000 }, .twistLeft = 1, },
    /* 79 */ { { 0, 0, -2000 }, .twistRight = 1, },
    /* 80 */ { { 0, 0, -16500 }, },
    /* 81 */ { { -121, 0, -879 }, },
    /* 82 */ { {  121, 0, -879 }, },
    /* 83 */ { { -828, 0, -1172 }, },
    /* 84 */ { {  828, 0, -1172 }, },
};

static const u8 uRoute101Track[] = {
3, 1, 3, 3, 3, 35, 3, 3, 3, 3, 34, 3, 3, 19, 12, 12, 20, 3, 3, 3, 33
, 3, 19, 12, 12, 12, 12, 20, 3, 3, 3, 18, 34, 3, 35, 3, 32, 3, 17, 3, 3, 50, 52, 52, 52, 54, 52, 52, 53
, 52, 52, 63, 64, 64, 64, 64, 65, 52, 52, 52, 52, 54, 52, 52, 52, 51, 3, 3, 3, 8, 3, 2, 3, 3, 18, 19, 12
, 20, 3, 3, 34, 39, 3, 3, 40, 3, 42, 3, 32, 3, 17, 3, 3, 3, 3, 15, 3, 3, 48, 3, 23, 24, 18, 3, 3, 18, 3
, 50, 52, 54, 53, 52, 54, 54, 52, 63, 64, 65, 52, 52, 52, 51, 3, 9, 80, 3, 3, 38, 3, 23, 16, 16, 24, 19
, 12, 20, 3, 3, 3, 3, 15, 3, 3, 3, 49, 50, 52, 52, 52, 53, 52, 63, 65, 52, 51, 3, 18, 3, 3, 3, 7, 3, 2, 3
, 3, 3, 3, 3, 3, 18, 23, 16, 16, 16, 24, 19, 12, 12, 20, 3, 21, 13, 22, 3, 3, 35, 3, 42, 35, 3, 19, 12, 12
, 20, 3, 18, 3, 38, 3, 19, 12, 20, 18, 3, 3, 49, 3, 49, 3, 23, 16, 24, 19, 18, 12, 12, 20, 3, 18, 3, 3, 36
, 32, 3, 50, 52, 63, 64, 64, 64, 64, 65, 60, 62, 54, 52, 52, 52, 52, 53, 52, 52, 55, 57, 69, 70, 70, 70, 70
, 70, 71, 66, 68, 57, 57, 57, 69, 70, 71, 57, 57, 66, 68, 56, 52, 54, 52, 52, 52, 53, 53, 52, 52, 51, 3, 3
, 39, 3, 3, 3, 10, 3, 8, 34, 3, 3, 34, 3, 3,
};

static const u8 uRoute280Track[] = {
3, 1, 3, 3, 18, 3, 19, 20, 18, 3, 34, 3, 17, 35, 3, 3, 11, 3, 17, 33, 17, 3, 3
, 18, 20, 32, 10, 50, 52, 52, 63, 64, 64, 64, 65, 52, 53, 63, 65, 52, 52, 52, 54, 52, 54, 52, 54, 52, 52
, 52, 51, 9, 80, 38, 3, 34, 3, 11, 18, 50, 52, 54, 54, 52, 51, 3, 3, 7, 2, 3, 3, 18, 23, 24, 3, 3, 3, 49
, 46, 3, 11, 18, 19, 12, 20, 3, 18, 3, 41, 42, 3, 3, 15, 19, 12, 12, 12, 20, 19, 20, 3, 18, 3, 41, 3, 41
, 41, 17, 3, 3, 39, 3, 3, 34, 3, 23, 16, 16, 24, 18, 3, 17, 3, 3, 3, 18, 3, 35, 3, 3, 34, 3, 3, 3, 3, 46
, 3, 34, 3, 19, 12, 12, 12, 20, 10, 15, 34, 3, 3, 3, 23, 16, 16, 16, 16, 24, 3, 3, 8, 3, 2, 3, 3, 3, 3, 18
, 50, 52, 54, 52, 53, 63, 64, 64, 64, 65, 52, 52, 54, 54, 52, 55, 57, 57, 57, 69, 70, 70, 70, 70, 71, 56
, 52, 52, 53, 53, 53, 54, 52, 52, 51, 3, 9, 80, 3, 18, 3, 3, 48, 47, 3, 3, 41, 11, 3, 18, 50, 53, 53, 52
, 63, 64, 64, 65, 60, 62, 53, 63, 64, 64, 64, 65, 52, 52, 54, 52, 55, 69, 70, 71, 56, 54, 54, 63, 64, 64
, 65, 63, 65, 55, 57, 57, 57, 57, 58, 57, 57, 57, 59, 57, 57, 58, 57, 57, 59, 58, 57, 66, 68, 56, 52, 52
, 54, 54, 52, 52, 54, 52, 53, 52, 52, 53, 53, 52, 63, 64, 64, 64, 64, 64, 64, 64, 64, 65, 52, 52, 51, 3
, 18, 50, 51, 3, 50, 51, 18, 50, 52, 53, 52, 54, 52, 51, 3, 11, 3, 3, 2, 33, 19, 20, 21, 22, 23, 24, 19
, 20, 21, 22, 23, 24, 19, 20, 21, 22, 23, 24, 33, 3, 3
};

static u8 uRNGScratch[250];

static const u8 uExpertTrack[] = {
   1, 
   
   // 35, 72, 73, 79, 79, 79, 74, 35, 9, 80,

   3, 3, 17, 3, 38, 3, 17, 34, 18, 3, 3, 3, 46, 35, 18, 3, 28, 26, 29, 18, 3, 36, 34, 3, 3, 3, 38, 39
, 3, 9, 80, 18, 3, 35, 17, 3, 3, 15, 17, 49, 50, 52, 52, 63, 65, 54, 53, 52, 52, 53, 53, 52, 55, 57, 57
, 59, 66, 68, 56, 52, 54, 52, 60, 62, 54, 51, 18, 50, 63, 65, 51, 35, 3, 42, 17, 11, 18, 35, 72, 73, 79
, 79, 79, 74, 35, 9, 80, 3, 48, 3, 48, 3, 9, 80, 39, 3, 17, 37, 3, 28, 25, 25, 26, 29, 10, 3, 49, 3, 17
, 3, 34, 17, 83, 10, 10, 10, 48, 17, 10, 28, 29, 3,  };

static const u8 uStandardTrack[] = {
1, 3, 10, 10, 34, 3, 18, 35, 3, 19, 20, 3, 39, 3, 3, 48, 3, 17, 3, 34, 18, 17, 3, 3, 3, 18, 3, 34, 50
, 52, 52, 53, 63, 65, 52, 52, 53, 54, 51, 18, 3, 49, 48, 3, 19, 12, 20, 82, 3, 3, 15, 10, 32, 34, 3, 3
, 50, 54, 52, 52, 60, 62, 52, 52, 52, 52, 54, 54, 51, 3, 3, 35, 17, 3, 3, 21, 22, 3, 3, 33, 3, 23, 24
, 10, 10, 37, 3, 3, 49, 3, 38, 35, 3, 3, 10, 10, 28, 25, 25, 26, 29, 3, 34, 18, 83, 10, 48, 28, 29, 9, 80, 29, 
};

static const u8 sBeginnerTrack[] = {
    29, 1, 3, 3, 3, 18, 3, 34, 3, 3, 17, 38, 3, 3, 3, 50, 52, 52, 52, 53, 52, 60, 62, 52, 63, 52, 52, 53
, 52, 52, 53, 63, 65, 52, 52, 51, 9, 80, 17, 3, 18, 35, 3, 3, 35, 18, 35, 17, 3, 3, 39, 34, 3, 50, 60
, 62, 52, 54, 52, 54, 52, 63, 65, 52, 54, 51, 3, 84, 39, 3, 28, 29, 3, 35, 3, 34, 3, 3, 3, 10, 10
, 10, 17, 3, 40, 7, 82, 28, 
};

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

extern s16 sSourceWarpNodeId;
static u8 sEnableProgress = 1;
static u8 sWalkLimit = 0;

#define oCtlFinalTime oF4

extern void seq_player_play_sequence(u8 player, u8 seqId, u16 arg2);

struct SpawnResult
{
    struct Object* firstPart;
    struct Object* lastPart;
};

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
    f32 x;
    f32 z;
    int walked;
};

static struct WalkResult walk_track(const u8* track, int trackSize)
{
    f32 minX = 0.f, maxX = 0.f;
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
        f32 prevMinZ = minZ, prevMaxZ = maxZ;

        if (sSpawnerState.pos[0] < minX) minX = sSpawnerState.pos[0];
        if (sSpawnerState.pos[0] > maxX) maxX = sSpawnerState.pos[0];
        if (sSpawnerState.pos[2] < minZ) minZ = sSpawnerState.pos[2];
        if (sSpawnerState.pos[2] > maxZ) maxZ = sSpawnerState.pos[2];

        if ((maxX - minX > 100000.f) || (maxZ - minZ > 100000.f))
        {
            return (struct WalkResult){ -(prevMaxX + prevMinX) / 2.f, -(prevMaxZ + prevMinZ) / 2.f, i };
        }
    }

    return (struct WalkResult){ 0, 0, trackSize };
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

    sSpawnerState.pos[0] = 0;
    sSpawnerState.pos[1] = 0;
    sSpawnerState.pos[2] = 0;
    sSpawnerState.angle = 0;

    const u8* track = NULL;
    int trackSize = 10;
    switch (sSourceWarpNodeId)
    {
        case 0x20: 
            track = sBeginnerTrack;
            trackSize = sizeof(sBeginnerTrack);
            break;
        case 0x21: 
            track = uStandardTrack;
            trackSize = sizeof(uStandardTrack);
            break;
        case 0x22: 
            track = uExpertTrack;
            trackSize = sizeof(uExpertTrack);
            break;
    }

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
        struct WalkResult walk = walk_track(uRNGScratch, sizeof(uRNGScratch));

        sSpawnerState.pos[0] = gMarioStates->pos[0] = o->oPosX = walk.x;
        sSpawnerState.pos[1] = 0;
        sSpawnerState.pos[2] = gMarioStates->pos[2] = o->oPosZ = walk.z;
        sSpawnerState.angle = 0;
        sWalkLimit = walk.walked;

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
        pos[2] = o->oPosZ - x * 250;
        struct MarioState* racer = coop_spawn_mario(pos, COOP_CM_NPC);
        racer->faceAngle[1] = 0x8000;
    }
}

extern void coop_mario_pin();

char timerLine[20];
char placeLine[30];
u8 sPlacement;

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
                seq_player_play_sequence(0, 0x26, 0);
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
            gMarioStates->pos[2] = o->oPosZ;
            coop_mario_pin();
        }
        else
        {
            int time = o->oCtlFinalTime ?: o->oTimer - 130;
            sprintf(timerLine, "%d:%02d.%02d", time / 60 / 30, (time / 30) % 60, (int) (3.33333f * (time % 30)));
            print_defer(20, 220, timerLine, 255, 0, 0);
        }

        print_text_fmt_int(20, 20, "%d", (int) (gMarioStates[0].kartProgress * 1000), 0);
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
                clear = gMarioStates->kartProgress > (sWalkLimit - 2);
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

    print_text_fmt_int(20, 180, "P %d", sWalkLimit);
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
    pos[2] = part->oPosZ + fuzz * sins(part->oFaceAngleYaw);
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

    m->input |= INPUT_NONZERO_ANALOG;
    m->intendedMag = 25.0f + m->kartVelFuzz;
    if (m->floor && m->floor->object)
    {
        struct Object* currPart = m->floor->object;
        struct PartConfig* currPartConfig = &sPartConfigs[currPart->oBehParams2ndByte];
        // print_text_fmt_int(200, 160, "P %d", currPart->oBehParams2ndByte);
        if (currPartConfig->turn)
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
            struct Object* nextPart1 = currPart->oPartNext ?: currPart;
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

void kart_deduce_progress(struct MarioState *m, struct Object* part)
{
    if (!sEnableProgress)
    {
        return;
    }

    if (gCurrCourseNum != COURSE_EXAMPLE)
    {
        return;
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
        return;
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
            print_text_fmt_int(20, 40, "D %d", (int) (divided * 1000));
        }
        progress = CLAMP(divided, 0.f, 0.9999f);
    }

    m->kartProgress = progress + (f32) id;
}

static const u8 bpe_pair_100[] = { 3,3 ,0 };
static const u8 bpe_pair_101[] = { 52,52 ,0 };
static const u8 bpe_pair_102[] = { 64,64 ,0 };
static const u8 bpe_pair_103[] = { 3,3,3 ,0 };
static const u8 bpe_pair_104[] = { 52,54 ,0 };
static const u8 bpe_pair_105[] = { 18,3 ,0 };
static const u8 bpe_pair_106[] = { 57,57 ,0 };
static const u8 bpe_pair_107[] = { 12,20 ,0 };
static const u8 bpe_pair_108[] = { 52,52,53 ,0 };
static const u8 bpe_pair_109[] = { 3,17 ,0 };
static const u8 bpe_pair_110[] = { 63,64,64 ,0 };
static const u8 bpe_pair_111[] = { 9,80 ,0 };
static const u8 bpe_pair_112[] = { 63,65 ,0 };
static const u8 bpe_pair_113[] = { 3,3,3,3 ,0 };
static const u8 bpe_pair_114[] = { 12,12,20 ,0 };
static const u8 bpe_pair_115[] = { 51,3 ,0 };
static const u8 bpe_pair_116[] = { 16,16 ,0 };
static const u8 bpe_pair_117[] = { 52,53 ,0 };
static const u8 bpe_pair_118[] = { 70,70 ,0 };
static const u8 bpe_pair_119[] = { 10,10 ,0 };
static const u8 bpe_pair_120[] = { 3,19 ,0 };
static const u8 bpe_pair_121[] = { 52,52,54 ,0 };
static const u8 bpe_pair_122[] = { 23,24 ,0 };
static const u8 bpe_pair_123[] = { 60,62 ,0 };
static const u8 bpe_pair_124[] = { 3,34 ,0 };
static const u8 bpe_pair_125[] = { 18,50 ,0 };
static const u8 bpe_pair_126[] = { 34,3,3 ,0 };
static const u8 bpe_pair_127[] = { 50,52,52 ,0 };
static const u8 bpe_pair_128[] = { 19,20 ,0 };
static const u8 bpe_pair_129[] = { 17,3,3 ,0 };
static const u8 bpe_pair_130[] = { 34,3 ,0 };
static const u8 bpe_pair_131[] = { 18,3,3 ,0 };
static const u8 bpe_pair_132[] = { 64,65 ,0 };
static const u8 bpe_pair_133[] = { 23,16,16 ,0 };
static const u8 bpe_pair_134[] = { 66,68 ,0 };
static const u8 bpe_pair_135[] = { 35,3,3 ,0 };
static const u8 bpe_pair_136[] = { 21,22 ,0 };
static const u8 bpe_pair_137[] = { 12,12,12,20 ,0 };
static const u8 bpe_pair_138[] = { 35,3 ,0 };
static const u8 bpe_pair_139[] = { 63,64,64,64,64 ,0 };
static const u8 bpe_pair_140[] = { 52,52,52 ,0 };
static const u8 bpe_pair_141[] = { 3,2 ,0 };
static const u8 bpe_pair_142[] = { 3,3,18 ,0 };
static const u8 bpe_pair_143[] = { 19,12,20 ,0 };
static const u8 bpe_pair_144[] = { 39,3,3 ,0 };
static const u8 bpe_pair_145[] = { 48,3 ,0 };
static const u8 bpe_pair_146[] = { 54,52 ,0 };
static const u8 bpe_pair_147[] = { 12,12,20,3 ,0 };
static const u8 bpe_pair_148[] = { 49,3 ,0 };
static const u8 bpe_pair_149[] = { 54,52,52 ,0 };
static const u8 bpe_pair_150[] = { 57,69 ,0 };
static const u8 bpe_pair_151[] = { 70,71 ,0 };
static const u8 bpe_pair_152[] = { 66,68,56 ,0 };
static const u8 bpe_pair_153[] = { 51,3,3 ,0 };
static const u8 bpe_pair_154[] = { 63,64,64,64,65 ,0 };
static const u8 bpe_pair_155[] = { 52,54,52,54 ,0 };
static const u8 bpe_pair_156[] = { 3,3,3,18,3 ,0 };
static const u8 bpe_pair_157[] = { 55,57,57 ,0 };
static const u8 bpe_pair_158[] = { 52,52,53,53 ,0 };
static const u8 bpe_pair_159[] = { 19,20,21,22 ,0 };
static const u8 bpe_pair_160[] = { 19,20,21,22,23,24 ,0 };
static const u8 bpe_pair_161[] = { 17,3 ,0 };
static const u8 bpe_pair_162[] = { 26,29 ,0 };
static const u8 bpe_pair_163[] = { 35,17,3,3 ,0 };
static const u8 bpe_pair_164[] = { 28,29 ,0 };
static const u8 bpe_pair_165[] = { 3,1 ,0 };
static const u8 bpe_pair_166[] = { 32,3,17 ,0 };
static const u8 bpe_pair_167[] = { 52,52,53,52,52 ,0 };
static const u8 bpe_pair_168[] = { 63,64,64,64,64,65 ,0 };
static const u8 bpe_pair_169[] = { 52,52,52,52,54 ,0 };
static const u8 bpe_pair_170[] = { 52,52,52,51 ,0 };
static const u8 bpe_pair_171[] = { 8,3,2 ,0 };
static const u8 bpe_pair_172[] = { 3,3,3,3,15 ,0 };
static const u8 bpe_pair_173[] = { 52,54,54,52 ,0 };
static const u8 bpe_pair_174[] = { 51,3,9,80 ,0 };
static const u8 bpe_pair_175[] = { 23,16,16,24 ,0 };
static const u8 bpe_pair_176[] = { 3,3,3,49 ,0 };
static const u8 bpe_pair_177[] = { 50,52,52,52,53 ,0 };
static const u8 bpe_pair_178[] = { 50,52,52,52,53,52 ,0 };
static const u8 bpe_pair_179[] = { 52,51,3 ,0 };
static const u8 bpe_pair_180[] = { 18,3,3,3 ,0 };
static const u8 bpe_pair_181[] = { 16,24 ,0 };
static const u8 bpe_pair_182[] = { 16,24,19 ,0 };
static const u8 bpe_pair_183[] = { 35,3,42 ,0 };
static const u8 bpe_pair_184[] = { 35,3,19 ,0 };
static const u8 bpe_pair_185[] = { 3,19,12,20 ,0 };
static const u8 bpe_pair_186[] = { 57,69,70,70 ,0 };
static const u8 bpe_pair_187[] = { 57,69,70,70,70,70 ,0 };
static const u8 bpe_pair_188[] = { 66,68,56,52,54 ,0 };
static const u8 bpe_pair_189[] = { 10,3 ,0 };
static const u8 bpe_pair_190[] = { 63,65,52,52 ,0 };
static const u8 bpe_pair_191[] = { 3,11 ,0 };
static const u8 bpe_pair_192[] = { 3,18,3 ,0 };
static const u8 bpe_pair_193[] = { 3,18,3,41 ,0 };
static const u8 bpe_pair_194[] = { 3,3,15 ,0 };
static const u8 bpe_pair_195[] = { 17,3,3,3,18,3 ,0 };
static const u8 bpe_pair_196[] = { 34,3,3,3 ,0 };
static const u8 bpe_pair_197[] = { 63,64,64,64,65,52,52,54 ,0 };
static const u8 bpe_pair_198[] = { 63,64,64,65 ,0 };
static const u8 bpe_pair_199[] = { 57,57,58 ,0 };
static const u8 bpe_pair_200[] = { 57,57,58,57,57 ,0 };
static const u8 bpe_pair_201[] = { 52,52,53,53,52 ,0 };
static const u8 bpe_pair_202[] = { 51,18,50 ,0 };
static const u8 bpe_pair_203[] = { 19,20,21,22,23,24,19,20,21,22,23,24 ,0 };
static const u8 bpe_pair_204[] = { 39,3 ,0 };
static const u8 bpe_pair_205[] = { 79,79 ,0 };
static const u8 bpe_pair_206[] = { 28,25 ,0 };
static const u8 bpe_pair_207[] = { 28,25,25 ,0 };
static const u8 bpe_pair_208[] = { 28,25,25,26,29 ,0 };
static const u8 bpe_pair_209[] = { 3,17,3,34 ,0 };
static const u8 bpe_pair_210[] = { 10,10,10 ,0 };
static const u8 bpe_pair_211[] = { 28,29,3 ,0 };

static const u8* bpe_pairs[] = {
    bpe_pair_100,
    bpe_pair_101,
    bpe_pair_102,
    bpe_pair_103,
    bpe_pair_104,
    bpe_pair_105,
    bpe_pair_106,
    bpe_pair_107,
    bpe_pair_108,
    bpe_pair_109,
    bpe_pair_110,
    bpe_pair_111,
    bpe_pair_112,
    bpe_pair_113,
    bpe_pair_114,
    bpe_pair_115,
    bpe_pair_116,
    bpe_pair_117,
    bpe_pair_118,
    bpe_pair_119,
    bpe_pair_120,
    bpe_pair_121,
    bpe_pair_122,
    bpe_pair_123,
    bpe_pair_124,
    bpe_pair_125,
    bpe_pair_126,
    bpe_pair_127,
    bpe_pair_128,
    bpe_pair_129,
    bpe_pair_130,
    bpe_pair_131,
    bpe_pair_132,
    bpe_pair_133,
    bpe_pair_134,
    bpe_pair_135,
    bpe_pair_136,
    bpe_pair_137,
    bpe_pair_138,
    bpe_pair_139,
    bpe_pair_140,
    bpe_pair_141,
    bpe_pair_142,
    bpe_pair_143,
    bpe_pair_144,
    bpe_pair_145,
    bpe_pair_146,
    bpe_pair_147,
    bpe_pair_148,
    bpe_pair_149,
    bpe_pair_150,
    bpe_pair_151,
    bpe_pair_152,
    bpe_pair_153,
    bpe_pair_154,
    bpe_pair_155,
    bpe_pair_156,
    bpe_pair_157,
    bpe_pair_158,
    bpe_pair_159,
    bpe_pair_160,
    bpe_pair_161,
    bpe_pair_162,
    bpe_pair_163,
    bpe_pair_164,
    bpe_pair_165,
    bpe_pair_166,
    bpe_pair_167,
    bpe_pair_168,
    bpe_pair_169,
    bpe_pair_170,
    bpe_pair_171,
    bpe_pair_172,
    bpe_pair_173,
    bpe_pair_174,
    bpe_pair_175,
    bpe_pair_176,
    bpe_pair_177,
    bpe_pair_178,
    bpe_pair_179,
    bpe_pair_180,
    bpe_pair_181,
    bpe_pair_182,
    bpe_pair_183,
    bpe_pair_184,
    bpe_pair_185,
    bpe_pair_186,
    bpe_pair_187,
    bpe_pair_188,
    bpe_pair_189,
    bpe_pair_190,
    bpe_pair_191,
    bpe_pair_192,
    bpe_pair_193,
    bpe_pair_194,
    bpe_pair_195,
    bpe_pair_196,
    bpe_pair_197,
    bpe_pair_198,
    bpe_pair_199,
    bpe_pair_200,
    bpe_pair_201,
    bpe_pair_202,
    bpe_pair_203,
    bpe_pair_204,
    bpe_pair_205,
    bpe_pair_206,
    bpe_pair_207,
    bpe_pair_208,
    bpe_pair_209,
    bpe_pair_210,
    bpe_pair_211,
};

static u8 bpe_relations_1[] = { 3,100,103,156 ,0 };
static u8 bpe_relations_2[] = { 3,33,100,113,142 ,0 };
static u8 bpe_relations_3[] = { 1,2,3,7,8,9,10,11,15,17,18,19,21,23,28,32,33,34,35,36,38,39,40,41,42,46,48,49,50,84,100,105,111,119,122,125,126,127,128,130,131,133,135,136,138,142,144,145,148,161,163,164,166,171,175,177,178,180,183,189,195,206,207,208,210,211 ,0 };
static u8 bpe_relations_7[] = { 2,3,82,141 ,0 };
static u8 bpe_relations_8[] = { 3,34,126,141 ,0 };
static u8 bpe_relations_9[] = { 80 ,0 };
static u8 bpe_relations_10[] = { 3,10,15,17,28,32,34,37,48,50,127,130,161,164,206,207,208,211 ,0 };
static u8 bpe_relations_11[] = { 3,18,100,109,125 ,0 };
static u8 bpe_relations_12[] = { 12,20,107,114,137 ,0 };
static u8 bpe_relations_13[] = { 22 ,0 };
static u8 bpe_relations_15[] = { 3,10,17,19,34,100,103,176,196 ,0 };
static u8 bpe_relations_16[] = { 16,24,116,181,182 ,0 };
static u8 bpe_relations_17[] = { 3,10,11,33,34,35,37,38,49,83,100,103,113,124,135,156,172 ,0 };
static u8 bpe_relations_18[] = { 3,12,17,19,20,23,34,35,50,83,100,103,114,122,130,133,135,143,147,163,184,195 ,0 };
static u8 bpe_relations_19[] = { 12,18,20,107,114,137,147 ,0 };
static u8 bpe_relations_20[] = { 3,10,18,19,21,32,82,100,103,105,113,128,131,136,172,192,193 ,0 };
static u8 bpe_relations_21[] = { 13,22 ,0 };
static u8 bpe_relations_22[] = { 3,23,100,122 ,0 };
static u8 bpe_relations_23[] = { 16,24,116,181,182 ,0 };
static u8 bpe_relations_24[] = { 3,10,18,19,33,100,103,105,119,128,131,143,159,160,176 ,0 };
static u8 bpe_relations_25[] = { 25,26,162 ,0 };
static u8 bpe_relations_26[] = { 29 ,0 };
static u8 bpe_relations_28[] = { 25,26,29,162 ,0 };
static u8 bpe_relations_29[] = { 1,3,9,10,18,105,111,124,189 ,0 };
static u8 bpe_relations_32[] = { 3,10,34,109,126 ,0 };
static u8 bpe_relations_33[] = { 3,17,19,100,120,128,129,159,160,203 ,0 };
static u8 bpe_relations_34[] = { 3,17,18,39,50,100,103,109,113,120,144,180,191 ,0 };
static u8 bpe_relations_35[] = { 3,9,17,18,72,100,105,111,113,120,124,129 ,0 };
static u8 bpe_relations_36[] = { 32,34,196 ,0 };
static u8 bpe_relations_37[] = { 3,100 ,0 };
static u8 bpe_relations_38[] = { 3,35,39,100,103,109,120,124,135,185,204 ,0 };
static u8 bpe_relations_39[] = { 3,34,100,103,109,130 ,0 };
static u8 bpe_relations_40[] = { 3,7 ,0 };
static u8 bpe_relations_41[] = { 3,11,17,41,42,129 ,0 };
static u8 bpe_relations_42[] = { 3,17,35,100,184,194 ,0 };
static u8 bpe_relations_46[] = { 3,35,124,191 ,0 };
static u8 bpe_relations_47[] = { 3,100 ,0 };
static u8 bpe_relations_48[] = { 3,17,28,47,109,120,164,185,209 ,0 };
static u8 bpe_relations_49[] = { 3,46,48,50,109,127,177,178,209 ,0 };
static u8 bpe_relations_50[] = { 51,52,53,54,60,63,101,104,108,112,115,117,123,149,173,202 ,0 };
static u8 bpe_relations_51[] = { 3,9,18,35,100,103,105,111,125,138,183 ,0 };
static u8 bpe_relations_52[] = { 51,52,53,54,55,60,63,101,104,108,110,112,115,117,121,123,139,153,154,155,157,167,168,174,198 ,0 };
static u8 bpe_relations_53[] = { 52,53,54,63,101,104,108,110,112,149,154,158,173,190,197,201 ,0 };
static u8 bpe_relations_54[] = { 51,52,53,54,63,101,104,108,110,115,117,121,140,146,153,167,170,179,198,202 ,0 };
static u8 bpe_relations_55[] = { 57,69,106,150,186,187 ,0 };
static u8 bpe_relations_56[] = { 52,54,101,104,108,121,158 ,0 };
static u8 bpe_relations_57[] = { 57,58,59,66,69,106,134,150,152,186,187,188,199,200 ,0 };
static u8 bpe_relations_58[] = { 57,106 ,0 };
static u8 bpe_relations_59[] = { 57,58,66,106,134,152,188,199,200 ,0 };
static u8 bpe_relations_60[] = { 62 ,0 };
static u8 bpe_relations_62[] = { 52,53,54,101,104,149,155,169 ,0 };
static u8 bpe_relations_63[] = { 52,64,65,101,102,108,132 ,0 };
static u8 bpe_relations_64[] = { 64,65,102,132 ,0 };
static u8 bpe_relations_65[] = { 51,52,54,55,60,63,101,104,108,112,117,121,123,140,157,169,179 ,0 };
static u8 bpe_relations_66[] = { 68 ,0 };
static u8 bpe_relations_68[] = { 56,57,106 ,0 };
static u8 bpe_relations_69[] = { 70,118,151 ,0 };
static u8 bpe_relations_70[] = { 70,71,118,151 ,0 };
static u8 bpe_relations_71[] = { 56,57,66,106,134 ,0 };
static u8 bpe_relations_72[] = { 73 ,0 };
static u8 bpe_relations_73[] = { 79,205 ,0 };
static u8 bpe_relations_74[] = { 35 ,0 };
static u8 bpe_relations_79[] = { 74,79 ,0 };
static u8 bpe_relations_80[] = { 3,17,18,29,38,39,100,105,161 ,0 };
static u8 bpe_relations_82[] = { 3,28,100,194 ,0 };
static u8 bpe_relations_83[] = { 10,119,210 ,0 };
static u8 bpe_relations_84[] = { 39,204 ,0 };
static u8 bpe_relations_100[] = { 2,3,7,8,10,11,15,17,18,19,21,33,34,35,36,38,39,40,41,46,48,49,50,100,105,119,125,126,127,130,136,138,142,144,145,148,161,163,171,183 ,0 };
static u8 bpe_relations_101[] = { 51,52,53,54,55,60,63,101,104,108,110,112,115,117,121,123,139,153,154,155,167,168,174 ,0 };
static u8 bpe_relations_102[] = { 64,65,102,132 ,0 };
static u8 bpe_relations_103[] = { 7,8,10,18,23,33,35,38,46,49,50,105,119,127,133,171,177,178,189,210 ,0 };
static u8 bpe_relations_104[] = { 51,52,53,54,101,104,108,115,117,140,146,167,170,179 ,0 };
static u8 bpe_relations_105[] = { 17,19,28,34,35,36,38,41,49,50,126,128,135,163,195 ,0 };
static u8 bpe_relations_106[] = { 57,58,59,66,106,134,150,152,186,187,188,199,200 ,0 };
static u8 bpe_relations_107[] = { 3,10,18,19,82,100,103,113,128,131,172,192,193 ,0 };
static u8 bpe_relations_108[] = { 52,53,54,63,101,108,112,190 ,0 };
static u8 bpe_relations_109[] = { 3,33,34,35,37,100,113,124,135,172 ,0 };
static u8 bpe_relations_110[] = { 64,65,102,132 ,0 };
static u8 bpe_relations_111[] = { 3,17,18,29,38,39,100,105,161 ,0 };
static u8 bpe_relations_112[] = { 51,52,54,55,101,104,108,157,179 ,0 };
static u8 bpe_relations_113[] = { 3,15,18,34,46,100,125,126,142 ,0 };
static u8 bpe_relations_114[] = { 3,10,19,100,103,128 ,0 };
static u8 bpe_relations_115[] = { 9,11,18,50,84,111,125,180 ,0 };
static u8 bpe_relations_116[] = { 16,24,116,181,182 ,0 };
static u8 bpe_relations_117[] = { 52,53,63,101,104,108,110,112,154,158,190,197,201 ,0 };
static u8 bpe_relations_118[] = { 70,71,118,151 ,0 };
static u8 bpe_relations_119[] = { 10,28,34,37,130,206,207,208 ,0 };
static u8 bpe_relations_120[] = { 12,20,107,114,137,147 ,0 };
static u8 bpe_relations_121[] = { 52,54,101,117,140,146,170 ,0 };
static u8 bpe_relations_122[] = { 3,10,18,19,33,100,103,119,128,131,159,160,176 ,0 };
static u8 bpe_relations_123[] = { 52,53,54,101,104,149,155,169 ,0 };
static u8 bpe_relations_124[] = { 3,17,18,100,103,120,191 ,0 };
static u8 bpe_relations_125[] = { 51,52,53,63,104,112,115,117,173 ,0 };
static u8 bpe_relations_126[] = { 17,19,34,50,126 ,0 };
static u8 bpe_relations_127[] = { 52,63,104,110,112,117,154 ,0 };
static u8 bpe_relations_128[] = { 3,18,21,105,136,192,193 ,0 };
static u8 bpe_relations_129[] = { 15,18,21,39,136,144 ,0 };
static u8 bpe_relations_130[] = { 18,23,35,50,133,138,175 ,0 };
static u8 bpe_relations_131[] = { 18,36,48,49,105,148 ,0 };
static u8 bpe_relations_132[] = { 52,101,117,121,140 ,0 };
static u8 bpe_relations_133[] = { 16,24,116,181,182 ,0 };
static u8 bpe_relations_134[] = { 56,57,106 ,0 };
static u8 bpe_relations_135[] = { 10,11,34,35,119 ,0 };
static u8 bpe_relations_136[] = { 3,23,100,122 ,0 };
static u8 bpe_relations_137[] = { 3,10,19,100,103,128 ,0 };
static u8 bpe_relations_138[] = { 32,42,166 ,0 };
static u8 bpe_relations_139[] = { 64,65,102 ,0 };
static u8 bpe_relations_140[] = { 51,115,174 ,0 };
static u8 bpe_relations_141[] = { 3,100,113,142 ,0 };
static u8 bpe_relations_142[] = { 19,23,122,133,143 ,0 };
static u8 bpe_relations_143[] = { 3,100,113,172,192,193 ,0 };
static u8 bpe_relations_144[] = { 34,40,48,130 ,0 };
static u8 bpe_relations_145[] = { 9,23,48,111,122,145 ,0 };
static u8 bpe_relations_146[] = { 51,55,63,153,157 ,0 };
static u8 bpe_relations_147[] = { 18,21,105,131 ,0 };
static u8 bpe_relations_148[] = { 23,38,49,148 ,0 };
static u8 bpe_relations_149[] = { 51,52,60,101,108,115,123,167,174 ,0 };
static u8 bpe_relations_150[] = { 70,118,151 ,0 };
static u8 bpe_relations_151[] = { 56,57,66,106,134 ,0 };
static u8 bpe_relations_152[] = { 52,101,104,121 ,0 };
static u8 bpe_relations_153[] = { 7,35,39,163 ,0 };
static u8 bpe_relations_154[] = { 52,101,117,121 ,0 };
static u8 bpe_relations_155[] = { 52,104 ,0 };
static u8 bpe_relations_156[] = { 34,35,126,135 ,0 };
static u8 bpe_relations_157[] = { 57,59,106,150,186,187,199,200 ,0 };
static u8 bpe_relations_158[] = { 52,53 ,0 };
static u8 bpe_relations_159[] = { 23,122 ,0 };
static u8 bpe_relations_160[] = { 19,33,128,159,160 ,0 };
static u8 bpe_relations_161[] = { 18,38,40 ,0 };
static u8 bpe_relations_162[] = { 3,10,18,105,124,189 ,0 };
static u8 bpe_relations_163[] = { 15,21,39,136 ,0 };
static u8 bpe_relations_164[] = { 3,9,111 ,0 };
static u8 bpe_relations_165[] = { 3,100,103 ,0 };
static u8 bpe_relations_166[] = { 3,100,113,172 ,0 };
static u8 bpe_relations_167[] = { 55,63,110,139,168 ,0 };
static u8 bpe_relations_168[] = { 52,60,101,123,169 ,0 };
static u8 bpe_relations_169[] = { 52,54,101,140,170 ,0 };
static u8 bpe_relations_170[] = { 3,9,100,103,111 ,0 };
static u8 bpe_relations_171[] = { 3,100,113,142 ,0 };
static u8 bpe_relations_172[] = { 3,100,103,176 ,0 };
static u8 bpe_relations_173[] = { 51,63,153 ,0 };
static u8 bpe_relations_174[] = { 3,100 ,0 };
static u8 bpe_relations_175[] = { 18,19,105,143 ,0 };
static u8 bpe_relations_176[] = { 46,50,127,177,178 ,0 };
static u8 bpe_relations_177[] = { 52 ,0 };
static u8 bpe_relations_178[] = { 60,63,112,123 ,0 };
static u8 bpe_relations_179[] = { 11,18,180 ,0 };
static u8 bpe_relations_180[] = { 7,46 ,0 };
static u8 bpe_relations_181[] = { 19 ,0 };
static u8 bpe_relations_182[] = { 12,18,114,147 ,0 };
static u8 bpe_relations_183[] = { 17,35,184 ,0 };
static u8 bpe_relations_184[] = { 12,20,114,147 ,0 };
static u8 bpe_relations_185[] = { 18,82,131 ,0 };
static u8 bpe_relations_186[] = { 70,118 ,0 };
static u8 bpe_relations_187[] = { 70,71,151 ,0 };
static u8 bpe_relations_188[] = { 52,101 ,0 };
static u8 bpe_relations_189[] = { 8,49 ,0 };
static u8 bpe_relations_190[] = { 51,52,104,155 ,0 };
static u8 bpe_relations_191[] = { 18,125 ,0 };
static u8 bpe_relations_192[] = { 41 ,0 };
static u8 bpe_relations_193[] = { 3,42 ,0 };
static u8 bpe_relations_194[] = { 10,19 ,0 };
static u8 bpe_relations_195[] = { 34,35,135 ,0 };
static u8 bpe_relations_196[] = { 23,38,133 ,0 };
static u8 bpe_relations_197[] = { 52,54,146 ,0 };
static u8 bpe_relations_198[] = { 60,63,112,123 ,0 };
static u8 bpe_relations_199[] = { 57,106 ,0 };
static u8 bpe_relations_200[] = { 57,59 ,0 };
static u8 bpe_relations_201[] = { 55,63,110,139,157 ,0 };
static u8 bpe_relations_202[] = { 52,63,112,117 ,0 };
static u8 bpe_relations_203[] = { 19,128,159,160 ,0 };
static u8 bpe_relations_204[] = { 9,28,111,164,211 ,0 };
static u8 bpe_relations_205[] = { 79 ,0 };
static u8 bpe_relations_206[] = { 25 ,0 };
static u8 bpe_relations_207[] = { 26,162 ,0 };
static u8 bpe_relations_208[] = { 3,10,124,189 ,0 };
static u8 bpe_relations_209[] = { 17,18 ,0 };
static u8 bpe_relations_210[] = { 17,48,161 ,0 };
static u8 bpe_relations_211[] = { 35 ,0 };

static const u8* bpe_relations[] = {
    NULL,
    bpe_relations_1,
    bpe_relations_2,
    bpe_relations_3,
    NULL,
    NULL,
    NULL,
    bpe_relations_7,
    bpe_relations_8,
    bpe_relations_9,
    bpe_relations_10,
    bpe_relations_11,
    bpe_relations_12,
    bpe_relations_13,
    NULL,
    bpe_relations_15,
    bpe_relations_16,
    bpe_relations_17,
    bpe_relations_18,
    bpe_relations_19,
    bpe_relations_20,
    bpe_relations_21,
    bpe_relations_22,
    bpe_relations_23,
    bpe_relations_24,
    bpe_relations_25,
    bpe_relations_26,
    NULL,
    bpe_relations_28,
    bpe_relations_29,
    NULL,
    NULL,
    bpe_relations_32,
    bpe_relations_33,
    bpe_relations_34,
    bpe_relations_35,
    bpe_relations_36,
    bpe_relations_37,
    bpe_relations_38,
    bpe_relations_39,
    bpe_relations_40,
    bpe_relations_41,
    bpe_relations_42,
    NULL,
    NULL,
    NULL,
    bpe_relations_46,
    bpe_relations_47,
    bpe_relations_48,
    bpe_relations_49,
    bpe_relations_50,
    bpe_relations_51,
    bpe_relations_52,
    bpe_relations_53,
    bpe_relations_54,
    bpe_relations_55,
    bpe_relations_56,
    bpe_relations_57,
    bpe_relations_58,
    bpe_relations_59,
    bpe_relations_60,
    NULL,
    bpe_relations_62,
    bpe_relations_63,
    bpe_relations_64,
    bpe_relations_65,
    bpe_relations_66,
    NULL,
    bpe_relations_68,
    bpe_relations_69,
    bpe_relations_70,
    bpe_relations_71,
    bpe_relations_72,
    bpe_relations_73,
    bpe_relations_74,
    NULL,
    NULL,
    NULL,
    NULL,
    bpe_relations_79,
    bpe_relations_80,
    NULL,
    bpe_relations_82,
    bpe_relations_83,
    bpe_relations_84,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    bpe_relations_100,
    bpe_relations_101,
    bpe_relations_102,
    bpe_relations_103,
    bpe_relations_104,
    bpe_relations_105,
    bpe_relations_106,
    bpe_relations_107,
    bpe_relations_108,
    bpe_relations_109,
    bpe_relations_110,
    bpe_relations_111,
    bpe_relations_112,
    bpe_relations_113,
    bpe_relations_114,
    bpe_relations_115,
    bpe_relations_116,
    bpe_relations_117,
    bpe_relations_118,
    bpe_relations_119,
    bpe_relations_120,
    bpe_relations_121,
    bpe_relations_122,
    bpe_relations_123,
    bpe_relations_124,
    bpe_relations_125,
    bpe_relations_126,
    bpe_relations_127,
    bpe_relations_128,
    bpe_relations_129,
    bpe_relations_130,
    bpe_relations_131,
    bpe_relations_132,
    bpe_relations_133,
    bpe_relations_134,
    bpe_relations_135,
    bpe_relations_136,
    bpe_relations_137,
    bpe_relations_138,
    bpe_relations_139,
    bpe_relations_140,
    bpe_relations_141,
    bpe_relations_142,
    bpe_relations_143,
    bpe_relations_144,
    bpe_relations_145,
    bpe_relations_146,
    bpe_relations_147,
    bpe_relations_148,
    bpe_relations_149,
    bpe_relations_150,
    bpe_relations_151,
    bpe_relations_152,
    bpe_relations_153,
    bpe_relations_154,
    bpe_relations_155,
    bpe_relations_156,
    bpe_relations_157,
    bpe_relations_158,
    bpe_relations_159,
    bpe_relations_160,
    bpe_relations_161,
    bpe_relations_162,
    bpe_relations_163,
    bpe_relations_164,
    bpe_relations_165,
    bpe_relations_166,
    bpe_relations_167,
    bpe_relations_168,
    bpe_relations_169,
    bpe_relations_170,
    bpe_relations_171,
    bpe_relations_172,
    bpe_relations_173,
    bpe_relations_174,
    bpe_relations_175,
    bpe_relations_176,
    bpe_relations_177,
    bpe_relations_178,
    bpe_relations_179,
    bpe_relations_180,
    bpe_relations_181,
    bpe_relations_182,
    bpe_relations_183,
    bpe_relations_184,
    bpe_relations_185,
    bpe_relations_186,
    bpe_relations_187,
    bpe_relations_188,
    bpe_relations_189,
    bpe_relations_190,
    bpe_relations_191,
    bpe_relations_192,
    bpe_relations_193,
    bpe_relations_194,
    bpe_relations_195,
    bpe_relations_196,
    bpe_relations_197,
    bpe_relations_198,
    bpe_relations_199,
    bpe_relations_200,
    bpe_relations_201,
    bpe_relations_202,
    bpe_relations_203,
    bpe_relations_204,
    bpe_relations_205,
    bpe_relations_206,
    bpe_relations_207,
    bpe_relations_208,
    bpe_relations_209,
    bpe_relations_210,
    bpe_relations_211,
    NULL,
    NULL,
    NULL,
};

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
    int amountGenerated = 0;
    u8 lastGenerated = 3;
    while (amountGenerated < 230)
    {
        u8 next = bpe_pick(bpe_relations[lastGenerated]);
        lastGenerated = next;
        if (lastGenerated < 100)
        {
            uRNGScratch[amountGenerated] = lastGenerated;
            amountGenerated++;
        }
        else
        {
            const u8* desc = bpe_pairs[lastGenerated - 100];
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

    while (maxProgress + 7 > sAmountGenerated)
    {
        struct SpawnResult result = spawn_track(sAmountGenerated, uRNGScratch + sAmountGenerated, 1);
        sAmountGenerated++;
        sLastPart->oPartNext = result.firstPart;
        result.firstPart->oPartPrev = sLastPart;
        sLastPart = result.lastPart;
    }
}
