#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include <wchar.h>

#include "adapter-register.h"

char* adapt_index(const char* s, int c) {
  (void)s;
  (void)c;
  errno = ENOTSUP;
  adapter_log("Not support index");
  return NULL;
}

char* adapt_rindex(const char* s, int c) {
  (void)s;
  (void)c;
  errno = ENOTSUP;
  adapter_log("Not support rindex");
  return NULL;
}

static int adapt_bcmp(const void* s1, const void* s2, size_t n) { return memcmp(s1, s2, n); }

static void adapt_bcopy(const void* src, void* dest, size_t n) { memcpy(dest, src, n); }

static void adapt_bzero(void* s, size_t n) { memset(s, 0, n); }

static void* adapt_mesa_memmove(void* dest, const void* src, size_t n) {
  // adapter_logv("mesa_memmove(%p, %p, %lu)\n", dest, src, n);
  return memmove(dest, src, n);
}

static void* adapt_mesa_memset(void* s, int c, size_t n) {
  // adapter_logv("mesa_memset(%p, %i, %lu)\n", s, c, n);
  return memset(s, c, n);
}

static void* adapt_mesa_memcpy(void* dest, const void* src, size_t n) {
  // adapter_logv("mesa_memcpy(%p, %p, %lu)\n", dest, src, n);
  return memcpy(dest, src, n);
}

static void* adapt_rawmemchr(const void* s, int c) { return memchr(s, c, -1); }

static void* adapt_mempcpy(void* dest, const void* src, size_t n) {
  // adapter_logv("__memcpy %p, %p, %d", dest, src, n);
  memcpy(dest, src, n);
  return (char*)dest + n;
}

static void* direct_memcpy(void* dest, const void* src, size_t n) {
  // adapter_logv("memcpy %p, %p, %d", dest, src, n);
  memcpy(dest, src, n);
  return (char*)dest + n;
}

extern void* __memcpy_chk(void* dest, const void* src, size_t len, size_t destlen);
static void* adapt___memcpy_chk(void* dest, const void* src, size_t len, size_t destlen) {
  // adapter_logv("__memcpy_chk %p, %p, %d-%d", dest, src, len, destlen);
  return __memcpy_chk(dest, src, len, destlen);
}

extern char* __strcpy_chk(char* dest, const char* src, size_t destlen);
static char* adapt___strcpy_chk(char* dest, const char* src, size_t destlen) {
  return __strcpy_chk(dest, src, destlen);
}

extern char* strchrnul(const char* s, int c);

static struct glibc_adapter_t string_adapters[] = {
    /* string.h */
    ADAPT_DIRECT(memccpy),
    ADAPT_DIRECT(memchr),
    ADAPT_DIRECT(memrchr),
    ADAPT_DIRECT(memcmp),
    ADAPT_DIRECT(memcpy),
    // ADAPT_TO(memcpy, direct_memcpy),

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

    ADAPT_INDIRECT(__strcpy_chk),

    ADAPT_INDIRECT(mesa_memmove),
    ADAPT_INDIRECT(mesa_memset),
    ADAPT_INDIRECT(mesa_memcpy),

    ADAPT_INDIRECT(rawmemchr),
    ADAPT_INDIRECT(mempcpy),
    ADAPT_TO(__mempcpy, adapt_mempcpy),
    ADAPT_INDIRECT(__memcpy_chk),

    ADAPT_DIRECT(strcoll),
    ADAPT_DIRECT(strcoll_l),
    ADAPT_TO(__strcoll_l, strcoll_l),
    ADAPT_DIRECT(strxfrm),
    ADAPT_DIRECT(strxfrm_l),
    ADAPT_TO(__strxfrm_l, strxfrm_l),

    ADAPT_DIRECT(strftime),
    ADAPT_DIRECT(strftime_l),
    ADAPT_TO(__strftime_l, strftime_l),

    ADAPT_DIRECT(wcsftime),
    ADAPT_DIRECT(wcsftime_l),
    ADAPT_TO(__wcsftime_l, wcsftime_l),

    ADAPT_DIRECT(wcscoll),
    ADAPT_DIRECT(wcscoll_l),
    ADAPT_TO(__wcscoll_l, wcscoll_l),

    ADAPT_DIRECT(wcsxfrm),
    ADAPT_DIRECT(wcsxfrm_l),
    ADAPT_TO(__wcsxfrm_l, wcsxfrm_l),

    ADAPT_DIRECT(strtoimax),
    ADAPT_DIRECT(strtoumax),

    ADAPT_DIRECT(strchrnul),

    /* strings.h */
    ADAPT_INDIRECT(index),
    ADAPT_INDIRECT(rindex),
    ADAPT_INDIRECT(bcmp),
    ADAPT_INDIRECT(bcopy),
    ADAPT_INDIRECT(bzero),
    ADAPT_DIRECT(ffs),

    // time.h
    ADAPT_DIRECT(nanosleep),
};

void register_adapters_string() { REGISTER_ADAPTERS_BY_CLASS(string); }
