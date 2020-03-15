/**
 * bitset.c
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
#include "mylib/bitset.h"
#include <assert.h>

int main() {
  Bitset bs;
  assert(!bitset_init(&bs, 10));

  // The bitset should be empty.
  assert(bitset_count(&bs) == 0);

  assert(!bitset_incl(&bs, 1));
  assert(bitset_count(&bs) == 1);

  // Set 10 and check.
  assert(!bitset_incl(&bs, 10));
  assert(bitset_has(&bs, 10));

  // Count should be 2.
  assert(bitset_count(&bs) == 2);

  {
    // Test bitset_first.
    size_t first;
    assert(bitset_first(&bs, &first));
    assert(first == 1);
  }

  Bitset other;
  assert(!bitset_clone(&bs, &other));

  // Check that the clone is initialized correctly, count should be 2.
  assert(bitset_count(&other) == 2);

  // Clear the clone and make sure it is now empty.
  bitset_clear(&other);
  assert(bitset_count(&other) == 0);

  size_t values[] = {5, 10, 15, 20, 25};
  // Fill up `other` with the values the array.
  for (size_t i = 0; i < 5; i++)
    bitset_incl(&other, values[i]);

  // Check we have an intersection.
  assert(bitset_intersects(&bs, &other));

  // Remove 10 from `other` and there should no longer be an intersection.
  bitset_excl(&other, 10);
  assert(!bitset_intersects(&bs, &other));

  // Test the iterator.
  {
    size_t j = 0;
    for (size_t i = 0; bitset_next(&other, &i); i++, j++) {
      if (j == 2)
        continue;
      assert(i == values[j]);
    }
  }

  bitset_deinit(&bs);
  bitset_deinit(&other);

  return EXIT_SUCCESS;
}
