#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>

#include "adapter-register.h"

char *adapt_index(const char *s, int c) {
  (void)s;
  (void)c;
  errno = ENOTSUP;
  adapter_log("Not support index");
  return NULL;
}

char *adapt_rindex(const char *s, int c) {
  (void)s;
  (void)c;
  errno = ENOTSUP;
  adapter_log("Not support rindex");
  return NULL;
}

static int adapt_bcmp(const void *s1, const void *s2, size_t n) {
  return memcmp(s1, s2, n);
}

static void adapt_bcopy(const void *src, void *dest, size_t n) {
  memcpy(dest, src, n);
}

static void adapt_bzero(void *s, size_t n) { memset(s, 0, n); }

static struct glibc_adapter_t string_adapters[] = {
    /* string.h */
    ADAPT_DIRECT(memccpy),
    ADAPT_DIRECT(memchr),
    ADAPT_DIRECT(memrchr),
    ADAPT_DIRECT(memcmp),
    ADAPT_DIRECT(memcpy),
    ADAPT_DIRECT(memmove),
    ADAPT_DIRECT(memset),
    ADAPT_DIRECT(memmem),
    ADAPT_DIRECT(stpcpy),
    ADAPT_DIRECT(stpncpy),
    ADAPT_DIRECT(strchr),
    ADAPT_DIRECT(strrchr),
    ADAPT_DIRECT(strlen),
    ADAPT_DIRECT(strcmp),
    ADAPT_DIRECT(strcpy),
    ADAPT_DIRECT(strcat),
    ADAPT_DIRECT(strcasecmp),
    ADAPT_DIRECT(strncasecmp),
    ADAPT_DIRECT(strdup),
    ADAPT_DIRECT(strstr),
    ADAPT_DIRECT(strtok),
    ADAPT_DIRECT(strtok_r),
    ADAPT_DIRECT(strerror),
    ADAPT_DIRECT(strerror_r),
    ADAPT_DIRECT(strnlen),
    ADAPT_DIRECT(strncat),
    ADAPT_DIRECT(strndup),
    ADAPT_DIRECT(strncmp),
    ADAPT_DIRECT(strncpy),
    ADAPT_DIRECT(strcspn),
    ADAPT_DIRECT(strpbrk),
    ADAPT_DIRECT(strsep),
    ADAPT_DIRECT(strspn),
    ADAPT_DIRECT(strsignal),
    ADAPT_DIRECT(strcoll),
    ADAPT_DIRECT(strxfrm),

    /* strings.h */
    ADAPT_INDIRECT(index),
    ADAPT_INDIRECT(rindex),
    ADAPT_INDIRECT(bcmp),
    ADAPT_INDIRECT(bcopy),
    ADAPT_INDIRECT(bzero),
    ADAPT_DIRECT(ffs),
};

void register_adapters_string() { REGISTER_ADAPTERS_BY_CLASS(string); }
