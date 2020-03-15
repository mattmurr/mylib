/**
 * mylib/vector.h
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
#ifndef MYLIB_VECTOR_H
#define MYLIB_VECTOR_H

#include <stdlib.h>

typedef struct Vector {
  size_t size;
  size_t capacity;
  size_t element_size;

  void *data;
} Vector;

int vector_init_with_capacity(Vector *result, size_t element_size,
                              size_t capacity);
int vector_init(Vector *result, size_t element_size);
int vector_clone(const Vector *src, Vector *result);
void vector_deinit(Vector *vec);
int vector_resize(Vector *vec, size_t new_capacity);
size_t vector_len(const Vector *vec);
size_t vector_size_in_bytes(const Vector *vec);
int vector_assign(Vector *vec, size_t idx, void *element);
int vector_append(Vector *vec, void *element);
int vector_insert(Vector *vec, size_t idx, void *element);
void *vector_get(Vector *vec, size_t idx);
const void *vector_get_const(const Vector *vec, size_t idx);
void vector_delete(Vector *vec, size_t idx);
void vector_swap_delete(Vector *vec, size_t idx);
void vector_clear(Vector *vec);
void vector_shrink_to_fit(Vector *vec);

#endif
