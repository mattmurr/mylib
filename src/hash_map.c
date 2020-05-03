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
#include "mylib/linked_list.h"

#include <assert.h>
#include <string.h>

#define HASHMAP_DEFAULT_INIT_CAPACITY 16

static int init_buckets(LinkedList *buckets, size_t count) {
  for (size_t i = 0; i < count; i++)
    buckets[i] = linked_list_init();

  return EXIT_SUCCESS;
}

static void deinit_buckets(LinkedList *buckets, size_t count) {
  // Just return if buckets is NULL.
  if (buckets == NULL)
    return;

  for (size_t i = 0; i < count; i++) {
    LinkedListNode *node = buckets[i].first;
    if (node) {
      do {
        HashMapKV *kv = node->data;
        free(kv->key);
        free(kv->value);
      } while ((node = node->next));
    }
  }

  for (size_t i = 0; i < count; i++) {
    linked_list_deinit(&buckets[i]);
  }
}

static LinkedList *get_bucket(const HashMap *map, const void *key) {
  return &map->buckets[map->hash(key) % map->capacity];
}

static int prepend(HashMap *map, LinkedList *bucket, void *key, void *value) {
  HashMapKV kv;

  // Allocate memory for the key and value.
  kv.key = malloc(map->key_size);
  if (!kv.key)
    return EXIT_FAILURE;
  kv.value = malloc(map->value_size);
  if (!kv.value)
    goto err;

  // Copy the values into the newly allocated memory.
  memcpy(kv.key, key, map->key_size);
  if (value)
    memcpy(kv.value, value, map->value_size);

  // Prepend the kv to the bucket.
  if (linked_list_prepend(bucket, &kv, sizeof(HashMapKV)))
    goto err;

  map->size++;

  return EXIT_SUCCESS;

err:
  free(kv.value);
  free(kv.key);

  return EXIT_FAILURE;
}

static int resize(HashMap *map, size_t new_capacity) {
  LinkedList *new_buckets =
      realloc(map->buckets, new_capacity * sizeof(LinkedList));
  if (!new_buckets)
    return EXIT_FAILURE;

  map->buckets = new_buckets;

  // Initialize the new buckets.
  init_buckets(map->buckets + map->capacity, new_capacity - map->capacity);

  // Assign the new capacity.
  map->capacity = new_capacity;

  return EXIT_SUCCESS;
}

static int ensure_capacity(HashMap *map) {
  if (map->capacity > 0 && map->size <= map->capacity)
    return EXIT_SUCCESS;

  // We use the default init capacity or double the current capacity.
  size_t new_capacity =
      map->capacity == 0 ? HASHMAP_DEFAULT_INIT_CAPACITY : map->capacity * 2;
  if (resize(map, new_capacity))
    return EXIT_FAILURE;

  return EXIT_SUCCESS;
}

int hash_map_init(HashMap *result, HashMapHashFn hash, HashMapEqlFn eql,
                  size_t key_size, size_t value_size) {
  assert(result != NULL);
  assert(hash != NULL);
  assert(eql != NULL);

  *result = (HashMap){0};

  if (ensure_capacity(result))
    return EXIT_FAILURE;

  result->size = 0;
  result->key_size = key_size;
  result->value_size = value_size;
  result->hash = hash;
  result->eql = eql;

  return EXIT_SUCCESS;
}

// Just clears all of the buckets and sets the size to zero.
void hash_map_clear(HashMap *map) {
  assert(map != NULL);

  deinit_buckets(map->buckets, map->capacity);
  map->size = 0;
  map->capacity = 0;
}

// Clear all of the buckets and deinitialize the buckets array.
void hash_map_deinit(HashMap *map) {
  assert(map != NULL);

  // We also ensure is not NULL inside hash_map_clear.
  hash_map_clear(map);
  free(map->buckets);
}

size_t hash_map_count(const HashMap *map) {
  assert(map != NULL);
  return map->size;
}

static HashMapKV *find_key(LinkedListNode *node, const void *key,
                           HashMapEqlFn eql) {
  do {
    HashMapKV *kv = node->data;
    if (eql(key, kv->key))
      return kv;
  } while ((node = node->next));

  return NULL;
}

int hash_map_put(HashMap *map, void *key, void *value) {
  assert(map != NULL);
  assert(key != NULL);

  // Ensure that the map has enough capacity for this new kv.
  if (ensure_capacity(map))
    return EXIT_FAILURE;

  // Get the bucket.
  LinkedList *bucket = get_bucket(map, key);
  LinkedListNode *node = bucket->first;

  if (!node) {
    // Construct the kv and try to prepend it to the bucket.
    if (prepend(map, bucket, key, value))
      return EXIT_FAILURE;
  } else {
    HashMapKV *kv = find_key(node, key, map->eql);
    if (kv) {
      // Assign the new value.
      kv->value = value;

      return EXIT_SUCCESS;
    }

    // The bucket does not contain the key so we can prepend a new node.
    if (prepend(map, bucket, key, value))
      return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

const HashMapKV *hash_map_get_or_put(HashMap *map, void *key,
                                     int *has_existing) {
  assert(map != NULL);
  assert(key != NULL);

  LinkedList *bucket = get_bucket(map, key);
  LinkedListNode *node = bucket->first;

  if (!node) {
    if (prepend(map, bucket, key, NULL))
      return NULL;
  } else {
    HashMapKV *kv = find_key(node, key, map->eql);
    // The key exists, return it now.
    if (kv) {
      if (has_existing)
        *has_existing = 1;
      return kv;
    } else if (prepend(map, bucket, key, NULL)) // Else we prepend the kv.
      return NULL;
  }

  if (has_existing)
    *has_existing = 0;

  return bucket->first->data;
}

HashMapKV *hash_map_get(const HashMap *map, const void *key) {
  assert(map != NULL);
  assert(key != NULL);

  // HashMap contains no entries, quick exit.
  if (map->size == 0)
    return NULL;

  LinkedListNode *node = get_bucket(map, key)->first;

  if (!node)
    return NULL;

  return find_key(node, key, map->eql);
}

void *hash_map_get_value(const HashMap *map, const void *key) {
  HashMapKV *kv = hash_map_get(map, key);
  return kv ? kv->value : NULL;
}

int hash_map_has(const HashMap *map, const void *key) {
  return hash_map_get(map, key) != NULL;
}

void hash_map_delete(HashMap *map, const void *key) {
  assert(map != NULL);
  assert(key != NULL);

  // HashMap is empty, quick exit.
  if (map->size == 0)
    return;

  LinkedList *bucket = get_bucket(map, key);
  LinkedListNode *node = bucket->first;

  if (!node)
    return;

  // Find the node that contains the key then call linked_list_delete.
  HashMapKV *kv = find_key(node, key, map->eql);

  free(kv->key);
  free(kv->value);

  linked_list_delete(bucket, node);
}

void hash_map_kv_assign(HashMap *map, const HashMapKV *kv, void *value) {
  assert(map != NULL);
  assert(kv != NULL);
  assert(value != NULL);

  memcpy(kv->value, value, map->value_size);
}

HashMapIterator hash_map_iter(const HashMap *map) {
  HashMapIterator result = {0};
  result.map = map;

  return result;
}

HashMapKV *hash_map_next(HashMapIterator *iterator) {
  if (iterator->map == NULL || iterator->map->size == 0)
    return NULL;

  if (iterator->node) {
    iterator->node = iterator->node->next;
    if (!iterator->node)
      goto next_bucket;
  } else {
  next_bucket:
    do {
      if (++iterator->bucket_idx >= iterator->map->capacity)
        return NULL;
      iterator->node = iterator->map->buckets[iterator->bucket_idx].first;
    } while (!iterator->node);
  }

  return iterator->node->data;
}
