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
  struct bitset *bs = bitset_init(10);
  assert(bs != NULL);

  assert(bitset_count(bs) == 0);

  assert(!bitset_incl(bs, 1));
  assert(bitset_count(bs) == 1);

  assert(!bitset_incl(bs, 10));
  // Ensure that the bitset has 10 set.
  assert(bitset_has(bs, 10));

  // Test bitset_first.
  {
    size_t first;
    assert(bitset_first(bs, &first));
    assert(first == 1);
  }

  // Check that the bitset now has 2 bits set.
  assert(bitset_count(bs) == 2);

  struct bitset *other = bitset_clone(bs);
  assert(other != NULL);

  // Check that the clone count is correct.
  assert(bitset_count(other) == 2);

  // and that it contains both values.
  assert(bitset_has(other, 1));
  assert(bitset_has(other, 10));

  bitset_deinit(other);

  // Clear the bitset.
  bitset_clear(bs);

  // Include all of the values in the array.
  uint8_t values[] = {1, 2, 5, 9, 10};
  for (int i = 0; i < 5; i++) {
    assert(!bitset_incl(bs, values[i]));
  }

  // Iterate the bitset and test that all the values are set.
  size_t j = 0;
  for (size_t i = 0; bitset_next(bs, &i); i++, j++) {
    assert(i == values[j]);
  }

  // Clone and test eql
  other = bitset_clone(bs);
  assert(other != NULL);
  assert(bitset_eql(bs, other));

  // Ensure that bitset_is_proper_subset works.
  assert(!bitset_is_proper_subset(other, bs));
  // It should also be a subset.
  assert(bitset_is_subset(bs, other));
  // Add a new value to the original bitset so that the cloned
  // is a proper subset.
  assert(!bitset_incl(bs, 3));
  assert(bitset_is_proper_subset(other, bs));

  // other should no longer be a subset of bs.
  assert(!bitset_is_subset(bs, other));
  // But bs is a subset of other.
  assert(bitset_is_subset(other, bs));

  bitset_deinit(other);

  struct bitset *intersect = bitset_init(15);
  assert(intersect != NULL);

  // bs should not intersect with last as they share no common bits.
  assert(!bitset_intersects(bs, intersect));

  // Add a bit to intersect which bs contains.
  assert(!bitset_incl(intersect, 1));

  // There should now be an intersection.
  assert(bitset_intersects(bs, intersect));

  // Create a union bitset of bs and intersect.
  struct bitset *unionized = bitset_union(bs, intersect);
  assert(unionized != NULL);

  bitset_deinit(bs);

  // Check that it contains the of both bitsets.
  for (size_t i = 0; i < 5; i++) {
    assert(bitset_has(unionized, values[i]));
  }

  // Include a value into the intersect bitset which is not in the
  // unionized bitset.
  assert(!bitset_incl(intersect, 13));

  struct bitset *difference = bitset_difference(unionized, intersect);
  assert(difference != NULL);

  bitset_deinit(intersect);
  bitset_deinit(unionized);

  // Should contain 13 and no other bits.
  assert(bitset_has(difference, 13));

  for (size_t i = 0; i < 5; i++) {
    assert(!bitset_has(difference, values[i]));
  }

  bitset_deinit(difference);

  // Test hash
  bs = bitset_init(50);
  assert(bs != NULL);

  assert(!bitset_incl(bs, 10));
  assert(!bitset_incl(bs, 22));

  other = bitset_init(50);
  assert(other != NULL);

  assert(!bitset_incl(other, 49));
  assert(!bitset_incl(other, 5));
  assert(bitset_hash(bs) != bitset_hash(other));

  bitset_clear(other);

  assert(!bitset_incl(other, 10));
  assert(!bitset_incl(other, 22));
  assert(bitset_hash(bs) == bitset_hash(other));

  assert(!bitset_incl(other, 50));
  assert(bitset_hash(bs) != bitset_hash(other));

  bitset_deinit(bs);
  bitset_deinit(other);

  return EXIT_SUCCESS;
}
