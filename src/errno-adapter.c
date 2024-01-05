#include <errno.h>
#include <netdb.h>

#include "adapter-register.h"

static int *adapt___errno_location() { return __errno(); }

static int *adapt___h_errno_location() { return __get_h_errno(); }

static struct glibc_adapter_t errno_adapters[] = {
    ADAPT_INDIRECT(__errno_location),
    ADAPT_INDIRECT(__h_errno_location),
};

void register_adapters_errno() { REGISTER_ADAPTERS_BY_CLASS(errno); }
