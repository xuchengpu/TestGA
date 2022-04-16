/**
 *
 * Agora Real Time Engagement
 * Created by Zheng, Ender in 2021-08.
 * Copyright (c) 2021 Agora IO. All rights reserved.
 *
 */
#pragma once

#include "rte_config.h"

typedef struct rte_kvop_t {
  int (*compare)(const void*, const void*);
  void* (*clone)(const void*);
  void (*free)(const void*);
} rte_kvop_t;

typedef struct rte_kvpair_t rte_kvpair_t;

typedef enum RTE_KNOWN_OP {
  RTE_KNOWN_OP_C_STR,
  RTE_KNOWN_OP_RTE_STR,
} RTE_KNOWN_OP;

AGORA_RTE_API const rte_kvop_t* rte_rbop_get_known_op(RTE_KNOWN_OP type);