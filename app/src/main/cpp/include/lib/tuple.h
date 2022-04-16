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

/** \addtogroup container
 *  @{
 */

typedef void (*rte_tuple_field_destroy_func_t)(uintptr_t);
typedef struct rte_tuple_field_t {
  uintptr_t value;
  rte_tuple_field_destroy_func_t destroy;
} rte_tuple_field_t;

typedef struct rte_tuple_t {
  size_t field_count;
  rte_tuple_field_t* fields;
} rte_tuple_t;

AGORA_RTE_API rte_tuple_t* rte_tuple_create(size_t field_count);

AGORA_RTE_API void rte_tuple_destroy(rte_tuple_t* tuple);

AGORA_RTE_API int rte_tuple_init(rte_tuple_t* tuple, size_t field_count);

AGORA_RTE_API int rte_tuple_deinit(rte_tuple_t* tuple);

AGORA_RTE_API int rte_tuple_set(rte_tuple_t* tuple,
                                int index,
                                uintptr_t val,
                                rte_tuple_field_destroy_func_t destroy);

AGORA_RTE_API uintptr_t rte_tuple_get(rte_tuple_t* tuple, int index);