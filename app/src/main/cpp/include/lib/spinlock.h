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
#include "lib/task.h"
#include "lib/thread.h"

/**
 * This is actually a "busy loop" with "pause" instruction.
 * It's not possible to implement a "real" spin lock in userspace because
 * you have no way to disable thread schedule and interrupts.
 */

typedef struct rte_spinlock_t {
  rte_atomic_t lock;
} rte_spinlock_t;

#define RTE_SPINLOCK_INIT \
  { 0 }

/**
 * @brief Initialize a spinlock.
 */
AGORA_RTE_API void rte_spinlock_init(rte_spinlock_t* spin);

/**
 * @brief Initialize a spinlock from address
 * @note If |addr| exists in a shared memory, this lock can be used as IPC lock
 */
AGORA_RTE_API rte_spinlock_t* rte_spinlock_from_addr(rte_atomic_t* addr);

/**
 * @brief Acquire a spinlock.
 * @note This function will block if the lock is held by others. Recursivly
 *       acquire the same lock will result in dead lock
 */
AGORA_RTE_API void rte_spinlock_lock(rte_spinlock_t* spin);

/**
 * @brief Try to acquire a spinlock.
 * @return 0 if the lock is acquired, -1 otherwise.
 */
AGORA_RTE_API int rte_spinlock_trylock(rte_spinlock_t* spin);

/**
 * @brief Release a spinlock.
 */
AGORA_RTE_API void rte_spinlock_unlock(rte_spinlock_t* spin);

typedef struct rte_recursive_spinlock_t {
  rte_spinlock_t lock;
  volatile rte_pid_t pid;
  volatile rte_tid_t tid;
  volatile int64_t count;
} rte_recursive_spinlock_t;

#define RTE_RECURSIVE_SPINLOCK_INIT \
  { RTE_SPINLOCK_INIT, -1, -1, 0 }

/**
 * @brief Initialize a recursive spinlock
 */
AGORA_RTE_API void rte_recursive_spinlock_init(rte_recursive_spinlock_t* spin);

/**
 * @brief Initialize a recursive spinlock from address
 * @note If |addr| exists in a shared memory, this lock can be used as IPC lock
 */
AGORA_RTE_API rte_recursive_spinlock_t* rte_recursive_spinlock_from_addr(
  uint8_t addr[sizeof(rte_recursive_spinlock_t)]);

/**
 * @brief Acquire a recursive spinlock.
 * @note This function will block if the lock is held by another thread.
 */
AGORA_RTE_API void rte_recursive_spinlock_lock(rte_recursive_spinlock_t* spin);

/**
 * @brief Try to acquire a recursive spinlock.
 * @return 0 if the lock is acquired, -1 otherwise.
 */
AGORA_RTE_API int rte_recursive_spinlock_trylock(
  rte_recursive_spinlock_t* spin);

/**
 * @brief Release a recursive spinlock.
 */
AGORA_RTE_API void rte_recursive_spinlock_unlock(
  rte_recursive_spinlock_t* spin);
