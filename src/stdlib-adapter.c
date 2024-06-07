#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include <xlocale.h>

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
    ADAPT_DIRECT(getenv),
    ADAPT_DIRECT(clearenv),
    ADAPT_DIRECT(setenv),
    ADAPT_DIRECT(unsetenv),
    ADAPT_DIRECT(putenv),
    ADAPT_DIRECT(wctomb),
    ADAPT_DIRECT(mbstowcs),
    ADAPT_DIRECT(mblen),
    ADAPT_DIRECT(mbtowc),
    ADAPT_DIRECT(wcstombs),

    ADAPT_DIRECT(strtod),
    ADAPT_DIRECT(strtof),
    ADAPT_DIRECT(strtol),
    ADAPT_DIRECT(strtold),
    ADAPT_DIRECT(strtoll),
    ADAPT_DIRECT(strtoul),
    ADAPT_DIRECT(strtoull),

    ADAPT_DIRECT(strtod_l),
    ADAPT_DIRECT(strtof_l),
    ADAPT_DIRECT(strtol_l),
    ADAPT_DIRECT(strtold_l),
    ADAPT_DIRECT(strtoll_l),
    ADAPT_DIRECT(strtoul_l),
    ADAPT_DIRECT(strtoull_l),
    ADAPT_TO(__strtod_l, strtod_l),
    ADAPT_TO(__strtof_l, strtof_l),
    ADAPT_TO(__strtol_l, strtol_l),
    ADAPT_TO(__strtold_l, strtold_l),
    ADAPT_TO(__strtoll_l, strtoll_l),
    ADAPT_TO(__strtoul_l, strtoul_l),
    ADAPT_TO(__strtoull_l, strtoull_l),
};

void register_adapters_stdlib() { REGISTER_ADAPTERS_BY_CLASS(stdlib); }
