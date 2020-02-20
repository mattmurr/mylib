/**
 * include/mylib/hash_map.h
 * Copyright (c) 2020 Matthew Murray <matt@compti.me>
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */
#ifndef MY_LIB_HASHMAP_H
#define MY_LIB_HASHMAP_H

#include "linked_list.h"
#include <stdint.h>
#include <stdlib.h>

typedef uint32_t (*HashFn)(const void *key);
typedef int32_t (*EqlFn)(const void *a, const void *b);

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

    HashFn hash; // The hash function.
    EqlFn eql;   // The eql function.
};

struct hash_map_iterator {
    const struct hash_map *map;    // Pointer to the map.
    size_t bucket_idx;             // The current bucket being iterated.
    struct linked_list_node *node; // The current node.
};

struct hash_map_iterator hash_map_iter(const struct hash_map *map);
struct hash_map_kv *hash_map_next(struct hash_map_iterator *iterator);

struct hash_map *hash_map_init(HashFn hash, EqlFn eql, size_t key_size,
                               size_t value_size);
void hash_map_deinit(struct hash_map *map);
void hash_map_clear(struct hash_map *map);
size_t hash_map_count(const struct hash_map *map);
int hash_map_put(struct hash_map *map, void *key, void *value);
struct hash_map_kv *hash_map_get(const struct hash_map *map, const void *key);
void *hash_map_get_value(const struct hash_map *map, const void *key);
int hash_map_has(const struct hash_map *map, const void *key);
void hash_map_delete(struct hash_map *map, const void *key);

#endif
