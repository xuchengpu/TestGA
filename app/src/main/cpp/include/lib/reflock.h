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
#include "lib/event.h"

typedef enum REFLOCK_FLAG {
  RTE_REFLOCK_REF = (int64_t)0x00000001,
  RTE_REFLOCK_DESTROY = (int64_t)0x10000000,
  RTE_REFLOCK_POISON = (int64_t)0x300dead0,
  RTE_REFLOCK_DESTROY_MASK = (int64_t)0xf0000000,
  RTE_REFLOCK_REF_MASK = (int64_t)0x0fffffff,
} REFLOCK_FLAG;

typedef struct rte_reflock_t {
  rte_atomic_t state;
  rte_event_t* event;
} rte_reflock_t;

/**
 * @brief Initialize a reflock.
 * @param reflock The reflock to initialize.
 */
AGORA_RTE_API void rte_reflock_init(rte_reflock_t* lock);

/**
 * @brief Increase the reference count of a reflock.
 * @param reflock The reflock to increase the reference count.
 */
AGORA_RTE_API void rte_reflock_ref(rte_reflock_t* lock);

/**
 * @brief Decrease the reference count of a reflock.
 * @param reflock The reflock to decrease the reference count.
 */
AGORA_RTE_API void rte_reflock_unref(rte_reflock_t* lock);

/**
 * @brief Decrease reference count and destroy after it's zero.
 * @param reflock The reflock to decrease the reference count.
 * @note This function will wait until the reflock is zero
 */
AGORA_RTE_API void rte_reflock_unref_destroy(rte_reflock_t* lock);