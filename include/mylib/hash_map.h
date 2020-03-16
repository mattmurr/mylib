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

typedef struct HashMapKV {
  void *key;
  void *value;
} HashMapKV;

typedef struct HashMap {
  size_t size;         // How many entries are in the map.
  size_t capacity;     // How many buckets are allocated.
  size_t key_size;     // Byte size of the key.
  size_t value_size;   // Byte size of the value.
  LinkedList *buckets; // Array of linked_list to avoid collisions.

  HashMapHashFn hash; // The hash function.
  HashMapEqlFn eql;   // The eql function.
} HashMap;

typedef struct HashMapIterator {
  const HashMap *map;   // Pointer to the map.
  size_t bucket_idx;    // The current bucket being iterated.
  LinkedListNode *node; // The current node.
} HashMapIterator;

int hash_map_init(HashMap *result, HashMapHashFn hash, HashMapEqlFn eql,
                  size_t key_size, size_t value_size);

void hash_map_deinit(HashMap *map);

void hash_map_clear(HashMap *map);

size_t hash_map_count(const HashMap *map);

int hash_map_put(HashMap *map, void *key, void *value);

const HashMapKV *hash_map_get_or_put(HashMap *map, void *key,
                                     int *has_existing);
HashMapKV *hash_map_get(const HashMap *map, const void *key);

void *hash_map_get_value(const HashMap *map, const void *key);

int hash_map_has(const HashMap *map, const void *key);

void hash_map_delete(HashMap *map, const void *key);

void hash_map_kv_assign(HashMap *map, const HashMapKV *kv, void *value);

HashMapIterator hash_map_iter(const HashMap *map);

HashMapKV *hash_map_next(HashMapIterator *iterator);

#endif
