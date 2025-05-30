#include <PR/ultratypes.h>

#include "sm64.h"

#include "buffers/buffers.h"
#include "slidec.h"
#include "game/game_init.h"
#include "game/main.h"
#include "game/memory.h"
#include "segment_symbols.h"
#include "segments.h"
#ifdef GZIP
#include <gzip.h>
#endif
#if defined(RNC1) || defined(RNC2)
#include <rnc.h>
#endif
#ifdef UNF
#include "usb/usb.h"
#include "usb/debug.h"
#endif
#include "game/puppyprint.h"

#define	DOWN(s, align)	(((u32)(s)) & ~((align)-1))
#define DOWN4(s) DOWN(s, 4)

struct MainPoolState {
    struct MainPoolContext ctx;
    struct MainPoolState* prev;
};

struct MemoryBlock {
    struct MemoryBlock *next;
    u32 size;
};

struct MemoryPool {
    u32 totalSpace;
    struct MemoryBlock *firstBlock;
    struct MemoryBlock freeList;
};

extern uintptr_t sSegmentTable[32];


/**
 * Memory pool for small graphical effects that aren't connected to Objects.
 * Used for colored text, paintings, and environmental snow and bubbles.
 */
struct MemoryPool *gEffectsMemoryPool;



uintptr_t sSegmentTable[32];
uintptr_t sSegmentROMTable[32];
struct MainPoolContext sMainPool;

static struct MainPoolState *gMainPoolState = NULL;

uintptr_t set_segment_base_addr(s32 segment, void *addr) {
    sSegmentTable[segment] = ((uintptr_t) addr & 0x1FFFFFFF);
    return sSegmentTable[segment];
}

UNUSED void *get_segment_base_addr(s32 segment) {
    return (void *) (sSegmentTable[segment] | 0x80000000);
}

#ifndef NO_SEGMENTED_MEMORY
void *segmented_to_virtual(const void *addr) {
    size_t segment = ((uintptr_t) addr >> 24);
    size_t offset  = ((uintptr_t) addr & 0x00FFFFFF);

    return (void *) ((sSegmentTable[segment] + offset) | 0x80000000);
}

void *virtual_to_segmented(u32 segment, const void *addr) {
    size_t offset = ((uintptr_t) addr & 0x1FFFFFFF) - sSegmentTable[segment];

    return (void *) ((segment << 24) + offset);
}

void move_segment_table_to_dmem(void) {
    Gfx *tempGfxHead = gDisplayListHead;

    for (s32 i = 0; i < 16; i++) {
        gSPSegment(tempGfxHead++, i, sSegmentTable[i]);
    }

    gDisplayListHead = tempGfxHead;
}
#else
void *segmented_to_virtual(const void *addr) {
    return (void *) addr;
}

void *virtual_to_segmented(u32 segment, const void *addr) {
    return (void *) addr;
}

void move_segment_table_to_dmem(void) {
}
#endif

/**
 * Initialize the main memory pool. This pool is conceptually a pair of stacks
 * that grow inward from the left and right. It therefore only supports
 * freeing the object that was most recently allocated from a side.
 */
void main_pool_init(void *start, void *end) {
    sMainPool.start = (u8 *) ALIGN16((uintptr_t) start) + 16;
    sMainPool.end = (u8 *) ALIGN16((uintptr_t) end) - 16;
}

/**
 * Return the size of the largest block that can currently be allocated from the
 * pool.
 */
u32 main_pool_available(void) {
    return sMainPool.end - sMainPool.start;
}

/**
 * Push pool state, to be restored later. Return the amount of free space left
 * in the pool.
 */
void main_pool_push_state(void) {
    struct MainPoolState *prevState = gMainPoolState;
    struct MainPoolContext ctx = sMainPool;

    gMainPoolState = main_pool_alloc(sizeof(*gMainPoolState));
    gMainPoolState->ctx = ctx;
    gMainPoolState->prev = prevState;
}

/**
 * Restore pool state from a previous call to main_pool_push_state. Return the
 * amount of free space left in the pool.
 */
void main_pool_pop_state(void) {
    sMainPool = gMainPoolState->ctx;
    gMainPoolState = gMainPoolState->prev;
}

/**
 * Perform a DMA read from ROM, allocating space in the memory pool to write to.
 * Return the destination address.
 */
static void *dynamic_dma_read(u8 *srcStart, u8 *srcEnd, u32 alignment, u32 bssLength) {
    u32 size = ALIGN16(srcEnd - srcStart);
    void* dest = main_pool_alloc_aligned(size + bssLength, alignment);
    if (dest != NULL) {
        dma_read(((u8 *)dest), srcStart, srcEnd);
        if (bssLength) {
            bzero(((u8 *)dest + size), bssLength);
        }
    }
    return dest;
}

#define TLB_PAGE_SIZE 4096 // Blocksize of TLB transfers. Larger values can be faster to transfer, but more wasteful of RAM.
s32 gTlbEntries = 0;
u8 gTlbSegments[NUM_TLB_SEGMENTS] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

void mapTLBPages(uintptr_t virtualAddress, uintptr_t physicalAddress, s32 length, s32 segment) {
    while (length > 0) {
        if (length > TLB_PAGE_SIZE) {
            osMapTLB(gTlbEntries++, OS_PM_4K, (void *)virtualAddress, physicalAddress, (physicalAddress + TLB_PAGE_SIZE), -1);
            virtualAddress  += TLB_PAGE_SIZE;
            physicalAddress += TLB_PAGE_SIZE;
            length          -= TLB_PAGE_SIZE;
            gTlbSegments[segment]++;
        } else {
            osMapTLB(gTlbEntries++, OS_PM_4K, (void *)virtualAddress, physicalAddress, -1, -1);
            gTlbSegments[segment]++;
        }
        virtualAddress  += TLB_PAGE_SIZE;
        physicalAddress += TLB_PAGE_SIZE;
        length          -= TLB_PAGE_SIZE;
    }
}

#ifndef NO_SEGMENTED_MEMORY
/**
 * Load data from ROM into a newly allocated block, and set the segment base
 * address to this block.
 */
void *load_segment(s32 segment, u8 *srcStart, u8 *srcEnd, u8 *bssStart, u8 *bssEnd) {
    void *addr;

    if ((bssStart != NULL)) {
        addr = dynamic_dma_read(srcStart, srcEnd, 0, ((uintptr_t)bssEnd - (uintptr_t)bssStart));
        if (addr != NULL) {
            set_segment_base_addr(segment, addr); sSegmentROMTable[segment] = (uintptr_t) srcStart;
        }
    } else {
        addr = dynamic_dma_read(srcStart, srcEnd, 0, 0);
        if (addr != NULL) {
            set_segment_base_addr(segment, addr); sSegmentROMTable[segment] = (uintptr_t) srcStart;
        }
    }
#ifdef PUPPYPRINT_DEBUG
    u32 ppSize = ALIGN16(srcEnd - srcStart);
    set_segment_memory_printout(segment, ppSize);
#endif
    return addr;
}

/*
 * Allocate a block of memory starting at destAddr and ending at the end of
 * the memory pool. Then copy srcStart through srcEnd from ROM to this block.
 * If this block is not large enough to hold the ROM data, or that portion
 * of the pool is already allocated, return NULL.
 */
void *load_to_fixed_pool_addr(u8 *destAddr, u8 *srcStart, u8 *srcEnd) {
    u8* dest = (u8*) DOWN(destAddr, 16);
    u32 srcSize = ALIGN16(srcEnd - srcStart);
    u32 destSize = (u8*) RAM_END - srcStart;
    if (srcSize <= destSize) {
#ifdef USE_EXT_RAM
        bzero(dest, ((u8*)0x80800000) - dest);
#else
        bzero(dest, ((u8*)0x80400000) - dest);
#endif
        osWritebackDCacheAll();
        dma_read(dest, srcStart, srcEnd);
        osInvalICache(dest, destSize);
        osInvalDCache(dest, destSize);
    } else {
        DEBUG_ASSERT("Fixed addr is not be loaded");
    }
    return dest;
}

struct MainPoolFreeableHeader {
    // either a pointer to 'start' or 'end'
    u8* ptr;
    u8 data[0];
};

static ALWAYS_INLINE void* main_pool_region_try_alloc_from_end_freeable(u32 size, int alignment) {
    u8* region_end = sMainPool.end;
    u8* new_end;
    if (alignment > 0)
        new_end = (u8*) DOWN(region_end - size, alignment) - sizeof(struct MainPoolFreeableHeader);
    else
        new_end = region_end - size - sizeof(struct MainPoolFreeableHeader);

    if (new_end < sMainPool.start)
        return NULL;

    size = region_end - new_end;

    struct MainPoolFreeableHeader* header = (struct MainPoolFreeableHeader*) new_end;
    header->ptr = region_end;

    sMainPool.end -= size;

    return header->data;
}

static void main_pool_free(void *addr) {
    const struct MainPoolFreeableHeader* header = (struct MainPoolFreeableHeader*) ((u8*) addr - sizeof(struct MainPoolFreeableHeader));
    struct MainPoolContext* region = &sMainPool;
    region->end = header->ptr;
}

/**
 * Decompress the block of ROM data from srcStart to srcEnd and return a
 * pointer to an allocated buffer holding the decompressed data. Set the
 * base address of segment to this address.
 */
void *load_segment_decompress(s32 segment, u8 *srcStart, u8 *srcEnd) {
    void *dest = NULL;

#ifdef GZIP
    u32 compSize = (srcEnd - 4 - srcStart);
#else
    u32 compSize = ALIGN16(srcEnd - srcStart);
#endif
    u8 *compressed = main_pool_region_try_alloc_from_end_freeable(compSize, 16);
#ifdef GZIP
    // Decompressed size from end of gzip
    u32 *size = (u32 *) (compressed + compSize);
#else
    // Decompressed size from header (This works for non-mio0 because they also have the size in same place)
    u32 *size = (u32 *) (compressed + 4);
#endif
    if (compressed != NULL) {
#ifdef UNCOMPRESSED
        dest = main_pool_alloc(compSize);
        dma_read(dest, srcStart, srcEnd);
#else
        dma_read(compressed, srcStart, srcEnd);
        dest = main_pool_alloc_aligned(*size, 0);
#endif
        if (dest != NULL) {
            osSyncPrintf("start decompress\n");
#ifdef GZIP
            expand_gzip(compressed, dest, compSize, (u32)size);
#elif RNC1
            Propack_UnpackM1(compressed, dest);
#elif RNC2
            Propack_UnpackM2(compressed, dest);
#elif YAY0
            slidstart(compressed, dest);
#elif MIO0
            decompress(compressed, dest);
#endif
            osSyncPrintf("end decompress\n");
            set_segment_base_addr(segment, dest); sSegmentROMTable[segment] = (uintptr_t) srcStart;
            main_pool_free(compressed);
        }
    }
#ifdef PUPPYPRINT_DEBUG
    u32 ppSize = ALIGN16((u32)*size) + 16;
    set_segment_memory_printout(segment, ppSize);
#endif
    return dest;
}
#endif

/**
 * Allocate a memory pool from the main pool. This pool supports arbitrary
 * order for allocation/freeing.
 * Return NULL if there is not enough space in the main pool.
 */
struct MemoryPool *mem_pool_init(u32 size) {
    void *addr;
    struct MemoryBlock *block;
    struct MemoryPool *pool = NULL;

    size = ALIGN4(size);
    addr = main_pool_alloc(size + sizeof(struct MemoryPool));
    if (addr != NULL) {
        pool = (struct MemoryPool *) addr;

        pool->totalSpace = size;
        pool->firstBlock = (struct MemoryBlock *) ((u8 *) addr + sizeof(struct MemoryPool));
        pool->freeList.next = (struct MemoryBlock *) ((u8 *) addr + sizeof(struct MemoryPool));

        block = pool->firstBlock;
        block->next = NULL;
        block->size = pool->totalSpace;
    }
#ifdef PUPPYPRINT_DEBUG
    gPoolMem += ALIGN16(size) + 16;
#endif
    return pool;
}

/**
 * Allocate from a memory pool. Return NULL if there is not enough space.
 */
void *mem_pool_alloc(struct MemoryPool *pool, u32 size) {
    struct MemoryBlock *freeBlock = &pool->freeList;
    void *addr = NULL;

    size = ALIGN4(size) + sizeof(struct MemoryBlock);
    while (freeBlock->next != NULL) {
        if (freeBlock->next->size >= size) {
            addr = (u8 *) freeBlock->next + sizeof(struct MemoryBlock);
            if (freeBlock->next->size - size <= sizeof(struct MemoryBlock)) {
                freeBlock->next = freeBlock->next->next;
            } else {
                struct MemoryBlock *newBlock = (struct MemoryBlock *) ((u8 *) freeBlock->next + size);
                newBlock->size = freeBlock->next->size - size;
                newBlock->next = freeBlock->next->next;
                freeBlock->next->size = size;
                freeBlock->next = newBlock;
            }
            break;
        }
        freeBlock = freeBlock->next;
    }
    return addr;
}

/**
 * Free a block that was allocated using mem_pool_alloc.
 */
void mem_pool_free(struct MemoryPool *pool, void *addr) {
    struct MemoryBlock *block = (struct MemoryBlock *) ((u8 *) addr - sizeof(struct MemoryBlock));
    struct MemoryBlock *freeList = pool->freeList.next;

    if (pool->freeList.next == NULL) {
        pool->freeList.next = block;
        block->next = NULL;
    } else {
        if (block < pool->freeList.next) {
            if ((u8 *) pool->freeList.next == (u8 *) block + block->size) {
                block->size += freeList->size;
                block->next = freeList->next;
                pool->freeList.next = block;
            } else {
                block->next = pool->freeList.next;
                pool->freeList.next = block;
            }
        } else {
            while (freeList->next != NULL) {
                if (freeList < block && block < freeList->next) {
                    break;
                }
                freeList = freeList->next;
            }
            if ((u8 *) freeList + freeList->size == (u8 *) block) {
                freeList->size += block->size;
                block = freeList;
            } else {
                block->next = freeList->next;
                freeList->next = block;
            }
            if (block->next != NULL && (u8 *) block->next == (u8 *) block + block->size) {
                block->size = block->size + block->next->size;
                block->next = block->next->next;
            }
        }
    }
}

static void *main_pool_alloc_aligned_freeable(int region, u32 size, u32 alignment) {
    if (!alignment)
        alignment = 16;

    size = ALIGN4(size);
    return main_pool_region_try_alloc_from_end_freeable(size, MAIN_POOL_ALIGNMENT_DISABLE);
}

static void *dynamic_dma_read_freeable(int region, u8 *srcStart, u8 *srcEnd, u32 alignment, u32 bssLength) {
    u32 size = ALIGN16(srcEnd - srcStart);
    void* dest = main_pool_alloc_aligned_freeable(region, size + bssLength, alignment);
    if (dest != NULL) {
        dma_read(((u8 *)dest), srcStart, srcEnd);
        if (bssLength) {
            bzero(((u8 *)dest + size), bssLength);
        }
    }
    return dest;
}

static struct DmaTable *load_dma_table_address(u8 *srcAddr) {
    struct DmaTable *table = dynamic_dma_read_freeable(0, srcAddr, srcAddr + sizeof(u32), 0, 0);
    u32 size = table->count * sizeof(struct OffsetSizePair) +
        sizeof(struct DmaTable) - sizeof(struct OffsetSizePair);
    main_pool_free(table);

    table = dynamic_dma_read(srcAddr, srcAddr + size, 0, 0);
    table->srcAddr = srcAddr;
    return table;
}

void setup_dma_table_list(struct DmaHandlerList *list, void *srcAddr, void *buffer) {
    if (srcAddr != NULL) {
        list->dmaTable = load_dma_table_address(srcAddr);
    }
    list->currentAddr = NULL;
    list->bufTarget = buffer;
}

s32 load_patchable_table(struct DmaHandlerList *list, s32 index) {
    struct DmaTable *table = list->dmaTable;

    if ((u32)index < table->count) {
        u8 *addr = table->srcAddr + table->anim[index].offset;
        s32 size = table->anim[index].size;

        if (list->currentAddr != addr) {
            dma_read(list->bufTarget, addr, addr + size);
            list->currentAddr = addr;
            return TRUE;
        }
    }
    return FALSE;
}
