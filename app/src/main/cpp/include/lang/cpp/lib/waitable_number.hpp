/**
 *
 * Agora Real Time Engagement
 * Created by Zheng, Ender in 2021-08.
 * Copyright (c) 2021 Agora IO. All rights reserved.
 *
 */

#pragma once

#include "lang/cpp/common/traits.h"
#include "lib/waitable_number.h"

namespace agora {
namespace utils {

class WaitableNumber : private noncopyable, private nonmovable {
 public:
  explicit WaitableNumber(int64_t init_value = 0)
    : obj_(rte_waitable_number_create(init_value)),
      shadow_(init_value) {
  }

  ~WaitableNumber() {
    if (obj_) {
      rte_waitable_number_destroy(obj_);
    }
  }

  int WaitUntil(int64_t n, int wait_ms = -1) {
    if (!obj_) {
      return -1;
    }

    return rte_waitable_number_wait_until(obj_, n, wait_ms);
  }

  int WaitWhile(int64_t n, int wait_ms = -1) {
    if (!obj_) {
      return -1;
    }

    return rte_waitable_number_wait_while(obj_, n, wait_ms);
  }

  int64_t get() const {
    if (!obj_) {
      return shadow_;
    }

    return rte_waitable_number_get(obj_);
  }

  WaitableNumber& operator=(int64_t n) {
    if (!obj_) {
      return *this;
    }

    rte_waitable_number_set(obj_, n);
    return *this;
  }

  void operator+=(int64_t n) {
    if (!obj_) {
      return;
    }

    rte_waitable_number_increase(obj_, n);
  }

  void operator-=(int64_t n) {
    if (!obj_) {
      return;
    }

    rte_waitable_number_decrease(obj_, n);
  }
  void operator*=(int64_t n) {
    if (!obj_) {
      return;
    }

    rte_waitable_number_multiply(obj_, n);
  }

  void operator/=(int64_t n) {
    if (!obj_) {
      return;
    }

    rte_waitable_number_devide(obj_, n);
  }

  bool operator==(int64_t n) {
    if (!obj_) {
      return shadow_ == n;
    }

    return rte_waitable_number_get(obj_) == n;
  }

  bool operator!=(int64_t n) {
    if (!obj_) {
      return shadow_ != n;
    }

    return rte_waitable_number_get(obj_) != n;
  }

 private:
  rte_waitable_number_t* obj_;
  int64_t shadow_;
};

}  // namespace utils
}  // namespace agora