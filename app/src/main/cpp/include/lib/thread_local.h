/**
 *
 * Agora Real Time Engagement
 * Created by Zheng, Ender in 2021-08.
 * Copyright (c) 2021 Agora IO. All rights reserved.
 *
 */
#pragma once

#include "rte_config.h"

#if defined(_WIN32)
#include <Windows.h>
typedef DWORD rte_thread_key_t;
#else
#include <pthread.h>
typedef pthread_key_t rte_thread_key_t;
#endif

#define kInvalidTlsKey ((rte_thread_key_t)-1)

/**
 * @brief Create a thread local storage key.
 * @return The key.
 */
AGORA_RTE_API rte_thread_key_t rte_thread_key_create();

/**
 * @brief Delete a thread local storage key.
 * @param key The key.
 */
AGORA_RTE_API void rte_thread_key_destroy(rte_thread_key_t key);

/**
 * @brief Set the value of a thread local storage key.
 * @param key The key.
 * @param value The value.
 */
AGORA_RTE_API int rte_thread_set_key(rte_thread_key_t key, void* value);

/**
 * @brief Get the value of a thread local storage key.
 * @param key The key.
 * @return The value.
 */
AGORA_RTE_API void* rte_thread_get_key(rte_thread_key_t key);
