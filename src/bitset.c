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
#include "mylib/hash.h"
#include <assert.h>
#include <string.h>

static size_t get_byte(size_t bit) { return bit >> 3; }

static size_t byte_count(size_t bit) { return get_byte(bit) + 1; }

static uint8_t get_bit_offset(size_t bit) { return 7 - (bit % 8); }

int bitset_init(Bitset *result, size_t max) {
  assert(result != NULL);

  *result = (Bitset){0};

  size_t required_bytes = byte_count(max);

  if (!(result->bytes = calloc(required_bytes, sizeof(uint8_t))))
    return EXIT_FAILURE;

  result->max = max;

  return EXIT_SUCCESS;
}

void bitset_deinit(Bitset *bs) {
  assert(bs != NULL);

  free(bs->bytes);
}

int bitset_clone(const Bitset *src, Bitset *result) {
  assert(src != NULL);

  if (bitset_init(result, src->max))
    return EXIT_FAILURE;

  size_t bytes_to_copy = byte_count(src->max);
  memcpy(result->bytes, src->bytes, sizeof(uint8_t) * bytes_to_copy);

  return EXIT_SUCCESS;
}

// Adapted for uint8_t from Hacker's Delight, p. 66, Figure 5-2.
static uint8_t popcount(uint8_t byte) {
  byte = byte - ((byte >> 1) & 0x55);
  byte = (byte & 0x33) + ((byte >> 2) & 0x33);
  byte = (byte + (byte >> 4)) & 0x0F;

  return byte;
}

size_t bitset_count(const Bitset *bs) {
  assert(bs != NULL);

  size_t bytes_to_count = byte_count(bs->max);

  size_t result = 0;
  for (size_t i = 0; i < bytes_to_count; i++)
    result += popcount(bs->bytes[i]);

  return result;
}

size_t bitset_size_in_bytes(const Bitset *bs) {
  assert(bs != NULL);

  return byte_count(bs->max);
}

void bitset_clear(Bitset *bs) {
  assert(bs != NULL);

  size_t bytes_to_clear = byte_count(bs->max);
  memset(bs->bytes, 0, sizeof(uint8_t) * bytes_to_clear);
}

int bitset_has(const Bitset *bs, size_t bit) {
  assert(bs != NULL);

  if (bit > bs->max)
    return 0;

  size_t byte = get_byte(bit);
  size_t offset = get_bit_offset(bit);
  return bs->bytes[byte] & (1 << offset);
}

int bitset_incl(Bitset *bs, size_t bit) {
  assert(bs != NULL);

  if (bit > bs->max)
    return EXIT_FAILURE;

  size_t byte = get_byte(bit);
  size_t offset = get_bit_offset(bit);
  bs->bytes[byte] |= ((uint8_t)1 << offset);

  return EXIT_SUCCESS;
}

void bitset_excl(Bitset *bs, size_t bit) {
  assert(bs != NULL);

  if (bit > bs->max)
    return;

  size_t byte = get_byte(bit);
  size_t offset = get_bit_offset(bit);
  bs->bytes[byte] &= ~((uint8_t)1 << offset);
}

int bitset_next(const Bitset *bs, size_t *i) {
  assert(bs != NULL);

  assert(i != NULL);

  while (*i <= bs->max) {
    if (bitset_has(bs, *i))
      return 1;
    *i += 1;
  }

  return 0;
}

int bitset_first(const Bitset *bs, size_t *first) {
  assert(bs != NULL);

  assert(first != NULL);

  *first = 0;
  return bitset_next(bs, first);
}

int bitset_is_subset(const Bitset *a, const Bitset *b) {
  assert(a != NULL);
  assert(b != NULL);

  for (size_t i = 0; bitset_next(a, &i); i++)
    if (!bitset_has(b, i))
      return 0;
  return 1;
}

int bitset_is_proper_subset(const Bitset *a, const Bitset *b) {
  return bitset_is_subset(a, b) && !bitset_is_subset(b, a);
}

int bitset_eql(const void *a, const void *b) {
  // Cast both of the bitsets.
  const Bitset *bs_a = a;
  const Bitset *bs_b = b;

  assert(bs_a != NULL);
  assert(bs_b != NULL);

  for (size_t i = 0; i < (bs_a->max > bs_b->max ? bs_a->max : bs_b->max); i++) {
    if (!(bitset_has(bs_a, i) == bitset_has(bs_b, i)))
      return 0;
  }
  return 1;
}

int bitset_intersect(const Bitset *a, const Bitset *b, Bitset *result) {
  assert(a != NULL);
  assert(b != NULL);

  assert(result != NULL);

  const Bitset *smallest = a->max > b->max ? b : a;

  if (bitset_clone(smallest, result))
    return EXIT_FAILURE;

  const Bitset *largest = smallest == a ? b : a;
  // Loop over the bits in the smallest bitset and check if they also exist in
  // the largest bitset, if not, simply remove the bit from the result
  for (size_t i = 0; bitset_next(smallest, &i); i++) {
    if (!bitset_has(largest, i))
      bitset_excl(result, i);
  }

  return EXIT_SUCCESS;
}

int bitset_intersects(const Bitset *a, const Bitset *b) {
  assert(a != NULL);
  assert(b != NULL);

  for (size_t i = 0; bitset_next(a, &i); i++) {
    if (bitset_has(b, i))
      return 1;
  }

  return 0;
}

int bitset_union(const Bitset *a, const Bitset *b, Bitset *result) {
  assert(a != NULL);
  assert(b != NULL);

  assert(result != NULL);

  const Bitset *largest = a->max > b->max ? a : b;

  if (bitset_clone(largest, result))
    return EXIT_FAILURE;

  // Loop over the all of the bytes in the smaller bitset and incl set bits.
  const Bitset *smallest = largest == a ? b : a;
  size_t bytes_to_incl = byte_count(smallest->max);
  for (size_t i = 0; i < bytes_to_incl; i++)
    result->bytes[i] |= smallest->bytes[i];

  return EXIT_SUCCESS;
}

int bitset_difference(const Bitset *a, const Bitset *b, Bitset *result) {
  assert(a != NULL);
  assert(b != NULL);

  assert(result != NULL);

  const Bitset *largest = a->max > b->max ? a : b;

  if (bitset_clone(largest, result))
    return EXIT_FAILURE;

  const Bitset *smallest = largest == a ? b : a;
  size_t bytes_to_excl = byte_count(smallest->max);
  for (size_t i = 0; i < bytes_to_excl; i++)
    result->bytes[i] &= ~(smallest->bytes[i]);

  return EXIT_SUCCESS;
}

uint32_t bitset_hash(const void *bs) {
  const Bitset *casted = bs;
  assert(casted != NULL);

  return fnv1a_32_hash(casted->bytes, byte_count(casted->max));
}
