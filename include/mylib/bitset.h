/**
 * mylib/bitset.h
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
#ifndef MYLIB_BITSET_H
#define MYLIB_BITSET_H

#include <stdint.h>
#include <stdlib.h>

struct bitset {
  uint8_t *bytes; // Allocated bytes.
  size_t max;
};

struct bitset *bitset_init(size_t max);
void bitset_deinit(struct bitset *bs);
struct bitset *bitset_clone(const struct bitset *src);
size_t bitset_count(const struct bitset *bs);
size_t bitset_size_in_bytes(const struct bitset *bs);
void bitset_clear(struct bitset *bs);
int bitset_has(const struct bitset *bs, size_t bit);
int bitset_incl(struct bitset *bs, size_t bit);
void bitset_excl(struct bitset *bs, size_t bit);
int bitset_next(const struct bitset *bs, size_t *i);

// Simply uses bitset_next to retrieve the first set bit in the set.
int bitset_first(const struct bitset *bs, size_t *first);

// Returns true if `a` has all of it's elements in `b`.
int bitset_is_subset(const struct bitset *a, const struct bitset *b);

// Returns true if `a` has all of it's elements in `b`, but `b` does not have
// all of its elements in `a`.
int bitset_is_proper_subset(const struct bitset *a, const struct bitset *b);

// Returns true if both sets are the same size, `a` has all of it's elements in
// `b` and `b` has all of it's elements in `a`.
int bitset_eql(const void *a, const void *b);

// If there is no intersections between the two bitsets, this function will
// return 0, otherwise the returned value is the cardinality of the intersecting
// bits.
size_t bitset_intersects(const struct bitset *a, const struct bitset *b);

struct bitset *bitset_union(const struct bitset *a, const struct bitset *b);
struct bitset *bitset_difference(const struct bitset *a,
                                 const struct bitset *b);
uint32_t bitset_hash(const void *bs);

#endif
