#ifndef __SMEM_H__
#define __SMEM_H__

#include <sys/mman.h>
#include <stdlib.h>
#include <errno.h>

typedef unsigned int uint;
typedef unsigned char uchar;

typedef enum {
    FREE = (1 << 0),
    BUSY = (1 << 1)
} SMemBlockStatus;

typedef struct SMemBlock {
    // Data for the block
    uint block_size; // Block size in bytes
    uchar status; // Block status
    // Memory data
    void* this_block; // Pointer to space memory that this block refers to.
    SMemBlock* next_block; // Pointer to next block in list.
} SMemBlock;

typedef struct SMemSpace {
    void* start;
    uint size;
    SMemBlock* blocks;
} SMemSpace;


/**
 * @brief Sets the necessary context in "space" for smem_alloc, smem_free and smem_calloc functions.
 * @param space Pointer to the space used for allocation.
 * @param size The size, in bytes, of the new space to be claimed.
 * @returns Number of bytes allocated if successful. If failed, returns -1 and sets errno.
 */
int smem_claim_new_space(SMemSpace* space, uint size) {
    
    // Making sure size is multiple of 4 bytes (assuming 32 bit word)
    size = size % 4 == 0 ? size : size + (4 - size % 4);
    void* map = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS, -1, 0);

    if (map == MAP_FAILED) {
        return -1;
    }

    space->start = map;
    space->size = size;
    (*space->blocks) = {size, FREE, map, NULL};

    return size;
} 

/**
 * @param space Pointer to the space of allocation.
 * @param size Size to be allocated, in bytes;
 * @returns Pointer to the allocated block if successful. If smem_alloc fails, returns NULL and sets errno.
 */
void* smem_alloc(SMemSpace* space, uint size) {
    // TODO: Maybe there is a better way to align size to 4 bytes
    size = size % 4 == 0 ? size : size + (4 - size % 4);
    
    if (size > space->size) {
        errno = ENOMEM;
        return NULL;
    }
    
    void* allocated_pointer = NULL;

    SMemBlock* current_free_block = space->blocks;
    while (current_free_block != NULL) {
        if (current_free_block->block_size >= size) {
            current_free_block->block_size = size;
            current_free_block->status = BUSY;
            allocated_pointer = current_free_block->this_block;
            (*current_free_block->next_block) = {
                current_free_block->block_size - size, // I am not sure this math works on all cases
                FREE,
                allocated_pointer + size,
                current_free_block->next_block // Possibly NULL but that should be OK
            };
            return allocated_pointer;
        } else {
            current_free_block = current_free_block->next_block;
        }
    }

    if (current_free_block == NULL) {
        // Could not find a single block with enough size, will try to split the allocation (assuming freed blocks are coalesced when sequential)
        // TODO: Implement
        return NULL;
    }
}

void* smem_alloc_zeroes(SMemSpace* space, uint size);
void* smem_alloc_init(SMemSpace* space, uint size, void* data);

#endif
