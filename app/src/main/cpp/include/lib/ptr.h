/**
 *
 * Agora Real Time Engagement
 * Created by Wei Hu in 2021-07.
 * Copyright (c) 2021 Agora IO. All rights reserved.
 *
 */
#pragma once

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>

#define PTR_FILL_VALUE(ptr, value)                  \
  ({                                                \
    *(typeof(value)*)ptr = value;                   \
    ptr = rte_ptr_move_in_byte(ptr, sizeof(value)); \
  })

#define PTR_FILL_STR(ptr, str)                    \
  ({                                              \
    *(typeof(value)*)ptr = value;                 \
    strcpy(ptr, str);                             \
    ptr = rte_ptr_move_in_byte(ptr, strlen(str)); \
  })

// NOLINTNEXTLINE(clang-diagnostic-unused-function)
static inline void* rte_ptr_move_in_byte(void* ptr, ptrdiff_t offset) {
  assert(ptr);
  return (void*)((uint8_t*)ptr + offset);
}

// NOLINTNEXTLINE(clang-diagnostic-unused-function)
static inline const void* rte_const_ptr_move_in_byte(const void* ptr,
                                                     ptrdiff_t offset) {
  assert(ptr);
  return (const void*)((const uint8_t*)ptr + offset);
}

// NOLINTNEXTLINE(clang-diagnostic-unused-function)
static inline ptrdiff_t rte_ptr_diff_in_byte(void* a, void* b) {
  assert(a && b && a >= b);
  return (uint8_t*)a - (uint8_t*)b;
}

// NOLINTNEXTLINE(clang-diagnostic-unused-function)
static inline ptrdiff_t rte_const_ptr_diff_in_byte(const void* a,
                                                   const void* b) {
  assert(a && b && a >= b);
  return (const uint8_t*)a - (const uint8_t*)b;
}
