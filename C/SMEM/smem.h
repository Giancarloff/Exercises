#ifndef __SMEM_H__
#define __SMEM_H__

#include <sys/mman.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

// For debugging
#define SMEM_DEBUG
#ifdef SMEM_DEBUG
    #include <stdio.h>
    #include <unistd.h>
#endif

// Common size factors
#define KB 1000
#define MB 1000*KB
#define GB 1000*MB

#define KiB 1024
#define MiB 1024*KiB
#define GiB 1024*MiB

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
    struct SMemBlock* next_block; // Pointer to next block in list.
} SMemBlock;

typedef struct SMemSpace {
    void* start;
    uint size;
    SMemBlock* blocks;
} SMemSpace;

/** 
 * FIXME: The current implementation seems badly modeled. I am now considering using static expressions, however
 * they need to be constant.
 */

/**
 * @brief Sets the necessary context in "space" for smem_alloc, smem_free and smem_calloc functions.
 * @param space Pointer to the space used for allocation.
 * @param size The size, in kilobytes, of the new space to be claimed.
 * @returns Number of bytes allocated if successful. If failed, returns -1 and sets errno.
 */
int smem_claim_new_space(SMemSpace* space, uint size) {
    
    // Making sure size is multiple of 4 kilobytes (assuming 32 bit word)
    size = size % 4 == 0 ? size : size + (4 - size % 4);
    size = size * KiB;
    #ifdef SMEM_DEBUG
    printf("smem_claim_new_space: size = %d.\n", size);
    printf("smem_claim_new_space: page size = %ld\n", sysconf(_SC_PAGESIZE));
    #endif
    void* map = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (map == MAP_FAILED) {
        return -1;
    }

    #ifdef SMEM_DEBUG
    printf("smem_claim_new_space: setting space vars.\n");
    #endif

    space->start = map;
    space->size = size;
    space->blocks->block_size = size;
    space->blocks->status = FREE;
    space->blocks->this_block = map;
    space->blocks->next_block = NULL;

    #ifdef SMEM_DEBUG
    printf("smem_claim_new_space: returning.\n");
    #endif

    return size;
} 
/**
 * @brief Frees the claimed space. Behaviour is undefined if space is attempted to be used after bequeathing.
 * @param space The space to be bequeathed.
 * @return 0 for success, -1 for failure. Sets errno.
 */
int smem_bequeath_space(SMemSpace* space) {

    if (space->start == NULL) {
        return EXIT_SUCCESS;
    }

    void* addr = space->start;
    uint size = space->size;
    space->start = NULL;
    space->size = 0;
    space->blocks = NULL;

    return munmap(addr, size);

}

/**
 * @param space Pointer to the space of allocation.
 * @param size Size to be allocated, in bytes;
 * @returns Pointer to the allocated block if successful. If smem_alloc fails, returns NULL.
 */
void* smem_alloc(SMemSpace* space, uint size) {
    // TODO: Maybe there is a better way to align size to 4 bytes
    size = size % 4 == 0 ? size : size + (4 - size % 4);
    
    if (size > space->size) {
        return NULL;
    }
    
    void* allocated_pointer = NULL;

    SMemBlock* current_free_block = space->blocks;
    while (current_free_block != NULL) {
        if (current_free_block->block_size >= size) {
            current_free_block->block_size = size;
            current_free_block->status = BUSY;
            allocated_pointer = current_free_block->this_block;

            SMemBlock next_block = {
                current_free_block->block_size - size, // I am not sure this math works on all cases
                FREE,
                (uchar*) allocated_pointer + size, // sizeof (char) | (uchar) is guaranteed 1 byte
                current_free_block->next_block // Possibly NULL but that should be OK
            };
            memcpy(current_free_block->next_block, &next_block, sizeof(SMemBlock));

            return allocated_pointer;
        } else {
            current_free_block = current_free_block->next_block;
        }
    }

    // Could not find a single block with enough size. Right now we're just going to not do anything.
    // TODO: Maybe we could split the allocation? I would prefer not to have a "smem_copy" though, and right now
    // I see no other way to have a split allocation and just allow for the user to freely work with the pointer.
    return NULL;
}

void* smem_alloc_zeroes(SMemSpace* space, uint size);
void* smem_alloc_init(SMemSpace* space, uint size, void* data);

#endif
