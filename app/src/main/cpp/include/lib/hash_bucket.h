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

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define HASH_BKT_CAPACITY_THRESH 10U  // Expand when bucket items reaches

typedef struct rte_hashbucket_t rte_hashbucket_t;
typedef struct rte_hashhandle_t rte_hashhandle_t;

struct rte_hashbucket_t {
  rte_hashhandle_t* head;
  uint32_t items_cnt;

  // expand_mult is normally set to 0. In this situation, the max chain length
  // threshold is enforced at its default value, HASH_BKT_CAPACITY_THRESH. (If
  // the bucket's chain exceeds this length, bucket expansion is triggered).
  // However, setting expand_mult to a non-zero value delays bucket expansion
  // (that would be triggered by additions to this particular bucket)
  // until its chain length reaches a *multiple* of HASH_BKT_CAPACITY_THRESH.
  // (The multiplier is simply expand_mult+1). The whole idea of this
  // multiplier is to reduce bucket expansions, since they are expensive, in
  // situations where we know that a particular bucket tends to be overused.
  // It is better to let its chain length grow to a longer yet-still-bounded
  // value, than to do a O(n) bucket expansion too often.
  uint32_t expand_mult;
};

AGORA_RTE_API void rte_hashbucket_add(rte_hashbucket_t* self,
                                      rte_hashhandle_t* hh);

AGORA_RTE_API void rte_hashbucket_del(rte_hashbucket_t* self,
                                      rte_hashhandle_t* hh);

AGORA_RTE_API rte_hashhandle_t* rte_hashbucket_find(rte_hashbucket_t* self,
                                                    uint32_t hashval,
                                                    const void* key,
                                                    size_t keylen);
