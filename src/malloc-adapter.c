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

static void *adapt___memalign_hook(size_t __alignment, size_t __byte_count, const void* _Nonnull __caller) {
  (void)__caller;
  return memalign(__alignment, __byte_count);
}

static void *adapt___malloc_hook(size_t __byte_count, const void* _Nonnull __caller) {
  (void)__caller;
  return malloc(__byte_count);
}

static void adapt___free_hook(void* _Nullable __ptr, const void* _Nonnull __caller) {
  (void)__caller;
  free(__ptr);
  return ;
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
    ADAPT_DIRECT(mallinfo),

    ADAPT_INDIRECT(__memalign_hook),
    ADAPT_INDIRECT(__malloc_hook),
    ADAPT_INDIRECT(__free_hook),
};

void register_adapters_malloc() { REGISTER_ADAPTERS_BY_CLASS(malloc); }
