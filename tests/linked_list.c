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
