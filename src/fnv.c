/**
 * fnv.c
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
#include "mylib/bitset.h"
#include "mylib/hash.h"

const uint32_t PRIME_32 = 16777619;
const uint32_t OFFSET_32 = 2166136261;

uint32_t fnv1a_32_init() { return OFFSET_32; }

void fnv1a_32_update(uint32_t *val, const uint8_t *arr, size_t size) {
  for (size_t i = 0; i < size; i++) {
    *val *= PRIME_32;
    *val ^= arr[i];
  }
}

uint32_t fnv1a_32_hash(const uint8_t *arr, size_t size) {
  uint32_t val = fnv1a_32_init();
  fnv1a_32_update(&val, arr, size);
  return val;
}
