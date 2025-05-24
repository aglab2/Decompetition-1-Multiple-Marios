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
    /* 11 */ { { 0, 0, -20000 } },
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
    /* 46 */ { { -2000, 0, -2000 }, .turn = TURN_LEFT, },
    /* 47 */ { {  2000, 0, -2000 }, .turn = TURN_RIGHT, },
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

static const u8 uExpertTrack[] = {
   1, 3, 3, 17, 3, 38, 3, 17, 34, 18, 3, 3, 3, 46, 35, 18, 3, 28, 26, 29, 18, 3, 36, 34, 3, 3, 3, 38, 39
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

struct SpawnerState
{
    Vec3f pos;
    s16 angle;
};

#define SCALE 0.5f

void bhv_ctl_init()
{
    gMarioStates->faceAngle[1] = 0x8000;

    struct SpawnerState spawner = {};
    const u8* track = uExpertTrack;
    int trackSize = sizeof(uExpertTrack);

    for (int i = 0; i < trackSize; i++)
    {
        int entry = track[i];

        Collision* partCollision;
        if (partCollision = sCollisionHeaders[entry])
        {
            struct Object* part = spawn_object(o, 0x20 + entry, bhvPart);
            part->oBehParams2ndByte = entry;

            obj_set_collision_data(part, partCollision);

            part->oFaceAngleYaw = spawner.angle;
            part->oPosX = spawner.pos[0];
            part->oPosY = spawner.pos[1];
            part->oPosZ = spawner.pos[2];
            
            obj_scale(part, SCALE);
        }

        const struct PartConfig* partConfig = &sPartConfigs[entry];

        // We need to rotate the shift around spawner.angle around (0, 0)
        f32 shiftXRotated = partConfig->shift[0] * coss(spawner.angle) 
                          + partConfig->shift[2] * sins(spawner.angle);
        f32 shiftZRotated = -partConfig->shift[0] * sins(spawner.angle)
                          + partConfig->shift[2] * coss(spawner.angle);

        spawner.pos[0] += shiftXRotated * SCALE;
        spawner.pos[1] += partConfig->shift[1] * SCALE;
        spawner.pos[2] += shiftZRotated * SCALE;
        spawner.angle += partConfig->turn;
    }
}

void bhv_ctl_loop()
{
    if (gMarioStates->floor && gMarioStates->floor->object)
    {
        struct Object* part = gMarioStates->floor->object;
        print_text_fmt_int(120, 20, "T %d", part->oBehParams2ndByte);
        print_text_fmt_int(120, 40, "A %d", part->oFaceAngleYaw);
    }

    print_text_fmt_int(20, 180, "X %d", (int) gMarioStates->pos[0]);
    print_text_fmt_int(20, 160, "Y %d", (int) gMarioStates->pos[1]);
    print_text_fmt_int(20, 140, "Z %d", (int) gMarioStates->pos[2]);
#if 0
    if (gPlayer1Controller->buttonPressed & L_JPAD)
    {
        o->oBehParams2ndByte++;
    }
    if (gPlayer1Controller->buttonPressed & R_JPAD)
    {
        o->oBehParams2ndByte--;
    }

    obj_set_model(o, o->oBehParams2ndByte);
    print_text_fmt_int(20, 20, "%d", o->oBehParams2ndByte - 0x20);
#endif
}

void bhv_part_loop()
{
    o->oDrawingDistance = 30000.0f;
}
