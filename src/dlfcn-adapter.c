#include <assert.h>
#include <dlfcn.h>
#include <errno.h>

#include "adapter-register.h"
#include "glibc-adapter.h"

extern void *__loader_dlopen(const char *filename, int flags, const void *caller_addr);
static void *adapt_dlopen(const char *filename, int flag) {
  // use caller's namespace instead of adpater
  // caller is a gnu library, but adapter is a bionic libraray.
  const void *caller_addr = __builtin_return_address(0);
  void *handle = __loader_dlopen(filename, flag, caller_addr);
  if (handle) {
    adapter_logv("success to dlopen %s flag 0x%x, handle %p", filename, flag, handle);
  } else {
    adapter_log("fail to dlopen %s flag 0x%x. %s", filename, flag, dlerror());
  }
  return handle;
}

extern void *__loader_dlsym(void *handle, const char *symbol, const void *caller_addr);
static void *adapt_dlsym(void *handle, const char *symbol) {
  void *v = (void *)find_symbol_adapter(symbol);
  if (v == NULL) {
    const void *caller_addr = __builtin_return_address(0);
    v = __loader_dlsym(handle, symbol, caller_addr);
  }
  if (v) {
    adapter_logv("dlsym handle %p symbol %s addr %p", handle, symbol, v);
  } else {
    adapter_log("dlsym handle %p symbol %s. %s", handle, symbol, dlerror());
  }
  return v;
}

static int adapt_dlclose(void *handle) {
  adapter_logv("dlclose handle %p", handle);
  return dlclose(handle);
}

static const char *adapt_dlerror(void) { return dlerror(); }

extern void *__loader_dlvsym(void *handle, const char *symbol, const char *version,
                             const void *caller_addr);
static void *adapt_dlvsym(void *handle, const char *symbol, const char *version) {
  void *v = (void *)find_symbol_adapter(symbol);

  if (v == NULL) {
    const void *caller_addr = __builtin_return_address(0);
    v = __loader_dlvsym(handle, symbol, version, caller_addr);
  }

  if (v) {
    adapter_logv("dlvsym(%p, %s, %s) = %p", handle, symbol, version, v);
  } else {
    adapter_log("dlvsym(%p, %s, %s) = %s", handle, symbol, version, dlerror());
  }
  return v;
}

static int adapt_dladdr1(void *addr, Dl_info *info, void **extra_info, int flags) {
  (void)addr;
  (void)info;
  (void)extra_info;
  (void)flags;
  adapter_log("dladdr1(%p, %p, %p, %d) not supported", addr, info, extra_info, flags);
  errno = ENOTSUP;
  return 0;
}

static void *adapt_dlmopen(long int lmid, const char *filename, int flags) {
  (void)lmid;
  (void)filename;
  (void)flags;
  adapter_log("dlmopen(%ld, %s, %d) not supported", lmid, filename, flags);
  errno = ENOTSUP;
  return NULL;
}

static int adapt_dlinfo(void *handle, int request, void *info) {
  (void)handle;
  (void)request;
  (void)info;
  adapter_log("dlinfo(%p, %d, %p) not supported", handle, request, info);
  errno = ENOTSUP;
  return -1;
}

struct dl_phdr_info;
extern int __loader_dl_iterate_phdr(int (*cb)(struct dl_phdr_info *info, size_t size, void *data),
                                    void *data);
static int adapt_dl_iterate_phdr(int (*callback)(struct dl_phdr_info *info, size_t size,
                                                 void *data),
                                 void *data) {
  return __loader_dl_iterate_phdr(callback, data);
}

static struct glibc_adapter_t dlfcn_adapters[] = {
    ADAPT_INDIRECT(dlopen),
    ADAPT_INDIRECT(dlerror),
    ADAPT_INDIRECT(dlclose),
    ADAPT_INDIRECT(dlsym),
    ADAPT_INDIRECT(dlvsym),
    ADAPT_DIRECT(dladdr),
    ADAPT_INDIRECT(dladdr1),
    ADAPT_INDIRECT(dlmopen),
    ADAPT_INDIRECT(dlinfo),
    ADAPT_INDIRECT(dl_iterate_phdr),
    ADAPT_TO(_dl_sym, __loader_dlsym),
};

void register_adapters_dlfcn() { REGISTER_ADAPTERS_BY_CLASS(dlfcn); }
