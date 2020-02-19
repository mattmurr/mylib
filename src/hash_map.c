/**
 * src/hash_map.c
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
#include "mylib/hash_map.h"
#include "mylib/linked_list.h"
#include <string.h>

#define HASHMAP_DEFAULT_INIT_CAPACITY 16

static int init_buckets(struct linked_list **buckets, size_t count) {
    size_t i;
    for (i = 0; i < count; i++) {
        // If we failed to initialize any of the buckets, we're going free
        // everything.
        if (!(buckets[i] = linked_list_init()))
            goto err;
    }

    return EXIT_SUCCESS;

err:
    while (i > 0) {
        free(buckets[i]);
        i--;
    }

    return EXIT_FAILURE;
}

static void deinit_buckets(struct linked_list **buckets, size_t count) {
    // Incase we are trying to deinit/clear a deinitialized map.
    if (buckets == NULL)
        return;

    for (size_t i = 0; i < count; i++) {
        struct linked_list_node *node = buckets[i]->first;
        if (node) {
            do {
                struct hash_map_kv *kv = node->data;
                free(kv->key);
                free(kv->value);
            } while ((node = node->next));
        }
    }

    for (size_t i = 0; i < count; i++) {
        linked_list_deinit(buckets[i]);
    }
}

static struct linked_list *get_bucket(const struct hash_map *map,
                                      const void *key) {
    return map->buckets[map->hash(key) % map->capacity];
}

static int prepend(const struct hash_map *map, struct linked_list *bucket,
                   void *key, void *value) {
    struct hash_map_kv kv;

    // Allocate memory for the key and value.
    kv.key = malloc(map->key_size);
    if (!kv.key)
        return EXIT_FAILURE;
    kv.value = malloc(map->value_size);
    if (!kv.value)
        goto err;

    // Copy the values into the newly allocated memory.
    memcpy(kv.key, key, map->key_size);
    memcpy(kv.value, value, map->value_size);

    // Prepend the kv to the bucket.
    if (linked_list_prepend(bucket, &kv, sizeof(struct hash_map_kv)))
        goto err;

    return EXIT_SUCCESS;

err:
    free(kv.value);
    free(kv.key);

    return EXIT_FAILURE;
}

static int resize(struct hash_map *map, size_t new_capacity) {
    struct linked_list **new_buckets =
        realloc(map->buckets, new_capacity * sizeof(struct linked_list *));
    if (!new_buckets)
        return EXIT_FAILURE;

    map->buckets = new_buckets;

    // Initialize the new buckets.
    init_buckets(map->buckets + map->capacity, new_capacity - map->capacity);

    // Assign the new capacity.
    map->capacity = new_capacity;

    return EXIT_SUCCESS;
}

static int ensure_capacity(struct hash_map *map) {
    if (map->capacity > 0 && map->size <= map->capacity)
        return EXIT_SUCCESS;

    // We use the default init capacity or double the current capacity.
    size_t new_capacity =
        map->capacity == 0 ? HASHMAP_DEFAULT_INIT_CAPACITY : map->capacity * 2;
    if (resize(map, new_capacity))
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

struct hash_map *hash_map_init(HashFn hash, EqlFn eql, size_t key_size,
                               size_t value_size) {
    struct hash_map *result = malloc(sizeof(struct hash_map));
    if (!result)
        return NULL;

    result->buckets = NULL;
    result->capacity = 0;
    result->size = 0;
    result->key_size = key_size;
    result->value_size = value_size;
    result->hash = hash;
    result->eql = eql;

    return result;
}

// Just clears all of the buckets and sets the size to zero.
void hash_map_clear(struct hash_map *map) {
    deinit_buckets(map->buckets, map->capacity);
    map->size = 0;
    map->capacity = 0;
}

// Clear all of the buckets and deinitialize the buckets array.
void hash_map_deinit(struct hash_map *map) {
    hash_map_clear(map);

    free(map->buckets);
    free(map);
}

size_t hash_map_count(const struct hash_map *map) { return map->size; }

static struct hash_map_kv *find_key(struct linked_list_node *node,
                                    const void *key, EqlFn eql) {
    do {
        struct hash_map_kv *kv = node->data;
        if (eql(key, kv->key))
            return kv;
    } while ((node = node->next));

    return NULL;
}

int hash_map_put(struct hash_map *map, void *key, void *value) {
    if (map == NULL || !key)
        return EXIT_FAILURE;

    // Ensure that the map has enough capacity for this new kv.
    if (ensure_capacity(map))
        return EXIT_FAILURE;

    // Get the bucket.
    struct linked_list *bucket = get_bucket(map, key);
    struct linked_list_node *node = bucket->first;

    if (!node) {
        // Construct the kv and try to prepend it to the bucket.
        if (prepend(map, bucket, key, value))
            return EXIT_FAILURE;
        map->size++;
    } else {
        struct hash_map_kv *kv = find_key(node, key, map->eql);
        if (kv) {
            // Assign the new value.
            kv->value = value;

            return EXIT_SUCCESS;
        }

        // The bucket does not contain the key so we can prepend a new node.
        if (prepend(map, bucket, key, value))
            return EXIT_FAILURE;
        map->size++;
    }

    return EXIT_SUCCESS;
}

struct hash_map_kv *hash_map_get(const struct hash_map *map, const void *key) {
    if (map == NULL || map->size == 0)
        return NULL;

    struct linked_list_node *node = get_bucket(map, key)->first;

    if (!node)
        return NULL;

    return find_key(node, key, map->eql);
}

void *hash_map_get_value(const struct hash_map *map, const void *key) {
    struct hash_map_kv *kv = hash_map_get(map, key);
    return kv ? kv->value : NULL;
}

int hash_map_has(const struct hash_map *map, const void *key) {
    return hash_map_get(map, key) != NULL;
}

void hash_map_delete(struct hash_map *map, const void *key) {
    if (map == NULL || map->size == 0)
        return;

    struct linked_list *bucket = get_bucket(map, key);
    struct linked_list_node *node = bucket->first;

    if (!node)
        return;

    // Find the node that contains the key then call linked_list_delete.
    struct hash_map_kv *kv = find_key(node, key, map->eql);

    free(kv->key);
    free(kv->value);

    linked_list_delete(bucket, node);
}

struct hash_map_iterator hash_map_iter(const struct hash_map *map) {
    struct hash_map_iterator result = {0};
    result.map = map;

    return result;
}

struct hash_map_kv *hash_map_next(struct hash_map_iterator *iterator) {
    if (iterator->map == NULL || iterator->map->size == 0)
        return NULL;

    while (iterator->bucket_idx < iterator->map->capacity) {
        if (iterator->node && (iterator->node = iterator->node->next)) {
            return iterator->node->data;
        }

        iterator->node = iterator->map->buckets[iterator->bucket_idx++]->first;
    }

    return NULL;
}
