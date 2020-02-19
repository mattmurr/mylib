#include "mylib/vector.h"
#include <assert.h>

int main() {
    struct vector *vec = vector_init(sizeof(int));
    assert(vec != NULL);

    int a = 5;

    // Add a value and ensure that the value is retrievable.
    assert(!vector_append(vec, &a));
    assert(*((int *)vector_get(vec, 0)) == a);

    // Fill the vector up
    for (int i = 0; i < 50; i++) {
        assert(!vector_insert(vec, 0, &i));
        assert(*((int *)vector_get(vec, 0)) == i);
    }

    // Clone the vector and ensure that it has the correct size.
    struct vector *clone = vector_clone(vec);
    assert(clone != NULL);

    assert(vector_len(clone) == 51);

    vector_delete(vec, 50);

    // Iterate the array.
    int *arr = vec->data;
    for (size_t i = 0; i < vector_len(vec); i++)
        assert(arr[i] == 49 - i);

    // Test swap delete.
    vector_swap_delete(vec, 20);
    assert(*((const int *)vector_get_const(vec, 20)) == 0);
}
