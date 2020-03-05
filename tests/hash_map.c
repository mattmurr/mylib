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
  struct hash_map *map =
      hash_map_init(hash_str, eql_str, sizeof(char *), sizeof(int));
  assert(map && "Failed to initialize map.\n");

  char *key = "myfirstkey";
  int val = 95;

  assert(!hash_map_put(map, &key, &val));

  assert(hash_map_count(map) == 1);

  assert(*((int *)hash_map_get_value(map, &key)) == 95);

  key = "wedonthavethiskey";
  assert(!hash_map_has(map, &key));

  key = "mysecondkey";
  val = 128;
  assert(!hash_map_put(map, &key, &val));

  assert(hash_map_count(map) == 2);

  // Try to get kv using key that isn't in the map.
  key = "wealsodonthavethiskey";
  assert(!hash_map_get(map, &key));

  // Clear the map then add some new values.
  hash_map_clear(map);

  for (unsigned long i = 1; i <= 15; i++) {
    const int n = snprintf(NULL, 0, "%lu", i);
    char buf[n + 1];
    assert(snprintf(buf, n + 1, "%lu", i) == n);

    // We cant add zero as a key because it would be a NULL pointer.
    assert(!hash_map_put(map, &buf, &i));
    // Check that the value is added correctly.
    assert(*((int *)hash_map_get_value(map, &buf)) == i);
  }

  // Iterate the map.
  struct hash_map_iterator iter = hash_map_iter(map);
  const struct hash_map_kv *kv;
  while ((kv = hash_map_next(&iter))) {
    val = *((int *)kv->value);
    // Val should be > 0 and less than or equal to 15.
    assert(val > 0 && val <= 15);
  }

  key = "getorputkey";
  int has_existing;
  assert(kv = hash_map_get_or_put(map, &key, &has_existing));
  assert(has_existing == 0);

  val = 8195;

  assert(!hash_map_kv_assign(map, kv, &val));

  assert(*((int *)hash_map_get_value(map, &key)) == val);

  hash_map_clear(map);
  assert(!hash_map_put(map, &key, &val));
  iter = hash_map_iter(map);

  assert(hash_map_next(&iter) != NULL);

  hash_map_deinit(map);
}
