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

#if !defined(NDEBUG)
#define RTE_SM_MAX_HISTORY 10
#else
#define RTE_SM_MAX_HISTORY 1
#endif

typedef struct rte_sm_t rte_sm_t;

typedef struct rte_sm_state_history_t {
  int from;
  int event;
  int reason;
  int to;
} rte_sm_state_history_t;

typedef void (*rte_sm_op)(rte_sm_t* sm,
                          const rte_sm_state_history_t* top,
                          void* arg);

#define RTE_REASON_ANY (-1)

typedef struct rte_sm_state_entry_t {
  int current;
  int event;
  int reason;
  int next;
  rte_sm_op operation;
} rte_sm_state_entry_t;

typedef struct rte_sm_auto_trans_t {
  int from_state;
  int to_state;
  int auto_trigger;
  int trigger_reason;
} rte_sm_auto_trans_t;

AGORA_RTE_API rte_sm_t* rte_state_machine_create();

AGORA_RTE_API void rte_state_machine_destroy(rte_sm_t* sm);

AGORA_RTE_API int rte_state_machine_init(rte_sm_t* sm,
                                         int begin_state,
                                         rte_sm_op default_op,
                                         const rte_sm_state_entry_t* entries,
                                         size_t entry_count,
                                         const rte_sm_auto_trans_t* trans,
                                         size_t trans_count);

AGORA_RTE_API int rte_state_machine_reset_state(rte_sm_t* sm);

AGORA_RTE_API int rte_state_machine_trigger(rte_sm_t* sm,
                                            int event,
                                            int reason,
                                            void* arg);

AGORA_RTE_API int rte_state_machine_current_state(const rte_sm_t* sm);
