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

AGORA_RTE_API void* rte_shm_map(const char* name, size_t size);

AGORA_RTE_API size_t rte_shm_get_size(void* addr);

AGORA_RTE_API void rte_shm_unmap(void* addr);

AGORA_RTE_API void rte_shm_unlink(const char* name);
