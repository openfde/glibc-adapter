#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "adapter-register.h"

static int adapt_open(const char *pathname, int flags, mode_t mode) {
  int fd = open(pathname, flags, mode);
  adapter_log("file : %s, fd : %d, errno : %d", pathname, fd, errno);
  return fd;
}

static int adapt_open64(const char* pathname, int flags, mode_t mode) {
  int fd = open(pathname, flags, mode);
  adapter_log("file : %s, fd : %d, errno : %d", pathname, fd, errno);
  return fd;
}

static struct glibc_adapter_t fcntl_adapters[] = {
    ADAPT_INDIRECT(open),
    ADAPT_INDIRECT(open64),
    ADAPT_DIRECT(close),
    ADAPT_DIRECT(fork),
    ADAPT_DIRECT(ttyname),
    ADAPT_DIRECT(fmemopen),
    ADAPT_DIRECT(open_memstream),

    ADAPT_DIRECT(isatty),
};

void register_adapters_fcntl() { REGISTER_ADAPTERS_BY_CLASS(fcntl); }
