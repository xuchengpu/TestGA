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

/** \addtogroup atomic
 *  @{
 */

typedef int64_t rte_atomic_t;

/**
 * @brief Load from an atomic variable.
 * @param a The pointer to the atomic variable.
 * @return The value of the atomic variable.
 */
AGORA_RTE_API int64_t rte_atomic_load(volatile rte_atomic_t* a);

/**
 * @brief Store to an atomic variable.
 * @param a The pointer to the atomic variable.
 * @param v The value to store.
 */
AGORA_RTE_API void rte_atomic_store(volatile rte_atomic_t* a, int64_t v);

/**
 * @brief Add to an atomic variable, and return the old value.
 * @param a The pointer to the atomic variable.
 * @param v The value to add.
 * @return The original value of the atomic variable.
 */
AGORA_RTE_API int64_t rte_atomic_fetch_add(volatile rte_atomic_t* a, int64_t v);

/**
 * @brief Add to an atomic variable, and return the new value.
 * @param a The pointer to the atomic variable.
 * @param v The value to add.
 * @return The new value of the atomic variable.
 */
AGORA_RTE_API int64_t rte_atomic_add_fetch(volatile rte_atomic_t* a, int64_t v);

/**
 * @brief Subtract from an atomic variable, and return the old value.
 * @param a The pointer to the atomic variable.
 * @param v The value to subtract.
 * @return The original value of the atomic variable.
 */
AGORA_RTE_API int64_t rte_atomic_fetch_sub(volatile rte_atomic_t* a, int64_t v);

/**
 * @brief Subtract from an atomic variable, and return the new value.
 * @param a The pointer to the atomic variable.
 * @param v The value to subtract.
 * @return The new value of the atomic variable.
 */
AGORA_RTE_API int64_t rte_atomic_sub_fetch(volatile rte_atomic_t* a, int64_t v);

/**
 * @brief Binary and to an atomic variable, and return the old value.
 * @param a The pointer to the atomic variable.
 * @param v The value to perform and operation.
 * @return The old value of the atomic variable.
 */
AGORA_RTE_API int64_t rte_atomic_fetch_and(volatile rte_atomic_t* a, int64_t v);

/**
 * @brief Binary or to an atomic variable, and return the old value.
 * @param a The pointer to the atomic variable.
 * @param v The value to perform or operation.
 * @return The old value of the atomic variable.
 */
AGORA_RTE_API int64_t rte_atomic_fetch_or(volatile rte_atomic_t* a, int64_t v);

/**
 * @brief Set an atomic variable to a value and return the old value.
 * @param a The pointer to the atomic variable.
 * @param v The value to set.
 * @return The original value of the atomic variable.
 */
AGORA_RTE_API int64_t rte_atomic_test_set(volatile rte_atomic_t* a, int64_t v);

/**
 * @brief Compare and exchange an atomic variable. Returns the origin value.
 * @param a The pointer to the atomic variable.
 * @param comp The expected value.
 * @param xchg The new value.
 * @return The original value of the atomic variable.
 */
AGORA_RTE_API int64_t rte_atomic_val_compare_swap(volatile rte_atomic_t* a,
                                                  int64_t comp,
                                                  int64_t xchg);

/**
 * @brief Compare and exchange an atomic variable with a value.
 *        Returns the compare result of origin value and |comp|.
 * @param a The pointer to the atomic variable.
 * @param comp The expected value.
 * @param xchg The new value.
 * @return true if original value equals |comp|, false otherwise.
 */
AGORA_RTE_API int rte_atomic_bool_compare_swap(volatile rte_atomic_t* a,
                                               int64_t comp,
                                               int64_t xchg);

AGORA_RTE_API int64_t
rte_atomic_conditional_increment(volatile rte_atomic_t* a);

/**
 * @brief Put a full memory barrier
 */
AGORA_RTE_API void rte_memory_barrier();

#if defined(_WIN32)
#include <intrin.h>
#define rte_compiler_barrier() _ReadWriteBarrier()
#else
#define rte_compiler_barrier()       \
  do {                               \
    asm volatile("" : : : "memory"); \
  } while (0)
#endif

/** @}*/
