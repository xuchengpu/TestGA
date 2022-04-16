/**
 *
 * Agora Real Time Engagement
 * Created by Zheng, Ender in 2021-08.
 * Copyright (c) 2021 Agora IO. All rights reserved.
 *
 */
#pragma once

#include "rte_config.h"

#include <stddef.h>
#include <stdint.h>
#include <string.h>

/** \addtogroup container
 *  @{
 */

typedef struct rte_array_t {
  size_t item_size;
  size_t item_count;
  void* items;
} rte_array_t;

AGORA_RTE_API rte_array_t* rte_array_create(size_t item_size,
                                            size_t item_count);

AGORA_RTE_API void rte_array_destroy(rte_array_t* array);

AGORA_RTE_API int rte_array_init(rte_array_t* array,
                                 size_t item_size,
                                 size_t item_count);

AGORA_RTE_API int rte_array_deinit(rte_array_t* array);

AGORA_RTE_API int rte_array_set(rte_array_t* array, int index, const void* val);

AGORA_RTE_API int rte_array_get(rte_array_t* array, int index, void* val);

AGORA_RTE_API void* rte_array_at(rte_array_t* array, int index);

static inline size_t rte_array_mem_size(rte_array_t* array) {
  return array ? (array->item_size * array->item_count) : 0;
}

static inline void rte_array_fill(rte_array_t* array, int c, size_t len) {
  if (array) {
    memset(array->items, c, len);
  }
}

static inline void rte_array_copy(rte_array_t* array,
                                  const void* src,
                                  size_t n) {
  if (array) {
    memcpy(array->items, src, n);
  }
}

/** @}*/