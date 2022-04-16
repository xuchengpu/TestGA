/**
 *
 * Agora Real Time Engagement
 * Created by Zheng, Ender in 2021-08.
 * Copyright (c) 2021 Agora IO. All rights reserved.
 *
 */
#pragma once

#include "rte_config.h"

/**
 * Reader-writer lock
 *
 * Keep in mind that RW lock is more expensive than mutex even in reader side
 *
 * The case you want an RW lock is that you need several readers working at the
 * same time. If you want a "faster-reader-side-locks", consider RCU
 *
 * Windows Slim Reader/Writer (SRW) lock is neither phase fair nor task fair.
 * Noticed that SRW lock may faster than this implementation (because of no
 * fairness), but we are not going to use it
 *
 * Darwin pthread_mutex is task fair with dramatically performance drops, we
 * have to give up fairness from pthread_mutex and grant it in rwlock by
 * ourselves if we use mutex-condition flavor implementation (which is also done
 * by Darwin pthread_rwlock_t). But here we use atomic-spin flavor, which is
 * faster than Darwin pthread_rwlock_t .
 *
 * Fairness of posix pthread_rwlock is depends on implementation. Generally
 * speaking it's task fair with some kind of preference on readers.
 * Comments from Oracle:
 *    If there are multiple threads waiting to acquire the read-write
 *    lock object for reading, the scheduling policy is used to determine the
 *    order in which the waiting threads acquire the read-write lock object for
 *    reading. If there are multiple threads blocked on rwlock for both read
 *    locks and write locks, it is unspecified whether the readers acquire the
 *    lock first or whether a writer acquires the lock first.
 * As a result of testing, Linux pthread_rwlock_t will re-balance fairness and
 * performance when switching between heavy contention and light contention,
 * which is really good. But we still use our own implementation because
 * (usually) we need phase fair.
 *
 * Here is the performance and fairness matrix
 * (y means better than native)
 * (x means worse than native)
 * (e means almost same as native)
 *
 * --------------------------------------------------
 *             | Linux  |  Darwin  |  Windows
 * performance |   e    |    yy    |   e
 * Fairness    |   y    |    y     |   yy
 * --------------------------------------------------
 */

typedef enum RTE_RW_FAIRNESS {
  RTE_RW_NATIVE,
  RTE_RW_PHASE_FAIR,
} RTE_RW_FAIRNESS;

#define RTE_RW_DEFAULT_FAIRNESS RTE_RW_PHASE_FAIR

typedef struct rte_rwlock_t rte_rwlock_t;

/**
 * @brief Create a reader-writer lock
 * @param fairness The fairness flavor of the lock
 * @return The reader-writer lock
 */
AGORA_RTE_API rte_rwlock_t* rte_rwlock_create(RTE_RW_FAIRNESS fair);

/**
 * @brief Destroy a reader-writer lock
 * @param lock The reader-writer lock
 */
AGORA_RTE_API void rte_rwlock_destroy(rte_rwlock_t* lock);

/**
 * @brief Acquire a reader-writer lock
 * @param lock The reader-writer lock
 * @param reader Whether it is a read lock
 */
AGORA_RTE_API int rte_rwlock_lock(rte_rwlock_t* lock, int reader);

/**
 * @brief Release a reader-writer lock
 * @param lock The reader-writer lock
 * @param reader Whether it is a read lock
 */
AGORA_RTE_API int rte_rwlock_unlock(rte_rwlock_t* lock, int reader);
