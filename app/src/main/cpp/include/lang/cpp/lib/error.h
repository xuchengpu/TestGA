/**
 *
 * Agora Real Time Engagement
 * Created by Wei Hu in 2022-01.
 * Copyright (c) 2022 Agora IO. All rights reserved.
 *
 */
#pragma once

#include "lib/error.h"

namespace rte {

class error_t {
 public:
  explicit error_t() {
    rte_error_init(&error_);
  }

  ~error_t() {
    rte_error_deinit(&error_);
  };

  rte_error_t* get_internal_representation() {
    return &error_;
  }

 private:
  rte_error_t error_;
};

}  // namespace rte
