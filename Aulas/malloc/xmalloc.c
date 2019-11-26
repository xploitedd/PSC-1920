#include "xmalloc.h"

typedef struct MemBlock {
    unsigned int dim;
    void *addr;
} Block;

static const int max = 4*1024*1024;
static char space[4*1024*1024];

static void *freeAddr = space;
static size_t freeSize = max;

void *xmalloc(size_t bytes) {
    if (freeSize < bytes)
        return NULL;

    printf("allocated between %p and %p\n", freeAddr, freeAddr + bytes + 1);
    freeSize -= bytes;
    void *addr = freeAddr;
    freeAddr += bytes + 1;
    return addr;
}

void xfree(void *addr) {

}