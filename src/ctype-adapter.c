#include <ctype.h>
#include <errno.h>

#include "adapter-register.h"
#include "ctype-locale.h"

static int32_t** adapt___ctype_tolower_loc(void) {
    static int32_t* loc = (int32_t*)_nl_C_LC_CTYPE_tolower + 128;
    errno = ENOTSUP;
    adapter_log("__ctype_tolower_loc not support");
    return &loc;
}

static int32_t** adapt___ctype_toupper_loc(void) {
    static int32_t* loc = (int32_t*)_nl_C_LC_CTYPE_toupper + 128;
    errno = ENOTSUP;
    adapter_log("__ctype_toupper_loc not support");
    return &loc;
}

static unsigned short int** adapt___ctype_b_loc(void) {
    static unsigned short int* loc = (unsigned short*)(_nl_C_LC_CTYPE_class) + 128;
    errno = ENOTSUP;
    adapter_log("__ctype_b_loc not support");
    return &loc;
}

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

    ADAPT_INDIRECT(__ctype_tolower_loc),
    ADAPT_INDIRECT(__ctype_toupper_loc),
    ADAPT_INDIRECT(__ctype_b_loc),
};

void register_adapters_ctype() { REGISTER_ADAPTERS_BY_CLASS(ctype); }
