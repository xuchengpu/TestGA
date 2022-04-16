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

struct rb_node;
struct rb_root {
  struct rb_node* rb_node;
};

typedef struct rte_rbtree_t {
  size_t size;
  rte_kvop_t keyop;
  rte_kvop_t valop;
  struct rb_root root;
} rte_rbtree_t;

/**
 * @brief Create a new red-black tree.
 * @param key The key operations for the tree.
 * @param value The value operations for the tree.
 * @return The new tree.
 */
AGORA_RTE_API rte_rbtree_t* rte_rbtree_create(const rte_kvop_t* key,
                                              const rte_kvop_t* val);

/**
 * @brief Destroy a red-black tree.
 * @param tree The tree to destroy.
 */
AGORA_RTE_API void rte_rbtree_destroy(rte_rbtree_t* tree);

/**
 * @brief Initializze a red-black tree.
 * @param tree The tree to destroy.
 * @param key The key operations for the tree.
 * @param value The value operations for the tree.
 */
AGORA_RTE_API void rte_rbtree_init(rte_rbtree_t* tree,
                                   const rte_kvop_t* key,
                                   const rte_kvop_t* val);

/**
 * @brief Get the size of a red-black tree.
 * @param tree The tree to get the size of.
 * @return The size of the tree.
 */
AGORA_RTE_API size_t rte_rbtree_size(rte_rbtree_t* tree);

/**
 * @brief Insert a key-value pair into a red-black tree.
 * @param tree The tree to insert into.
 * @param key The key to insert.
 * @param value The value to insert.
 * @return 0 on success, -1 on failure or existing.
 */
AGORA_RTE_API int rte_rbtree_insert(rte_rbtree_t* tree, void* key, void* val);

typedef void (*rte_rbtree_destroy_func_t)(void*);
/**
 * @brief Insert a key-value pair into a red-black tree, with custom
 *        compare & destroy function
 * @param tree The tree to insert into.
 * @param key The key to insert.
 * @param key_comp The key compare function, if NULL will use operation from
 *                create, if still NULL will use binary compare
 * @param value The value to insert.
 * @return 0 on success, -1 on failure or existing.
 */
AGORA_RTE_API int rte_rbtree_ptr_insert(rte_rbtree_t* tree,
                                        void* key,
                                        void* val,
                                        int (*key_comp)(const void*,
                                                        const void*),
                                        rte_rbtree_destroy_func_t key_destroy,
                                        rte_rbtree_destroy_func_t val_destroy);

/**
 * @brief Remove a key-value pair from a red-black tree.
 * @param tree The tree to remove from.
 * @param key The key to remove.
 * @return 0 on success, -1 on failure or not existing.
 */
AGORA_RTE_API int rte_rbtree_erase(rte_rbtree_t* tree, void* key);

/**
 * @brief Clear all key-value pairs from a red-black tree.
 * @param tree The tree to clear.
 */
AGORA_RTE_API void rte_rbtree_clear(rte_rbtree_t* tree);

/**
 * @brief Find a key in a red-black tree.
 * @param tree The tree to find in.
 * @param key The key to find.
 * @return The key-value pair associated with the key, or NULL if not found.
 */
AGORA_RTE_API rte_kvpair_t* rte_rbtree_find(rte_rbtree_t* tree, void* key);

/**
 * @brief Get the first key-value pair in a red-black tree.
 * @param tree The tree to get the first key-value pair from.
 * @return The first key-value pair in the tree, or NULL if the tree is empty.
 */
AGORA_RTE_API rte_kvpair_t* rte_rbtree_first(rte_rbtree_t* tree);

/**
 * @brief Get the last key-value pair in a red-black tree.
 * @param tree The tree to get the last key-value pair from.
 * @return The last key-value pair in the tree, or NULL if the tree is empty.
 */
AGORA_RTE_API rte_kvpair_t* rte_rbtree_last(rte_rbtree_t* tree);

/**
 * @brief Get the next key-value pair in a red-black tree.
 * @param pair The key-value pair to get the next key-value pair from.
 * @return The next key-value pair in the tree, or NULL if end .
 */
AGORA_RTE_API rte_kvpair_t* rte_rbtree_next(rte_kvpair_t* pair);

/**
 * @brief Get the previous key-value pair in a red-black tree.
 * @param pair The key-value pair to get the previous key-value pair from.
 * @return The previous key-value pair in the tree, or NULL if begin.
 */
AGORA_RTE_API rte_kvpair_t* rte_rbtree_prev(rte_kvpair_t* pair);

/**
 * @brief Get key of a key-value pair
 * @param pair The key-value pair to get the key from.
 * @return The key of the key-value pair.
 */
AGORA_RTE_API void* rte_rbnode_key(rte_kvpair_t* pair);

/**
 * @brief Get value of a key-value pair
 * @param pair The key-value pair to get the value from.
 * @return The value of the key-value pair.
 */
AGORA_RTE_API void* rte_rbnode_value(rte_kvpair_t* pair);

#define rte_rbtree_foreach(tree, itor)                            \
  for (rte_kvpair_t* itor = rte_rbtree_first(tree); itor != NULL; \
       itor = rte_rbtree_next(itor))

/** @}*/