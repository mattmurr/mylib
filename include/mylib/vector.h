/**
 * include/mylib/vector.h
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
#ifndef MY_LIB_VECTOR_H
#define MY_LIB_VECTOR_H

#include <stdlib.h>

struct vector {
    size_t size;
    size_t capacity;
    size_t element_size;

    void *data;
};

struct vector *vector_init_with_capacity(size_t element_size, size_t capacity);
struct vector *vector_init(size_t element_size);
struct vector *vector_clone(const struct vector *src);
void vector_deinit(struct vector *vec);
int vector_resize(struct vector *vec, size_t new_capacity);
size_t vector_len(const struct vector *vec);
size_t vector_size_in_bytes(const struct vector *vec);
int vector_assign(struct vector *vec, size_t idx, void *element);
int vector_append(struct vector *vec, void *element);
int vector_insert(struct vector *vec, size_t idx, void *element);
void *vector_get(struct vector *vec, size_t idx);
const void *vector_get_const(const struct vector *vec, size_t idx);
void vector_delete(struct vector *vec, size_t idx);
void vector_swap_delete(struct vector *vec, size_t idx);
void vector_clear(struct vector *vec);
void vector_shrink_to_fit(struct vector *vec);

#endif
