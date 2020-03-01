/**
 * include/mylib/bitset.h
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
