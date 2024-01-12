#include <errno.h>
#include <stdlib.h>

#include "adapter-register.h"

// NOTE: all interfaces of memory operations are in malloc-adapter.c

static struct glibc_adapter_t stdlib_adapters[] = {ADAPT_DIRECT(system)};

void register_adapters_stdlib() { REGISTER_ADAPTERS_BY_CLASS(stdlib); }
