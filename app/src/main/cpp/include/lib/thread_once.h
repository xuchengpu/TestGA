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
#define rte_thread_once_t INIT_ONCE
#define RTE_THREAD_ONCE_INIT INIT_ONCE_STATIC_INIT
#else
#include <pthread.h>
#define rte_thread_once_t pthread_once_t
#define RTE_THREAD_ONCE_INIT PTHREAD_ONCE_INIT
#endif

/**
 * @brief Initialize a thread-once object.
 * @param once Pointer to the thread-once object.
 * @param init_routine Pointer to the initialization routine.
 * @return 0 on success, or a negative error code.
 * @note This function is will guarantee that the initialization routine will be
 *       called only once.
 */
AGORA_RTE_API int rte_thread_once(rte_thread_once_t* once,
                                  void (*init_routine)(void));
