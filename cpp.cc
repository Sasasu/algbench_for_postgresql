#include "run.h"

#include <cstdint>
#include <list>
#include <memory_resource>
#include <tuple>
#include <unordered_map>

#include <boost/dynamic_bitset.hpp>

using namespace std;
using namespace boost;
using std::pmr::unsynchronized_pool_resource;

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

void _cpp_list_sort(list<int> &a) { a.sort(); }

long long cpp_list_sort(void) {
  list<int> a;

  for (int i = 0; i < NNN * 10; i++) {
    a.push_back(NNN * 10 - i);
  }

  TIME_START;
  _cpp_list_sort(a);
  TIME_END;

  x += a.back();

  TIME_RETURN;
}

void _cpp_list_iter(list<int> &a) {
  for (auto &i : a) {
    x += i;
  }
}

long long cpp_list_iter(void) {
  list<int> a;

  for (int i = 0; i < 1024; i++) {
    a.push_back(i);
  }

  TIME_START;
  for (int i = 0; i < NNN; ++i) {
    _cpp_list_iter(a);
  }
  TIME_END;

  TIME_RETURN;
}

void _cpp_list_append(list<int> &a) {
  for (int i = 0; i < 1024; i++) {
    a.push_back(i);
  }
}

long long cpp_list_append(void) {
  list<int> a;

  TIME_START;
  for (int i = 0; i < NNN / 10; ++i) {
    _cpp_list_append(a);
  }
  TIME_END;

  TIME_RETURN;
}

static void _cpp_hash_set(unordered_map<int32_t, int32_t> &htab) {
  for (int i = 0; i < NNN; ++i) {
    htab[i] = i;
  }
}

long long cpp_hash_set_alloc(void) {
  unordered_map<int32_t, int32_t> a;

  TIME_START;
  _cpp_hash_set(a);
  TIME_END;

  TIME_RETURN;
}

long long cpp_hash_set_noalloc(void) {
  unordered_map<int32_t, int32_t> a(NNN);

  TIME_START;
  _cpp_hash_set(a);
  TIME_END;

  TIME_RETURN;
}

long long cpp_hash_get_rand(void) {
  unordered_map<int32_t, int32_t> a(NNN);
  _cpp_hash_set(a);

  TIME_START;
  for (int i = 0; i < NNN; ++i) {
    x += a[i];
  }
  TIME_END;

  TIME_RETURN;
}

long long cpp_hash_get_scan(void) {
  unordered_map<int32_t, int32_t> a(NNN);
  _cpp_hash_set(a);

  TIME_START;
  for (auto i : a) {
    x += i.second;
  }
  TIME_END;

  TIME_RETURN;
}

long long cpp_hash_delete(void) {
  unordered_map<int32_t, int32_t> a(NNN);
  _cpp_hash_set(a);

  TIME_START;
  for (int i = 0; i < NNN; ++i) {
    a.erase(i);
  }
  TIME_END;

  TIME_RETURN;
}

int sum1(int a) { return a; }
int sum2(int a, int b) { return a + b; }
int sum7(int a, int b, int c, int d, int e, int f, int g) {
  return a + b + c + d + e + f + g;
}
int sum9(int a, int b, int c, int d, int e, int f, int g, int h, int i, int j) {
  return a + b + c + d + e + f + g + h + i + g;
}

static tuple<decltype(&sum1), decltype(&sum2), decltype(&sum7), decltype(&sum9)>
    f(sum1, sum2, sum7, sum9);

long long cpp_functioncall_1(void) {
  int a = 1;

  TIME_START;
  for (int i = 0; i < NNN; ++i) {
    x += get<0>(f)(a);
  }
  TIME_END;

  TIME_RETURN;
}

long long cpp_functioncall_2(void) {
  int a = 1;

  TIME_START;
  for (int i = 0; i < NNN; ++i) {
    x += get<1>(f)(a, a);
  }
  TIME_END;

  TIME_RETURN;
}

long long cpp_functioncall_7(void) {
  int a = 1;

  TIME_START;
  for (int i = 0; i < NNN; ++i) {
    x += get<2>(f)(a, a, a, a, a, a, a);
  }
  TIME_END;

  TIME_RETURN;
}

long long cpp_functioncall_9(void) {
  int a = 1;

  TIME_START;
  for (int i = 0; i < NNN; ++i) {
    x += get<3>(f)(a, a, a, a, a, a, a, a, a, a);
  }
  TIME_END;

  TIME_RETURN;
}

long long cpp_memoryalloc_small(void) {
  unsynchronized_pool_resource pool = unsynchronized_pool_resource();

  TIME_START;
  for (int i = 0; i < NNN; ++i) {
    (void)pool.allocate(i);
  }
  TIME_END;

  pool.release();

  TIME_RETURN;
}

long long cpp_memoryalloc_big(void) {
  void *prt[NNN] = {};

  TIME_START;
  for (int i = 0; i < NNN; ++i) {
    prt[i] = malloc(i * 100);
  }
  TIME_END;

  for (int i = 0; i < NNN; ++i) {
    free(prt[i]);
  }

  TIME_RETURN;
}

long long cpp_memoryalloc_free(void) {
  void *prt[NNN] = {};

  for (int i = 0; i < NNN; ++i) {
    prt[i] = malloc(i);
  }

  TIME_START;
  for (int i = 0; i < NNN; ++i) {
    free(prt[i]);
  }
  TIME_END;

  TIME_RETURN;
}

long long cpp_memoryalloc_free_b(void) {
  unsynchronized_pool_resource pool = unsynchronized_pool_resource();

  void *prt[NNN] = {};

  for (int i = 0; i < NNN; ++i) {
    prt[i] = pool.allocate(i);
  }

  TIME_START;
  pool.release();
  TIME_END;

  TIME_RETURN;
}
