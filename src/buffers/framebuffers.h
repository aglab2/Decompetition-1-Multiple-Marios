#ifndef FRAMEBUFFERS_H
#define FRAMEBUFFERS_H

#include <PR/ultratypes.h>

#include "config.h"
#include "types.h"

// level_script.c assumes that the frame buffers are adjacent, while game.c's
// -g codegen implies that they are separate variables. This is impossible to
// reconcile without undefined behavior. Avoid that when possible.
extern RGBA16 gFramebuffer0[SCREEN_WIDTH * SCREEN_HEIGHT];
extern RGBA16 gFramebuffer1[SCREEN_WIDTH * SCREEN_HEIGHT];
extern RGBA16 gFramebuffer2[SCREEN_WIDTH * SCREEN_HEIGHT];

static inline RGBA16* getFramebuffer(int val)
{
    switch (val) {
        case 0:
            return gFramebuffer0;
        case 1:
            return gFramebuffer1;
        case 2:
            return gFramebuffer2;
    }
}

#endif // FRAMEBUFFERS_H
