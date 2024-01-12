#include <assert.h>
#include <errno.h>
#include <malloc.h>
#include <stdlib.h>

#include "adapter-register.h"

static void *adapt_valloc(size_t size) {
  (void)size;
  adapter_log("valloc not supported");
  errno = ENOTSUP;
  assert(0);
  return NULL;
}

static void *adapt_pvalloc(size_t size) {
  (void)size;
  adapter_log("pvalloc not supported");
  errno = ENOTSUP;
  assert(0);
  return NULL;
}

static struct glibc_adapter_t malloc_adapters[] = {
    // stdlib.h
    ADAPT_DIRECT(malloc),
    ADAPT_DIRECT(free),
    ADAPT_DIRECT(calloc),
    ADAPT_DIRECT(realloc),
    ADAPT_DIRECT(reallocarray),

    ADAPT_DIRECT(posix_memalign),
    ADAPT_DIRECT(aligned_alloc),
    ADAPT_INDIRECT(valloc),

    // malloc.h
    ADAPT_DIRECT(memalign),
    ADAPT_INDIRECT(pvalloc),
};

void register_adapters_malloc() { REGISTER_ADAPTERS_BY_CLASS(malloc); }
