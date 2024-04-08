#include "postgres.h"

#include "nodes/bitmapset.h"

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
