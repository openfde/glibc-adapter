#include <assert.h>
#include <dirent.h>
#include <errno.h>
#include <fnmatch.h>

#include "adapter-register.h"

static struct dirent* adapt_readdir(DIR* dirp) { return readdir(dirp); }

static struct dirent64* adapt_readdir64(DIR* dirp) { return readdir64(dirp); }

static int adapt_readdir_r(DIR* dirp, struct dirent* entry, struct dirent** result) {
  (void)dirp;
  (void)entry;
  (void)result;
  assert(0);
  errno = EOPNOTSUPP;
  return -1;
}

static int adapt_scandir(const char* dirp, struct dirent*** namelist,
                         int (*filter)(const struct dirent*),
                         int (*compar)(const struct dirent**, const struct dirent**)) {
  int ret = scandir(dirp, namelist, filter, compar);
  if (ret >= 0) {
    adapter_logv("scandir %s has %d files\n", dirp, ret);
  } else {
    adapter_log("scandir %s errno %d\n", dirp, errno);
  }
  return ret;
}

static int adapt_alphasort(const struct dirent** a, const struct dirent** b) {
  return alphasort(a, b);
}

static int adapt_alphasort64(const struct dirent** a, const struct dirent** b) {
  return alphasort(a, b);
}

static int adapt_versionsort(const struct dirent** a, const struct dirent** b) {
  (void)a;
  (void)b;
  assert(0);
  errno = EOPNOTSUPP;
  return -1;
}

static int adapt_fnmatch(const char* pattern, const char* string, int flags) {
  return fnmatch(pattern, string, flags);
}

static struct glibc_adapter_t dirent_adapters[] = {
    ADAPT_DIRECT(opendir),
    ADAPT_DIRECT(fdopendir),
    ADAPT_DIRECT(closedir),
    // ADAPT_DIRECT(__fsetlocking),
    ADAPT_INDIRECT(readdir),
    ADAPT_INDIRECT(readdir64),
    ADAPT_INDIRECT(readdir_r),
    ADAPT_DIRECT(rewinddir),
    ADAPT_DIRECT(seekdir),
    ADAPT_DIRECT(telldir),
    ADAPT_DIRECT(dirfd),
    ADAPT_INDIRECT(scandir),
    ADAPT_TO(scandir64, adapt_scandir),
    ADAPT_INDIRECT(alphasort),
    ADAPT_INDIRECT(alphasort64),
    ADAPT_INDIRECT(versionsort),
    ADAPT_INDIRECT(fnmatch),

    // ADAPT_DIRECT(mkdir),
    ADAPT_DIRECT(qsort),
};

void register_adapters_dirent() { REGISTER_ADAPTERS_BY_CLASS(dirent); }
