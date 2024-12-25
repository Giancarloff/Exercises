#include "smem.h"
#include "stdio.h"

int main(void) {

    uint space_size = 2;
    
    printf("Trying to claim %d kilobytes.\n", space_size);
    short int true_space_size = smem_claim_new_space(space_size);

    if (true_space_size == -1) {
        printf("Claiming space failed with status -1, errno is %d.\n", errno);
        return EXIT_FAILURE;
    }

    uint alloc_size = (space_size * 10);
    printf("Trying to allocate %d bytes.\n", alloc_size);
    void* ptr = smem_alloc(alloc_size);

    if (ptr == NULL) {
        printf("Error: Allocation failed. Expected success.\n");
        return EXIT_FAILURE;
    }
    
    printf("Allocation successful. Pointer is %p.\n", ptr);

    char* hw = "Hello World.";
    printf("Writing %ld bytes to allocated pointer %p.\n", strlen(hw) + 1, ptr);
    memcpy(ptr, hw, strlen(hw) + 1);
    printf("Bytes written:\n\t%s\n", (char*) ptr);

    void* ptr = malloc(10);
    ptr = realloc(ptr, 20);

    alloc_size = true_space_size + 1;

    printf("Trying to allocate %d bytes.\n", alloc_size);
    void* ptr2 = smem_alloc(alloc_size);

    if (ptr2 == NULL) {
        printf("Allocation failed. This was expected.\n");
    }

    alloc_size = 100;
    printf("Trying to allocate %d bytes.\n", alloc_size);
    ptr2 = smem_alloc(alloc_size);

    if (ptr2 == NULL) {
        printf("Error: Allocation failed. Expected success.\n");
        return EXIT_FAILURE;
    }

    printf("Bequeathing space.\n");
    true_space_size = smem_bequeath_space(space_size);
    if (true_space_size == -1) {
        printf("Failed bequeathing space. Errno is %d.\n", errno);
        return EXIT_FAILURE;
    }

    printf("Space bequeathed. Returning.\n");
    return EXIT_SUCCESS;
}