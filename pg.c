#include "postgres.h"

#include "nodes/bitmapset.h"
#include "nodes/pg_list.h"

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

static int list_cmp_int(const void *a, const void *b) {
  return lfirst_int(*(const ListCell **)a) - lfirst_int(*(const ListCell **)b);
}

long long pg_list_sort(void) {
  List *a = NIL;
  for (int i = 0; i < NNN * 100; i++) {
    a = lappend_int(a, NNN * 100 - i);
  }

  TIME_START;
  list_qsort(a, list_cmp_int);
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

  TIME_START
  for (int i = 0; i < NNN / 10; ++i) {
    a = _pg_list_append(a);
  }
  TIME_END;

  TIME_RETURN;
}
