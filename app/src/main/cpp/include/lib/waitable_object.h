/**
 *
 * Agora Real Time Engagement
 * Created by Zheng, Ender in 2021-08.
 * Copyright (c) 2021 Agora IO. All rights reserved.
 *
 */
#pragma once

#include "rte_config.h"

#include "stdint.h"

typedef struct rte_waitable_object_t rte_waitable_object_t;

AGORA_RTE_API rte_waitable_object_t* rte_waitable_object_create(
  void* init_value);

AGORA_RTE_API void rte_waitable_object_destroy(rte_waitable_object_t* obj);

AGORA_RTE_API void rte_waitable_object_set(rte_waitable_object_t* obj,
                                           void* value);

AGORA_RTE_API void* rte_waitable_object_get(rte_waitable_object_t* obj);

AGORA_RTE_API void rte_waitable_object_update(rte_waitable_object_t* obj);

AGORA_RTE_API int rte_waitable_object_wait_until(rte_waitable_object_t* obj,
                                                 int (*compare)(const void* l,
                                                                const void* r),
                                                 int timeout);

AGORA_RTE_API int rte_waitable_object_wait_while(rte_waitable_object_t* obj,
                                                 int (*compare)(const void* l,
                                                                const void* r),
                                                 int timeout);