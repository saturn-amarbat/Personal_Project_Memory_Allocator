#include "allocator.h"
#include <stdio.h>

int main() {
    printf("Custom Allocator Demo\n");

    allocator_init(1024 * 1024);

    void* p1 = s_malloc(128);
    if (p1 == NULL) {
        printf("Allocation failed (expected for starter commit)\n");
    } else {
        printf("Allocated 128 bytes at %p\n", p1);
    }

    s_free(p1);

    return 0;
}
