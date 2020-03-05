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
#include <assert.h>

int main() {
  struct vector *vec = vector_init(sizeof(int));
  assert(vec != NULL);

  int a = 5;

  // Add a value and ensure that the value is retrievable.
  assert(!vector_append(vec, &a));
  assert(*((int *)vector_get(vec, 0)) == a);

  // Fill the vector up
  for (int i = 0; i < 50; i++) {
    assert(!vector_insert(vec, 0, &i));
    assert(*((int *)vector_get(vec, 0)) == i);
  }

  // Clone the vector and ensure that it has the correct size.
  struct vector *clone = vector_clone(vec);
  assert(clone != NULL);

  assert(vector_len(clone) == 51);

  vector_delete(vec, 50);

  // Iterate the array.
  int *arr = vec->data;
  for (size_t i = 0; i < vector_len(vec); i++)
    assert(arr[i] == 49 - i);

  // Test swap delete.
  vector_swap_delete(vec, 20);
  assert(*((const int *)vector_get_const(vec, 20)) == 0);

  vector_deinit(vec);
  vector_deinit(clone);
}
