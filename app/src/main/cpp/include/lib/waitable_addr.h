/**
 *
 * Agora Real Time Engagement
 * Created by Zheng, Ender in 2021-08.
 * Copyright (c) 2021 Agora IO. All rights reserved.
 *
 */
#pragma once

#include "rte_config.h"

#include <stdint.h>

typedef struct rte_spinlock_t rte_spinlock_t;
typedef struct rte_waitable_t {
  uint32_t sig;
} rte_waitable_t;

#define RTE_WAITABLE_INIT \
  { 0 }

AGORA_RTE_API void rte_waitable_init(rte_waitable_t* wb);

AGORA_RTE_API rte_waitable_t* rte_waitable_from_addr(uint32_t* address);

AGORA_RTE_API int rte_waitable_wait(rte_waitable_t* wb,
                                    uint32_t expect,
                                    rte_spinlock_t* lock,
                                    int timeout);

AGORA_RTE_API void rte_waitable_notify(rte_waitable_t* wb);

AGORA_RTE_API void rte_waitable_notify_all(rte_waitable_t* wb);

AGORA_RTE_API uint32_t rte_waitable_get(rte_waitable_t* wb);

AGORA_RTE_API void rte_waitable_set(rte_waitable_t* wb, uint32_t val);
