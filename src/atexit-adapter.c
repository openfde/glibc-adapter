

#include "adapter-register.h"

#include <sched.h>
#include <mntent.h>
#include <signal.h>
#include <arpa/inet.h>
#include <langinfo.h>
#include <termios.h>
#include <getopt.h>
#include <syslog.h>
#include <err.h>

extern void __cxa_finalize(void* dso_handle);
static void adapt___cxa_finalize(void* dso_handle) {
  __cxa_finalize(dso_handle);
  return ;
}

extern int __cxa_atexit(void (*func)(void *), void *arg, void *dso);
static int adapt___cxa_atexit(void (*func)(void *), void *arg, void *dso) {
  return __cxa_atexit(func, arg, dso);
}

extern void __stack_chk_fail();
static void adapt___stack_chk_fail(void) {
  __stack_chk_fail();
  return ;
}


static struct glibc_adapter_t atexit_adapters[] = {
    ADAPT_INDIRECT(__cxa_finalize),
    ADAPT_INDIRECT(__cxa_atexit),
    ADAPT_INDIRECT(__stack_chk_fail),
    ADAPT_DIRECT(sched_setparam),
    ADAPT_DIRECT(sched_yield),
    ADAPT_DIRECT(setmntent),
    ADAPT_DIRECT(endmntent),
    ADAPT_DIRECT(hasmntopt),
    ADAPT_DIRECT(getmntent_r),
    ADAPT_DIRECT(sigemptyset),
    ADAPT_DIRECT(inet_addr),
    ADAPT_DIRECT(inet_ntop),
    ADAPT_DIRECT(inet_pton),
    ADAPT_DIRECT(sigaddset),
    ADAPT_DIRECT(nl_langinfo),
    ADAPT_DIRECT(sigfillset),
    ADAPT_DIRECT(tcgetattr),
    ADAPT_DIRECT(tcsetattr),
    ADAPT_DIRECT(getopt),
    ADAPT_DIRECT(syslog),
    ADAPT_DIRECT(kill),
    ADAPT_DIRECT(warnx),
    ADAPT_DIRECT(sigpending),
};

void register_adapters_atexit() { REGISTER_ADAPTERS_BY_CLASS(atexit); }
