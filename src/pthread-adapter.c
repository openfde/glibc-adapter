#include <errno.h>
#include <pthread.h>

#include "adapter-register.h"

static struct glibc_adapter_t pthread_adapters[] = {
    ADAPT_DIRECT(pthread_create),
    ADAPT_DIRECT(pthread_exit),
    ADAPT_DIRECT(pthread_kill),
    ADAPT_DIRECT(pthread_join),
    ADAPT_DIRECT(pthread_detach),
    ADAPT_DIRECT(pthread_self),
    ADAPT_DIRECT(pthread_equal),
    ADAPT_DIRECT(pthread_getschedparam),
    ADAPT_DIRECT(pthread_setschedparam),

    // thread attributes
    ADAPT_DIRECT(pthread_attr_init),
    ADAPT_DIRECT(pthread_attr_destroy),
};

void register_adapters_pthread() { REGISTER_ADAPTERS_BY_CLASS(pthread); }
