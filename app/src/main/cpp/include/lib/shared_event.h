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

#include "lib/atomic.h"
#include "lib/waitable_addr.h"

typedef struct rte_shared_event_t rte_shared_event_t;

AGORA_RTE_API rte_shared_event_t* rte_shared_event_create(
  uint32_t* addr_for_sig,
  rte_atomic_t* addr_for_lock,
  int init_state,
  int auto_reset);

AGORA_RTE_API int rte_shared_event_wait(rte_shared_event_t* event, int wait_ms);

AGORA_RTE_API void rte_shared_event_set(rte_shared_event_t* event);

AGORA_RTE_API void rte_shared_event_reset(rte_shared_event_t* event);

AGORA_RTE_API void rte_shared_event_destroy(rte_shared_event_t* event);
