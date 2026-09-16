#include <errno.h>
#include <pthread.h>
#include <stdint.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>

#include "adapter-register.h"


#define mutex_kind_index_in_64bits_machine 4
static pthread_mutex_t g_pthread_mutex_lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t* get_or_initialize_real_mutex(pthread_mutex_t* mutex) {
  assert(mutex);
  pthread_mutex_t* real_mutex = (pthread_mutex_t*)*(uintptr_t*)mutex;
  if (!real_mutex) {
    pthread_mutex_lock(&g_pthread_mutex_lock);
    real_mutex = (pthread_mutex_t*)*(uintptr_t*)mutex;
    if (!real_mutex) {
      // the kind has the same layout in aarch64 and x86_64
      int mutex_kind = ((int*)mutex)[mutex_kind_index_in_64bits_machine];
      // use PTHREAD_MUTEX_INITIALIZER to initialize mutex by user.
      // warning: memory leak if user not call pthread_mutex_destory.
      pthread_mutex_t* native_mutex = malloc(sizeof(pthread_mutex_t));
      assert(native_mutex);
      pthread_mutexattr_t attr;
      pthread_mutexattr_init(&attr);
      pthread_mutexattr_settype(&attr, mutex_kind);
      if (pthread_mutex_init(native_mutex, &attr) == 0) {
        *((uintptr_t*)mutex) = (uintptr_t)native_mutex;
        real_mutex = native_mutex;
      } else {
        free(native_mutex);
      }
    }
    pthread_mutex_unlock(&g_pthread_mutex_lock);
  }
  return real_mutex;
}

static pthread_mutex_t g_pthread_cond_lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t* get_or_initialize_real_cond(pthread_cond_t* cond) {
  assert(cond);
  pthread_cond_t* real_cond = (pthread_cond_t*)*(uintptr_t*)cond;
  if (!real_cond) {
    pthread_mutex_lock(&g_pthread_cond_lock);
    real_cond = (pthread_cond_t*)*(uintptr_t*)cond;
    if (!real_cond) {
      // use PTHREAD_COND_INITIALIZER to initialize mutex by user.
      // warning: memory leak if user not call pthread_cond_destory.
      pthread_cond_t* native_cond = malloc(sizeof(pthread_cond_t));
      assert(native_cond);
      pthread_condattr_t attr;
      pthread_condattr_init(&attr);
      if (pthread_cond_init(native_cond, &attr) == 0) {
        *((uintptr_t*)cond) = (uintptr_t)native_cond;
        real_cond = native_cond;
      } else {
        free(native_cond);
      }
    }
    pthread_mutex_unlock(&g_pthread_cond_lock);
  }
  return real_cond;
}

static pthread_mutex_t g_pthread_rwlock_lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_rwlock_t* get_or_initialize_real_rwlock(pthread_rwlock_t* rwlock) {
  assert(rwlock);
  pthread_rwlock_t* real_rwlock = (pthread_rwlock_t*)*(uintptr_t*)rwlock;
  if (!real_rwlock) {
    pthread_mutex_lock(&g_pthread_rwlock_lock);
    real_rwlock = (pthread_rwlock_t*)*(uintptr_t*)rwlock;
    if (!real_rwlock) {
      // use PTHREAD_RWLOCK_INITIALIZER to initialize mutex by user.
      // warning: memory leak if user not call pthread_rwlock_destory.
      pthread_rwlock_t* native_rwlock = malloc(sizeof(pthread_rwlock_t));
      assert(native_rwlock);
      pthread_rwlockattr_t attr;
      pthread_rwlockattr_init(&attr);
      if (pthread_rwlock_init(native_rwlock, &attr) == 0) {
        *((uintptr_t*)rwlock) = (uintptr_t)native_rwlock;
        real_rwlock = native_rwlock;
      } else {
        free(native_rwlock);
      }
    }
    pthread_mutex_unlock(&g_pthread_rwlock_lock);
  }
  return real_rwlock;
}

static int adapt_pthread_create(pthread_t* thread, const pthread_attr_t* attr,
    void* (*start_routine) (void*), void* arg) {
    pthread_attr_t* real_attr = NULL;
    if (attr != NULL) {
        real_attr = (pthread_attr_t*)*(uintptr_t*)attr;
        assert(real_attr != NULL);
    }
    return pthread_create(thread, real_attr, start_routine, arg);
}

static int adapter_register_atfork(void (*__prepare)(void), void (*__parent)(void),
                                 void(*__child)(void), void* dso)
{
    return pthread_atfork(__prepare, __parent, __child);
}

static int adapt_pthread_kill(pthread_t thread, int sig) {
    if (thread == 0) {
        return ESRCH;
    }
    return pthread_kill(thread, sig);
}


static int adapt_pthread_attr_init(pthread_attr_t* attr) {
    assert(attr != NULL);
    pthread_attr_t* real_attr = malloc(sizeof(pthread_attr_t));
    *(uintptr_t*)attr = (uintptr_t)real_attr;
    return pthread_attr_init(real_attr);
}

static int adapt_pthread_attr_destroy(pthread_attr_t* attr) {
    assert(attr != NULL);
    pthread_attr_t* real_attr = (pthread_attr_t*)*(uintptr_t*)attr;
    int ret = pthread_attr_destroy(real_attr);
    free(real_attr);
    return ret;
}

static int adapt_pthread_attr_setdetachstate(pthread_attr_t* attr, int detachstate) {
    pthread_attr_t* real_attr = (pthread_attr_t*)*(uintptr_t*)attr;
    return pthread_attr_setdetachstate(real_attr, detachstate);
}

static int adapt_pthread_attr_getdetachstate(const pthread_attr_t* attr,
    int* detachstate) {
    pthread_attr_t* real_attr = (pthread_attr_t*)*(uintptr_t*)attr;
    return pthread_attr_getdetachstate(real_attr, detachstate);
}

static int adapt_pthread_attr_setschedpolicy(pthread_attr_t* attr, int policy) {
    pthread_attr_t* real_attr = (pthread_attr_t*)*(uintptr_t*)attr;
    return pthread_attr_setschedpolicy(real_attr, policy);
}

static int adapt_pthread_attr_getschedpolicy(const pthread_attr_t* attr, int* policy) {
    pthread_attr_t* real_attr = (pthread_attr_t*)*(uintptr_t*)attr;
    return pthread_attr_getschedpolicy(real_attr, policy);
}


static int adapt_pthread_attr_setschedparam(pthread_attr_t* attr,
    const struct sched_param* param) {
    pthread_attr_t* real_attr = (pthread_attr_t*)*(uintptr_t*)attr;
    return pthread_attr_setschedparam(real_attr, param);
}

static int adapt_pthread_attr_getschedparam(const pthread_attr_t* attr,
    struct sched_param* param) {
    pthread_attr_t* real_attr = (pthread_attr_t*)*(uintptr_t*)attr;
    return pthread_attr_getschedparam(real_attr, param);
}

static int adapt_pthread_attr_setstacksize(pthread_attr_t* attr, size_t stacksize) {
    pthread_attr_t* real_attr = (pthread_attr_t*)*(uintptr_t*)attr;
    return pthread_attr_setstacksize(real_attr, stacksize);
}

static int adapt_pthread_attr_getstacksize(const pthread_attr_t* attr,
    size_t* stacksize) {
    pthread_attr_t* real_attr = (pthread_attr_t*)*(uintptr_t*)attr;
    return pthread_attr_getstacksize(real_attr, stacksize);
}

static int adapt_pthread_attr_setstackaddr(pthread_attr_t* attr, void* stackaddr) {
    pthread_attr_t* real_attr = (pthread_attr_t*)*(uintptr_t*)attr;
    size_t stacksize = 0;
    int ret = pthread_attr_getstack(real_attr, stackaddr, &stacksize);
    if (ret != 0) {
        return ret;
    }
    return pthread_attr_setstack(real_attr, stackaddr, stacksize);
}

static int adapt_pthread_attr_getstackaddr(const pthread_attr_t* attr,
    void** stackaddr) {
    pthread_attr_t* real_attr = (pthread_attr_t*)*(uintptr_t*)attr;
    return pthread_attr_getstack(real_attr, stackaddr, NULL);
}

static int adapt_pthread_attr_setstack(pthread_attr_t* attr,
    void* stackaddr, size_t stacksize) {
    pthread_attr_t* real_attr = (pthread_attr_t*)*(uintptr_t*)attr;
    return pthread_attr_setstack(real_attr, stackaddr, stacksize);
}

static int adapt_pthread_attr_getstack(const pthread_attr_t* attr,
    void** stackaddr, size_t* stacksize) {
    pthread_attr_t* real_attr = (pthread_attr_t*)*(uintptr_t*)attr;
    return pthread_attr_getstack(real_attr, stackaddr, stacksize);
}

static int adapt_pthread_attr_setguardsize(pthread_attr_t* attr, size_t guardsize) {
    pthread_attr_t* real_attr = (pthread_attr_t*)*(uintptr_t*)attr;
    return pthread_attr_setguardsize(real_attr, guardsize);
}

static int adapt_pthread_attr_getguardsize(const pthread_attr_t* attr,
    size_t* guardsize) {
    pthread_attr_t* real_attr = (pthread_attr_t*)*(uintptr_t*)attr;
    return pthread_attr_getguardsize(real_attr, guardsize);
}

static int adapt_pthread_attr_setscope(pthread_attr_t* attr, int scope) {
    pthread_attr_t* real_attr = (pthread_attr_t*)*(uintptr_t*)attr;
    return pthread_attr_setscope(real_attr, scope);
}

static int adapt_pthread_attr_getscope(const pthread_attr_t* attr, int* scope) {
    pthread_attr_t* real_attr = (pthread_attr_t*)*(uintptr_t*)attr;
    return pthread_attr_getscope(real_attr, scope);
}

static int adapt_pthread_getattr_np(pthread_t thread, pthread_attr_t* attr) {
    assert(attr != NULL);
    pthread_attr_t* real_attr = malloc(sizeof(pthread_attr_t));
    *(uintptr_t*)attr = (uintptr_t)real_attr;
    return pthread_getattr_np(thread, real_attr);
}

static int adapt_pthread_mutex_init(pthread_mutex_t* restrict mutex,
                                    const pthread_mutexattr_t* restrict attr) {
  pthread_mutex_t* real_mutex = NULL;
  int pshared = PTHREAD_PROCESS_PRIVATE;
  int ret = EINVAL;
  if (attr != NULL) {
    pthread_mutexattr_getpshared(attr, &pshared);
  }
  if (pshared == PTHREAD_PROCESS_PRIVATE) {
    real_mutex = malloc(sizeof(pthread_mutex_t));
    ret = pthread_mutex_init(real_mutex, attr);
    if (ret == 0) {
      *((uintptr_t*)mutex) = (uintptr_t)real_mutex;
    } else if (real_mutex) {
      free(real_mutex);
    }
  } else {
    adapter_log("pthread_mutex_init: Not support process shared mutex!");
    assert(0);
  }
  return ret;
}

static int adapt_pthread_mutex_destroy(pthread_mutex_t* mutex) {
    int ret = 0;
    if (mutex == NULL) {
        return EINVAL;
    }
    pthread_mutex_t* real_mutex = (pthread_mutex_t*)*(uintptr_t*)mutex;
    if (real_mutex == NULL) {
        return EINVAL;
    }
    ret = pthread_mutex_destroy(real_mutex);
    free(real_mutex);
    *((uintptr_t*)mutex) = 0;
    return ret;
}

static int adapt_pthread_mutex_lock(pthread_mutex_t* mutex) {
    if (!mutex) {
        return 0;
    }
    pthread_mutex_t* real_mutex = get_or_initialize_real_mutex(mutex);
    if (!real_mutex) {
        return EAGAIN;
    }
    return pthread_mutex_lock(real_mutex);
}

static int adapt_pthread_mutex_unlock(pthread_mutex_t* mutex) {
    if (mutex == NULL) {
        return 0;
    }
    pthread_mutex_t* real_mutex = (pthread_mutex_t*)*(uintptr_t*)mutex;
    if (!real_mutex) {
        return 0;
    }
    return pthread_mutex_unlock(real_mutex);
}

static int adapt_pthread_mutex_trylock(pthread_mutex_t* mutex) {
    if (!mutex) {
        return 0;
    }
    pthread_mutex_t* real_mutex = get_or_initialize_real_mutex(mutex);
    if (!real_mutex) {
        return EAGAIN;
    }
    return pthread_mutex_trylock(real_mutex);
}

static int adapt_pthread_mutex_timedlock(pthread_mutex_t* restrict mutex,
    const struct timespec* restrict abstime) {
    if (!mutex) {
        return 0;
    }
    pthread_mutex_t* real_mutex = get_or_initialize_real_mutex(mutex);
    if (!real_mutex) {
        return EAGAIN;
    }
    return pthread_mutex_timedlock(real_mutex, abstime);
}

static int adapt_pthread_cond_init(pthread_cond_t* restrict cond,
    const pthread_condattr_t* restrict attr) {
    pthread_cond_t* real_cond = NULL;
    int pshared = PTHREAD_PROCESS_PRIVATE;
    int ret = EINVAL;
    if (attr != NULL) {
        pthread_condattr_getpshared(attr, &pshared);
    }
    if (pshared == PTHREAD_PROCESS_PRIVATE) {
        real_cond = malloc(sizeof(pthread_cond_t));
        ret = pthread_cond_init(real_cond, attr);
        if (ret == 0) {
          *((uintptr_t*)cond) = (uintptr_t)real_cond;
        } else if (real_cond) {
          free(real_cond);
        }
    }
    else {
        adapter_log("pthread_cond_init: Not support process shared conditional variables!");
        assert(0);
    }
    return ret;
}

static int adapt_pthread_cond_destroy(pthread_cond_t* cond) {
    int ret = 0;
    if (cond == NULL) {
        return EINVAL;
    }
    pthread_cond_t* real_cond = (pthread_cond_t*)*(uintptr_t*)cond;
    if (real_cond == NULL) {
        return EINVAL;
    }
    ret = pthread_cond_destroy(real_cond);
    free(real_cond);
    *((uintptr_t*)cond) = 0;
    return ret;
}

static int adapt_pthread_cond_broadcast(pthread_cond_t* cond) {
    assert(cond);
    pthread_cond_t* real_cond = get_or_initialize_real_cond(cond);
    assert(real_cond);
    return pthread_cond_broadcast(real_cond);
}

static int adapt_pthread_cond_signal(pthread_cond_t* cond) {
    assert(cond);
    pthread_cond_t* real_cond = get_or_initialize_real_cond(cond);
    assert(real_cond);
    return pthread_cond_signal(real_cond);
}

static int adapt_pthread_cond_timedwait(pthread_cond_t* restrict cond,
    pthread_mutex_t* restrict mutex,
    const struct timespec* restrict abstime) {
    assert(cond);
    assert(mutex);
    pthread_cond_t* real_cond = get_or_initialize_real_cond(cond);
    pthread_mutex_t* real_mutex = get_or_initialize_real_mutex(mutex);
    assert(real_cond);
    assert(real_mutex);
    return pthread_cond_timedwait(real_cond, real_mutex, abstime);
}

static int adapt_pthread_cond_wait(pthread_cond_t* restrict cond,
    pthread_mutex_t* restrict mutex) {
    assert(cond);
    assert(mutex);
    pthread_cond_t* real_cond = get_or_initialize_real_cond(cond);
    pthread_mutex_t* real_mutex = get_or_initialize_real_mutex(mutex);
    assert(real_cond);
    assert(real_mutex);
    return pthread_cond_wait(real_cond, real_mutex);
}

int adapt_pthread_condattr_setpshared(pthread_condattr_t* attr,
    int pshared) {
    int ret = pthread_condattr_setpshared(attr, pshared);
    if (ret != 0 || pshared != PTHREAD_PROCESS_PRIVATE) {
        return ret;
    }
    // Bionic don't clear the bit before operating or pshared
    // The first bit represents pshared.
    (*attr) &= ~1;
    return 0;
}

int adapt_pthread_condattr_setclock(pthread_condattr_t* attr,
    clockid_t clock_id) {
    // Only support CLOCK_REALTIME and CLOCK_MONOTONIC clock ids.
    int ret = pthread_condattr_setclock(attr, clock_id);
    if (ret != 0 || clock_id != CLOCK_REALTIME) {
        return ret;
    }
    // Bionic don't clear the bit before operating or clock_id
    // The second bit represents clock id.
    (*attr) &= ~2;
    return 0;
}

static int adapt_pthread_rwlock_init(pthread_rwlock_t* restrict rwlock,
                                     const pthread_rwlockattr_t* restrict attr) {
  pthread_rwlock_t* real_rwlock = NULL;
  pthread_rwlockattr_t* real_attr = NULL;
  int pshared = PTHREAD_PROCESS_PRIVATE;
  int ret = EINVAL;
  if (attr != NULL) {
    real_attr = (pthread_rwlockattr_t*)*(uintptr_t*)attr;
    pthread_rwlockattr_getpshared(real_attr, &pshared);
  }
  if (pshared == PTHREAD_PROCESS_PRIVATE) {
    real_rwlock = malloc(sizeof(pthread_rwlock_t));
    ret = pthread_rwlock_init(real_rwlock, real_attr);
    if (ret == 0) {
      *((uintptr_t*)rwlock) = (uintptr_t)real_rwlock;
    } else if (real_rwlock) {
      free(real_rwlock);
    }
  } else {
    adapter_log("pthread_rwlock_init: Not support process shared lock!");
    assert(0);
  }
  return ret;
}

static int adapt_pthread_rwlock_destroy(pthread_rwlock_t* rwlock) {
    assert(rwlock);
    pthread_rwlock_t* real_rwlock = (pthread_rwlock_t*)*(uintptr_t*)rwlock;
    if (!real_rwlock) {
        return 0;
    }
    int ret = pthread_rwlock_destroy(real_rwlock);
    free(real_rwlock);
    *((uintptr_t*)rwlock) = 0;
    return ret;
}

static int adapt_pthread_rwlock_unlock(pthread_rwlock_t* rwlock) {
    assert(rwlock);
    pthread_rwlock_t* real_rwlock = (pthread_rwlock_t*)*(uintptr_t*)rwlock;
    if (!real_rwlock) {
        return 0;
    }
    return pthread_rwlock_unlock(real_rwlock);
}

static int adapt_pthread_rwlock_trywrlock(pthread_rwlock_t* rwlock) {
    pthread_rwlock_t* real_rwlock = get_or_initialize_real_rwlock(rwlock);
    assert(real_rwlock);
    return pthread_rwlock_trywrlock(real_rwlock);
}

static int adapt_pthread_rwlock_wrlock(pthread_rwlock_t* rwlock) {
    pthread_rwlock_t* real_rwlock = get_or_initialize_real_rwlock(rwlock);
    assert(real_rwlock);
    return pthread_rwlock_wrlock(real_rwlock);

}

static int adapt_pthread_rwlock_rdlock(pthread_rwlock_t* rwlock) {
    pthread_rwlock_t* real_rwlock = get_or_initialize_real_rwlock(rwlock);
    assert(real_rwlock);
    return pthread_rwlock_rdlock(real_rwlock);

}

static int adapt_pthread_rwlock_tryrdlock(pthread_rwlock_t* rwlock) {
    pthread_rwlock_t* real_rwlock = get_or_initialize_real_rwlock(rwlock);
    assert(real_rwlock);
    return pthread_rwlock_tryrdlock(real_rwlock);

}

static int adapt_pthread_rwlock_timedrdlock(pthread_rwlock_t* restrict rwlock,
    const struct timespec* restrict abs_timeout) {
    pthread_rwlock_t* real_rwlock = get_or_initialize_real_rwlock(rwlock);
    assert(real_rwlock);
    return pthread_rwlock_timedrdlock(real_rwlock, abs_timeout);
}

static int adapt_pthread_rwlock_timedwrlock(pthread_rwlock_t* restrict rwlock,
    const struct timespec* restrict abs_timeout) {
    pthread_rwlock_t* real_rwlock = get_or_initialize_real_rwlock(rwlock);
    assert(real_rwlock);
    return pthread_rwlock_timedwrlock(real_rwlock, abs_timeout);
}

static int adapt_pthread_rwlockattr_init(pthread_rwlockattr_t* attr) {
    pthread_rwlockattr_t* real_attr = malloc(sizeof(pthread_rwlockattr_t));
    *((uintptr_t*)attr) = (uintptr_t)real_attr;
    return pthread_rwlockattr_init(real_attr);
}

static int adapt_pthread_rwlockattr_destroy(pthread_rwlockattr_t* attr) {
    pthread_rwlockattr_t* real_attr = (pthread_rwlockattr_t*)*(uintptr_t*)attr;
    int ret = pthread_rwlockattr_destroy(real_attr);
    free(real_attr);
    return ret;
}

static int adapt_pthread_rwlockattr_getpshared(const pthread_rwlockattr_t
    * restrict attr, int* restrict pshared) {
    pthread_rwlockattr_t* real_attr = (pthread_rwlockattr_t*)*(uintptr_t*)attr;
    return pthread_rwlockattr_getpshared(real_attr, pshared);
}

static int adapt_pthread_rwlockattr_setpshared(pthread_rwlockattr_t* attr,
    int pshared) {
    pthread_rwlockattr_t* real_attr = (pthread_rwlockattr_t*)*(uintptr_t*)attr;
    return pthread_rwlockattr_setpshared(real_attr, pshared);
}

extern int pthread_setname_np(pthread_t thread, const char* name);
extern int pthread_getname_np(pthread_t thread, char* name, size_t len);

static struct glibc_adapter_t pthread_adapters[] = {
    ADAPT_INDIRECT(pthread_create),
    ADAPT_DIRECT(pthread_atfork),
    ADAPT_DIRECT(pthread_exit),
    ADAPT_INDIRECT(pthread_kill),
    ADAPT_DIRECT(pthread_join),
    ADAPT_DIRECT(pthread_detach),
    ADAPT_DIRECT(pthread_self),
    ADAPT_DIRECT(pthread_equal),
    ADAPT_DIRECT(pthread_getschedparam),
    ADAPT_DIRECT(pthread_setschedparam),
    ADAPT_DIRECT(pthread_once),
    ADAPT_DIRECT(pthread_key_create),
    ADAPT_DIRECT(pthread_key_delete),
    ADAPT_DIRECT(pthread_setspecific),
    ADAPT_DIRECT(pthread_getspecific),
    ADAPT_DIRECT(pthread_getcpuclockid),
    ADAPT_DIRECT(pthread_getname_np),
    ADAPT_DIRECT(pthread_setname_np),


    ADAPT_TO(__pthread_once, pthread_once),
    ADAPT_TO(__pthread_key_create, pthread_key_create),
    ADAPT_TO(__pthread_setspecific, pthread_setspecific),
    ADAPT_TO(__pthread_getspecific, pthread_getspecific),
    ADAPT_TO(__pthread_atfork, pthread_atfork),
    ADAPT_TO(__register_atfork, adapter_register_atfork),

    // thread attributes
    ADAPT_INDIRECT(pthread_attr_init),
    ADAPT_INDIRECT(pthread_attr_destroy),
    ADAPT_INDIRECT(pthread_attr_setdetachstate),
    ADAPT_INDIRECT(pthread_attr_getdetachstate),
    ADAPT_INDIRECT(pthread_attr_setschedpolicy),
    ADAPT_INDIRECT(pthread_attr_getschedpolicy),
    ADAPT_INDIRECT(pthread_attr_setschedparam),
    ADAPT_INDIRECT(pthread_attr_getschedparam),
    ADAPT_INDIRECT(pthread_attr_setstacksize),
    ADAPT_INDIRECT(pthread_attr_getstacksize),
    ADAPT_INDIRECT(pthread_attr_setstackaddr),
    ADAPT_INDIRECT(pthread_attr_getstackaddr),
    ADAPT_INDIRECT(pthread_attr_setstack),
    ADAPT_INDIRECT(pthread_attr_getstack),
    ADAPT_INDIRECT(pthread_attr_setguardsize),
    ADAPT_INDIRECT(pthread_attr_getguardsize),
    ADAPT_INDIRECT(pthread_attr_setscope),
    ADAPT_INDIRECT(pthread_attr_getscope),
    ADAPT_INDIRECT(pthread_getattr_np),

    // pthread mutex
    ADAPT_INDIRECT(pthread_mutex_init),
    ADAPT_INDIRECT(pthread_mutex_destroy),
    ADAPT_INDIRECT(pthread_mutex_lock),
    ADAPT_INDIRECT(pthread_mutex_unlock),
    ADAPT_INDIRECT(pthread_mutex_trylock),
    ADAPT_INDIRECT(pthread_mutex_timedlock),
    ADAPT_DIRECT(pthread_mutexattr_init),
    ADAPT_DIRECT(pthread_mutexattr_destroy),
    ADAPT_DIRECT(pthread_mutexattr_gettype),
    ADAPT_DIRECT(pthread_mutexattr_settype),
    ADAPT_DIRECT(pthread_mutexattr_getpshared),
    ADAPT_DIRECT(pthread_mutexattr_setpshared),
    ADAPT_DIRECT(pthread_mutexattr_getprotocol),
    ADAPT_DIRECT(pthread_mutexattr_setprotocol),

    // conditional variable
    ADAPT_INDIRECT(pthread_cond_init),
    ADAPT_INDIRECT(pthread_cond_destroy),
    ADAPT_INDIRECT(pthread_cond_broadcast),
    ADAPT_INDIRECT(pthread_cond_signal),
    ADAPT_INDIRECT(pthread_cond_wait),
    ADAPT_INDIRECT(pthread_cond_timedwait),
    ADAPT_DIRECT(pthread_condattr_init),
    ADAPT_DIRECT(pthread_condattr_destroy),
    ADAPT_DIRECT(pthread_condattr_getpshared),
    ADAPT_INDIRECT(pthread_condattr_setpshared),
    ADAPT_DIRECT(pthread_condattr_getclock),
    ADAPT_INDIRECT(pthread_condattr_setclock),

    // rwlock
    ADAPT_INDIRECT(pthread_rwlock_init),
    ADAPT_INDIRECT(pthread_rwlock_destroy),
    ADAPT_INDIRECT(pthread_rwlock_unlock),
    ADAPT_INDIRECT(pthread_rwlock_wrlock),
    ADAPT_INDIRECT(pthread_rwlock_rdlock),
    ADAPT_INDIRECT(pthread_rwlock_tryrdlock),
    ADAPT_INDIRECT(pthread_rwlock_trywrlock),
    ADAPT_INDIRECT(pthread_rwlock_timedrdlock),
    ADAPT_INDIRECT(pthread_rwlock_timedwrlock),
    ADAPT_INDIRECT(pthread_rwlockattr_init),
    ADAPT_INDIRECT(pthread_rwlockattr_destroy),
    ADAPT_INDIRECT(pthread_rwlockattr_setpshared),
    ADAPT_INDIRECT(pthread_rwlockattr_getpshared),
};

void register_adapters_pthread() { REGISTER_ADAPTERS_BY_CLASS(pthread); }
