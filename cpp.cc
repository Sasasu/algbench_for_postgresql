#include "run.h"

#include <boost/dynamic_bitset.hpp>

using namespace std;
using namespace boost;

static int x = 0;

int _cpp_bitmapset_compare(dynamic_bitset<> &a, dynamic_bitset<> &b) {
  return a == b;
}

long long cpp_bitmapset_compare(void) {
  dynamic_bitset<> a(10240), b(10240);

  for (int i = 0; i < 10240; i += 3) {
    a[i] = true;
    b[i] = true;
  }

  TIME_START;
  for (int i = 0; i < NNN; ++i) {
    x += _cpp_bitmapset_compare(a, b);
  }
  TIME_END;

  TIME_RETURN;
}

int _cpp_bitmapset_is_subset(dynamic_bitset<> &a, dynamic_bitset<> &b) {
  int r = 0;
  for (int i = 0; i < NNN; ++i) {
    r += a.is_subset_of(b);
  }
  return r;
}

long long cpp_bitmapset_is_subset(void) {
  dynamic_bitset<> a(10240), b(10240);

  for (int i = 0; i < 10240; i += 3) {
    a[i] = true;
    b[i] = true;
  }

  TIME_START;
  x += _cpp_bitmapset_is_subset(a, b);
  TIME_END;

  TIME_RETURN;
}

void _cpp_bitmapset_add_member(dynamic_bitset<> &a) {
  for (int j = 0; j < NNN; ++j) {
    for (int i = 0; i < 1024; i += 3) {
      a[i] = true;
    }
  }
}

long long cpp_bitmapset_add_member(void) {
  dynamic_bitset<> a(10240);

  TIME_START;
  _cpp_bitmapset_add_member(a);
  TIME_END;

  x += a.size();

  TIME_RETURN;
}
