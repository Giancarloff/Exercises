#ifndef __SMEM_H__
#define __SMEM_H__

#include <sys/mman.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

// For debugging
// #define SMEM_DEBUG
#ifdef SMEM_DEBUG
    #include <stdio.h>
    #include <unistd.h>
#endif

// Common size factors
#define KB 1000 // Kilobytes
#define MB 1000*KB
#define GB 1000*MB

#define KiB 1024
#define MiB 1024*KiB
#define GiB 1024*MiB

typedef unsigned int uint;
typedef unsigned char uchar;

typedef enum {
    NALLOC = 0,
    ALLOC = 1
} SMemBlockStatus;

typedef struct SMemBlock {
    uint status : 1; // One bit for status (ALLOC, NALLOC)
    uint size : 31; // Thirty one bits for size. This should be in bytes.
} SMemBlock;

typedef struct SMemSpace {
    void* start; // Starting pointer of space (i.e. the beginning of the memory page)
    uint size; // Size of the space (this should be page-aligned, i.e. multiple of 4096 bytes in my system)
} SMemSpace;

// Spaces for allocation and metadata.
static SMemSpace smem_allocation_space = {NULL, 0};
static SMemSpace smem_metadata_space = {NULL, 0};

/**
 * @brief Claims space in memory for using smem_alloc, _calloc and _free functions.
 * @param size The size, in kilobytes, of the new space to be claimed.
 * @returns Number of bytes claimed if successful. If failed, returns -1 and sets errno.
 */
int smem_claim_new_space(uint size) {
    
    // Making sure size is multiple of 4 kilobytes
    size = size % 4 == 0 ? size : size + (4 - size % 4);
    size = size * KiB;
    #ifdef SMEM_DEBUG
    printf("smem_claim_new_space: size = %d.\n", size);
    printf("smem_claim_new_space: system page size = %ld\n", sysconf(_SC_PAGESIZE));
    #endif
    void* map = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | 0x20, -1, 0); // 0x20 should be MAP_ANONYMOUS but the IDE is complaining that it can't find the definition.

    if (map == MAP_FAILED) {
        return -1;
    }

    #ifdef SMEM_DEBUG
    printf("smem_claim_new_space: setting space vars.\n");
    #endif

    smem_allocation_space.start = map;
    smem_allocation_space.size = size;

    void* data_map = mmap(NULL, sysconf(_SC_PAGESIZE), PROT_READ | PROT_WRITE, MAP_PRIVATE | 0x20, -1, 0);

    smem_metadata_space.start = data_map;
    smem_metadata_space.size = sysconf(_SC_PAGESIZE);

    SMemBlock block = {NALLOC, size};

    memcpy(smem_metadata_space.start, &block, sizeof(SMemBlock)); 

    #ifdef SMEM_DEBUG
    printf("smem_claim_new_space: returning.\n");
    #endif

    return size;
} 
/**
 * @brief Frees claimed space. If size is greater than or equal to space size, the whole page is unmapped. Trying to access unmapped pages likely results in segmentation fault.
 * @param size The amount of kilobytes to be freed.
 * @return 0 for success, -1 for failure. Sets errno.
 */
int smem_bequeath_space(uint size) {
    /**
     * NOTE: This function is not very elegant, it also tricks anyone calling it by actually unmapping the least
     * amount of pages required to unmap 'size'. Too bad!
     */
    size = size % 4 == 0 ? size : size + (4 - size % 4);
    size = size * KiB;
    
    if (smem_allocation_space.start == NULL) {
        return EXIT_SUCCESS;
    }

    if (size >= smem_allocation_space.size) {
        void* alloc_ptr = smem_allocation_space.start;
        uint alloc_size = smem_allocation_space.size;
        smem_allocation_space.start = NULL;
        smem_allocation_space.size = 0;

        void* mdata_ptr = smem_metadata_space.start;
        uint mdata_size = smem_metadata_space.size;
        smem_metadata_space.start = NULL;
        smem_metadata_space.size = 0;

        int status = munmap(mdata_ptr, mdata_size);
        #ifdef SMEM_DEBUG
        printf("smem_bequeath_space: metadata space unmapping status = %d.\n", status);
        #endif
        return munmap(alloc_ptr, alloc_size);
    }

    void* new_start = (uchar*) smem_allocation_space.start + (size / 4);
    int status = munmap(smem_allocation_space.start, size);
    smem_allocation_space.start = new_start;
    // FIXME: Implement metadata space correction

    return status;
}

/**
 * @brief Allocate size bytes and returns pointer to memory.
 * @param size Size to be allocated, in bytes;
 * @returns Pointer to the allocated block if successful. If smem_alloc fails, returns NULL.
 */
void* smem_alloc(uint size) {
    // TODO: Maybe there is a better way to align size to 4 bytes
    size = size % 4 == 0 ? size : size + (4 - size % 4);
    
    if (size > smem_allocation_space.size || 2147483647 < size ) { // No can do sizes greater than 2^31 - 1.
        return NULL;
    }
    
    void* allocated_pointer = smem_allocation_space.start;
    uint* metadata_pointer = (uint*) smem_metadata_space.start;
    uint metadata_offset = 0;
    uint allocation_offset = 0;
    SMemBlock current_block = {NALLOC, 0};
    while (metadata_offset < sysconf(_SC_PAGESIZE) - 1) { // I am not sold on this while condition here.
        memcpy(&current_block, metadata_pointer, sizeof(SMemBlock));
        #ifdef SMEM_DEBUG
        printf("smem_alloc: current_block status = %d, size = %d, metadata_pointer = %p.\n", current_block.status, current_block.size, metadata_pointer);
        #endif

        if (current_block.status == NALLOC && size <= current_block.size) { // Found a free block.
            uint old_size = current_block.size;
            #ifdef SMEM_DEBUG
            printf("smem_alloc: found free block on allocation pointer %p, metadata pointer %p and block size %d.\n", allocated_pointer, metadata_pointer, current_block.size);
            printf("smem_alloc: will configure to allocated block with size %d.\n", size);
            #endif
            // Configure this block
            current_block.status = ALLOC;
            current_block.size = size; 
            memcpy(metadata_pointer, &current_block, sizeof(SMemBlock));
            #ifdef SMEM_DEBUG
            printf("smem_alloc: the metadata pointer is telling me %d.\n", *metadata_pointer);
            #endif
            // Split it.
            metadata_pointer++;
            current_block.status = NALLOC;
            current_block.size = (old_size - size);
            memcpy(metadata_pointer, &current_block, sizeof(SMemBlock));
            return allocated_pointer;
        }

        allocation_offset += (current_block.size / sizeof(void*)); // 
        if (allocation_offset > smem_allocation_space.size) {
            return NULL;
        }
        // Casting is a sign of good code, right?
        allocated_pointer = (void*) ((char*) allocated_pointer + allocation_offset);
        metadata_pointer++;
        metadata_offset++;
    }

    /** 
     * TODO: Right now if we dont find a cotinuous block of free memory with enough size, we just do not do anything.
     * Maybe we could do something smart to try and fit this allocation if there is enough free space, even when it is not
     * continuous.
     */
    return NULL;
}

/**
 * @brief Allocate size bytes and initialize them with given value.
 * @param size Amount of bytes to allocate.
 * @param value Value to initialize bytes. If it is greater than 255, value will be set to 255.
 * @returns Pointer to the allocated block if successful, NULL otherwise.
 */
void* smem_alloc_init(uint size, char value) {
    if (value > 255) {
        value = 255;
    }

    char* ptr = (char*) smem_alloc(size);

    if (ptr == NULL) {
        return NULL;
    }

    for (uint i = 0; i < size; i++) {
        ptr[i] = value;
    }
}

/**
 * @brief Allocate size bytes and initialize them with zero (0).
 * @param size Amount of bytes to allocate.
 * @returns Pointer to the allocated block if successful, NULL otherwise.
 */
void* smem_alloc_zeroes(uint size) {
    return smem_alloc_init(size, 0);
    /**
     * NOTE: This function is almost redundant, since mmap() initialize pages with zero. However,
     * free's followed by mallocs may result in garbage present in the memory blocks, so there is at least
     * one use case.
     */
}

/**
 * @brief Allocate size bytes and copy data over. Keep in mind only 'size' bytes will be copied.
 * @param size Amount of bytes to allocate.
 * @param data Pointer to data that will be copied over.
 * @returns Pointer to the allocated block if successful, NULL otherwise.
 */
void* smem_alloc_copy(uint size, void* data) {
    void* ptr = smem_alloc(size);

    if (ptr == NULL) {
        return NULL;
    }

    memcpy(ptr, data, size);
    return ptr;
}

/**
 * TODO: Implement. Note we might end up returning the same pointer if there is free space continuous to it.
 */
void* smem_realloc();

/**
 * TODO: Implement
 */
void smem_free(void* block);


#endif
