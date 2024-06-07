#include <errno.h>

#include <sys/time.h>
#include <sys/sysinfo.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <sys/resource.h>

#include "adapter-register.h"

static int adapt_get_nprocs(void) {
  int nprocs = get_nprocs();
  return nprocs;
}
static int adapt_get_nprocs_conf(void) {
  int nprocs = get_nprocs_conf();
  return nprocs;
}

// IFUNC
static void* adapt_gettimeofday() { return (void*)gettimeofday; }


static struct glibc_adapter_t sys_adapters[] = {
    ADAPT_DIRECT(getpwnam),
    ADAPT_DIRECT(getpwuid),
    ADAPT_DIRECT(getpwnam_r),
    ADAPT_DIRECT(getpwuid_r),

    ADAPT_INDIRECT(get_nprocs),
    ADAPT_INDIRECT(get_nprocs_conf),

    ADAPT_INDIRECT(gettimeofday),
    ADAPT_DIRECT(settimeofday),

    ADAPT_DIRECT(sysconf),
    ADAPT_DIRECT(truncate),
    ADAPT_DIRECT(ftruncate),
    ADAPT_DIRECT(fpathconf),
    ADAPT_DIRECT(pathconf),
    ADAPT_DIRECT(ftruncate),
    ADAPT_DIRECT(getrlimit),
    ADAPT_DIRECT(setrlimit),
    ADAPT_DIRECT(prlimit),

    ADAPT_TO(environ , &environ),
    ADAPT_TO(__environ, &environ),

};

void register_adapters_sys() { REGISTER_ADAPTERS_BY_CLASS(sys); }
