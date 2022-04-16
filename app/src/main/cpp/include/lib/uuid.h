/**
 *
 * Agora Real Time Engagement
 * Created by Wei Hu in 2021-12.
 * Copyright (c) 2022 Agora IO. All rights reserved.
 *
 * Modified from https://github.com/gpakosz/uuid4/
 */
#pragma once

#include "rte_config.h"

#include <stdbool.h>
#include <stdint.h>

#include "lib/string.h"

typedef uint64_t rte_uuid4_state_t;

typedef union rte_uuid4_t {
  uint8_t bytes[16];
  uint32_t dwords[4];
  uint64_t qwords[2];
} rte_uuid4_t;

AGORA_RTE_PRIVATE_API uint32_t rte_uuid4_mix(uint32_t x, uint32_t y);

AGORA_RTE_PRIVATE_API uint32_t rte_uuid4_hash(uint32_t value);

/**
 * Seeds the state of the PRNG used to generate version 4 UUIDs.
 *
 * @param a pointer to a variable holding the state.
 *
 * @return `true` on success, otherwise `false`.
 */
AGORA_RTE_API void rte_uuid4_seed(rte_uuid4_state_t* seed);

AGORA_RTE_API void rte_uuid4_init_to_zeros(rte_uuid4_t* self);

/**
 * Generates a version 4 UUID, see https://tools.ietf.org/html/rfc4122.
 *
 * @param state the state of the PRNG used to generate version 4 UUIDs.
 * @param out the recipient for the UUID.
 */
AGORA_RTE_API void rte_uuid4_gen(rte_uuid4_t* self, rte_uuid4_state_t* state);

AGORA_RTE_API void rte_uuid4_gen_string(rte_string_t* out);

AGORA_RTE_API bool rte_uuid4_is_equal(const rte_uuid4_t* a,
                                      const rte_uuid4_t* b);

AGORA_RTE_API void rte_uuid4_copy(rte_uuid4_t* self, rte_uuid4_t* src);

AGORA_RTE_API bool rte_uuid4_is_empty(rte_uuid4_t* self);

AGORA_RTE_API bool rte_uuid4_from_string(rte_uuid4_t* self, rte_string_t* in);

/**
 * Converts a UUID to a `NUL` terminated string.
 * The string format is like 'xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx', y is either
 * 8, 9, a or b
 *
 * @param out destination rte string
 *
 * @return `true` on success, otherwise `false`.
 */
AGORA_RTE_API bool rte_uuid4_to_string(const rte_uuid4_t* self,
                                       rte_string_t* out);
