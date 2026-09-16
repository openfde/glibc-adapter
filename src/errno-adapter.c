#include <errno.h>
#include <netdb.h>

#include "adapter-register.h"

static int *adapt___errno_location() { return __errno(); }

static int *adapt___h_errno_location() { return __get_h_errno(); }

extern const char* __progname;
extern int error_one_per_line;
extern unsigned int error_message_count;
static struct glibc_adapter_t errno_adapters[] = {
    ADAPT_INDIRECT(__errno_location),
    ADAPT_INDIRECT(__h_errno_location),
    ADAPT_TO(program_invocation_name, &__progname),
    ADAPT_TO(program_invocation_short_name, &__progname),
    ADAPT_TO(__progname_full, &__progname),
    ADAPT_TO(__progname, &__progname),
    ADAPT_TO(error_one_per_line, &error_one_per_line),
    ADAPT_TO(error_message_count, &error_message_count),
    ADAPT_DIRECT(freeaddrinfo),
    ADAPT_DIRECT(getaddrinfo),
    ADAPT_DIRECT(gethostbyname),
    ADAPT_DIRECT(gai_strerror),
};

void register_adapters_errno() { REGISTER_ADAPTERS_BY_CLASS(errno); }
