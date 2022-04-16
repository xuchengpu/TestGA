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

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define rte_hashtable_foreach(table, iter)                                   \
  for (rte_hashtable_iterator_t iter =                                       \
         {NULL,                                                              \
          rte_hashtable_front(table),                                        \
          rte_hashtable_front(table)                                         \
            ? rte_hashtable_front(table)->next                               \
                ? FIELD_OF_FROM_OFFSET(rte_hashtable_front(table)->next,     \
                                       (table)->hh_offset)                   \
                : NULL                                                       \
            : NULL,                                                          \
          0};                                                                \
       (iter).node;                                                          \
       ++((iter).index),                                                     \
                                (iter).prev = (iter).node,                   \
                                (iter).node = (iter).next,                   \
                                (iter).next = (iter).node                    \
                                                ? ((iter).node)->next        \
                                                    ? FIELD_OF_FROM_OFFSET(  \
                                                        ((iter).node)->next, \
                                                        (table)->hh_offset)  \
                                                    : NULL                   \
                                                : NULL)

typedef struct rte_hashtable_t rte_hashtable_t;
typedef struct rte_hashbucket_t rte_hashbucket_t;
typedef struct rte_hashhandle_t rte_hashhandle_t;

struct rte_hashtable_t {
  rte_hashbucket_t* bkts;

  rte_hashhandle_t* head;  // App-ordered list head.
  rte_hashhandle_t* tail;  // App-ordered list tail.
  ptrdiff_t hh_offset;

  uint32_t bkts_cnt;
  uint32_t bkts_cnt_in_log2;
  uint32_t items_cnt;

  // In a ideal situation (all buckets used equally), no bucket would have
  // more than ceil (#items/#buckets) items. that's the ideal chain length.
  uint32_t ideal_chain_maxlen;

  // nonideal_items_cnt is the number of items in the hash whose chain position
  // exceeds the ideal chain maxlen. These items pay the penalty for a uneven
  // hash distribution; reaching them in a chain traversal takes >ideal steps.
  uint32_t nonideal_items_cnt;

  // ineffective expands occur when a bucket doubling was performed, but
  // afterward, more than half the items in the hash had nonideal chain
  // positions. If this happens on two consecutive expansions we inhibit any
  // further expansion, as it's not helping; this happens when the hash
  // function isn't a good fit for the key domain. When expansion is inhibited
  // the hash will still work, albeit no longer in constant time. */
  uint32_t ineff_expands_times;
  bool noexpand;
};

typedef struct rte_hashtable_iterator_t {
  rte_hashhandle_t* prev;
  rte_hashhandle_t* node;
  rte_hashhandle_t* next;
  size_t index;
} rte_hashtable_iterator_t;

AGORA_RTE_API uint32_t rte_hash_function(const void* key,
                                         const uint32_t keylen);

/**
 * @brief Create a hash table.
 */
AGORA_RTE_API rte_hashtable_t* rte_hashtable_create(ptrdiff_t hh_offset);

/**
 * @brief Destroy a hash table.
 */
AGORA_RTE_API void rte_hashtable_destroy(rte_hashtable_t* self);

AGORA_RTE_API uint32_t rte_hashtable_items_cnt(rte_hashtable_t* self);

AGORA_RTE_API void rte_hashtable_init(rte_hashtable_t* self,
                                      ptrdiff_t hh_offset);

AGORA_RTE_API void rte_hashtable_deinit(rte_hashtable_t* self);

AGORA_RTE_API void rte_hashtable_clear(rte_hashtable_t* self);

/**
 * @brief Concatenate the second hash table into the first one.
 * @param self The hash table to be concatenated.
 * @param target The hash table to be concatenating to the first one.
 */
AGORA_RTE_API void rte_hashtable_concat(rte_hashtable_t* self,
                                        rte_hashtable_t* target);

AGORA_RTE_API void rte_hashtable_expand_bkts(rte_hashtable_t* self);

AGORA_RTE_API void rte_hashtable_add_by_key(rte_hashtable_t* self,
                                            rte_hashhandle_t* hh,
                                            const void* key,
                                            uint32_t keylen,
                                            void* destroy);

AGORA_RTE_API void rte_hashtable_replace_by_key(rte_hashtable_t* self,
                                                rte_hashhandle_t* hh,
                                                void* keyptr,
                                                uint32_t keylen_in,
                                                void* destroy);

AGORA_RTE_API void rte_hashtable_del(rte_hashtable_t* self,
                                     rte_hashhandle_t* hh);

AGORA_RTE_API rte_hashhandle_t* rte_hashtable_front(rte_hashtable_t* self);

AGORA_RTE_API rte_hashhandle_t* rte_hashtable_back(rte_hashtable_t* self);

AGORA_RTE_API rte_hashhandle_t* rte_hashtable_find_by_key(rte_hashtable_t* self,
                                                          const void* key,
                                                          uint32_t keylen);

AGORA_RTE_API rte_hashhandle_t* rte_hashtable_find(rte_hashtable_t* self,
                                                   uint32_t hashval,
                                                   const void* key,
                                                   uint32_t keylen);

/**
 * @brief Add an item to the hash table with string-type key.
 */
static inline void rte_hashtable_add_str(rte_hashtable_t* self,
                                         rte_hashhandle_t* hh,
                                         const char* str,
                                         void* destroy) {
  assert(self && hh && str);
  rte_hashtable_add_by_key(self, hh, str, strlen(str), destroy);
}

static inline void rte_hashtable_replace_str(rte_hashtable_t* self,
                                             rte_hashhandle_t* hh,
                                             char* str,
                                             void* destroy) {
  assert(self);
  rte_hashtable_replace_by_key(self, hh, str, strlen(str), destroy);
}

static inline rte_hashhandle_t* rte_hashtable_find_str(rte_hashtable_t* self,
                                                       const char* str) {
  assert(self && str);
  return rte_hashtable_find_by_key(self, str, strlen(str));
}

/**
 * @brief Add an item to the hash table with integer-type key.
 */
static inline void rte_hashtable_add_int(rte_hashtable_t* self,
                                         rte_hashhandle_t* hh,
                                         uint32_t value,
                                         void* destroy) {
  assert(self && hh);
  rte_hashtable_add_by_key(self, hh, &value, sizeof(uint32_t), destroy);
}

static inline void rte_hashtable_replace_int(rte_hashtable_t* self,
                                             rte_hashhandle_t* hh,
                                             uint32_t value,
                                             void* destroy) {
  assert(self && hh);
  rte_hashtable_replace_by_key(self, hh, &value, sizeof(uint32_t), destroy);
}

static inline rte_hashhandle_t* rte_hashtable_find_int(rte_hashtable_t* self,
                                                       uint32_t value) {
  assert(self);
  return rte_hashtable_find_by_key(self, &value, sizeof(uint32_t));
}

/**
 * @brief Add an item to the hash table with pointer-type key.
 */
static inline void rte_hashtable_add_ptr(rte_hashtable_t* self,
                                         rte_hashhandle_t* hh,
                                         void* ptr,
                                         void* destroy) {
  assert(self && hh);
  rte_hashtable_add_by_key(self, hh, &ptr, sizeof(void*), destroy);
}

static inline void rte_hashtable_replace_ptr(rte_hashtable_t* self,
                                             rte_hashhandle_t* hh,
                                             void* ptr,
                                             void* destroy) {
  assert(self && hh);
  rte_hashtable_replace_by_key(self, hh, &ptr, sizeof(void*), destroy);
}

static inline rte_hashhandle_t* rte_hashtable_find_ptr(rte_hashtable_t* self,
                                                       void* ptr) {
  assert(self);
  return rte_hashtable_find_by_key(self, &ptr, sizeof(void*));
}

/** @}*/