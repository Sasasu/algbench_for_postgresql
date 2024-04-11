#pragma once

#include <pthread.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

#define add(n)                                                                 \
  long long pg_##n(void);                                                      \
  long long cpp_##n(void);

	add(bitmapset_compare);
	add(bitmapset_is_subset);
	add(bitmapset_add_member);

	add(list_sort);
	add(list_iter);
	add(list_append);

#undef add
#ifdef __cplusplus
}
#endif

// hashtable
// functioncall
// memoryalloc
//
// regex

#define NNN 10000

#define TIME_START                                                             \
  sched_yield();                                                               \
  struct timespec __start = {}, __end = {};                                    \
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &__start);

#define TIME_END                                                               \
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &__end);                             \
  sched_yield();

#define TIME_RETURN                                                            \
  long long int __time_return = ((__end.tv_sec - __start.tv_sec) * 1e12);      \
  __time_return -= __start.tv_nsec;                                            \
  __time_return += __end.tv_nsec;                                              \
  return __time_return
