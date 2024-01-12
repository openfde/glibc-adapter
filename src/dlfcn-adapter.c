#include <assert.h>
#include <dlfcn.h>
#include <errno.h>

#include "adapter-register.h"

static void *adapt_dlopen(const char *filename, int flag) {
  adapter_log("filename %s flag %i", filename, flag);
  return dlopen(filename, flag);
}

static void *adapt_dlsym(void *handle, const char *symbol) {
  adapter_log("handle %p symbol %s", handle, symbol);
  return dlsym(handle, symbol);
}

static int adapt_dlclose(void *handle) {
  adapter_log("handle %p", handle);
  return dlclose(handle);
}

static const char *adapt_dlerror(void) { return dlerror(); }

static void *adapt_dlvsym(void *handle, const char *symbol,
                          const char *version) {
  (void)handle;
  (void)symbol;
  (void)version;
  adapter_log("dlvsym not supported");
  errno = ENOTSUP;
  assert(0);
  return NULL;
}

static int adapt_dladdr(void *addr, Dl_info *info) {
  (void)addr;
  (void)info;
  adapter_log("dladdr not supported");
  errno = ENOTSUP;
  assert(0);
  return 0;
}

static int adapt_dladdr1(void *addr, Dl_info *info, void **extra_info,
                         int flags) {
  (void)addr;
  (void)info;
  (void)extra_info;
  (void)flags;
  adapter_log("dladdr1 not supported");
  errno = ENOTSUP;
  assert(0);
  return 0;
}

static void *adapt_dlmopen(long int lmid, const char *filename, int flags) {
  (void)lmid;
  (void)filename;
  (void)flags;
  adapter_log("dlmopen not supported");
  errno = ENOTSUP;
  assert(0);
  return NULL;
}

static int adapt_dlinfo(void *handle, int request, void *info) {
  (void)handle;
  (void)request;
  (void)info;
  adapter_log("dlinfo not supported");
  errno = ENOTSUP;
  assert(0);
  return -1;
}

struct dl_phdr_info;
static int adapt_dl_iterate_phdr(int (*callback)(struct dl_phdr_info *info,
                                                 size_t size, void *data),
                                 void *data) {
  (void)callback;
  (void)data;
  adapter_log("dl_iterate_phdr not supported");
  errno = ENOTSUP;
  assert(0);
  return callback(NULL, 0, data);
}

static struct glibc_adapter_t dlfcn_adapters[] = {
    ADAPT_INDIRECT(dlopen),  ADAPT_INDIRECT(dlerror),
    ADAPT_INDIRECT(dlclose), ADAPT_INDIRECT(dlsym),
    ADAPT_INDIRECT(dlvsym),  ADAPT_INDIRECT(dladdr),
    ADAPT_INDIRECT(dladdr1), ADAPT_INDIRECT(dlmopen),
    ADAPT_INDIRECT(dlinfo),  ADAPT_INDIRECT(dl_iterate_phdr),
};

void register_adapters_dlfcn() { REGISTER_ADAPTERS_BY_CLASS(dlfcn); }
