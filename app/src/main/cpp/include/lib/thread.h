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

#include "lib/atomic.h"
#include "lib/event.h"
#include "lib/mutex.h"

typedef struct rte_thread_t {
  void* (*routine)(void*);
  void* args;
  rte_atomic_t id;
  rte_event_t* ready;
  rte_event_t* exit;
  const char* name;
  rte_atomic_t detached;
  void* aux;
} rte_thread_t;

typedef rte_atomic_t rte_tid_t;

/**
 * @brief Create a new thread.
 * @param name The name of the thread.
 * @param routine The routine of the thread.
 * @param args The arguments of the routine.
 * @return The thread object
 */
AGORA_RTE_API rte_thread_t* rte_thread_create(const char* name,
                                              void* (*routine)(void*),
                                              void* args);

/**
 * @brief Suspend the thread.
 * @param thread The thread object to suspend.
 * @return 0 on success, -1 on error.
 */
AGORA_RTE_API int rte_thread_suspend(rte_thread_t* thread);

/**
 * @brief Resume the thread.
 * @param thread The thread object to resume.
 * @return 0 on success, -1 on error.
 */
AGORA_RTE_API int rte_thread_resume(rte_thread_t* thread);

/**
 * @brief Join the thread.
 * @param thread The thread object to join.
 * @param wait_ms The timeout in milliseconds.
 * @return 0 on success, -1 on error.
 * @note You can not join a detached thread, crash will happen if you do.
 */
AGORA_RTE_API int rte_thread_join(rte_thread_t* thread, int wait_ms);

/**
 * @brief Detach the thread.
 * @param thread The thread object to detach.
 * @return 0 on success, -1 on error.
 * @note You can not detach a detached thread, crash will happen if you do.
 */
AGORA_RTE_API int rte_thread_detach(rte_thread_t* thread);

/**
 * @brief Get the thread id.
 * @param thread The thread object.
 * @return The thread id.
 */
AGORA_RTE_API rte_tid_t rte_thread_get_id(rte_thread_t* thread);

/**
 * @brief Get current thread.
 * @return The current thread object.
 * @note Will return NULL if it is not created by |rte_thread_create|
 */
AGORA_RTE_API rte_thread_t* rte_thread_self();

/**
 * @brief Yield the thread.
 */
AGORA_RTE_API void rte_thread_yield();

/**
 * @brief Set the thread name.
 * @param thread The thread object.
 * @param name The name of the thread.
 * @return 0 on success, -1 on error.
 */
AGORA_RTE_API int rte_thread_set_name(rte_thread_t* thread, const char* name);

/**
 * @brief Let current CPU run into low power mode until interrupt.
 */
AGORA_RTE_API void rte_thread_pause_cpu();

/**
 * @brief Set thread affinity.
 * @param thread The thread object.
 * @param mask The affinity mask.
 * @note The mask is a bit mask of CPU id. For example, if you want to set the
 *       thread to run on CPU 0 and 2, the mask is 0x3.
 *       Currently only support 64 CPUs.
 */
AGORA_RTE_API void rte_thread_set_affinity(rte_thread_t* thread, uint64_t mask);

/**
 * @brief Compare two threads.
 * @param thread The thread object to be compared
 * @param target The thread object to compare.
 * @return If the two thread are equal, it returns 1; otherwise, it returns 0.
 */
AGORA_RTE_API int rte_thread_equal(rte_thread_t* thread, rte_thread_t* target);

/**
 * @brief Create a new fake thread. The only usage of fake threads is for
 * debugging.
 * @return The thread object
 */
AGORA_RTE_API rte_thread_t* rte_thread_create_fake(void);

/**
 * @brief Join the fake thread.
 * @param thread The fake thread object to join.
 * @return 0 on success, -1 on error.
 */
AGORA_RTE_API int rte_thread_join_fake(rte_thread_t* thread);
