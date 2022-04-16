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

typedef struct rte_waitable_object_t rte_waitable_number_t;

AGORA_RTE_API rte_waitable_number_t* rte_waitable_number_create(
  int64_t init_value);

AGORA_RTE_API void rte_waitable_number_destroy(rte_waitable_number_t* number);

AGORA_RTE_API void rte_waitable_number_increase(rte_waitable_number_t* number,
                                                int64_t value);

AGORA_RTE_API void rte_waitable_number_decrease(rte_waitable_number_t* number,
                                                int64_t value);

AGORA_RTE_API void rte_waitable_number_multiply(rte_waitable_number_t* number,
                                                int64_t value);

AGORA_RTE_API void rte_waitable_number_devide(rte_waitable_number_t* number,
                                              int64_t value);

AGORA_RTE_API void rte_waitable_number_set(rte_waitable_number_t* number,
                                           int64_t value);

AGORA_RTE_API int64_t rte_waitable_number_get(rte_waitable_number_t* number);

AGORA_RTE_API int rte_waitable_number_wait_until(rte_waitable_number_t* number,
                                                 int64_t value,
                                                 int timeout);

AGORA_RTE_API int rte_waitable_number_wait_while(rte_waitable_number_t* number,
                                                 int64_t value,
                                                 int timeout);