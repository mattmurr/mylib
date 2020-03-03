/**
 * mylib/hash_map.h
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
#ifndef MYLIB_HASHMAP_H
#define MYLIB_HASHMAP_H

#include "linked_list.h"
#include <stdint.h>
#include <stdlib.h>

typedef uint32_t (*HashMapHashFn)(const void *key);
typedef int32_t (*HashMapEqlFn)(const void *a, const void *b);

struct hash_map_kv {
  void *key;
  void *value;
};

struct hash_map {
  size_t size;                  // How many entries are in the map.
  size_t capacity;              // How many buckets are allocated.
  size_t key_size;              // Byte size of the key.
  size_t value_size;            // Byte size of the value.
  struct linked_list **buckets; // Array of linked_list to avoid collisions.

  HashMapHashFn hash; // The hash function.
  HashMapEqlFn eql;   // The eql function.
};

struct hash_map_iterator {
  const struct hash_map *map;    // Pointer to the map.
  size_t bucket_idx;             // The current bucket being iterated.
  struct linked_list_node *node; // The current node.
};

struct hash_map *hash_map_init(HashMapHashFn hash, HashMapEqlFn eql,
                               size_t key_size, size_t value_size);
void hash_map_deinit(struct hash_map *map);
void hash_map_clear(struct hash_map *map);
size_t hash_map_count(const struct hash_map *map);
int hash_map_put(struct hash_map *map, void *key, void *value);
struct hash_map_kv *hash_map_get_or_put(struct hash_map *map, void *key,
                                        int *has_existing);
struct hash_map_kv *hash_map_get_or_put_value(struct hash_map *map, void *key,
                                              void *value, int *has_existing);
struct hash_map_kv *hash_map_get(const struct hash_map *map, const void *key);
void *hash_map_get_value(const struct hash_map *map, const void *key);
int hash_map_has(const struct hash_map *map, const void *key);
void hash_map_delete(struct hash_map *map, const void *key);

int hash_map_kv_assign(struct hash_map *map, struct hash_map_kv *kv,
                       void *value);

struct hash_map_iterator hash_map_iter(const struct hash_map *map);
struct hash_map_kv *hash_map_next(struct hash_map_iterator *iterator);

#endif
