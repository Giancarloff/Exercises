#include "smem.h"
#include "stdio.h"

int main(void) {

    SMemSpace* space;
    uint space_size = 2;
    
    printf("Trying to claim %d kilobytes.\n", space_size);
    short int status = smem_claim_new_space(space, space_size);

    if (status == -1) {
        printf("Claiming space failed with status -1, errno is %d.\n", errno);
        return EXIT_FAILURE;
    }

    uint alloc_size = (space_size * 10);
    printf("Trying to allocate %d bytes.\n", alloc_size);
    void* ptr = smem_alloc(space, alloc_size);

    if (ptr == NULL) {
        printf("Error: Allocation failed. Expected success.\n");
        return EXIT_FAILURE;
    }
    
    printf("Allocation successful. Pointer is %p.\n", ptr);

    char* hw = "Hello World.\n";
    printf("Writing %ld bytes to allocated pointer %p.\n", sizeof(hw), ptr);
    memcpy(ptr, hw, sizeof(hw));
    printf("Bytes written:\n\t%s", (char*) ptr);

    alloc_size = space_size + 1;

    printf("Trying to allocate %d bytes.\n", alloc_size);
    void* ptr2 = smem_alloc(space, alloc_size);

    if (ptr2 == NULL) {
        printf("Allocation failed. This was expected.\n");
    }

    alloc_size = 20;
    ptr2 = smem_alloc(space, alloc_size);

    printf("Trying to allocate %d bytes.\n", alloc_size);

    if (ptr2 == NULL) {
        printf("Error: Allocation failed. Expected success.\n");
        return EXIT_FAILURE;
    }

    printf("Bequeathing space.\n");
    status = smem_bequeath_space(space);
    if (status == -1) {
        printf("Failed bequeathing space. Errno is %d.\n", errno);
        return EXIT_FAILURE;
    }

    printf("Space bequeathed. Returning.\n");
    return EXIT_SUCCESS;
}