/**
 *
 * Agora Real Time Engagement
 * Created by Wei Hu in 2021-07.
 * Copyright (c) 2022 Agora IO. All rights reserved.
 *
 */
#pragma once

#include "rte_config.h"

#include <iomanip>
#include <nlohmann/json.hpp>
#include <sstream>
#include <string>

#include "jansson.h"
#include "lib/alloc.h"
#include "lib/string.h"

namespace rte {

static inline json_t* convert_cpp_json_to_c_json(nlohmann::json cpp_json) {
  try {
    std::string json_str = cpp_json.dump();

    json_t* c_json =
      json_loadb(json_str.c_str(), json_str.length(), 0, nullptr);
    assert(c_json);

    return c_json;
  } catch (...) {
    assert(0 && "Should not happen.");
    return NULL;
  }
}

static inline nlohmann::json convert_c_json_to_cpp_json(json_t* c_json) {
  size_t size = json_dumpb(c_json, nullptr, 0, 0);
  assert(size);

  bool stack_memory = false;
  char* c_json_str = NULL;
  if (size < 1023) {
    stack_memory = true;
    c_json_str = (char*)alloca(size + 1);
  } else {
    // NOLINTNEXTLINE(cppcoreguidelines-no-malloc,hicpp-no-malloc)
    c_json_str = (char*)rte_malloc(size + 1);
  }

  assert(c_json_str);
  c_json_str[size] = '\0';

  size_t rc = json_dumpb(c_json, c_json_str, size, 0);
  assert(rc && rc <= size);

  nlohmann::json cpp_json;
  try {
    cpp_json = nlohmann::json::parse(c_json_str);
  } catch (...) {
    assert(0 && "Should not happen.");
  }

  if (!stack_memory) {
    // NOLINTNEXTLINE(cppcoreguidelines-no-malloc,hicpp-no-malloc)
    rte_free(c_json_str);
  }

  return cpp_json;
}

static inline std::string json_get_string(const nlohmann::json& json,
                                          const std::string& field) {
  if (json.is_object() && json.contains(field) && json[field].is_string()) {
    return json[field].get<std::string>();
  }
  return "";
}

}  // namespace rte