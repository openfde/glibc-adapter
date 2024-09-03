#pragma once

#include <unistd.h>

// clang-format off
static const int sysconf_map[256] = {
    [30] = _SC_PAGE_SIZE,
    [70] = _SC_GETPW_R_SIZE_MAX,
    [80] = _SC_THREAD_PRIO_INHERIT,
    [81] = _SC_THREAD_PRIO_PROTECT,
    [82] = _SC_THREAD_PROCESS_SHARED,
    [83] = _SC_NPROCESSORS_CONF,
    [84] = _SC_NPROCESSORS_ONLN,
};

// clang-format on
