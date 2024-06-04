#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "adapter-register.h"

// NOTE: all interfaces of memory operations are in malloc-adapter.c

static char* adapt___realpath_chk(const char* path, char* resolved_path,
  size_t resolved_len) {
  char* real_path = NULL;
  if (!resolved_path || resolved_len == 0) {
    errno = EINVAL;
    return NULL;
  }
  real_path = realpath(path, NULL);
  if (!real_path) {
    return NULL;
  }
  if (strlen(real_path) >= resolved_len) {
    free(real_path);
    errno = ENAMETOOLONG;
    return NULL;
  }

  strcpy(resolved_path, real_path);
  free(real_path);
  return resolved_path;
}

static struct glibc_adapter_t stdlib_adapters[] = {
    ADAPT_DIRECT(system),
    ADAPT_DIRECT(realpath),
    ADAPT_INDIRECT(__realpath_chk),
    ADAPT_DIRECT(getenv)
};

void register_adapters_stdlib() { REGISTER_ADAPTERS_BY_CLASS(stdlib); }
