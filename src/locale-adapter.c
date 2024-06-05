#include <errno.h>
#include <locale.h>

#include "adapter-register.h"


static struct glibc_adapter_t locale_adapters[] = {
    ADAPT_DIRECT(newlocale),
    ADAPT_DIRECT(freelocale),
    ADAPT_DIRECT(duplocale),
    ADAPT_DIRECT(uselocale),
    ADAPT_TO(__uselocale, uselocale),
    ADAPT_DIRECT(localeconv),
    ADAPT_DIRECT(setlocale),
};

void register_adapters_locale() { REGISTER_ADAPTERS_BY_CLASS(locale); }
