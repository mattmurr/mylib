/**
 * include/mylib/hash.h
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
#ifndef MY_LIB_HASH_H
#define MY_LIB_HASH_H

#include <stddef.h>
#include <stdint.h>

uint32_t fnv1a_32_init();
void fnv1a_32_update(uint32_t *val, const uint8_t *arr, size_t size);
uint32_t fnv1a_32_hash(const uint8_t *arr, size_t size);

#endif
