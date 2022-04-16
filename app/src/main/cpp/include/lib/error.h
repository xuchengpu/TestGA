/**
 *
 * Agora Real Time Engagement
 * Created by ZhangXianyao in 2022-01.
 * Copyright (c) 2022 Agora IO. All rights reserved.
 *
 */
#pragma once

#include "rte_config.h"

#include <stdbool.h>
#include <stdint.h>

#include "lib/signature.h"
#include "lib/string.h"

typedef uint64_t rte_errno_t;

#define RTE_ERROR_SIGNATURE 0xCA49E5F63FC43623U

#define RTE_ERROR_INVALID_ERRNO ((rte_errno_t)-1)

typedef struct rte_error_t {
  rte_signature_t signature;

  rte_errno_t err_no;
  rte_string_t err_msg;
} rte_error_t;

AGORA_RTE_API bool rte_error_check_integrity(rte_error_t* self);

AGORA_RTE_API void rte_error_init(rte_error_t* self);

AGORA_RTE_API void rte_error_deinit(rte_error_t* self);

AGORA_RTE_API rte_error_t* rte_error_create(void);

// Set error info, return true if set success, false if err_no is
// RTE_ERROR_INVALID_ERRNO.
AGORA_RTE_API bool rte_error_set(rte_error_t* self,
                                 rte_errno_t err_no,
                                 const char* fmt,
                                 ...);

AGORA_RTE_API bool rte_error_prepend_errmsg(rte_error_t* self,
                                            const char* fmt,
                                            ...);

// Get last errno in current context, return RTE_ERROR_INVALID_ERRNO if no error
// set before.
AGORA_RTE_API rte_errno_t rte_error_last_errno(rte_error_t* self);

AGORA_RTE_API const char* rte_error_last_errmsg(rte_error_t* self);

AGORA_RTE_API void rte_error_destroy(rte_error_t* self);