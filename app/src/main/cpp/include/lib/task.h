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

typedef int64_t rte_pid_t;

/**
 * @brief Get process id.
 */
AGORA_RTE_API rte_pid_t rte_task_get_id();
