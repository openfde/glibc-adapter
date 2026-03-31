#pragma once
#include <stdlib.h>

// debug level
int adapter_log(const char* format, ...);
// verbose level
int adapter_logv(const char* format, ...);

struct glibc_adapter_t {
  const char* symbol;
  void* adapt_fun;
};

#define ADAPT_DIRECT(symbol)  {#symbol, (void *)symbol}
#define ADAPT_DIRECT_openat(symbol)  {#symbol, (void *)(int (*)(int, const char *, int, ...))symbol}
#define ADAPT_DIRECT_ioctl(symbol) {#symbol, (void*)(int (*)(int, int, ...))symbol}
#define ADAPT_INDIRECT(symbol)  {#symbol, (void *)adapt_##symbol}
#define ADAPT_TO_openat(symbol, hook)  {#symbol, (void *)(int (*)(int, const char *, int, ...))hook}
#define ADAPT_TO(symbol, hook)  {#symbol, (void *)hook}

int register_adapters(const char* classes, const struct glibc_adapter_t* adapters, size_t adapter_count);

#define ADAPTERS_SIZE(adapters) \
    (sizeof(adapters) / sizeof(adapters[0]))

#define REGISTER_ADAPTER_ARRAY(classes, adapters) \
  register_adapters(classes, (adapters), ADAPTERS_SIZE((adapters)))

#define REGISTER_ADAPTERS_BY_CLASS(classes) \
  REGISTER_ADAPTER_ARRAY(#classes, classes ## _adapters)
