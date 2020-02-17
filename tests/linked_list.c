/**
 * tests/linked_list.c
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
#include "../src/linked_list.h"
#include <assert.h>

int main() {
    struct linked_list *list = linked_list_init();
    assert(list && "Failed to initialize list.\n");

    const size_t i_size = sizeof(int);

    // Prepend to the list and check that the list->first contains the value.
    int val = 9994;
    assert(!linked_list_prepend(list, &val, i_size));

    assert(*((int *)list->first->data) == val);

    // Prepend another value and check that they are in the correct order.
    val = 15;
    assert(!linked_list_prepend(list, &val, i_size));

    struct linked_list_node *first = list->first;
    assert(*((int *)first->data) == val);
    assert(*((int *)first->next->data) == 9994);

    // Insert a value after the 2nd node (at the back of the list).
    val = 52;
    assert(!linked_list_insert_after(list, first->next, &val, i_size));

    // Pop all 3 nodes and check that the list is empty.
    struct linked_list_node *node = linked_list_pop_first(list);
    assert(*((int *)node->data) == 15);
    linked_list_node_deinit(node);

    node = linked_list_pop_first(list);
    assert(*((int *)node->data) == 9994);
    linked_list_node_deinit(node);

    node = linked_list_pop_first(list);
    assert(*((int *)node->data) == val);
    linked_list_node_deinit(node);

    assert(linked_list_pop_first(list) == NULL);

    // Add 20 values to the list.
    for (unsigned char i = 0; i < 20; i++) {
        assert(!linked_list_prepend(list, &i, sizeof(unsigned char)));
    }

    // Ensure that we can delete notes and the chain is not broken.
    first = list->first;
    linked_list_delete(list, first->next);

    assert(*((unsigned char *)first->next->data) != 18);

    assert(*((unsigned char *)list->first->data) == 19);

    // Clear the list
    linked_list_clear(list);

    assert(list->first == NULL &&
           "The node at the front of the list now be NULL.\n");

    for (int i = 0; i < 10; i++) {
        assert(!linked_list_prepend(list, &i, i_size));
    }

    // TODO Test iterator.
    struct linked_list_node *next = list->first;
    if (list->first) {
        int i = 9;
        do {
            assert(*((int *)next->data) == i);
            i--;
        } while ((next = next->next));
    }

    linked_list_deinit(list);
}
