#ifndef MEMORY_H
#define MEMORY_H

#include <PR/ultratypes.h>

#include "types.h"

#define NUM_TLB_SEGMENTS 32

struct OffsetSizePair {
    u32 offset;
    u32 size;
};

struct DmaTable {
    u32 count;
    u8 *srcAddr;
    struct OffsetSizePair anim[1]; // dynamic size
};

struct DmaHandlerList {
    struct DmaTable *dmaTable;
    void *currentAddr;
    void *bufTarget;
};

struct MainPoolRegion {
    u8 *start;
    u8 *end;
};

struct MainPoolContext {
    struct MainPoolRegion regions[2];
};

extern struct MainPoolContext sMainPool;
#define gMainPoolCurrentRegion (&sMainPool.regions[0])
#define gMainPoolLowPrioRegion (&sMainPool.regions[1])

#define MAIN_POOL_ALIGNMENT_DISABLE -1

// takes the first 'size' bytes from 'region'
static ALWAYS_INLINE void* main_pool_region_alloc_from_start(int regionIdx, u32 size, s32 alignment) {
    struct MainPoolRegion *region = &sMainPool.regions[regionIdx];
    u8* ret = alignment < 0 ? region->start : (u8*) ALIGN(region->start, alignment);
    u8* newStart = ret + size;
    region->start = newStart;
    if (!ret) __builtin_unreachable();
    return ret;
}

static ALWAYS_INLINE void *main_pool_alloc(u32 size) {
    void* buf = main_pool_region_alloc_from_start(0, ALIGN4(size), MAIN_POOL_ALIGNMENT_DISABLE);
    if (!buf) __builtin_unreachable();
    return buf;
}

static ALWAYS_INLINE void *main_pool_alloc_lowprio(u32 size) {
    void* buf = main_pool_region_alloc_from_start(1, ALIGN4(size), MAIN_POOL_ALIGNMENT_DISABLE);
    if (!buf) __builtin_unreachable();
    return buf;
}

static ALWAYS_INLINE void* main_pool_alloc_from_end(u32 size) {
    u8* region_end = gMainPoolCurrentRegion->end;
    u8* new_end = region_end - size;
    gMainPoolCurrentRegion->end -= size;
    return new_end;
}

#define EFFECTS_MEMORY_POOL 0x4000

extern struct MemoryPool *gEffectsMemoryPool;

uintptr_t set_segment_base_addr(s32 segment, void *addr);
void *get_segment_base_addr(s32 segment);
void *segmented_to_virtual(const void *addr);
void *virtual_to_segmented(u32 segment, const void *addr);
void move_segment_table_to_dmem(void);

void main_pool_init(void *start, void *end);
u32 main_pool_available(void);
void main_pool_push_state(void);
void main_pool_pop_state(void);

#ifndef NO_SEGMENTED_MEMORY
void *load_segment(s32 segment, u8 *srcStart, u8 *srcEnd, u8 *bssStart, u8 *bssEnd);
void *load_to_fixed_pool_addr(u8 *destAddr, u8 *srcStart, u8 *srcEnd);
#else
#define load_segment(...)
#define load_to_fixed_pool_addr(...)
#endif

struct MemoryPool *mem_pool_init(u32 size);
void *mem_pool_alloc(struct MemoryPool *pool, u32 size);
void mem_pool_free(struct MemoryPool *pool, void *addr);

extern u8 *gGfxPoolEnd;
#define alloc_display_list(_size) ({\
    u32 size = ALIGN16(_size); \
    void* ptr = gGfxPoolEnd - size; \
    if (__builtin_constant_p(_size)) { \
        switch (size) { \
            case 0x0: \
                break; \
            case 0x10: \
                __builtin_mips_cache(0xd, ptr + 0x0); \
                break; \
            case 0x20: \
                __builtin_mips_cache(0xd, ptr + 0x0); \
                __builtin_mips_cache(0xd, ptr + 0x10); \
                break; \
            case 0x30: \
                __builtin_mips_cache(0xd, ptr + 0x0); \
                __builtin_mips_cache(0xd, ptr + 0x10); \
                __builtin_mips_cache(0xd, ptr + 0x20); \
                break; \
            case 0x40: \
                __builtin_mips_cache(0xd, ptr + 0x0); \
                __builtin_mips_cache(0xd, ptr + 0x10); \
                __builtin_mips_cache(0xd, ptr + 0x20); \
                __builtin_mips_cache(0xd, ptr + 0x30); \
                break; \
            case 0x50: \
                __builtin_mips_cache(0xd, ptr + 0x0); \
                __builtin_mips_cache(0xd, ptr + 0x10); \
                __builtin_mips_cache(0xd, ptr + 0x20); \
                __builtin_mips_cache(0xd, ptr + 0x30); \
                __builtin_mips_cache(0xd, ptr + 0x40); \
                break; \
            case 0x60: \
                __builtin_mips_cache(0xd, ptr + 0x0); \
                __builtin_mips_cache(0xd, ptr + 0x10); \
                __builtin_mips_cache(0xd, ptr + 0x20); \
                __builtin_mips_cache(0xd, ptr + 0x30); \
                __builtin_mips_cache(0xd, ptr + 0x40); \
                __builtin_mips_cache(0xd, ptr + 0x50); \
                break; \
            case 0x70: \
                __builtin_mips_cache(0xd, ptr + 0x0); \
                __builtin_mips_cache(0xd, ptr + 0x10); \
                __builtin_mips_cache(0xd, ptr + 0x20); \
                __builtin_mips_cache(0xd, ptr + 0x30); \
                __builtin_mips_cache(0xd, ptr + 0x40); \
                __builtin_mips_cache(0xd, ptr + 0x50); \
                __builtin_mips_cache(0xd, ptr + 0x60); \
                break; \
            case 0x80: \
                __builtin_mips_cache(0xd, ptr + 0x0); \
                __builtin_mips_cache(0xd, ptr + 0x10); \
                __builtin_mips_cache(0xd, ptr + 0x20); \
                __builtin_mips_cache(0xd, ptr + 0x30); \
                __builtin_mips_cache(0xd, ptr + 0x40); \
                __builtin_mips_cache(0xd, ptr + 0x50); \
                __builtin_mips_cache(0xd, ptr + 0x60); \
                __builtin_mips_cache(0xd, ptr + 0x70); \
                break; \
            default: \
                break; \
        } \
    } \
    gGfxPoolEnd = ptr; \
    if (!ptr) __builtin_unreachable(); \
    if (0 != (((uintptr_t) ptr) & 0xf)) __builtin_unreachable(); \
    ptr; \
})

#define main_pool_alloc_aligned_cde(_size) ({ \
    u32 size = ALIGN16(_size); \
    u8* ptr = gMainPoolCurrentRegion->start; \
    if (__builtin_constant_p(_size)) { \
        switch (size) { \
            case 0x0: \
                break; \
            case 0x10: \
                __builtin_mips_cache(0xd, ptr + 0x0); \
                break; \
            case 0x20: \
                __builtin_mips_cache(0xd, ptr + 0x0); \
                __builtin_mips_cache(0xd, ptr + 0x10); \
                break; \
            case 0x30: \
                __builtin_mips_cache(0xd, ptr + 0x0); \
                __builtin_mips_cache(0xd, ptr + 0x10); \
                __builtin_mips_cache(0xd, ptr + 0x20); \
                break; \
            case 0x40: \
                __builtin_mips_cache(0xd, ptr + 0x0); \
                __builtin_mips_cache(0xd, ptr + 0x10); \
                __builtin_mips_cache(0xd, ptr + 0x20); \
                __builtin_mips_cache(0xd, ptr + 0x30); \
                break; \
            case 0x50: \
                __builtin_mips_cache(0xd, ptr + 0x0); \
                __builtin_mips_cache(0xd, ptr + 0x10); \
                __builtin_mips_cache(0xd, ptr + 0x20); \
                __builtin_mips_cache(0xd, ptr + 0x30); \
                __builtin_mips_cache(0xd, ptr + 0x40); \
                break; \
            case 0x60: \
                __builtin_mips_cache(0xd, ptr + 0x0); \
                __builtin_mips_cache(0xd, ptr + 0x10); \
                __builtin_mips_cache(0xd, ptr + 0x20); \
                __builtin_mips_cache(0xd, ptr + 0x30); \
                __builtin_mips_cache(0xd, ptr + 0x40); \
                __builtin_mips_cache(0xd, ptr + 0x50); \
                break; \
            case 0x70: \
                __builtin_mips_cache(0xd, ptr + 0x0); \
                __builtin_mips_cache(0xd, ptr + 0x10); \
                __builtin_mips_cache(0xd, ptr + 0x20); \
                __builtin_mips_cache(0xd, ptr + 0x30); \
                __builtin_mips_cache(0xd, ptr + 0x40); \
                __builtin_mips_cache(0xd, ptr + 0x50); \
                __builtin_mips_cache(0xd, ptr + 0x60); \
                break; \
            case 0x80: \
                __builtin_mips_cache(0xd, ptr + 0x0); \
                __builtin_mips_cache(0xd, ptr + 0x10); \
                __builtin_mips_cache(0xd, ptr + 0x20); \
                __builtin_mips_cache(0xd, ptr + 0x30); \
                __builtin_mips_cache(0xd, ptr + 0x40); \
                __builtin_mips_cache(0xd, ptr + 0x50); \
                __builtin_mips_cache(0xd, ptr + 0x60); \
                __builtin_mips_cache(0xd, ptr + 0x70); \
                break; \
            default: \
                break; \
        } \
    } \
    u8* newStart = ptr + size; \
    gMainPoolCurrentRegion->start = newStart; \
    if (!ptr) __builtin_unreachable(); \
    ptr; \
})

void setup_dma_table_list(struct DmaHandlerList *list, void *srcAddr, void *buffer);
s32 load_patchable_table(struct DmaHandlerList *list, s32 index);


extern uintptr_t sSegmentROMTable[32];
#endif // MEMORY_H
