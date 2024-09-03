#include <errno.h>
#include <iconv.h>
#include <locale.h>
#include <string.h>

#include "adapter-register.h"

static locale_t adapt_newlocale(int category_mask, const char* locale, locale_t base) {
  locale_t new_locale = NULL;
  int mask = category_mask;
  if (category_mask & (1 << LC_ALL)) {
    mask = LC_ALL_MASK;
  }
  new_locale = newlocale(mask, locale, base);
  if (new_locale) {
    adapter_logv("newlocale %p category mask 0x%x locale '%s', base %p", new_locale, category_mask,
                 locale, base);
  } else {
    adapter_log("newlocale category mask 0x%x locale '%s', base %p, %s", category_mask, locale,
                base, strerror(errno));
  }

  return new_locale;
}

static void adapt_freelocale(locale_t locobj) {
  adapter_logv("freelocale %p", locobj);
  freelocale(locobj);
}

static locale_t adapt_duplocale(locale_t locobj) {
  locale_t dup_locale = duplocale(locobj);
  if (dup_locale) {
    adapter_logv("duplocale %p from %p", dup_locale, locobj);
  } else {
    adapter_log("duplocale from %p, %s", locobj, strerror(errno));
  }

  return dup_locale;
}

static locale_t adapt_uselocale(locale_t newloc) {
  locale_t prev_locale = uselocale(newloc);
  adapter_logv("uselocale %p previous locale %p", newloc, prev_locale);
  return prev_locale;
}

static struct glibc_adapter_t locale_adapters[] = {
    ADAPT_INDIRECT(newlocale),  ADAPT_TO(__newlocale, adapt_newlocale),
    ADAPT_INDIRECT(freelocale), ADAPT_TO(__freelocale, adapt_freelocale),
    ADAPT_INDIRECT(duplocale),  ADAPT_TO(__duplocale, adapt_duplocale),
    ADAPT_INDIRECT(uselocale),  ADAPT_TO(__uselocale, adapt_uselocale),
    ADAPT_DIRECT(localeconv),   ADAPT_DIRECT(setlocale),

    ADAPT_DIRECT(iconv),        ADAPT_DIRECT(iconv_open),
    ADAPT_DIRECT(iconv_close),
};

void register_adapters_locale() { REGISTER_ADAPTERS_BY_CLASS(locale); }
