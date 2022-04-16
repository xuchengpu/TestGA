/**
 *
 * Agora Real Time Engagement
 * Created by Wei Hu in 2021-09.
 * Copyright (c) 2021 Agora IO. All rights reserved.
 *
 */
#pragma once

#include "rte_config.h"

#include <cstddef>

namespace rte {

typedef struct buf_t {
  void* buf;
  size_t buf_size;
} buf_t;

}  // namespace rte
