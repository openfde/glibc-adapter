#include "glibc-adapter.h"
#include "adapter-register.h"

#include <android/log.h>

#include <string.h>

extern const void *find_symbol_adapter(const char *sym)
    __attribute__((visibility("default")));

int adapter_log(const char *fmt, ...) {
  int ret = 0;
  va_list args;
  va_start(args, fmt);
  ret = __android_log_vprint(ANDROID_LOG_DEBUG, "glibc-adapter", fmt, args);
  va_end(args);
  return ret;
}

// do not use malloc to allocate memory when dyname linker loading.
static struct glibc_adapter_t all_adapters[8192];
static const size_t adapters_storage_max_size =
    sizeof(all_adapters) / sizeof(all_adapters[0]);
static size_t all_adapters_count = 0;

int register_adapters(const char *classes,
                      const struct glibc_adapter_t *adapters,
                      size_t adapter_count) {

  if (adapter_count == 0 || !classes) {
    return 0;
  }

  if (adapter_count + all_adapters_count > adapters_storage_max_size) {
    adapter_log("No enough memory for %s adapters", classes);
    return 0;
  }

  for (size_t idx = 0; idx < adapter_count; idx++, all_adapters_count++) {
    all_adapters[all_adapters_count] = adapters[idx];
  }

  adapter_log("Added %d symbols for %s adapters", adapter_count, classes);
  return 1;
}

static int inited = 0;

static int adapter_cmp(const void *a, const void *b) {
  return strcmp(((struct glibc_adapter_t *)a)->symbol,
                ((struct glibc_adapter_t *)b)->symbol);
}

#define REGISTER_ADAPTERS_BY_CLASSES(classes)                                  \
  extern void register_adapters_##classes();                                   \
  register_adapters_##classes()

static void register_all_adapters() {
  REGISTER_ADAPTERS_BY_CLASSES(fcntl);
  REGISTER_ADAPTERS_BY_CLASSES(stdio);
  REGISTER_ADAPTERS_BY_CLASSES(errno);
  REGISTER_ADAPTERS_BY_CLASSES(string);
}

static void sort_adapters() {
  qsort(all_adapters, all_adapters_count, sizeof(*all_adapters), adapter_cmp);
}

const void *find_symbol_adapter(const char *sym) {
  struct glibc_adapter_t key;
  key.symbol = sym;

  if (!inited) {
    register_all_adapters();
    sort_adapters();
    inited = 1;
  }

  void *found = bsearch(&key, all_adapters, all_adapters_count,
                        sizeof(*all_adapters), adapter_cmp);

  if (found) {
    return ((struct glibc_adapter_t *)found)->adapt_fun;
  }
  return NULL;
}
