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

typedef struct LinkedListNode {
  void *data;
  struct LinkedListNode *next;
} LinkedListNode;

typedef struct LinkedList {
  LinkedListNode *first; // Pointer to the first node in the list.
} LinkedList;

LinkedListNode *linked_list_node_init(void *data, size_t element_size);

void linked_list_node_deinit(LinkedListNode *node);

LinkedList linked_list_init();

LinkedListNode *linked_list_pop_first(LinkedList *list);

void linked_list_clear(LinkedList *list);

void linked_list_deinit(LinkedList *list);

int linked_list_prepend_node(LinkedList *list, LinkedListNode *node);

int linked_list_prepend(LinkedList *list, void *data, size_t element_size);

int linked_list_insert_node_after(LinkedListNode *node,
                                  LinkedListNode *new_node);

int linked_list_insert_after(LinkedList *list, LinkedListNode *node, void *data,
                             size_t element_size);

void linked_list_delete(LinkedList *list, LinkedListNode *node);

#endif
