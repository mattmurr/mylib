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

typedef struct Bitset {
  uint8_t *bytes; // Allocated bytes.
  size_t max;
} Bitset;

int bitset_init(Bitset *result, size_t max);
void bitset_deinit(Bitset *bs);
int bitset_clone(const Bitset *src, Bitset *result);
size_t bitset_count(const Bitset *bs);
size_t bitset_size_in_bytes(const Bitset *bs);
void bitset_clear(Bitset *bs);
int bitset_has(const Bitset *bs, size_t bit);
int bitset_incl(Bitset *bs, size_t bit);
void bitset_excl(Bitset *bs, size_t bit);
int bitset_next(const Bitset *bs, size_t *i);

// Simply uses bitset_next to retrieve the first set bit in the set.
int bitset_first(const Bitset *bs, size_t *first);

// Returns true if `a` has all of it's elements in `b`.
int bitset_is_subset(const Bitset *a, const Bitset *b);

// Returns true if `a` has all of it's elements in `b`, but `b` does not have
// all of its elements in `a`.
int bitset_is_proper_subset(const Bitset *a, const Bitset *b);

// Returns true if both sets are the same size, `a` has all of it's elements in
// `b` and `b` has all of it's elements in `a`.
int bitset_eql(const void *a, const void *b);

int bitset_intersect(const Bitset *a, const Bitset *b, Bitset *result);
int bitset_intersects(const Bitset *a, const Bitset *b);

int bitset_union(const Bitset *a, const Bitset *b, Bitset *result);
int bitset_difference(const Bitset *a, const Bitset *b, Bitset *result);
uint32_t bitset_hash(const void *bs);

#endif
