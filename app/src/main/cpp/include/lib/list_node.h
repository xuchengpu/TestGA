/**
 *
 * Agora Real Time Engagement
 * Created by Wei Hu in 2021-07.
 * Copyright (c) 2022 Agora IO. All rights reserved.
 *
 */
#pragma once

#include "rte_config.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "lib/atomic.h"
#include "lib/buf.h"
#include "lib/shared_ptr.h"
#include "lib/signature.h"
#include "lib/string.h"
#include "macro/align.h"
#include "macro/macros.h"

#define RTE_LISTNODE_SIGNATURE 0x00D7B10E642B105CU

typedef struct rte_listnode_t rte_listnode_t;
typedef struct rte_list_t rte_list_t;

struct rte_listnode_t {
  rte_signature_t signature;
  rte_listnode_t *next, *prev;
  void (*destroy)(rte_listnode_t*);
};

AGORA_RTE_API void rte_listnode_destroy(rte_listnode_t* self);

// ===================
// sharedptr list node
#define RTE_SHAREDPTR_LISTNODE_SIGNATURE 0x00C0ADEEF6B9A421U

typedef struct rte_sharedptr_listnode_t {
  rte_listnode_t hdr;
  rte_signature_t signature;
  rte_sharedptr_t* ptr;
} rte_sharedptr_listnode_t;

AGORA_RTE_API rte_listnode_t* rte_sharedptr_listnode_create(
  rte_sharedptr_t* ptr);

AGORA_RTE_API rte_sharedptr_listnode_t* rte_listnode_to_sharedptr_listnode(
  rte_listnode_t* self);

AGORA_RTE_API rte_listnode_t* rte_listnode_from_sharedptr_listnode(
  rte_sharedptr_listnode_t* self);

AGORA_RTE_API rte_sharedptr_t* rte_sharedptr_listnode_get(rte_listnode_t* self);

AGORA_RTE_API void* rte_sharedptr_listnode_get_data(rte_listnode_t* self);

// ========================
// normal pointer list node
#define RTE_NORMAL_PTR_LISTNODE_SIGNATURE 0xEBB1285007CA4A12U

typedef struct rte_ptr_listnode_t {
  rte_listnode_t hdr;
  rte_signature_t signature;
  void* ptr;
  void (*destroy)(void*);
} rte_ptr_listnode_t;

AGORA_RTE_API rte_listnode_t* rte_ptr_listnode_create(void* ptr, void* destroy);

AGORA_RTE_API rte_ptr_listnode_t* rte_listnode_to_ptr_listnode(
  rte_listnode_t* self);

AGORA_RTE_API rte_listnode_t* rte_listnode_from_ptr_listnode(
  rte_ptr_listnode_t* self);

AGORA_RTE_API void* rte_ptr_listnode_get(rte_listnode_t* self);

// ================
// string list node
#define RTE_STR_LISTNODE_SIGNATURE 0x15D33B50C27A1B20U

typedef struct rte_str_listnode_t {
  rte_listnode_t hdr;
  rte_signature_t signature;
  rte_string_t str;
} rte_str_listnode_t;

AGORA_RTE_API rte_listnode_t* rte_str_listnode_create(const char* str);

AGORA_RTE_API rte_listnode_t* rte_str_listnode_create_with_size(const char* str,
                                                                size_t size);

AGORA_RTE_API rte_str_listnode_t* rte_listnode_to_str_listnode(
  rte_listnode_t* self);

AGORA_RTE_API rte_listnode_t* rte_listnode_from_str_listnode(
  rte_str_listnode_t* self);

AGORA_RTE_API rte_string_t* rte_str_listnode_get(rte_listnode_t* self);

// ================
// int list node
#define RTE_INT32_LISTNODE_SIGNATURE 0x2A576F8836859FB5U

typedef struct rte_int32_listnode_t {
  rte_listnode_t hdr;
  rte_signature_t signature;
  int32_t int32;
} rte_int32_listnode_t;

AGORA_RTE_API rte_listnode_t* rte_int32_listnode_create(int32_t int32);

AGORA_RTE_API rte_int32_listnode_t* rte_listnode_to_int32_listnode(
  rte_listnode_t* self);

AGORA_RTE_API rte_listnode_t* rte_listnode_from_int32_listnode(
  rte_int32_listnode_t* self);

AGORA_RTE_API int32_t rte_int32_listnode_get(rte_listnode_t* self);
