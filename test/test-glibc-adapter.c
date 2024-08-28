#include <assert.h>
#include <dlfcn.h>
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>

#include "glibc-adapter.h"

typedef int (*open_t)(const char *, int, mode_t);
typedef int (*printf_t)(const char *fmt, ...);

static const char *libtest_adapter = "libtest-glibc-adapter.so";

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    const void *printf_addr = find_symbol_adapter("printf");
    assert(printf_addr);
    printf_t print = (printf_t)printf_addr;

    print("%s OK\n", "printf");

    const void *open_addr = find_symbol_adapter("open");
    assert(open_addr);
    open_t open = (open_t)open_addr;

    int fd = open("/tmp/null", 0, 0);
    assert(fd == -1);
    fprintf(stdout, "fd %d, errno %d\n", fd, errno);

    // unittest
    if (argc > 1) {
        libtest_adapter = argv[1];
    }
    void *libtest = dlopen(libtest_adapter, RTLD_GLOBAL);
    if (!libtest) {
        fprintf(stdout, "dlopen(%s) : %s\n", libtest_adapter, dlerror());
        return -1;
    }
    void *test_func = dlsym(libtest, "test_all");
    if (!test_func) {
        fprintf(stdout, "dlsym(\"test_all\") : %s\n", dlerror());
        return -1;
    }
    void (*test_all)() = test_func;
    test_all();
    return 0;
}