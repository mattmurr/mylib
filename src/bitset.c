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
#include "mylib/bitset.h"
#include "mylib/hash.h"
#include <string.h>

static size_t get_byte(size_t bit) { return bit >> 3; }

static size_t byte_count(size_t bit) { return get_byte(bit) + 1; }

static uint8_t get_bit_offset(size_t bit) { return 7 - (bit % 8); }

struct bitset *bitset_init(size_t max) {
    size_t required_bytes = byte_count(max);
    struct bitset *result = malloc(sizeof(struct bitset));
    if (!result)
        return NULL;

    if (!(result->bytes = calloc(required_bytes, sizeof(uint8_t)))) {
        free(result);
        return NULL;
    }

    result->max = max;

    return result;
}

void bitset_deinit(struct bitset *bs) {
    if (bs == NULL)
        return;

    free(bs->bytes);
    free(bs);
}

struct bitset *bitset_clone(const struct bitset *bs) {
    if (bs == NULL)
        return NULL;

    struct bitset *result = bitset_init(bs->max);
    if (!result)
        return NULL;

    size_t bytes_to_copy = byte_count(bs->max);
    memcpy(result->bytes, bs->bytes, sizeof(uint8_t) * bytes_to_copy);

    return result;
}

// Adapted for uint8_t from Hacker's Delight, p. 66, Figure 5-2.
static uint8_t popcount(uint8_t byte) {
    byte = byte - ((byte >> 1) & 0x55);
    byte = (byte & 0x33) + ((byte >> 2) & 0x33);
    byte = (byte + (byte >> 4)) & 0x0F;

    return byte;
}

size_t bitset_count(const struct bitset *bs) {
    if (bs == NULL)
        return 0;

    size_t bytes_to_count = byte_count(bs->max);

    int result = 0;
    for (int i = 0; i < bytes_to_count; i++) {
        result += popcount(bs->bytes[i]);
    }

    return result;
}

size_t bitset_size_in_bytes(const struct bitset *bs) {
    if (bs == NULL)
        return 0;

    return byte_count(bs->max);
}

void bitset_clear(struct bitset *bs) {
    if (bs == NULL)
        return;

    size_t bytes_to_clear = byte_count(bs->max);
    memset(bs->bytes, 0, sizeof(uint8_t) * bytes_to_clear);
}

int bitset_has(const struct bitset *bs, size_t bit) {
    if (bs == NULL)
        return 0;
    if (bit > bs->max)
        return 0;

    size_t byte = get_byte(bit);
    size_t offset = get_bit_offset(bit);
    return bs->bytes[byte] & (1 << offset);
}

int bitset_incl(struct bitset *bs, size_t bit) {
    if (bs == NULL)
        return EXIT_FAILURE;

    if (bit > bs->max)
        return EXIT_FAILURE;

    size_t byte = get_byte(bit);
    size_t offset = get_bit_offset(bit);
    bs->bytes[byte] |= ((uint8_t)1 << offset);

    return EXIT_SUCCESS;
}

void bitset_excl(struct bitset *bs, size_t bit) {
    if (bs == NULL)
        return;

    if (bit > bs->max)
        return;

    size_t byte = get_byte(bit);
    size_t offset = get_bit_offset(bit);
    bs->bytes[byte] &= ~((uint8_t)1 << offset);
}

int bitset_next(const struct bitset *bs, size_t *i) {
    if (bs == NULL)
        return 0;
    if (i == NULL)
        return 0;

    while (*i <= bs->max) {
        if (bitset_has(bs, *i))
            return 1;
        *i += 1;
    }

    return 0;
}

int bitset_is_subset(const struct bitset *a, const struct bitset *b) {
    // If both are NULL, they are then they are subsets of eachother?
    /*if (a == NULL && b == NULL)*/
    /*return 1;*/

    if (a == NULL || b == NULL)
        return 0;

    for (size_t i = 0; bitset_next(a, &i); i++) {
        if (!bitset_has(b, i))
            return 0;
    }
    return 1;
}

int bitset_is_proper_subset(const struct bitset *a, const struct bitset *b) {
    if (a == NULL || b == NULL)
        return 0;

    return bitset_is_subset(a, b) && !bitset_is_subset(b, a);
}

int bitset_eql(const void *a, const void *b) {
    // If both are NULL, they are equal right?
    /*if (a == NULL && b == NULL)*/
    /*return 1;*/

    if (a == NULL || b == NULL)
        return 0;

    // Cast both of the bitsets.
    const struct bitset *bs_a = a;
    const struct bitset *bs_b = b;

    for (size_t i = 0; i < (bs_a->max > bs_b->max ? bs_a->max : bs_b->max);
         i++) {
        if (!(bitset_has(bs_a, i) == bitset_has(bs_b, i)))
            return 0;
    }
    return 1;
}

int bitset_intersects(const struct bitset *a, const struct bitset *b) {
    if (a == NULL || b == NULL)
        return 0;

    for (size_t i = 0; bitset_next(a, &i); i++) {
        if (bitset_has(b, i))
            return 1;
    }

    return 0;
}

struct bitset *bitset_union(const struct bitset *a, const struct bitset *b) {
    if (a == NULL || b == NULL)
        return NULL;

    const struct bitset *largest = a->max > b->max ? a : b;

    struct bitset *result = bitset_clone(largest);
    if (!result)
        return NULL;

    // Loop over the all of the bytes in the smaller bitset and incl set bits.
    const struct bitset *smallest = largest == a ? b : a;
    size_t bytes_to_incl = byte_count(smallest->max);
    for (size_t i = 0; i < bytes_to_incl; i++) {
        result->bytes[i] |= smallest->bytes[i];
    }

    return result;
}

struct bitset *bitset_difference(const struct bitset *a,
                                 const struct bitset *b) {
    if (a == NULL || b == NULL)
        return NULL;

    const struct bitset *largest = a->max > b->max ? a : b;

    struct bitset *result = bitset_clone(largest);
    if (!result)
        return NULL;

    const struct bitset *smallest = largest == a ? b : a;
    size_t bytes_to_excl = byte_count(smallest->max);
    for (size_t i = 0; i < bytes_to_excl; i++) {
        result->bytes[i] &= ~(smallest->bytes[i]);
    }

    return result;
}

uint32_t bitset_hash(const void *bs) {
    if (bs == NULL)
        return EXIT_FAILURE;

    const struct bitset *casted = bs;
    return fnv1a_32_hash(casted->bytes, byte_count(casted->max));
}
