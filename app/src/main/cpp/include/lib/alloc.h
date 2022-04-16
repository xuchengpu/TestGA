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

/** \addtogroup memory
 *  @{
 */

/**
 * @brief Allocate a buffer
 * @param size: Size of buffer
 * @return: Address of buffer if success, NULL otherwise
 */
AGORA_RTE_API void* rte_malloc(size_t size);

/**
 * @brief Re-allocate a buffer with new size
 * @param p: Address of buffer
 * @param size: New size of buffer
 * @return: Address of buffer if success, NULL otherwise
 */
AGORA_RTE_API void* rte_realloc(void* p, size_t size);

/**
 * @brief Deallocate a buffer
 * @param p: Address of buffer
 * @note It is safe to free a NULL pointer
 */
AGORA_RTE_API void rte_free(void* p);

/**
 * @brief Duplicate a string
 * @param str: String that needs duplicate
 * @return: Address of new string
 * @note: Please free memory using |rte_free|
 */
AGORA_RTE_API char* rte_strdup(const char* str);

/** @}*/