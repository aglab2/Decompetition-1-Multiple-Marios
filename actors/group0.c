#include <ultra64.h>
#include "sm64.h"
#include "surface_terrains.h"
#include "geo_commands.h"

#include "make_const_nonconst.h"

// Note: This bin does not use bin IDs, unlike the other segmented bins.
#define	G_CC_AGLAB                   SHADE, 0, PRIMITIVE, 0, SHADE, 0, PRIMITIVE, 0
#include "mario/model_opt.inc.c"

#include "bubble/model.inc.c"

#include "walk_smoke/model.inc.c"

#include "burn_smoke/model.inc.c"

#include "small_water_splash/model.inc.c"

#include "water_wave/model.inc.c"

#include "sparkle/model.inc.c"

#include "water_splash/model.inc.c"

#include "white_particle_small/model.inc.c"

#include "sparkle_animation/model.inc.c"

#ifdef S2DEX_TEXT_ENGINE
#include "src/s2d_engine/s2d_config.h"
#include FONT_C_FILE
#endif

// #include "exampleSeesaw/collision.inc.c"
// #include "exampleSeesaw/model.inc.c"
// #include "raisePlatform/collision.inc.c"
// #include "raisePlatform/model.inc.c"
#include "doubleCherry/model.inc.c"
#include "mario_real/model.inc.c"