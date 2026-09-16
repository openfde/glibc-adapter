#include <errno.h>
#include <pwd.h>
#include <sys/file.h>
#include <sys/resource.h>
#include <sys/sysinfo.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/timerfd.h>
#include <sys/signalfd.h>
#include <sys/stat.h>
#include <sys/prctl.h>
#include <sys/vfs.h>
#include <sys/uio.h>
#include <sys/shm.h>
#include <sys/syscall.h>
#include <sys/utsname.h>
#include <sys/auxv.h>
#include <sys/statvfs.h>
#include <sys/eventfd.h>

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
    errno = EINVAL;
    return -1;
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

    ADAPT_DIRECT_ioctl(ioctl),
    ADAPT_DIRECT(poll),
    ADAPT_DIRECT(epoll_wait),
    ADAPT_DIRECT(epoll_ctl),
    ADAPT_DIRECT(epoll_create),
    ADAPT_DIRECT(send),
    ADAPT_DIRECT(connect),
    ADAPT_DIRECT(socket),
    ADAPT_DIRECT(getsockopt),
    ADAPT_DIRECT(setsockopt),
    ADAPT_DIRECT(getsockname),
    ADAPT_DIRECT(recvmsg),
    ADAPT_DIRECT(listen),
    ADAPT_DIRECT(accept),
    ADAPT_DIRECT(sendmsg),
    ADAPT_DIRECT(timerfd_settime),
    ADAPT_DIRECT(signalfd),
    ADAPT_DIRECT(shutdown),
    ADAPT_DIRECT(bind),
    ADAPT_DIRECT(sysinfo),
    ADAPT_DIRECT(clock_gettime),
    ADAPT_DIRECT(mkdir),
    ADAPT_DIRECT(chmod),
    ADAPT_DIRECT(uname),
    // ADAPT_DIRECT(umask),

    ADAPT_DIRECT(timerfd_create),
    ADAPT_DIRECT(epoll_create1),
    ADAPT_DIRECT(accept4),
    ADAPT_DIRECT(recv),
    ADAPT_DIRECT(prctl),
    ADAPT_DIRECT(statfs),
    ADAPT_DIRECT(getpeername),
    ADAPT_DIRECT(getrusage),
    ADAPT_DIRECT(readv),
    ADAPT_DIRECT(writev),
    ADAPT_DIRECT(shmdt),
    ADAPT_DIRECT(shmat),
    ADAPT_DIRECT(shmctl),
    ADAPT_DIRECT(syscall),
    ADAPT_DIRECT(sendto),
    ADAPT_DIRECT(recvfrom),
    ADAPT_DIRECT(getauxval),
    ADAPT_DIRECT(fstatfs),
    ADAPT_DIRECT(getpriority),
    ADAPT_DIRECT(fstatvfs),
    ADAPT_DIRECT(statvfs),

    ADAPT_DIRECT(fstat),
    ADAPT_TO(fstat64, fstat),
    ADAPT_DIRECT(stat),
    ADAPT_TO(stat64, stat),

    ADAPT_DIRECT(lstat),
    ADAPT_TO(lstat64, lstat),
    ADAPT_DIRECT(fstatat),
    ADAPT_TO(fstatat64, fstatat),

    ADAPT_DIRECT(eventfd),
    ADAPT_DIRECT(setpriority),
    ADAPT_DIRECT(mkfifoat),
    ADAPT_DIRECT(mknod),

};

void register_adapters_sys() { REGISTER_ADAPTERS_BY_CLASS(sys); }
