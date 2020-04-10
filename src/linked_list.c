/**
 * linked_list.c
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
#include "mylib/linked_list.h"
#include <assert.h>
#include <string.h>

LinkedListNode *linked_list_node_init(void *data, size_t element_size) {
  assert(data != NULL);

  LinkedListNode *result = calloc(1, sizeof(LinkedListNode));
  if (!result)
    return NULL;

  result->data = malloc(element_size);
  if (!result->data) {
    free(result);
    return NULL;
  }

  memcpy(result->data, data, element_size);

  return result;
}

void linked_list_node_deinit(LinkedListNode *node) {
  assert(node != NULL);

  free(node->data);
  free(node);
}

LinkedList linked_list_init() { return (LinkedList){0}; }

LinkedListNode *linked_list_pop_first(LinkedList *list) {
  assert(list != NULL);

  LinkedListNode *first;
  if (!(first = list->first))
    return NULL;

  list->first = first->next;

  return first;
}

void linked_list_clear(LinkedList *list) {
  // Repeatedly pop the first node until empty.
  LinkedListNode *node;
  while ((node = linked_list_pop_first(list))) {
    linked_list_node_deinit(node);
  }

  list->first = NULL;
}

void linked_list_deinit(LinkedList *list) { linked_list_clear(list); }

int linked_list_prepend_node(LinkedList *list, LinkedListNode *node) {
  assert(list != NULL);
  assert(node != NULL);

  node->next = list->first;

  list->first = node;

  return EXIT_SUCCESS;
}

int linked_list_prepend(LinkedList *list, void *data, size_t element_size) {
  assert(list != NULL);

  LinkedListNode *node = linked_list_node_init(data, element_size);
  if (!node)
    return EXIT_FAILURE;

  return linked_list_prepend_node(list, node);
}

int linked_list_insert_node_after(LinkedListNode *node,
                                  LinkedListNode *new_node) {
  assert(node != NULL);
  assert(new_node != NULL);

  new_node->next = node->next;
  node->next = new_node;

  return EXIT_SUCCESS;
}

int linked_list_insert_after(LinkedList *list, LinkedListNode *node, void *data,
                             size_t element_size) {
  assert(list != NULL);

  LinkedListNode *new_node = linked_list_node_init(data, element_size);
  if (!new_node)
    return EXIT_FAILURE;

  return linked_list_insert_node_after(node, new_node);
}

void linked_list_delete(LinkedList *list, LinkedListNode *node) {
  assert(list != NULL);
  assert(node != NULL);

  LinkedListNode *curr = list->first;

  if (curr == node) {
    list->first = curr->next;
  } else {
    LinkedListNode *prev; // Keep track of the previous node.

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
