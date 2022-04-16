/**
 *
 * Agora Real Time Engagement
 * Created by Wei Hu in 2021-12.
 * Copyright (c) 2022 Agora IO. All rights reserved.
 *
 */
#pragma once

#include "rte_config.h"

#include <stdint.h>

#if defined(__i386__) || defined(_M_X86) || defined(_M_IX86)
typedef uint32_t rte_signature_t;
#else
typedef uint64_t rte_signature_t;
#endif

AGORA_RTE_API void rte_signature_set(rte_signature_t* signature,
                                     rte_signature_t value);

AGORA_RTE_API rte_signature_t rte_signature_get(rte_signature_t* signature);

AGORA_RTE_API rte_signature_t
rte_signature_get_const(const rte_signature_t* signature);

AGORA_RTE_API rte_signature_t rte_signature_generate(void);
