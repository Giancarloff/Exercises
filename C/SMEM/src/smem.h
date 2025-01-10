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
} SMeMBlockStatus;

typedef struct SMeMBlock {
    uint status : 1; // One bit for status (ALLOC, NALLOC)
    uint size : 31; // Thirty one bits for size. This should be in bytes.
} SMeMBlock;

typedef struct SMeMSpace {
    void* start; // Starting pointer of space (i.e. the beginning of the memory page)
    uint size; // Size of the space (this should be page-aligned, i.e. multiple of 4096 bytes in my system)
} SMeMSpace;

/**
 * @brief Claims space in memory for using smem_alloc, _calloc and _free functions.
 * @param size The size, in kilobytes, of the new space to be claimed.
 * @returns Number of bytes claimed if successful. If failed, returns -1 and sets errno.
 */
int smem_claim_new_space(uint size);

/**
 * @brief Frees claimed space. If size is greater than or equal to space size, the whole page is unmapped. Trying to access unmapped pages likely results in segmentation fault.
 * @param size The amount of kilobytes to be freed.
 * @return 0 for success, -1 for failure. Sets errno.
 */
int smem_bequeath_space(uint size);

/**
 * @brief Allocate size bytes and returns pointer to memory.
 * @param size Size to be allocated, in bytes;
 * @returns Pointer to the allocated block if successful. If smem_alloc fails, returns NULL.
 */
void* smem_alloc(uint size);

/**
 * @brief Allocate size bytes and initialize them with given value.
 * @param size Amount of bytes to allocate.
 * @param value Value to initialize bytes. If it is greater than 255, value will be set to 255.
 * @returns Pointer to the allocated block if successful, NULL otherwise.
 */
void* smem_alloc_init(uint size, char value);

/**
 * @brief Allocate size bytes and initialize them with zero (0).
 * @param size Amount of bytes to allocate.
 * @returns Pointer to the allocated block if successful, NULL otherwise.
 */
void* smem_alloc_zeroes(uint size);

/**
 * @brief Allocate size bytes and copy data over. Keep in mind only 'size' bytes will be copied.
 * @param size Amount of bytes to allocate.
 * @param data Pointer to data that will be copied over.
 * @returns Pointer to the allocated block if successful, NULL otherwise.
 */
void* smem_alloc_copy(uint size, void* data);

/**
 * @brief Frees memory for allocation.
 * @param block Pointer to memory block that should be freed.
 * @return 0 if free was successful, -1 if not.
 */
int smem_free(void* block);

/**
 * TODO: Implement. Note we might end up returning the same pointer if there is free space continuous to it.
 */
void* smem_realloc();


#endif
