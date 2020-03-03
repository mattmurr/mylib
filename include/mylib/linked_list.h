/**
 * mylib/linked_list.h
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
#ifndef MYLIB_LINKED_LIST_H
#define MYLIB_LINKED_LIST_H

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
