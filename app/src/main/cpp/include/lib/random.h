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

/**
 * @brief Generate random buffer with given size.
 * @param buf The buffer to store random data.
 * @param size The size of the buffer.
 * @return 0 on success, -1 on failure.
 *
 * This function generates drbg random data. It may fail.
 */
AGORA_RTE_API int rte_random(void* buf, size_t size);

/**
 * @brief Generate random int n that start <= n < end
 * @param start The begin of random.
 * @param end The end of random.
 * @return A random int number
 */
AGORA_RTE_API int rte_random_int(int start, int end);

/**
 * @brief Generate random printable string with given size.
 * @param buf The buffer to store random data.
 * @param size The size of the buffer.
 * @return 0 on success, -1 on failure.
 */
AGORA_RTE_API int rte_random_string(char* buf, size_t size);

/**
 * @brief Generate random hex string with given size.
 * @param buf The buffer to store random data.
 * @param size The size of the buffer.
 */
AGORA_RTE_API int rte_random_hex_string(char* buf, size_t size);

/**
 * @brief Generate random base64 string with given size.
 * @param buf The buffer to store random data.
 * @param size The size of the buffer.
 */
AGORA_RTE_API int rte_random_base64_string(char* buf, size_t size);

/**
 * @brief Generate UUID string.
 * @param buf The buffer to store random data.
 * @param size The size of the buffer.
 */
AGORA_RTE_API int rte_uuid_new(char* buf, size_t size);
