/**
 *
 * Agora Real Time Engagement
 * Created by Zheng, Ender in 2021-08.
 * Copyright (c) 2022 Agora IO. All rights reserved.
 *
 */
#pragma once

#include "rte_config.h"

#include "lib/json.h"
#include "lib/list.h"
#include "lib/string.h"

#define RTE_PROTOCOL_TCP "tcp"
#define RTE_PROTOCOL_RAW "raw"
#define RTE_PROTOCOL_PIPE "pipe"
#define RTE_STR_TAG "tag"
#define RTE_STR_URI "uri"
#define RTE_STR_VALUE "value"

typedef struct rte_uri_info_t {
  rte_string_t uri;
  rte_list_t protocol_tags;
} rte_uri_info_t;

typedef struct rte_value_t rte_value_t;

AGORA_RTE_API rte_string_t* rte_uri_get_protocol(const char* uri);

AGORA_RTE_API bool rte_uri_is_protocol_equal(const char* uri,
                                             const char* protocol);

AGORA_RTE_API rte_string_t* rte_uri_get_host(const char* uri);

AGORA_RTE_API uint16_t rte_uri_get_port(const char* uri);

AGORA_RTE_API rte_uri_info_t* rte_uri_info_create(void);

AGORA_RTE_API rte_uri_info_t* rte_uri_info_create_with_uri(const char* uri);

AGORA_RTE_API void rte_uri_info_init(rte_uri_info_t* self);

AGORA_RTE_API void rte_uri_info_init_with_uri(rte_uri_info_t* self,
                                              const char* uri);

AGORA_RTE_API void rte_uri_info_deinit(rte_uri_info_t* self);

AGORA_RTE_API void rte_uri_info_destroy(rte_uri_info_t* self);

AGORA_RTE_API void rte_uri_info_clear(rte_uri_info_t* self);

AGORA_RTE_API void rte_uri_info_copy_tags(rte_uri_info_t* self,
                                          rte_list_t protocol_tags);

AGORA_RTE_API void rte_uri_info_copy(rte_uri_info_t* self, rte_uri_info_t* src);

AGORA_RTE_API bool rte_uri_info_is_uri_equal(rte_uri_info_t* self,
                                             rte_uri_info_t* other);

AGORA_RTE_API bool rte_uri_info_is_equal(rte_uri_info_t* self,
                                         rte_uri_info_t* other);

AGORA_RTE_API void rte_uri_info_flatten_append_to_json(const char* uri,
                                                       rte_list_t tags,
                                                       json_t* json);

AGORA_RTE_API void rte_uri_info_append_to_json(rte_uri_info_t* self,
                                               json_t* json);

AGORA_RTE_API void rte_uri_info_from_json(rte_uri_info_t* self, json_t* json);

AGORA_RTE_API void rte_uri_info_from_value(rte_uri_info_t* self,
                                           rte_value_t* value);
