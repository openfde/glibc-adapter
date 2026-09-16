#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "adapter-register.h"

static int adapt_open(const char *pathname, int flags, mode_t mode) {
  int fd = open(pathname, flags, mode);
  adapter_logv("file : %s, fd : %d, errno : %d", pathname, fd, errno);
  return fd;
}

extern pid_t __getpid();
static int adapt___getpid(void) {
  return __getpid();
}

static struct glibc_adapter_t fcntl_adapters[] = {
    ADAPT_INDIRECT(open),
    ADAPT_DIRECT(close),
    ADAPT_DIRECT(fork),
    ADAPT_DIRECT(ttyname),
    ADAPT_DIRECT(fmemopen),
    ADAPT_DIRECT(open_memstream),
    ADAPT_DIRECT(read),
    ADAPT_DIRECT(write),
    // ADAPT_DIRECT(openat),
    ADAPT_DIRECT_openat(openat),
    ADAPT_DIRECT(lockf),
    ADAPT_DIRECT(fcntl),
    ADAPT_DIRECT(lseek),
    ADAPT_DIRECT(posix_fallocate),
    ADAPT_DIRECT(posix_fadvise),

    ADAPT_TO(open64, adapt_open),
    ADAPT_TO(close64, close),
    ADAPT_TO(read64, read),
    ADAPT_TO(write64, write),
    // ADAPT_TO(openat64, openat),
    ADAPT_TO_openat(openat64, openat),
    ADAPT_TO(lockf64, lockf),
    ADAPT_TO(fcntl64, fcntl),
    ADAPT_TO(lseek64, lseek),
    ADAPT_TO(posix_fallocate64, posix_fallocate),
    ADAPT_TO(posix_fadvise64, posix_fadvise),

    ADAPT_DIRECT(pread),
    ADAPT_DIRECT(pwrite),

    ADAPT_TO(pread64, pread),
    ADAPT_TO(pwrite64, pwrite),

    ADAPT_DIRECT(isatty),

    ADAPT_DIRECT(chown),
    ADAPT_DIRECT(link),
    ADAPT_DIRECT(unlink),
    ADAPT_DIRECT(access),
    ADAPT_DIRECT(dup),
    ADAPT_DIRECT(usleep),
    ADAPT_DIRECT(geteuid),
    ADAPT_DIRECT(remove),
    ADAPT_DIRECT(readlink),
    ADAPT_DIRECT(getcwd),
    ADAPT_DIRECT(exit),
    ADAPT_DIRECT(sigaction),

    ADAPT_DIRECT(select),
    ADAPT_DIRECT(raise),
    ADAPT_DIRECT(sigprocmask),

    ADAPT_DIRECT(getpid),
    ADAPT_DIRECT(getuid),
    ADAPT_DIRECT(getgid),
    ADAPT_DIRECT(getsid),
    ADAPT_DIRECT(getegid),
    ADAPT_DIRECT(getppid),
    ADAPT_DIRECT(getresuid),
    ADAPT_DIRECT(getresgid),
    ADAPT_DIRECT(gethostname),
    // ADAPT_INDIRECT(__getpid), // cause bionic did_read fail

    ADAPT_DIRECT(sleep),
};

void register_adapters_fcntl() { REGISTER_ADAPTERS_BY_CLASS(fcntl); }
