#include <errno.h>
#include <ctype.h>

#include "adapter-register.h"


static struct glibc_adapter_t ctype_adapters[] = {
    ADAPT_DIRECT(isalnum),
    ADAPT_DIRECT(isalpha),
    ADAPT_DIRECT(iscntrl),
    ADAPT_DIRECT(isdigit),
    ADAPT_DIRECT(isgraph),
    ADAPT_DIRECT(islower),
    ADAPT_DIRECT(isprint),
    ADAPT_DIRECT(ispunct),
    ADAPT_DIRECT(isspace),
    ADAPT_DIRECT(isupper),
    ADAPT_DIRECT(isxdigit),
    ADAPT_DIRECT(isascii),
    ADAPT_DIRECT(isblank),

    ADAPT_DIRECT(isalnum_l),
    ADAPT_DIRECT(isalpha_l),
    ADAPT_DIRECT(iscntrl_l),
    ADAPT_DIRECT(isdigit_l),
    ADAPT_DIRECT(isgraph_l),
    ADAPT_DIRECT(islower_l),
    ADAPT_DIRECT(isprint_l),
    ADAPT_DIRECT(ispunct_l),
    ADAPT_DIRECT(isspace_l),
    ADAPT_DIRECT(isupper_l),
    ADAPT_DIRECT(isxdigit_l),
    // ADAPT_DIRECT(isascii_l),

    ADAPT_DIRECT(toupper),
    ADAPT_DIRECT(tolower),

    ADAPT_DIRECT(toupper_l),
    ADAPT_DIRECT(tolower_l),
};

void register_adapters_ctype() { REGISTER_ADAPTERS_BY_CLASS(ctype); }
