
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

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
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
#pragma GCC diagnostic pop
