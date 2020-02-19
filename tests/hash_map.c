/**
 * tests/hash_map.c
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
#include "mylib/hash_map.h"
#include "mylib/hash.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

static unsigned int hash_str(const void *str) {
    return fnv1a_32_hash(str, strlen(str) + 1);
}

static int eql_str(const void *a, const void *b) {
    return strcmp((const char *)a, (const char *)b) == 0;
}

int main() {
    struct hash_map *map =
        hash_map_init(hash_str, eql_str, sizeof(char *), sizeof(int));
    assert(map && "Failed to initialize map.\n");

    char *key = "myfirstkey";
    int val = 95;

    assert(!hash_map_put(map, &key, &val));

    assert(hash_map_count(map) == 1);

    assert(*((int *)hash_map_get_value(map, &key)) == 95);

    key = "wedonthavethiskey";
    assert(!hash_map_has(map, &key));

    key = "mysecondkey";
    val = 128;
    assert(!hash_map_put(map, &key, &val));

    assert(hash_map_count(map) == 2);

    // Try to get kv using key that isn't in the map.
    key = "wealsodonthavethiskey";
    assert(!hash_map_get(map, &key));

    // Clear the map then add some new values.
    hash_map_clear(map);

    for (unsigned long i = 1; i <= 15; i++) {
        const int n = snprintf(NULL, 0, "%lu", i);
        char buf[n + 1];
        assert(snprintf(buf, n + 1, "%lu", i) == n);

        // We cant add zero as a key because it would be a NULL pointer.
        assert(!hash_map_put(map, &buf, &i));
        // Check that the value is added correctly.
        assert(*((int *)hash_map_get_value(map, &buf)) == i);
    }

    // Iterate the map.
    struct hash_map_iterator iter = hash_map_iter(map);
    struct hash_map_kv *kv;
    while ((kv = hash_map_next(&iter))) {
        val = *((int *)kv->value);
        // Val should be > 0 and less than or equal to 15.
        assert(val > 0 && val <= 15);
    }

    hash_map_deinit(map);
}
