/**
 *
 * Agora Real Time Engagement
 * Created by Wei Hu in 2021-07.
 * Copyright (c) 2021 Agora IO. All rights reserved.
 *
 */
#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "lib/atomic.h"

#define RTE_SHAREDPTR_SIGNATURE 0x7BB9769E3A5CBA5FU

#define RTE_SHAREDPTR_COUNTER_REDZONE 10000

typedef enum RTE_SHAREDPTR_TYPE {
  RTE_SHAREDPTR,
  RTE_WEAKPTR,
} RTE_SHAREDPTR_TYPE;

typedef struct rte_sharedptr_counter_t {
  rte_atomic_t ref_cnt;  // The reference count.
  rte_atomic_t weak_ref_cnt;
  void (*destroy)(void* data);
} rte_sharedptr_counter_t;

typedef struct rte_sharedptr_t {
  rte_atomic_t signature;
  RTE_SHAREDPTR_TYPE type;

  // Control the life of the memory space pointed by this rte_sharedptr_t
  // object.
  rte_sharedptr_counter_t* counter;

  // Control the life of this rte_sharedptr_t object.
  rte_atomic_t ref_cnt;

  void* data;
} rte_sharedptr_t;

#ifdef __cplusplus
#define rte_sharedptr_create(ptr, destroy) \
  rte_sharedptr_create_(ptr, reinterpret_cast<void (*)(void*)>(destroy))
#else
#define rte_sharedptr_create(ptr, destroy) \
  rte_sharedptr_create_(ptr, (void (*)(void*))(destroy))
#endif

// Shared pointer
AGORA_RTE_API rte_sharedptr_t* rte_sharedptr_create_(void* ptr,
                                                     void (*destroy)(void*));

AGORA_RTE_API int64_t rte_sharedptr_inc_rc(rte_sharedptr_t* self);

AGORA_RTE_API rte_sharedptr_t* rte_sharedptr_dec_rc(rte_sharedptr_t* self);

AGORA_RTE_API void* rte_sharedptr_get_data(rte_sharedptr_t* self);

// Weak pointer
AGORA_RTE_API rte_sharedptr_t* rte_weakptr_create(rte_sharedptr_t* shared_ptr);

AGORA_RTE_API rte_sharedptr_t* rte_weakptr_lock(rte_sharedptr_t* self);
