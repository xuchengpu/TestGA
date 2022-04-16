/**
 *
 * Agora Real Time Engagement
 * Created by Wei Hu in 2021-10.
 * Copyright (c) 2022 Agora IO. All rights reserved.
 *
 */
#pragma once

#include "rte_config.h"

#include <stdbool.h>

typedef struct rte_string_t rte_string_t;

AGORA_RTE_API const char* rte_file_get_extension(rte_string_t* filename);

AGORA_RTE_API char* rte_file_read(const char* filename);
