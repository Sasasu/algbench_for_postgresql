#pragma once

#include <time.h>
#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif

long long pg_bitmapset_compare(void);
long long pg_bitmapset_is_subset(void);
long long pg_bitmapset_add_member(void);

long long cpp_bitmapset_compare(void);
long long cpp_bitmapset_is_subset(void);
long long cpp_bitmapset_add_member(void);

long long pg_list_sort(void);
long long pg_list_iter(void);
long long pg_list_apppend(void);

long long cpp_list_sort(void);
long long cpp_list_iter(void);
long long cpp_list_apppend(void);

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
  sched_yield();                                                             \
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
