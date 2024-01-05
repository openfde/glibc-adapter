#include "glibc-adapter.h"

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>

typedef int (*open_t)(const char *, int, mode_t);
typedef int (*printf_t)(const char *fmt, ...);

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

  return 0;
}