#include <ultra64.h>
#include "sm64.h"
#include "behavior_data.h"
#include "model_ids.h"
#include "seq_ids.h"
#include "dialog_ids.h"
#include "segment_symbols.h"
#include "level_commands.h"

#include "game/level_update.h"

#include "levels/scripts.h"

#include "actors/group0.h"
#include "make_const_nonconst.h"
#include "levels/example/header.h"

/* Fast64 begin persistent block [scripts] */
/* Fast64 end persistent block [scripts] */

const LevelScript level_example_entry[] = {
	INIT_LEVEL(),
	LOAD_MIO0(0x7, _example_segment_7SegmentRomStart, _example_segment_7SegmentRomEnd), 
	LOAD_YAY0(0x7, _example_segment_7SegmentRomStart, _example_segment_7SegmentRomEnd), 
	ALLOC_LEVEL_POOL(),
	MARIO(MODEL_MARIO, 0x00000001, bhvMario), 
	LOAD_MODEL_FROM_GEO(0x20 + 1, p1_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 2, p2_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 3, p3_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 4, p4_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 5, p5_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 6, p6_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 7, p7_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 8, p8_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 9, p9_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 10, p10_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 11, p11_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 12, p12_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 13, p13_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 14, p14_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 15, p15_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 16, p16_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 17, p17_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 18, p18_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 19, p19_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 20, p20_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 21, p21_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 22, p22_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 23, p23_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 24, p24_geo),
	// LOAD_MODEL_FROM_GEO(0x20 + 25, p25_geo),
	// LOAD_MODEL_FROM_GEO(0x20 + 26, p26_geo),
	// LOAD_MODEL_FROM_GEO(0x20 + 27, p27_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 28, p28_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 29, p29_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 30, p30_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 31, p31_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 32, p32_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 33, p33_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 34, p34_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 35, p35_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 36, p36_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 37, p37_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 38, p38_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 39, p39_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 40, p40_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 41, p41_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 42, p42_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 43, p43_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 44, p44_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 45, p45_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 46, p46_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 47, p47_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 48, p48_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 49, p49_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 50, p50_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 51, p51_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 52, p52_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 53, p53_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 54, p54_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 55, p55_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 56, p56_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 57, p57_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 58, p58_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 59, p59_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 60, p60_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 61, p61_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 62, p62_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 63, p63_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 64, p64_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 65, p65_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 66, p66_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 67, p67_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 68, p68_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 69, p69_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 70, p70_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 71, p71_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 72, p72_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 73, p73_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 74, p74_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 75, p75_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 76, p76_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 77, p77_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 78, p78_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 79, p79_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 80, p80_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 81, p81_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 82, p82_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 83, p83_geo),
	LOAD_MODEL_FROM_GEO(0x20 + 84, p84_geo),
	/* Fast64 begin persistent block [level commands] */
	/* Fast64 end persistent block [level commands] */

	AREA(1, example_area_1),
		WARP_NODE(0x0A, LEVEL_CASTLE_GROUNDS, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		WARP_NODE(0xf0, LEVEL_CASTLE_GROUNDS, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		WARP_NODE(0xf1, LEVEL_CASTLE_GROUNDS, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		OBJECT(MODEL_NONE, 0, 0, 0, 0, 0, 0, 0x00000000, bhvCtl),
		OBJECT(MODEL_NONE, 0, 452, 0, 0, 0, 0, (0xa << 16), bhvInstantActiveWarp),
		TERRAIN(example_area_1_collision),
		MACRO_OBJECTS(example_area_1_macro_objs),
		SET_BACKGROUND_MUSIC(0x00, SEQ_LEVEL_GRASS),
		TERRAIN_TYPE(TERRAIN_GRASS),
		/* Fast64 begin persistent block [area commands] */
		/* Fast64 end persistent block [area commands] */
	END_AREA(),
	FREE_LEVEL_POOL(),
	MARIO_POS(1, 0, 0, 0, 0),
	CALL(0, lvl_init_or_update),
	CALL_LOOP(1, lvl_init_or_update),
	CLEAR_LEVEL(),
	SLEEP_BEFORE_EXIT(1),
	EXIT(),
};