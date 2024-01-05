#pragma once
#include <stdlib.h>

int adapter_log(const char* format, ...);

struct glibc_adapter_t {
  const char* symbol;
  void* adapt_fun;
};

#define ADAPT_DIRECT(symbol)  {#symbol, (void *)symbol}
#define ADAPT_INDIRECT(symbol)  {#symbol, (void *)adapt_##symbol}

int register_adapters(const char* classes, const struct glibc_adapter_t* adapters, size_t adapter_count);

#define ADAPTERS_SIZE(adapters) \
    (sizeof(adapters) / sizeof(adapters[0]))

#define REGISTER_ADAPTER_ARRAY(classes, adapters) \
  register_adapters(classes, (adapters), ADAPTERS_SIZE((adapters)))

#define REGISTER_ADAPTERS_BY_CLASS(classes) \
  REGISTER_ADAPTER_ARRAY(#classes, classes ## _adapters)
