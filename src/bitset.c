/**
 * src/bitset.c
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
#include "bitset.h"
#include "hash.h"
#include <string.h>

static size_t get_byte(size_t bit) { return bit >> 3; }

static size_t byte_count(size_t bit) { return get_byte(bit) + 1; }

static uint8_t get_bit_offset(size_t bit) { return 7 - (bit % 8); }

int bitset_init(struct bitset *bs, size_t max) {
    size_t required_bytes = byte_count(max);
    if (!(bs->bytes = calloc(required_bytes, sizeof(uint8_t))))
        return EXIT_FAILURE;

    bs->max = max;

    return EXIT_SUCCESS;
}

void bitset_deinit(struct bitset *bs) { free(bs->bytes); }

int bitset_clone(const struct bitset *src, struct bitset *dest) {
    if (src == NULL || dest == NULL)
        return EXIT_FAILURE;

    if (bitset_init(dest, src->max))
        return EXIT_FAILURE;

    size_t bytes_to_copy = byte_count(src->max);
    memcpy(dest->bytes, src->bytes, sizeof(uint8_t) * bytes_to_copy);

    return EXIT_SUCCESS;
}

// Adapted for uint8_t from Hacker's Delight, p. 66, Figure 5-2.
static uint8_t popcount(uint8_t byte) {
    byte = byte - ((byte >> 1) & 0x55);
    byte = (byte & 0x33) + ((byte >> 2) & 0x33);
    byte = (byte + (byte >> 4)) & 0x0F;

    return byte;
}

size_t bitset_count(const struct bitset *bs) {
    size_t bytes_to_count = byte_count(bs->max);

    size_t result = 0;
    for (size_t i = 0; i < bytes_to_count; i++) {
        result += popcount(bs->bytes[i]);
    }

    return result;
}

size_t bitset_size_in_bytes(const struct bitset *bs) {
    return byte_count(bs->max);
}

void bitset_clear(struct bitset *bs) {
    size_t bytes_to_clear = byte_count(bs->max);
    memset(bs->bytes, 0, sizeof(uint8_t) * bytes_to_clear);
}

int bitset_has(const struct bitset *bs, size_t bit) {
    if (bit > bs->max)
        return 0;
    size_t byte = get_byte(bit);
    size_t offset = get_bit_offset(bit);
    return bs->bytes[byte] & (1 << offset);
}

void bitset_incl(struct bitset *bs, size_t bit) {
    if (bit > bs->max)
        return;
    size_t byte = get_byte(bit);
    size_t offset = get_bit_offset(bit);
    bs->bytes[byte] |= ((uint8_t)1 << offset);
}

void bitset_excl(struct bitset *bs, size_t bit) {
    if (bit > bs->max)
        return;
    size_t byte = get_byte(bit);
    size_t offset = get_bit_offset(bit);
    bs->bytes[byte] &= ~((uint8_t)1 << offset);
}

int bitset_next(const struct bitset *bs, size_t *i) {
    while (*i <= bs->max) {
        if (bitset_has(bs, *i))
            return 1;
        *i += 1;
    }

    return 0;
}

int bitset_is_subset(const struct bitset *a, const struct bitset *b) {
    for (size_t i = 0; bitset_next(a, &i); i++) {
        if (!bitset_has(b, i))
            return 0;
    }
    return 1;
}

int bitset_is_proper_subset(const struct bitset *a, const struct bitset *b) {
    return bitset_is_subset(a, b) && !bitset_is_subset(b, a);
}

int bitset_eql(const struct bitset *a, const struct bitset *b) {
    for (size_t i = 0; i < (a->max > b->max ? a->max : b->max); i++) {
        if (!(bitset_has(a, i) == bitset_has(b, i)))
            return 0;
    }
    return 1;
}

int bitset_intersects(const struct bitset *a, const struct bitset *b) {
    for (size_t i = 0; bitset_next(a, &i); i++) {
        if (bitset_has(b, i))
            return 1;
    }

    return 0;
}

int bitset_union(struct bitset *result, const struct bitset *a,
                 const struct bitset *b) {
    const struct bitset *largest = a->max > b->max ? a : b;

    if (bitset_clone(largest, result))
        return EXIT_FAILURE;

    const struct bitset *smallest = largest == a ? b : a;
    // Loop over the all of the bytes in the smallest bitset and incl set bits.
    size_t bytes_to_incl = byte_count(smallest->max);
    for (size_t i = 0; i < bytes_to_incl; i++) {
        result->bytes[i] |= smallest->bytes[i];
    }

    return EXIT_SUCCESS;
}

int bitset_difference(struct bitset *result, const struct bitset *a,
                      const struct bitset *b) {
    const struct bitset *largest = a->max > b->max ? a : b;

    if (bitset_clone(largest, result))
        return EXIT_FAILURE;

    const struct bitset *smallest = largest == a ? b : a;
    size_t bytes_to_excl = byte_count(smallest->max);
    for (size_t i = 0; i < bytes_to_excl; i++) {
        result->bytes[i] &= ~(smallest->bytes[i]);
    }

    return EXIT_SUCCESS;
}

uint32_t bitset_hash(void *bs_ptr) {
    struct bitset *bs = (struct bitset *)bs_ptr;
    return fnv1a_32_hash(bs->bytes, byte_count(bs->max));
}
