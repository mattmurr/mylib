/**
 * include/linked_list.h
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
#ifndef MY_LIB_LINKED_LIST_H
#define MY_LIB_LINKED_LIST_H

#include <stdlib.h>

struct linked_list_node {
    void *data;
    struct linked_list_node *next;
};

struct linked_list {
    struct linked_list_node *first; // Pointer to the first node in the list.
};

struct linked_list_node *linked_list_node_init(void *data, size_t element_size);

void linked_list_node_deinit(struct linked_list_node *node);

struct linked_list *linked_list_init();

void linked_list_clear(struct linked_list *list);

void linked_list_deinit(struct linked_list *list);

int linked_list_prepend_node(struct linked_list *list,
                             struct linked_list_node *node);

int linked_list_prepend(struct linked_list *list, void *data,
                        size_t element_size);

int linked_list_insert_node_after(struct linked_list *list,
                                  struct linked_list_node *node,
                                  struct linked_list_node *new_node);

int linked_list_insert_after(struct linked_list *list,
                             struct linked_list_node *node, void *data,
                             size_t element_size);

void linked_list_delete(struct linked_list *list,
                        struct linked_list_node *node);

struct linked_list_node *linked_list_pop_first(struct linked_list *list);

#endif
