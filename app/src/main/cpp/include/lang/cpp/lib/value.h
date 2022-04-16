/**
 *
 * Agora Real Time Engagement
 * Created by Wei Hu in 2021-10.
 * Copyright (c) 2022 Agora IO. All rights reserved.
 *
 */
#pragma once

#include <cassert>
#include <cstddef>
#include <map>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

#include "lang/cpp/lib/json.h"
#include "lib/json.h"
#include "lib/string.h"
#include "schema/value.h"

using rte_value_t = struct rte_value_t;

namespace rte {

class rte_t;

class value_t {
 public:
  explicit value_t() {
    value_ = rte_value_create_null();
  }

  explicit value_t(::rte_value_t* value) {
    is_ref = true;
    value_ = value;
  }

  explicit value_t(int32_t value) {
    value_ = rte_value_create_int32(value);
  }

  explicit value_t(int64_t value) {
    value_ = rte_value_create_int64(value);
  }

  explicit value_t(float value) {
    value_ = rte_value_create_float32(value);
  }

  explicit value_t(double value) {
    value_ = rte_value_create_float64(value);
  }

  explicit value_t(bool value) {
    value_ = rte_value_create_boolean(value);
  }

  explicit value_t(const std::string& value, bool is_filename = false) {
    if (is_filename) {
      value_ = rte_value_create_file(value.c_str());
    } else {
      value_ = rte_value_create_string(value.c_str());
    }
  }

  explicit value_t(const char* value) {
    value_ = rte_value_create_string(value);
  }

  template <typename V>
  explicit value_t(const std::map<std::string, V>& map) {
    rte_list_t m;
    rte_list_init(&m);

    for (const auto& pair : map) {
      rte_value_kv_t* rte_pair = rte_value_kv_create_empty(pair.first.c_str());
      if (!rte_pair) {
        continue;
      }

      rte_pair->value = create(pair.second);
      rte_list_push_ptr_back(&m, rte_pair, (void*)rte_value_kv_destroy);
    }

    value_ = rte_value_create_object(&m);
    rte_list_clear(&m);
  }

  template <typename V>
  explicit value_t(const std::vector<V>& list) {
    rte_list_t m;
    rte_list_init(&m);

    for (const auto& v : list) {
      rte_list_push_ptr_back(&m, create(v), (void*)rte_value_destroy);
    }

    value_ = rte_value_create_array(&m);
    rte_list_clear(&m);
  }

  std::vector<value_t> get_array(
    const std::vector<value_t>& default_value = {}) {
    if (empty()) {
      return default_value;
    }

    std::vector<value_t> result;
    if (!rte_value_is_array(value_)) {
      return default_value;
    }

    auto size = rte_value_array_size(value_);
    for (int i = 0; i < size; ++i) {
      auto v = rte_value_array_get(value_, i);
      result.push_back(value_t(v));
    }

    return result;
  }

  std::map<std::string, value_t> get_object(
    const std::map<std::string, value_t>& default_value = {}) {
    if (empty()) {
      return default_value;
    }

    std::map<std::string, value_t> result;
    if (!rte_value_is_object(value_)) {
      return default_value;
    }

    rte_value_object_foreach(value_, iter) {
      rte_value_kv_t* kv = (rte_value_kv_t*)rte_ptr_listnode_get(iter.node);
      result.insert(
        std::make_pair(rte_string_c_str(&kv->key), value_t(kv->value)));
    };

    return result;
  }

  ~value_t() {
    if (value_ && !is_ref) {
      rte_value_destroy(value_);
    }
  }

  bool empty() const {
    if (value_ == nullptr) {
      return true;
    }

    return rte_value_is_null(value_);
  }

  int32_t get_int32(int32_t default_value = 0) {
    if (empty()) {
      return default_value;
    }

    if (!rte_value_is_int32(value_)) {
      return default_value;
    }

    return rte_value_int32_value(value_);
  }

  int64_t get_int64(int64_t default_value = 0) {
    if (empty()) {
      return default_value;
    }

    if (!rte_value_is_int64(value_)) {
      return default_value;
    }

    return rte_value_int64_value(value_);
  }

  bool get_boolean(bool default_value = false) {
    if (empty()) {
      return default_value;
    }

    if (!rte_value_is_boolean(value_)) {
      return default_value;
    }

    return rte_value_boolean_value(value_);
  }

  std::string get_string(const std::string& default_value = "") {
    if (empty()) {
      return default_value;
    }

    if (!rte_value_is_string(value_)) {
      return default_value;
    }

    return rte_string_c_str(rte_value_string_value(value_));
  }

  void set_string(const std::string& str) {
    is_ref = false;

    if (!value_) {
      value_ = rte_value_create_string(str.c_str());
    } else {
      rte_value_deinit(value_);
      rte_value_set_string(value_, str.c_str(), str.length());
    }
  }

  double get_double(double default_value = 0.0) {
    if (empty()) {
      return default_value;
    }

    if (!rte_value_is_float64(value_)) {
      return default_value;
    }

    return rte_value_float64_value(value_);
  }

  float get_float32(float default_value = 0.0) {
    if (empty()) {
      return default_value;
    }

    if (!rte_value_is_float32(value_)) {
      return default_value;
    }

    return rte_value_float32_value(value_);
  }

  nlohmann::json to_json() const {
    json_t* c_json = rte_value_to_json_conform_to_rfc4627(value_);
    nlohmann::json cpp_json = convert_c_json_to_cpp_json(c_json);
    json_decref(c_json);
    return cpp_json;
  }

 private:
  friend class rte_t;
  friend class msg_t;

  template <typename T>
  ::rte_value_t* create(const T& v) {
    value_t tmp(v);
    ::rte_value_t* ret = tmp.value_;
    tmp.value_ = nullptr;
    return ret;
  }

  bool is_ref = false;
  ::rte_value_t* value_ = nullptr;
};

}  // namespace rte
