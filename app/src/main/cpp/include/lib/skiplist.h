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

#include "lib/kvpair.h"

/** \addtogroup container
 *  @{
 */

typedef struct rte_skiplist_t rte_skiplist_t;

/**
 * @brief Create a new skiplist.
 * @param key The key operations for the skiplist.
 * @param value The value operations for the skiplist.
 * @return The new skiplist.
 */
AGORA_RTE_API rte_skiplist_t* rte_skiplist_create(rte_kvop_t* key,
                                                  rte_kvop_t* val);

/**
 * @brief Destroy a skiplist.
 * @param sl The skiplist to destroy.
 */
AGORA_RTE_API void rte_skiplist_destroy(rte_skiplist_t* sl);

/**
 * @brief Get the size of a skiplist.
 * @param sl The skiplist.
 * @return The size of the skiplist.
 */
AGORA_RTE_API size_t rte_skiplist_size(rte_skiplist_t* sl);

/**
 * @brief Insert a new key-value pair into a skiplist.
 * @param sl The skiplist.
 * @param key The key of the new pair.
 * @param value The value of the new pair.
 * @return 0 on success, -1 on failure or existing.
 */
AGORA_RTE_API int rte_skiplist_insert(rte_skiplist_t* sl, void* key, void* val);

/**
 * @brief Remove a key-value pair from a skiplist.
 * @param sl The skiplist.
 * @param key The key of the pair to remove.
 * @return 0 on success, -1 on failure or not existing.
 */
AGORA_RTE_API int rte_skiplist_erase(rte_skiplist_t* sl, void* key);

/**
 * @brief Clear a skiplist.
 * @param sl The skiplist.
 */
AGORA_RTE_API void rte_skiplist_clear(rte_skiplist_t* sl);

/**
 * @brief Find a key-value pair in a skiplist.
 * @param sl The skiplist.
 * @param key The key of the pair to find.
 * @return The value of the pair, or NULL if not found.
 */
AGORA_RTE_API rte_kvpair_t* rte_skiplist_find(rte_skiplist_t* sl, void* key);

/**
 * @brief Get the first key-value pair in a skiplist.
 * @param sl The skiplist.
 * @return The first key-value pair, or NULL if empty.
 */
AGORA_RTE_API rte_kvpair_t* rte_skiplist_first(rte_skiplist_t* sl);

/**
 * @brief Get the next key-value pair in a skiplist.
 * @param pair The current key-value pair.
 * @return The next key-value pair, or NULL if end.
 */
AGORA_RTE_API rte_kvpair_t* rte_skiplist_next(rte_kvpair_t* pair);

/**
 * @brief Get the key of a key-value pair.
 * @param pair The key-value pair.
 * @return The key of the pair.
 */
AGORA_RTE_API void* rte_skipnode_key(rte_kvpair_t* pair);

/**
 * @brief Get the value of a key-value pair.
 * @param pair The key-value pair.
 * @return The value of the pair.
 */
AGORA_RTE_API void* rte_skipnode_value(rte_kvpair_t* pair);

/** @}*/
