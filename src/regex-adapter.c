#include <errno.h>
#include <regex.h>

#include "adapter-register.h"


static struct glibc_adapter_t regex_adapters[] = {
    ADAPT_DIRECT(regcomp),
    ADAPT_DIRECT(regerror),
    ADAPT_DIRECT(regexec),
    ADAPT_DIRECT(regfree),
};

void register_adapters_regex() { REGISTER_ADAPTERS_BY_CLASS(regex); }
