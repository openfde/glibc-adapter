#include <errno.h>
#define _XOPEN_SOURCE
#include <wchar.h>
#include <wctype.h>

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

    ADAPT_DIRECT(wcwidth),
    ADAPT_DIRECT(wctrans),
    ADAPT_DIRECT(wctype),
    ADAPT_DIRECT(wctype_l),
    ADAPT_TO(__wctype_l, wctype_l),
    ADAPT_DIRECT(mbsrtowcs),
    ADAPT_DIRECT(mbsnrtowcs),
    ADAPT_DIRECT(mbsinit),
    ADAPT_DIRECT(mbrlen),
    ADAPT_DIRECT(mbrtowc),
    ADAPT_DIRECT(mbsrtowcs),
    ADAPT_DIRECT(wcsrtombs),

    ADAPT_DIRECT(wcscoll),
    ADAPT_DIRECT(wcscoll_l),
    ADAPT_TO(__wcscoll_l, wcscoll_l),

    ADAPT_DIRECT(towupper),
    ADAPT_DIRECT(towupper_l),
    ADAPT_TO(__towupper_l, towupper_l),

    ADAPT_DIRECT(towlower),
    ADAPT_DIRECT(towlower_l),
    ADAPT_TO(__towlower_l, towlower_l),

    ADAPT_DIRECT(towctrans),

    ADAPT_DIRECT(iswalnum),
    ADAPT_DIRECT(iswalpha),
    ADAPT_DIRECT(iswblank),
    ADAPT_DIRECT(iswcntrl),
    ADAPT_DIRECT(iswdigit),
    ADAPT_DIRECT(iswgraph),
    ADAPT_DIRECT(iswlower),
    ADAPT_DIRECT(iswprint),
    ADAPT_DIRECT(iswpunct),
    ADAPT_DIRECT(iswspace),
    ADAPT_DIRECT(iswupper),
    ADAPT_DIRECT(iswxdigit),
    ADAPT_DIRECT(iswctype),

    ADAPT_DIRECT(iswalnum_l),
    ADAPT_DIRECT(iswalpha_l),
    ADAPT_DIRECT(iswblank_l),
    ADAPT_DIRECT(iswcntrl_l),
    ADAPT_DIRECT(iswdigit_l),
    ADAPT_DIRECT(iswgraph_l),
    ADAPT_DIRECT(iswlower_l),
    ADAPT_DIRECT(iswprint_l),
    ADAPT_DIRECT(iswpunct_l),
    ADAPT_DIRECT(iswspace_l),
    ADAPT_DIRECT(iswupper_l),
    ADAPT_DIRECT(iswxdigit_l),
    ADAPT_DIRECT(iswctype_l),

    ADAPT_TO(__iswctype_l, iswctype_l),
};

void register_adapters_wchar() { REGISTER_ADAPTERS_BY_CLASS(wchar); }
