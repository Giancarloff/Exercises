#include "smem.h"

// Spaces for allocation and metadata.

static SMeMSpace smem_allocation_space = {NULL, 0};
static SMeMSpace smem_metadata_space = {NULL, 0};

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

    SMeMBlock block = {NALLOC, size};

    memcpy(smem_metadata_space.start, &block, sizeof(SMeMBlock)); 

    #ifdef SMEM_DEBUG
    printf("smem_claim_new_space: returning.\n");
    #endif

    return size;
} 

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

        #ifdef SMEM_DEBUG
        int status = munmap(mdata_ptr, mdata_size);
        printf("smem_bequeath_space: metadata space unmapping status = %d.\n", status);
        #else
        munmap(mdata_ptr, mdata_size);
        #endif
        return munmap(alloc_ptr, alloc_size);
    }

    void* new_start = (uchar*) smem_allocation_space.start + (size / 4);
    int status = munmap(smem_allocation_space.start, size);
    smem_allocation_space.start = new_start;
    // FIXME: Implement metadata space correction

    return status;
}

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
    SMeMBlock current_block = {NALLOC, 0};
    while (metadata_offset < sysconf(_SC_PAGESIZE) - 1) { // I am not sold on this while condition here.
        memcpy(&current_block, metadata_pointer, sizeof(SMeMBlock));
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
            memcpy(metadata_pointer, &current_block, sizeof(SMeMBlock));
            #ifdef SMEM_DEBUG
            printf("smem_alloc: the metadata pointer is telling me %d.\n", *metadata_pointer);
            #endif
            // Split it.
            metadata_pointer++;
            current_block.status = NALLOC;
            current_block.size = (old_size - size);
            memcpy(metadata_pointer, &current_block, sizeof(SMeMBlock));
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

void* smem_alloc_init(uint size, char value) {
    // if (value > 255) {
    //     value = 255;
    // }

    char* ptr = (char*) smem_alloc(size);

    if (ptr == NULL) {
        return NULL;
    }

    for (uint i = 0; i < size; i++) {
        ptr[i] = value;
    }

    return (void*) ptr;
}

void* smem_alloc_zeroes(uint size) {
    return smem_alloc_init(size, 0);
    /**
     * NOTE: This function is almost redundant, since mmap() initialize pages with zero. However,
     * free's followed by mallocs may result in garbage present in the memory blocks, so there is at least
     * one use case.
     */
}

void* smem_alloc_copy(uint size, void* data) {
    void* ptr = smem_alloc(size);

    if (ptr == NULL) {
        return NULL;
    }

    memcpy(ptr, data, size);
    return ptr;
}

int smem_free(void* block) {
    void* freed_pointer = smem_allocation_space.start;
    uint* metadata_pointer = (uint*) smem_metadata_space.start;
    uint metadata_offset = 0;
    uint allocation_offset = 0;
    SMeMBlock current_block = {NALLOC, 0};
    while (metadata_offset < sysconf(_SC_PAGESIZE) - 1) { // I am not sold on this while condition here.
        memcpy(&current_block, metadata_pointer, sizeof(SMeMBlock));

        if (freed_pointer == block) { // Found the block we are supposed to free
            if (current_block.status == NALLOC) { // I tried to free something that was not allocated.
                return -1;
            }
            current_block.status = NALLOC;
            SMeMBlock next_block = {NALLOC, 0};
            if (metadata_offset + 1 < sysconf(_SC_PAGESIZE) - 1) { // Checking if there is a next block.
                memcpy(&next_block, metadata_pointer + 1, sizeof(SMeMBlock));
                if (next_block.status == NALLOC) {
                    // The next block is also free. Let us coalesce them into the same block.
                    current_block.size += next_block.size;
                }
            }
            memcpy(metadata_pointer, &current_block, sizeof(SMeMBlock));
            return 0;
        }

        allocation_offset += (current_block.size / sizeof(void*)); // 
        // Casting is a sign of good code, right?
        freed_pointer = (void*) ((char*) freed_pointer + allocation_offset);
        metadata_pointer++;
        metadata_offset++;
    }

    return -1;
}

void* smem_realloc();
