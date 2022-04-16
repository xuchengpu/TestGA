/**
 *
 * Agora Real Time Engagement
 * Created by Zheng, Ender in 2021-08.
 * Copyright (c) 2021 Agora IO. All rights reserved.
 *
 */
#pragma once

#include "rte_config.h"

typedef struct rte_mutex_t rte_mutex_t;

/**
 * @brief Create a mutex.
 * @return The mutex handle.
 */
AGORA_RTE_API rte_mutex_t* rte_mutex_create();

/**
 * @brief Lock a mutex.
 * @param mutex The mutex handle.
 * @return 0 if success, otherwise failed.
 *
 * @note This function will block until the mutex is unlocked.
 */
AGORA_RTE_API int rte_mutex_lock(rte_mutex_t* mutex);

/**
 * @brief Unlock a mutex.
 * @param mutex The mutex handle.
 * @return 0 if success, otherwise failed.
 */
AGORA_RTE_API int rte_mutex_unlock(rte_mutex_t* mutex);

/**
 * @brief Destroy a mutex.
 * @param mutex The mutex handle.
 */
AGORA_RTE_API void rte_mutex_destroy(rte_mutex_t* mutex);

/**
 * @brief Get system mutex handle.
 * @param mutex The mutex handle.
 * @return The system mutex handle.
 */
AGORA_RTE_API void* rte_mutex_get_native_handle(rte_mutex_t* mutex);
