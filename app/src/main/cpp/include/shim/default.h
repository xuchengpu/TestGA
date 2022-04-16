/**
 *
 * Agora Real Time Engagement
 * Created by Zheng, Ender in 2021-08.
 * Copyright (c) 2021 Agora IO. All rights reserved.
 *
 */
#pragma once

#include "rte_config.h"

#include "common/config.h"

typedef struct rte_runloop_t rte_runloop_t;

typedef struct rte_facility_config_t {
  rte_tp_log_t logger;
  const char* appid;
  const char* install_id;
  const char* version;
  const char* area;
  uint16_t* port_list;
  size_t port_list_size;
  uint16_t port_rang_start;
  uint16_t port_rang_end;
} rte_facility_config_t;

AGORA_RTE_API rte_facility_delegate_t* rte_facility_delegate_create_default(
  rte_runloop_t* loop,
  const rte_facility_config_t* config);

AGORA_RTE_API void rte_facility_delegate_destroy(
  rte_facility_delegate_t* delegate);
