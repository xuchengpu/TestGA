/**
 *
 * Agora Real Time Engagement
 * Created by Zheng, Ender in 2021-08.
 * Copyright (c) 2021 Agora IO. All rights reserved.
 *
 */
#pragma once

#include "rte_config.h"

#include "lib/buf.h"
#include "lib/string.h"

AGORA_RTE_API void rte_base64_to_string(rte_string_t* result,
                                                rte_buf_t buf);

AGORA_RTE_API rte_buf_t rte_base64_from_string(rte_string_t* str);
