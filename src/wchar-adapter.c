#include <errno.h>
#define _XOPEN_SOURCE
#include <wchar.h>

#include "adapter-register.h"


static wchar_t* adapt_wmempcpy(wchar_t* dest, const wchar_t* src, size_t n) {
  wmemcpy(dest, src, n);
  return dest + n;
}

static struct glibc_adapter_t wchar_adapters[] = {
    ADAPT_DIRECT(btowc),
    ADAPT_DIRECT(wmemchr),
    ADAPT_DIRECT(wmemcmp),
    ADAPT_DIRECT(wmemcpy),
    ADAPT_DIRECT(wmemmove),
    ADAPT_DIRECT(wmemset),
    ADAPT_INDIRECT(wmempcpy),

    ADAPT_DIRECT(wcrtomb),
    ADAPT_DIRECT(wcschr),
    ADAPT_DIRECT(wcscmp),
    ADAPT_DIRECT(wcscspn),
    ADAPT_DIRECT(wcsdup),
    ADAPT_DIRECT(wcslen),

    ADAPT_DIRECT(wcsncmp),
    ADAPT_DIRECT(wcsncpy),
    ADAPT_DIRECT(wcsnrtombs),
    ADAPT_DIRECT(wcsstr),
    ADAPT_DIRECT(wcstol),
    ADAPT_DIRECT(wcstombs),
    ADAPT_DIRECT(wctob),

    ADAPT_DIRECT(wctomb),
    ADAPT_DIRECT(wcwidth),
};

void register_adapters_wchar() { REGISTER_ADAPTERS_BY_CLASS(wchar); }
