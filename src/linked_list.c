/**
 * src/linked_list.c
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
#include "linked_list.h"
#include <string.h>

struct linked_list_node *node_init(void *data, size_t element_size) {
    // Allocate memory for the actual node.
    struct linked_list_node *node = malloc(sizeof(struct linked_list_node));
    if (!node)
        return NULL;

    // Then the node's data.
    node->data = malloc(element_size);
    // If the allocation fails then we also need to free the node.
    if (!node->data) {
        free(node);
        return NULL;
    }

    memcpy(node->data, data, element_size);

    return node;
}

void linked_list_node_deinit(struct linked_list_node *node) {
    if (node == NULL)
        return;

    free(node->data);
    free(node);
}

struct linked_list *linked_list_init() {
    return calloc(1, sizeof(struct linked_list));
}

void linked_list_clear(struct linked_list *list) {
    // Repeatedly pop the first node until empty.
    struct linked_list_node *node;
    while ((node = linked_list_pop_first(list))) {
        linked_list_node_deinit(node);
    }

    list->first = NULL;
}

void linked_list_deinit(struct linked_list *list) {
    linked_list_clear(list);
    free(list);
}

int linked_list_prepend_node(struct linked_list *list,
                             struct linked_list_node *node) {
    node->next = list->first;

    list->first = node;

    return EXIT_SUCCESS;
}

int linked_list_prepend(struct linked_list *list, void *data,
                        size_t element_size) {
    struct linked_list_node *node = node_init(data, element_size);
    if (!node)
        return EXIT_FAILURE;

    return linked_list_prepend_node(list, node);
}

int linked_list_insert_node_after(struct linked_list *list,
                                  struct linked_list_node *node,
                                  struct linked_list_node *new_node) {
    new_node->next = node->next;
    node->next = new_node;

    return EXIT_SUCCESS;
}

int linked_list_insert_after(struct linked_list *list,
                             struct linked_list_node *node, void *data,
                             size_t element_size) {
    struct linked_list_node *new_node = node_init(data, element_size);
    if (!node)
        return EXIT_FAILURE;

    return linked_list_insert_node_after(list, node, new_node);
}

void linked_list_delete(struct linked_list *list,
                        struct linked_list_node *node) {
    struct linked_list_node *curr = list->first;

    if (curr == node) {
        list->first = curr->next;
    } else {
        struct linked_list_node *prev; // Keep track of the previous node.

        while (curr != node) {
            prev = curr; // Assign the previous node.
            // Return NULL if we reach the end of the list.
            if (!(curr = curr->next))
                return;
        }

        // Modify the previous node's next, essentially relinking the chain.
        prev->next = curr->next;
    }

    // deinit curr or node, they are the same.
    // TODO Is this one of those situations where I should actually use assert?
    linked_list_node_deinit(curr);
}

struct linked_list_node *linked_list_pop_first(struct linked_list *list) {
    struct linked_list_node *first;
    // If node == NULL return NULL or else we are returning a pointer to a
    // random address.
    if (!(first = list->first))
        return NULL;

    list->first = first->next;

    return first;
}
