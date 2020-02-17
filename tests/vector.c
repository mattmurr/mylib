#include "vector.h"
#include <assert.h>

int main() {
    struct vector *vec = vector_init(sizeof(int));
    assert(vec != NULL);

    int a = 5;

    assert(!vector_append(vec, &a));
    assert(*((int *)vector_get(vec, 0)) == a);

    for (int i = 0; i < 50; i++) {
        assert(!vector_insert(vec, 0, &i));
        assert(*((int *)vector_get(vec, 0)) == i);
    }

    struct vector *clone = vector_clone(vec);
    assert(clone != NULL);

    assert(vector_len(clone) == 51);
}
