/**
 *
 * Agora Real Time Engagement
 * Created by Wei Hu in 2021-10.
 * Copyright (c) 2021 Agora IO. All rights reserved.
 *
 */
#pragma once

#if defined(__EXCEPTIONS)
#include <stdexcept>
#endif
#include <string>

#include "lib/string.h"

namespace rte {

template <typename... Args>
std::string cpp_string_format(const std::string& format, Args... args) {
  int size_s = std::snprintf(nullptr, 0, format.c_str(), args...) +
               1;  // Extra space for '\0'
  if (size_s <= 0) {
#if defined(__EXCEPTIONS)
    throw std::runtime_error("Error during formatting.");
#else
    return "";
#endif
  }
  auto size = static_cast<size_t>(size_s);
  auto buf = std::make_unique<char[]>(size);
  std::snprintf(buf.get(), size, format.c_str(), args...);
  return std::string(buf.get(),
                     buf.get() + size - 1);  // We don't want the '\0' inside
}

static inline std::string cpp_string_uri_encode(const std::string& str) {
  rte_string_t uri_encoded;
  rte_string_init(&uri_encoded);

  rte_c_string_uri_encode(str.c_str(), str.length(), &uri_encoded);
  std::string result = rte_string_c_str(&uri_encoded);

  rte_string_deinit(&uri_encoded);
  return result;
}

static inline std::string cpp_string_uri_decode(const std::string& str) {
  rte_string_t uri_decoded;
  rte_string_init(&uri_decoded);

  rte_c_string_uri_decode(str.c_str(), str.length(), &uri_decoded);
  std::string result = rte_string_c_str(&uri_decoded);

  rte_string_deinit(&uri_decoded);
  return result;
}

static inline std::string cpp_string_escaped(const std::string& str) {
  rte_string_t escaped_str;
  rte_string_init(&escaped_str);

  rte_c_string_escaped(str.c_str(), &escaped_str);
  std::string result = rte_string_c_str(&escaped_str);

  rte_string_deinit(&escaped_str);
  return result;
}

class RteString {
 public:
  RteString() : str_(nullptr) {
  }

  RteString(const char* str)
    : str_(str != nullptr ? rte_string_create_with_value(str) : nullptr) {
  }

  RteString(const std::string& str)
    : str_((!str.empty()) ? rte_string_create_with_value(str.c_str())
                          : nullptr) {
  }

  RteString(const rte_string_t* str) : str_(const_cast<rte_string_t*>(str)) {
  }

  RteString(const RteString& rhs) : str_(nullptr) {
    operator=(rhs);
  }

  RteString(RteString&& rhs) noexcept : str_(nullptr) {
    operator=(std::move(rhs));
  }

  RteString& operator=(const RteString& rhs) {
    if (this == &rhs) {
      return *this;
    }

    if (str_ != nullptr) {
      rte_string_destroy(str_);
    }

    str_ = nullptr;
    if (rhs.str_ != nullptr) {
      str_ = rte_string_clone(rhs.str_);
    }

    return *this;
  }

  RteString& operator=(RteString&& rhs) noexcept {
    if (this == &rhs) {
      return *this;
    }

    if (str_ != nullptr) {
      rte_string_destroy(str_);
    }

    str_ = rhs.str_;
    rhs.str_ = nullptr;
    return *this;
  }

  operator const rte_string_t*() const {
    return str_;
  }

  bool operator==(const RteString& rhs) const {
    if (str_ == nullptr && rhs.str_ == nullptr) {
      return true;
    }

    if (str_ == nullptr && rhs.str_ != nullptr) {
      return false;
    }

    if (str_ != nullptr && rhs.str_ == nullptr) {
      return false;
    }

    return rte_string_is_equal(str_, rhs.str_);
  }

  bool operator==(const rte_string_t* rhs) const {
    if (str_ == nullptr && rhs == nullptr) {
      return true;
    }

    if (str_ == nullptr && rhs != nullptr) {
      return false;
    }

    if (str_ != nullptr && rhs == nullptr) {
      return false;
    }

    return rte_string_is_equal(str_, rhs);
  }

  bool operator==(const char* rhs) const {
    if (str_ == nullptr && rhs == nullptr) {
      return true;
    }

    if (str_ == nullptr && rhs != nullptr) {
      return false;
    }

    if (str_ != nullptr && rhs == nullptr) {
      return false;
    }

    return rte_string_is_equal_c_str(str_, rhs);
  }

  bool operator==(const std::string& rhs) const {
    if (str_ == nullptr && rhs.empty()) {
      return true;
    }

    if (str_ == nullptr && !rhs.empty()) {
      return false;
    }

    if (str_ != nullptr && rhs.empty()) {
      return false;
    }

    return rte_string_is_equal_c_str(str_, rhs.c_str());
  }

  bool operator!=(const RteString& rhs) const {
    return !operator==(rhs);
  }

  bool operator!=(const rte_string_t* rhs) const {
    return !operator==(rhs);
  }

  bool operator!=(const char* rhs) const {
    return !operator==(rhs);
  }

  bool operator!=(const std::string& rhs) const {
    return !operator==(rhs);
  }

  RteString& operator+=(const RteString& rhs) {
    if (rhs.empty()) {
      return *this;
    }

    if (empty()) {
      str_ = rte_string_clone(rhs.str_);
      return *this;
    }

    rte_string_concat(str_, rhs.str_->buf);
    return *this;
  }

  RteString& operator+=(const rte_string_t* rhs) {
    if (rhs == nullptr || rte_string_is_empty(rhs)) {
      return *this;
    }

    if (empty()) {
      str_ = rte_string_clone(const_cast<rte_string_t*>(rhs));
      return *this;
    }

    rte_string_concat(str_, rhs->buf);
    return *this;
  }

  RteString& operator+=(const char* rhs) {
    if (rhs == nullptr || *rhs == '\0') {
      return *this;
    }

    if (empty()) {
      str_ = rte_string_create_with_value(rhs);
      return *this;
    }

    rte_string_concat(str_, rhs);
    return *this;
  }

  RteString& operator+=(const std::string& rhs) {
    if (rhs.empty()) {
      return *this;
    }

    if (empty()) {
      str_ = rte_string_create_with_value(rhs.c_str());
      return *this;
    }

    rte_string_concat(str_, rhs.c_str());
    return *this;
  }

  RteString operator+(const RteString& rhs) const {
    RteString result(*this);
    result += rhs;
    return result;
  }

  RteString operator+(const rte_string_t* rhs) const {
    RteString result(*this);
    result += rhs;
    return result;
  }

  RteString operator+(const char* rhs) const {
    RteString result(*this);
    result += rhs;
    return result;
  }

  RteString operator+(const std::string& rhs) const {
    RteString result(*this);
    result += rhs;
    return result;
  }

  bool empty() const {
    return str_ == nullptr || rte_string_is_empty(str_);
  }

  ~RteString() {
    if (str_ != nullptr) {
      rte_string_destroy(str_);
    }
  }

  const char* c_str() const {
    return str_ ? str_->buf : nullptr;
  }

  size_t size() const {
    return str_ ? rte_string_len(str_) : 0;
  }

 private:
  rte_string_t* str_;
};

}  // namespace rte
