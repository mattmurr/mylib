/**
 * vector.c
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
#include "mylib/vector.h"
#include <string.h>

#define DEFAULT_INIT_CAPACITY 4

static void *get_offset(struct vector *vec, size_t idx) {
  return vec->data + (idx * vec->element_size);
}

static const void *get_offset_const(const struct vector *vec, size_t idx) {
  return vec->data + (idx * vec->element_size);
}

static int try_grow(struct vector *vec) {
  return vec->size >= vec->capacity ? vector_resize(vec, vec->size * 2)
                                    : EXIT_SUCCESS;
}

static void assign(struct vector *vec, size_t idx, void *element) {
  void *offset = get_offset(vec, idx);
  memcpy(offset, element, vec->element_size);
}

struct vector *vector_init_with_capacity(size_t element_size, size_t capacity) {
  if (element_size == 0)
    return NULL;

  struct vector *result = malloc(sizeof(struct vector));
  if (!result)
    return NULL;

  result->data = malloc(capacity * element_size);
  if (!result->data) {
    free(result);
    return NULL;
  }

  result->size = 0;
  result->capacity = capacity;
  result->element_size = element_size;

  return result;
}

struct vector *vector_init(size_t element_size) {
  return vector_init_with_capacity(element_size, DEFAULT_INIT_CAPACITY);
}

struct vector *vector_clone(const struct vector *src) {
  if (src == NULL)
    return NULL;

  struct vector *result =
      vector_init_with_capacity(src->element_size, src->size * 2);
  if (!result)
    return NULL;

  memcpy(result->data, src->data, src->size * src->element_size);

  result->size = src->size;

  return result;
}

void vector_deinit(struct vector *vec) { free(vec->data); }

int vector_resize(struct vector *vec, size_t new_capacity) {
  if (vec == NULL)
    return EXIT_FAILURE;

  void *data = realloc(vec->data, vec->element_size * new_capacity);
  if (!data)
    return EXIT_FAILURE;
  vec->data = data;

  vec->capacity = new_capacity;
  if (vec->size > new_capacity)
    vec->size = new_capacity;

  return EXIT_SUCCESS;
}

size_t vector_len(const struct vector *vec) { return vec->size; }

size_t vector_size_in_bytes(const struct vector *vec) {
  return vec->size * vec->element_size;
}

int vector_assign(struct vector *vec, size_t idx, void *element) {
  if (vec == NULL)
    return EXIT_FAILURE;
  if (element == NULL)
    return EXIT_FAILURE;
  if (idx >= vec->size)
    return EXIT_FAILURE;

  assign(vec, idx, element);

  return EXIT_SUCCESS;
}

int vector_append(struct vector *vec, void *element) {
  if (vec == NULL)
    return EXIT_FAILURE;
  if (element == NULL)
    return EXIT_FAILURE;

  if (try_grow(vec))
    return EXIT_FAILURE;

  assign(vec, vec->size, element);
  vec->size++;

  return EXIT_SUCCESS;
}

int vector_insert(struct vector *vec, size_t idx, void *element) {
  if (vec == NULL)
    return EXIT_FAILURE;
  if (element == NULL)
    return EXIT_FAILURE;
  if (idx > vec->size)
    return EXIT_FAILURE;

  if (try_grow(vec))
    return EXIT_FAILURE;

  // Move elements to the right.
  // The offset to the desired index.
  void *offset = get_offset(vec, idx);

  // Size in bytes of elements after the offset.
  size_t bytes_to_move = (vec->size - idx) * vec->element_size;

  // Offset of the next element is offset + element size.
  memmove(offset + vec->element_size, offset, bytes_to_move);

  // Assign
  memcpy(offset, element, vec->element_size);
  vec->size++;

  return EXIT_SUCCESS;
}

void *vector_get(struct vector *vec, size_t idx) {
  if (vec == NULL)
    return NULL;
  if (idx >= vec->size)
    return NULL;

  return get_offset(vec, idx);
}

const void *vector_get_const(const struct vector *vec, size_t idx) {
  if (vec == NULL)
    return NULL;
  if (idx >= vec->size)
    return NULL;

  return get_offset_const(vec, idx);
}

void vector_delete(struct vector *vec, size_t idx) {
  if (vec == NULL)
    return;
  if (idx >= vec->size)
    return;

  // Move elements to the left overwriting the element at idx.
  void *offset = get_offset(vec, idx);
  size_t bytes_to_move = (vec->size - idx - 1) * vec->element_size;

  memmove(offset, offset + vec->element_size, bytes_to_move);
  vec->size--;

  // Shrink the vector size is <= 25% capacity.
  if (vec->size <= vec->capacity / 4)
    vector_resize(vec, vec->size / 2);
}

void vector_swap_delete(struct vector *vec, size_t idx) {
  if (vec == NULL)
    return;
  if (idx >= vec->size)
    return;

  void *dest = get_offset(vec, idx);
  void *src = get_offset(vec, vec->size - 1);

  // Move the last element on of the element at idx overwriting it.
  memmove(dest, src, vec->element_size);

  vector_delete(vec, vec->size - 1);
}

void vector_clear(struct vector *vec) { vector_resize(vec, 0); }

void vector_shrink_to_fit(struct vector *vec) { vector_resize(vec, vec->size); }
