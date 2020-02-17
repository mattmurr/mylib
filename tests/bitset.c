/**
 * bitset/bitset_init.c
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
#include "../src/bitset.h"

#include <assert.h>

int main() {
	struct bitset bs;
	assert(!bitset_init(&bs, 10));

	assert(bitset_count(&bs) == 0);

	bitset_incl(&bs, 1);
	assert(bitset_count(&bs) == 1);

	bitset_incl(&bs, 10);
	// Ensure that the bitset has 10 set.
	assert(bitset_has(&bs, 10));

	// Check that the bitset now has 2 bits set.
	assert(bitset_count(&bs) == 2);

	struct bitset other;
	assert(!bitset_clone(&bs, &other));

	// Check that the clone count is correct.
	assert(bitset_count(&other) == 2);

	// and that it contains both values.
	assert(bitset_has(&other, 1));
	assert(bitset_has(&other, 10));

	bitset_deinit(&other);

	// Clear the bitset.
	bitset_clear(&bs);

	// Include all of the values in the array.
	uint8_t values[] = {1, 2, 5, 9, 10};
	for (int i = 0; i < 5; i++) {
		bitset_incl(&bs, values[i]);
	}

	// Iterate the bitset and test that all the values are set.
	size_t j = 0;
	for (size_t i = 0; bitset_next(&bs, &i); i++, j++) {
		assert(i == values[j]);
	}

	// Clone and test eql
	assert(!bitset_clone(&bs, &other));
	assert(bitset_eql(&bs, &other));

	// Ensure that bitset_is_proper_subset works.
	assert(!bitset_is_proper_subset(&other, &bs));
	// It should also be a subset.
	assert(bitset_is_subset(&bs, &other));
	// Add a new value to the original bitset so that the cloned
	// is a proper subset.
	bitset_incl(&bs, 3);
	assert(bitset_is_proper_subset(&other, &bs));

	// other should no longer be a subset of bs.
	assert(!bitset_is_subset(&bs, &other));
	// But bs is a subset of other.
	assert(bitset_is_subset(&other, &bs));

	bitset_deinit(&other);

	struct bitset intersect;
	assert(!bitset_init(&intersect, 15));

	// bs should not intersect with last as they share no common bits.
	assert(!bitset_intersects(&bs, &intersect));

	// Add a bit to intersect which bs contains.
	bitset_incl(&intersect, 1);

	// There should now be an intersection.
	assert(bitset_intersects(&bs, &intersect));

	// Create a union bitset of bs and intersect.
	struct bitset unionized;
	assert(!bitset_union(&unionized, &bs, &intersect));

	bitset_deinit(&bs);

	// Check that it contains the of both bitsets.
	for (size_t i = 0; i < 5; i++) {
		assert(bitset_has(&unionized, values[i]));
	}

	// Include a value into the intersect bitset which is not in the
	// unionized bitset.
	bitset_incl(&intersect, 13);

	struct bitset difference;
	assert(!bitset_difference(&difference, &unionized, &intersect));

	bitset_deinit(&intersect);
	bitset_deinit(&unionized);

	// Should contain 13 and no other bits.
	assert(bitset_has(&difference, 13));

	for (size_t i = 0; i < 5; i++) {
		assert(!bitset_has(&difference, values[i]));
	}

	bitset_deinit(&difference);

	// Test hash
	assert(!bitset_init(&bs, 50));
	bitset_incl(&bs, 10);
	bitset_incl(&bs, 22);

	assert(!bitset_init(&other, 50));
	bitset_incl(&other, 49);
	bitset_incl(&other, 5);
	assert(bitset_hash(&bs) != bitset_hash(&other));

	bitset_clear(&other);

	bitset_incl(&other, 10);
	bitset_incl(&other, 22);
	assert(bitset_hash(&bs) == bitset_hash(&other));

	bitset_incl(&other, 50);
	assert(bitset_hash(&bs) != bitset_hash(&other));

	bitset_deinit(&bs);
	bitset_deinit(&other);

	return EXIT_SUCCESS;
}
