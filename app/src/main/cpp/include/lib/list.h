/**
 *
 * Agora Real Time Engagement
 * Created by Wei Hu in 2021-07.
 * Copyright (c) 2022 Agora IO. All rights reserved.
 *
 */
#pragma once

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>

#include "lib/atomic.h"
#include "lib/list_node.h"
#include "lib/signature.h"

/** \addtogroup container
 *  @{
 */

#define RTE_LIST_SIGNATURE 0x5D6CC60B9833B104U

#define rte_list_foreach(list, iter)                                       \
  for (rte_list_iterator_t iter = {NULL,                                   \
                                   rte_list_front(list),                   \
                                   rte_list_front(list)                    \
                                     ? rte_list_front(list)->next          \
                                     : NULL,                               \
                                   0};                                     \
       (iter).node;                                                        \
       ++((iter).index),                                                   \
                           (iter).prev = (iter).node,                      \
                           (iter).node = (iter).next,                      \
                           (iter).next = (iter).node ? ((iter).node)->next \
                                                     : NULL)

#define RTE_LIST_INIT_VAL                                                   \
  (rte_list_t) {                                                            \
    .signature = RTE_LIST_SIGNATURE, .size = 0, .front = NULL, .back = NULL \
  }

typedef struct rte_list_t {
  rte_signature_t signature;
  size_t size;
  rte_listnode_t *front, *back;
} rte_list_t;

typedef struct rte_list_iterator_t {
  rte_listnode_t* prev;
  rte_listnode_t* node;
  rte_listnode_t* next;
  size_t index;
} rte_list_iterator_t;

/**
 * @brief Initialize a list.
 * @param self The list to be initialized.
 */
AGORA_RTE_API void rte_list_init(rte_list_t* self);

/**
 * @brief Reset a list to an empty list.
 * @param self The list to be reset.
 */
AGORA_RTE_API void rte_list_reset(rte_list_t* self);

/**
 * @brief Clear a list and release all the nodes.
 * @param self The list to be cleared.
 */
AGORA_RTE_API void rte_list_clear(rte_list_t* self);

/**
 * @brief Check if a list is empty.
 * @param self The list to be checked.
 * @return true if the list is empty, false otherwise.
 */
AGORA_RTE_API bool rte_list_is_empty(rte_list_t* self);

/**
 * @brief Get the size of a list.
 * @param self The list to be checked.
 * @return the size of the list.
 */
AGORA_RTE_API size_t rte_list_size(rte_list_t* self);

/**
 * @brief Swap two lists.
 * @param self The list to be swapped.
 * @param target The target list to be swapped.
 */
AGORA_RTE_API void rte_list_swap(rte_list_t* self, rte_list_t* target);

/**
 * @brief Concatenate two lists.
 * @param self The list to be concatenated.
 * @param target The target list to be concatenated.
 */
AGORA_RTE_API void rte_list_concat(rte_list_t* self, rte_list_t* target);

/**
 * @brief Remove a node from a list and keep node memory.
 * @param self The list to be removed from.
 * @param node The node to be removed.
 */
AGORA_RTE_API void rte_list_remove_node_only(rte_list_t* self,
                                             rte_listnode_t* node);

/**
 * @brief Remove a node from a list and release node memory.
 * @param self The list to be removed from.
 * @param node The node to be removed.
 */
AGORA_RTE_API void rte_list_remove_node(rte_list_t* self, rte_listnode_t* node);

/**
 * @brief Get the front node of a list.
 * @param self The list to be checked.
 * @return the front node of the list. NULL if the list is empty.
 */
AGORA_RTE_API rte_listnode_t* rte_list_front(rte_list_t* self);

/**
 * @brief Get the back node of a list.
 * @param self The list to be checked.
 * @return the back node of the list. NULL if the list is empty.
 */
AGORA_RTE_API rte_listnode_t* rte_list_back(rte_list_t* self);

/**
 * @brief Push a node to the front of a list.
 * @param self The list to be pushed to.
 * @param node The node to be pushed.
 */
AGORA_RTE_API void rte_list_push_front(rte_list_t* self, rte_listnode_t* node);

/**
 * @brief Push a node to the back of a list.
 * @param self The list to be pushed to.
 * @param node The node to be pushed.
 */
AGORA_RTE_API void rte_list_push_back(rte_list_t* self, rte_listnode_t* node);

/**
 * @brief Pop the front node of a list.
 * @param self The list to be popped from.
 * @return the front node of the list. NULL if the list is empty.
 */
AGORA_RTE_API rte_listnode_t* rte_list_pop_front(rte_list_t* self);

/**
 * @brief Pop the back node of a list.
 * @param self The list to be popped from.
 * @return the back node of the list. NULL if the list is empty.
 */
AGORA_RTE_API rte_listnode_t* rte_list_pop_back(rte_list_t* self);

// normal pointer list
AGORA_RTE_API rte_listnode_t* rte_list_find_ptr(rte_list_t* self,
                                                const void* ptr);

#define rte_list_find_ptr_custom(self, ptr, equal_to) \
  rte_list_find_ptr_custom_(self,                     \
                            (const void*)ptr,         \
                            (bool (*)(const void*, const void*))(equal_to));

AGORA_RTE_API rte_listnode_t* rte_list_find_ptr_custom_(
  rte_list_t* self,
  const void* ptr,
  bool (*equal_to)(const void*, const void*));

#define rte_list_find_ptr_cnt_custom(self, ptr, equal_to) \
  rte_list_find_ptr_cnt_custom_(                          \
    self,                                                 \
    (const void*)ptr,                                     \
    (bool (*)(const void*, const void*))(equal_to));

AGORA_RTE_API size_t
rte_list_find_ptr_cnt_custom_(rte_list_t* self,
                              const void* ptr,
                              bool (*equal_to)(const void*, const void*));

#define rte_list_cnt_custom(self, predicate) \
  rte_list_cnt_custom_(self, (bool (*)(const void*))(predicate));

AGORA_RTE_API size_t rte_list_cnt_custom_(rte_list_t* self,
                                          bool (*predicate)(const void*));

AGORA_RTE_API bool rte_list_remove_ptr(rte_list_t* self, void* ptr);

AGORA_RTE_API void rte_list_push_ptr_back(rte_list_t* self,
                                          void* ptr,
                                          void* destroy);

AGORA_RTE_API void rte_list_push_ptr_front(rte_list_t* self,
                                           void* ptr,
                                           void* destroy);

AGORA_RTE_API void rte_list_push_str_back(rte_list_t* self, const char* str);

AGORA_RTE_API void rte_list_push_str_with_size_back(rte_list_t* self,
                                                    const char* str,
                                                    size_t size);

// shared pointer list
AGORA_RTE_API void rte_list_push_sharedptr_front(rte_list_t* self,
                                                 rte_sharedptr_t* ptr);

AGORA_RTE_API void rte_list_push_sharedptr_back(rte_list_t* self,
                                                rte_sharedptr_t* ptr);

AGORA_RTE_API bool rte_list_remove_sharedptr_from_ptr(rte_list_t* self,
                                                      void* ptr);

AGORA_RTE_API bool rte_list_remove_sharedptr(rte_list_t* self,
                                             rte_sharedptr_t* ptr);

#define rte_list_find_sharedptr_custom(self, ptr, equal_to) \
  rte_list_find_sharedptr_custom_(                          \
    self,                                                   \
    ptr,                                                    \
    (bool (*)(const void*, const void*))(equal_to))

AGORA_RTE_API rte_listnode_t* rte_list_find_sharedptr_custom_(
  rte_list_t* self,
  const void* ptr,
  bool (*equal_to)(const void*, const void*));

#define rte_list_find_sharedptr_custom_3(self, ptr_1, ptr_2, ptr_3, equal_to) \
  rte_list_find_sharedptr_custom_3_(                                          \
    self,                                                                     \
    ptr_1,                                                                    \
    ptr_2,                                                                    \
    ptr_3,                                                                    \
    (bool (*)(const void*, const void*, const void*, const void*))(equal_to))

AGORA_RTE_API rte_listnode_t* rte_list_find_sharedptr_custom_3_(
  rte_list_t* self,
  const void* ptr_1,
  const void* ptr_2,
  const void* ptr_3,
  bool (*equal_to)(const void*, const void*, const void*, const void*));

#define rte_list_find_sharedptr_custom_4(self,             \
                                         ptr_1,            \
                                         ptr_2,            \
                                         ptr_3,            \
                                         ptr_4,            \
                                         equal_to)         \
  rte_list_find_sharedptr_custom_4_(self,                  \
                                    ptr_1,                 \
                                    ptr_2,                 \
                                    ptr_3,                 \
                                    ptr_4,                 \
                                    (bool (*)(const void*, \
                                              const void*, \
                                              const void*, \
                                              const void*, \
                                              const void*))(equal_to))

AGORA_RTE_API rte_listnode_t* rte_list_find_sharedptr_custom_4_(
  rte_list_t* self,
  const void* ptr_1,
  const void* ptr_2,
  const void* ptr_3,
  const void* ptr_4,
  bool (*equal_to)(const void*,
                   const void*,
                   const void*,
                   const void*,
                   const void*));

// rte_string_t listnode
AGORA_RTE_API rte_listnode_t* rte_list_find_str(rte_list_t* self,
                                                const char* str);

// int32_t listnode
AGORA_RTE_API rte_listnode_t* rte_list_find_int32(rte_list_t* self,
                                                  int32_t int32);

AGORA_RTE_API rte_list_iterator_t rte_list_begin(rte_list_t* self);

AGORA_RTE_API rte_list_iterator_t
rte_list_iterator_next(rte_list_iterator_t self);

AGORA_RTE_API bool rte_list_iterator_is_end(rte_list_iterator_t self);

AGORA_RTE_API rte_listnode_t* rte_list_iterator_to_listnode(
  rte_list_iterator_t self);

/** @}*/
