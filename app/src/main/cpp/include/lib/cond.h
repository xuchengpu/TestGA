/**
 *
 * Agora Real Time Engagement
 * Created by Zheng, Ender in 2021-08.
 * Copyright (c) 2021 Agora IO. All rights reserved.
 *
 */
#pragma once

#include "rte_config.h"

#include <stdint.h>

typedef struct rte_cond_t rte_cond_t;
typedef struct rte_mutex_t rte_mutex_t;

/**
 * @brief Create a condition variable.
 */
AGORA_RTE_API rte_cond_t* rte_cond_create(void);

/**
 * @brief Destroy a condition variable.
 */
AGORA_RTE_API void rte_cond_destroy(rte_cond_t* cond);

/**
 * @brief Wait on a condition variable.
 * @param cond The condition variable to wait on.
 * @param mutex The mutex to unlock while waiting.
 * @param wait_ms The maximum time to wait in milliseconds.
 * @return 0 on success, -1 on error.
 *
 * @note This function will unlock the mutex before waiting and lock it again
 *       when it is signaled. Suprise wakeup still happens just like pthread
 *       version of condition variable.
 */
AGORA_RTE_API int rte_cond_wait(rte_cond_t* cond,
                                rte_mutex_t* mutex,
                                int64_t wait_ms);

/**
 * @bried Wait on a condition variable while predicate() is true.
 * @param cond The condition variable to wait on.
 * @param mutex The mutex to unlock while waiting.
 * @param predicate The predicate to check.
 * @param arg The argument to pass to predicate().
 * @param wait_ms The maximum time to wait in milliseconds.
 * @return 0 on success, -1 on error.
 *
 * @note This function will unlock the mutex before waiting and lock it again
 *       when it is signaled. Suprise wakeup does _not_ happen because we
 *       instanly check predicate() before leaving.
 */
AGORA_RTE_API int rte_cond_wait_while(rte_cond_t* cond,
                                      rte_mutex_t* mutex,
                                      int (*predicate)(void*),
                                      void* arg,
                                      int64_t wait_ms);

/**
 * @brief Signal a condition variable.
 * @param cond The condition variable to signal.
 * @return 0 on success, -1 on error.
 */
AGORA_RTE_API int rte_cond_signal(rte_cond_t* cond);

/**
 * @brief Broadcast a condition variable.
 * @param cond The condition variable to broadcast.
 * @return 0 on success, -1 on error.
 */
AGORA_RTE_API int rte_cond_broadcast(rte_cond_t* cond);