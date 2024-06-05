#include <errno.h>
#define _GNU_SOURCE
#include <sys/mman.h>

#include "adapter-register.h"


static struct glibc_adapter_t mman_adapters[] = {
    ADAPT_DIRECT(mprotect),
    ADAPT_DIRECT(mmap),
    ADAPT_TO(mmap64, mmap),
    ADAPT_DIRECT(munmap),
    ADAPT_DIRECT(msync),
    ADAPT_DIRECT(mremap),
};

void register_adapters_mman() { REGISTER_ADAPTERS_BY_CLASS(mman); }
