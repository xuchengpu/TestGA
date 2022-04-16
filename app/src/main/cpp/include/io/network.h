/**
 *
 * Agora Real Time Engagement
 * Created by Wei Hu in 2021-07.
 * Copyright (c) 2021 Agora IO. All rights reserved.
 *
 */
#pragma once

#include "rte_config.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "lib/string.h"

typedef enum rte_network_type_t {
  RTE_NETWORK_TYPE_IPV4 = 1,
  RTE_NETWORK_TYPE_NAT_IPV6 = 1 << 1,
  RTE_NETWORK_TYPE_PURE_IPV6 = 1 << 2,
  RTE_NETWORK_TYPE_IPV6 =
    RTE_NETWORK_TYPE_NAT_IPV6 | RTE_NETWORK_TYPE_PURE_IPV6,
} rte_network_type_t;

#define IPV4_STR_MAX_LEN (15 + 1)  // 1 for the null terminator
#define IPV6_STR_MAX_LEN (39 + 1)  // 1 for the null terminator
#define IP_STR_MAX_LEN IPV6_STR_MAX_LEN
#define URI_MAX_LEN 256
#define PORT_MIN_NUM 1
#define PORT_MAX_NUM 65535

// FIXME(Ender): buggy interface
// Have to handle multiple interface cases
AGORA_RTE_API void rte_host_get(char* hostname_buffer,
                                size_t hostname_buffer_capacity,
                                char* ip_buffer,
                                size_t ip_buffer_capacity);

AGORA_RTE_API bool rte_host_split(const char* uri,
                                  char* name_buffer,
                                  size_t name_buffer_capacity,
                                  int32_t* port);

AGORA_RTE_API bool rte_get_ipv6_prefix(const char* ifid, rte_string_t* prefix);
