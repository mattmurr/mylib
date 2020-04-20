/**
 * hash_map.c
 * Copyright (c) 2020 Matthew Murray <matt@compti.me>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "mylib/hash_map.h"
#include "mylib/hash.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

static unsigned int hash_str(const void *str) {
  return fnv1a_32_hash(str, strlen(str) + 1);
}

static int eql_str(const void *a, const void *b) {
  return strcmp((const char *)a, (const char *)b) == 0;
}

int main() {
  HashMap map;
  assert(!hash_map_init(&map, hash_str, eql_str, sizeof(char *), sizeof(int)));

  {
    const char *key = "myfirstkey";
    int val = 95;

    // Put the key and value into the map, check that the count is now 1 and
    // that the value can be accessed.
    assert(!hash_map_put(&map, &key, &val));
    assert(hash_map_count(&map) == 1);
    assert(*((int *)hash_map_get_value(&map, &key)) == val);
  }

  // Try to get a value using a key that isn't in the map
  {
    const char *key = "wedonthavethiskey";
    assert(!hash_map_has(&map, &key));
  }

  hash_map_clear(&map);
  assert(hash_map_count(&map) == 0);

  // Generate the keys and values.
  for (unsigned long i = 0; i < 15; i++) {
    const size_t n = snprintf(NULL, 0, "%lu", i);
    char buf[n + 1];
    assert(snprintf(buf, n + 1, "%lu", i) == n);

    assert(!hash_map_put(&map, &buf, &i));
    assert(*((int *)hash_map_get_value(&map, &buf)) == i);
  }

  // Iterate the map, each value should be under 15 like the above loop
  HashMapIterator iter = hash_map_iter(&map);
  const HashMapKV *kv;
  while ((kv = hash_map_next(&iter)))
    assert(*(int *)kv->value < 15);

  // Use `hash_map_get_or_put()` to put a new kv pair into the map
  {
    const char *key = "getorputkey";
    // `has_existing` should be 0
    int has_existing;
    assert(kv = hash_map_get_or_put(&map, &key, &has_existing));
    assert(has_existing == 0);

    // Assign a value to `kv` and ensure that we can get that value again
    int val = 999;
    hash_map_kv_assign(&map, kv, &val);
    assert(*((int *)hash_map_get_value(&map, &key)) == val);
  }

  hash_map_deinit(&map);
}
