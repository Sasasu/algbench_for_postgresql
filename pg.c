#include "postgres.h"

#include "catalog/pg_operator_d.h"
#include "catalog/pg_type_d.h"
#include "common/hashfn.h"
#include "fmgr.h"
#include "nodes/bitmapset.h"
#include "nodes/pg_list.h"
#include "utils/hsearch.h"
#include "utils/memutils.h"
#include "utils/palloc.h"
#include "utils/tuplesort.h"

#include <stdbool.h>
#include <stdint.h>

#include "run.h"

static int x = 0;

static int _pg_bitmapset_compare(Bitmapset *a, Bitmapset *b) {
  return bms_compare(a, b);
}

long long pg_bitmapset_compare(void) {
  Bitmapset *a, *b;

  a = bms_make_singleton(10240);
  b = bms_make_singleton(10240);
  for (int i = 0; i < 10240; i += 3) {
    a = bms_add_member(a, i);
    b = bms_add_member(b, i);
  }

  TIME_START
  for (int i = 0; i < NNN; ++i) {
    x += _pg_bitmapset_compare(a, b);
  }
  TIME_END

  bms_free(a);
  bms_free(b);

  TIME_RETURN;
}

static int _pg_bitmapset_is_subset(Bitmapset *a, Bitmapset *b) {
  int r = 0;
  for (int i = 0; i < NNN; ++i) {
    r += bms_is_subset(a, b);
  }
  return r;
}

long long pg_bitmapset_is_subset(void) {
  Bitmapset *a, *b;

  a = bms_make_singleton(10240);
  b = bms_make_singleton(10240);
  for (int i = 0; i < 10240; i += 3) {
    a = bms_add_member(a, i);
    b = bms_add_member(b, i);
  }

  TIME_START
  x += _pg_bitmapset_is_subset(a, b);
  TIME_END

  bms_free(a);
  bms_free(b);

  TIME_RETURN;
}

static void _pg_bitmapset_add_member(Bitmapset *a) {
  for (int j = 0; j < NNN; ++j) {
    for (int i = 0; i < 1024; i += 3) {
      a = bms_add_member(a, i);
    }
  }
}

long long pg_bitmapset_add_member(void) {
  Bitmapset *a;

  a = bms_make_singleton(1024);

  TIME_START
  _pg_bitmapset_add_member(a);
  TIME_END

  bms_free(a);

  TIME_RETURN;
}

#if PG_VERSION_NUM >= 130000
#define list_qsort list_sort
static int list_cmp(const ListCell *p1, const ListCell *p2) {
  int v1 = lfirst_int(p1);
  int v2 = lfirst_int(p2);

  if (v1 < v2)
    return -1;
  if (v1 > v2)
    return 1;
  return 0;
}
#else
static int list_cmp(const void *a, const void *b) {
  return lfirst_int(*(const ListCell **)a) - lfirst_int(*(const ListCell **)b);
}
#endif

long long pg_list_sort(void) {
  List *a = NIL;
  for (int i = 0; i < NNN * 10; i++) {
    a = lappend_int(a, NNN * 10 - i);
  }

  TIME_START;
  list_qsort(a, list_cmp);
  TIME_END;

  TIME_RETURN;
}

static void _pg_list_iter(List *a) {
  ListCell *l;
  foreach (l, a) {
    x += lfirst_int(l);
  }
}

long long pg_list_iter(void) {
  List *a = NIL;
  for (int i = 0; i < 1024; i++) {
    a = lappend_int(a, i);
  }

  TIME_START;
  for (int i = 0; i < NNN; ++i) {
    _pg_list_iter(a);
  }
  TIME_END;

  TIME_RETURN;
}

static List *_pg_list_append(List *a) {
  for (int i = 0; i < 1024; i++) {
    a = lappend_int(a, i);
  }

  return a;
}

long long pg_list_append(void) {
  List *a = NIL;

  TIME_START;
  for (int i = 0; i < NNN / 10; ++i) {
    a = _pg_list_append(a);
  }
  TIME_END;

  TIME_RETURN;
}

static void _pg_hash_set(HTAB *htab) {
  for (int i = 0; i < NNN; ++i) {
    int64_t kv = ((int64_t)i << 32) + i;
    hash_search(htab, &kv, HASH_ENTER, NULL);
  }
}

long long pg_hash_set_alloc(void) {
  HASHCTL ctl = {
      .keysize = sizeof(int32_t),
      .entrysize = sizeof(int32_t),
      .hcxt = CurrentMemoryContext,
      .hash = tag_hash,
  };

  HTAB *htab =
      hash_create("htab", 0, &ctl, HASH_ELEM | HASH_CONTEXT | HASH_FUNCTION);

  TIME_START;
  _pg_hash_set(htab);
  TIME_END;

  TIME_RETURN;
}

long long pg_hash_set_noalloc(void) {
  HASHCTL ctl = {
      .keysize = sizeof(int32_t),
      .entrysize = sizeof(int32_t),
      .hcxt = CurrentMemoryContext,
      .hash = tag_hash,
  };

  HTAB *htab = hash_create("htab", NNN * 100, &ctl,
                           HASH_ELEM | HASH_CONTEXT | HASH_FUNCTION);

  TIME_START;
  _pg_hash_set(htab);
  TIME_END;

  TIME_RETURN;
}

long long pg_hash_get_rand(void) {
  HASHCTL ctl = {
      .keysize = sizeof(int32_t),
      .entrysize = sizeof(int32_t),
      .hcxt = CurrentMemoryContext,
      .hash = tag_hash,
  };
  HTAB *htab =
      hash_create("htab", NNN, &ctl, HASH_ELEM | HASH_CONTEXT | HASH_FUNCTION);

  _pg_hash_set(htab);

  TIME_START;
  for (int i = 0; i < NNN; ++i) {
    int64_t *p = hash_search(htab, &i, HASH_FIND, NULL);
    x += *p;
  }
  TIME_END;

  TIME_RETURN;
}

long long pg_hash_get_scan(void) {
  HASHCTL ctl = {
      .keysize = sizeof(int32_t),
      .entrysize = sizeof(int32_t),
      .hcxt = CurrentMemoryContext,
      .hash = tag_hash,
  };
  HTAB *htab =
      hash_create("htab", NNN, &ctl, HASH_ELEM | HASH_CONTEXT | HASH_FUNCTION);

  _pg_hash_set(htab);

  TIME_START;
  HASH_SEQ_STATUS s;
  uint64_t *t;
  hash_seq_init(&s, htab);
  while ((t = hash_seq_search(&s)) != NULL) {
    x += *t;
  }
  TIME_END;

  TIME_RETURN;
}

long long pg_hash_delete(void) {
  HASHCTL ctl = {
      .keysize = sizeof(int32_t),
      .entrysize = sizeof(int32_t),
      .hcxt = CurrentMemoryContext,
      .hash = tag_hash,
  };
  HTAB *htab =
      hash_create("htab", NNN, &ctl, HASH_ELEM | HASH_CONTEXT | HASH_FUNCTION);

  _pg_hash_set(htab);

  TIME_START;
  for (int i = 0; i < NNN; ++i) {
    hash_search(htab, &i, HASH_REMOVE, NULL);
  }

  TIME_END;

  TIME_RETURN;
}

static Datum sum1(PG_FUNCTION_ARGS) { PG_RETURN_INT32(PG_GETARG_INT32(0)); }
static Datum sum2(PG_FUNCTION_ARGS) {
  PG_RETURN_INT32(PG_GETARG_INT32(0) + PG_GETARG_INT32(2));
}
static Datum sum7(PG_FUNCTION_ARGS) {
  PG_RETURN_INT32(PG_GETARG_INT32(0) + PG_GETARG_INT32(1) + PG_GETARG_INT32(2) +
                  PG_GETARG_INT32(3) + PG_GETARG_INT32(4) + PG_GETARG_INT32(5) +
                  PG_GETARG_INT32(6));
}
static Datum sum9(PG_FUNCTION_ARGS) {
  PG_RETURN_INT32(PG_GETARG_INT32(0) + PG_GETARG_INT32(1) + PG_GETARG_INT32(2) +
                  PG_GETARG_INT32(3) + PG_GETARG_INT32(4) + PG_GETARG_INT32(5) +
                  PG_GETARG_INT32(6) + PG_GETARG_INT32(7) + PG_GETARG_INT32(8));
}

long long pg_functioncall_1(void) {
  Datum a = Int32GetDatum(1);

  TIME_START;
  for (int i = 0; i < NNN; ++i) {
    x += DirectFunctionCall1(sum1, a);
  }
  TIME_END;
  TIME_RETURN;
}

long long pg_functioncall_2(void) {
  Datum a = Int32GetDatum(1);

  TIME_START;
  for (int i = 0; i < NNN; ++i) {
    x += DirectFunctionCall2(sum2, a, a);
  }
  TIME_END;
  TIME_RETURN;
}

long long pg_functioncall_7(void) {
  Datum a = Int32GetDatum(1);

  TIME_START;
  for (int i = 0; i < NNN; ++i) {
    x += DirectFunctionCall7(sum7, a, a, a, a, a, a, a);
  }
  TIME_END;
  TIME_RETURN;
}

long long pg_functioncall_9(void) {
  Datum a = Int32GetDatum(1);

  TIME_START;
  for (int i = 0; i < NNN; ++i) {
    x += DirectFunctionCall9(sum9, a, a, a, a, a, a, a, a, a);
  }
  TIME_END;
  TIME_RETURN;
}

long long pg_memoryalloc_small(void) {
  MemoryContext c =
      AllocSetContextCreate(CurrentMemoryContext, "", ALLOCSET_DEFAULT_SIZES);
  void *prt[NNN] = {};

  MemoryContext old = MemoryContextSwitchTo(c);

  TIME_START;
  for (int i = 0; i < NNN; ++i) {
    prt[i] = palloc(i);
  }
  TIME_END;

  for (int i = 0; i < NNN; ++i) {
    pfree(prt[i]);
  }

  MemoryContextSwitchTo(old);
  TIME_RETURN;
}

long long pg_memoryalloc_big(void) {
  MemoryContext c =
      AllocSetContextCreate(CurrentMemoryContext, "", ALLOCSET_DEFAULT_SIZES);
  void *prt[NNN] = {};

  MemoryContext old = MemoryContextSwitchTo(c);

  TIME_START;
  for (int i = 0; i < NNN; ++i) {
    prt[i] = palloc(i * 100);
  }
  TIME_END;

  for (int i = 0; i < NNN; ++i) {
    pfree(prt[i]);
  }

  MemoryContextSwitchTo(old);
  TIME_RETURN;
}

long long pg_memoryalloc_free(void) {
  MemoryContext c =
      AllocSetContextCreate(CurrentMemoryContext, "", ALLOCSET_DEFAULT_SIZES);
  void *prt[NNN] = {};

  MemoryContext old = MemoryContextSwitchTo(c);

  for (int i = 0; i < NNN; ++i) {
    prt[i] = palloc(i);
  }

  TIME_START;
  for (int i = 0; i < NNN; ++i) {
    pfree(prt[i]);
  }
  TIME_END;

  MemoryContextSwitchTo(old);
  TIME_RETURN;
}

long long pg_memoryalloc_free_batch(void) {
  MemoryContext c =
      AllocSetContextCreate(CurrentMemoryContext, "", ALLOCSET_DEFAULT_SIZES);
  void *prt[NNN] = {};

  MemoryContext old = MemoryContextSwitchTo(c);

  for (int i = 0; i < NNN; ++i) {
    prt[i] = palloc(i);
  }

  MemoryContextSwitchTo(old);
  TIME_START;
  MemoryContextDelete(c);
  TIME_END;

  TIME_RETURN;
}

#ifndef TUPLESORT_RANDOMACCESS
#define TUPLESORT_RANDOMACCESS 1
#endif

long long pg_sort_qsort(void) {
  Tuplesortstate *state =
      tuplesort_begin_datum(INT4OID, Int8LessOperator, InvalidOid, false, 100,
                            NULL, TUPLESORT_RANDOMACCESS);

  for (int i = 0; i < NNN; ++i) {
    tuplesort_putdatum(state, Int32GetDatum(NNN - i), false);
  }

  TIME_START;
  tuplesort_performsort(state);
  TIME_END;

  TIME_RETURN;
}

long long pg_sort_cppsort(void) { return pg_sort_qsort(); }
