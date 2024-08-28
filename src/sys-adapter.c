#include <errno.h>
#include <pwd.h>
#include <sys/file.h>
#include <sys/resource.h>
#include <sys/sysinfo.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "adapter-register.h"
#include "misc/confname-maps.h"

static int adapt_get_nprocs(void) {
  int nprocs = get_nprocs();
  return nprocs;
}
static int adapt_get_nprocs_conf(void) {
  int nprocs = get_nprocs_conf();
  return nprocs;
}

static int adapt_gettimeofday(struct timeval* tv, struct timezone* tz) {
  return gettimeofday(tv, tz);
}

static long adapt_sysconf(int name) {
  int map_name = sysconf_map[name];
  if (name != map_name && map_name <= 0) {
    adapter_log("sysconf name 0x%02x not implement", name);
  }
  return sysconf(map_name);
}

static struct glibc_adapter_t sys_adapters[] = {
    ADAPT_DIRECT(getpwnam),
    ADAPT_DIRECT(getpwuid),
    ADAPT_DIRECT(getpwnam_r),
    ADAPT_DIRECT(getpwuid_r),

    ADAPT_INDIRECT(get_nprocs),
    ADAPT_INDIRECT(get_nprocs_conf),

    ADAPT_INDIRECT(gettimeofday),
    ADAPT_DIRECT(settimeofday),

    ADAPT_DIRECT(truncate),
    ADAPT_DIRECT(ftruncate),
    ADAPT_DIRECT(fpathconf),
    ADAPT_DIRECT(pathconf),
    ADAPT_DIRECT(getrlimit),
    ADAPT_DIRECT(setrlimit),
    ADAPT_DIRECT(prlimit),
    ADAPT_INDIRECT(sysconf),
    ADAPT_DIRECT(flock),
    ADAPT_DIRECT(getpagesize),

    ADAPT_TO(environ, &environ),
    ADAPT_TO(__environ, &environ),
    ADAPT_TO(ftruncate64, ftruncate),

};

void register_adapters_sys() { REGISTER_ADAPTERS_BY_CLASS(sys); }
