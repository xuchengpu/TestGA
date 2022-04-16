/**
 *
 * Agora Real Time Engagement
 * Created by Wei Hu in 2021-07.
 * Copyright (c) 2021 Agora IO. All rights reserved.
 *
 */
#pragma once

#include "rte_config.h"

#include <stdint.h>

/**
 * @brief Get the current time in milliseconds.
 * @return The current time in milliseconds.
 * @note The returned value is not steady and is not guaranteed to be monotonic.
 */
AGORA_RTE_API int64_t rte_current_time(void);

/**
 * @brief Get the current time in microseconds.
 * @return The current time in milliseconds.
 * @note The returned value is not steady and is not guaranteed to be monotonic.
 */
AGORA_RTE_API int64_t rte_current_time_us(void);

/**
 * @beief Sleep for a specified time in milliseconds.
 * @param msec The time to sleep in milliseconds.
 */
AGORA_RTE_API void rte_sleep(int64_t msec);

/**
 * @beief Sleep for a specified time in microseconds.
 * @param msec The time to sleep in microseconds.
 */
AGORA_RTE_API void rte_usleep(int64_t usec);
