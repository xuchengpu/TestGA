/**
 *
 * Agora Real Time Engagement
 * Created by Wei Hu in 2021-07.
 * Copyright (c) 2021 Agora IO. All rights reserved.
 *
 */
/* Modified from https://github.com/troydhanson/uthash. */
#pragma once

#include "rte_config.h"

#include <stdint.h>

typedef struct rte_hashtable_t rte_hashtable_t;
typedef struct rte_hashhandle_t rte_hashhandle_t;

struct rte_hashhandle_t {
  rte_hashtable_t* tbl;

  void* prev;  // previous hash handle in app-ordered list
  void* next;  // next hash handle in app-ordered list

  rte_hashhandle_t* hh_prev;  // previous item in bucket
  rte_hashhandle_t* hh_next;  // next item in bucket

  const void* key;  // ptr to key data
  uint32_t keylen;  // key len
  uint32_t hashval;  // result of hash function

  void (*destroy)(rte_hashhandle_t*);
};

AGORA_RTE_API void rte_hashhandle_init(rte_hashhandle_t* self,
                                       rte_hashtable_t* table,
                                       const void* key,
                                       uint32_t keylen,
                                       void* destroy);

AGORA_RTE_API void rte_hashhandle_del_from_app_list(rte_hashhandle_t* hh);
