/**
 *
 * Agora Real Time Engagement
 * Created by Zheng, Ender in 2021-08.
 * Copyright (c) 2021 Agora IO. All rights reserved.
 *
 */
#pragma once

#include "rte_config.h"

#include "crypto/sslengine.h"

#if defined(__cplusplus)

#include <cstdint>
#include <functional>
#include <list>
#include <memory>
#include <string>
#include <vector>

namespace agora {
namespace utils {
namespace crypto {

class SslEngine {
 public:
  using ApplicationOnData = std::function<void(const uint8_t*, size_t)>;
  using TransportWrite = std::function<size_t(const uint8_t*, size_t)>;
  using HandshakeEvent = std::function<void(bool)>;
  struct Config {
    bool client_mode = true;
    rte_ssl_version_t min_version = RTE_SSL_VERSION_SSL3;
    rte_ssl_version_t max_version = RTE_SSL_VERSION_TLS_1_2;
    std::vector<std::string> cipher_suites;
    std::list<std::string> user_cas;
    std::string verify_host;
    ApplicationOnData on_app_data;
    TransportWrite transport_write;
    HandshakeEvent handshake_done;
  };

 public:
  static std::shared_ptr<SslEngine> Create(const Config& config) {
    rte_ssl_config_t ssl_config;
    ssl_config.client_mode = config.client_mode ? 1 : 0;
    ssl_config.min_version = config.min_version;
    ssl_config.max_version = config.max_version;
    std::vector<const char*> cipher_suites;
    for (const auto& cipher_suite : config.cipher_suites) {
      cipher_suites.push_back(cipher_suite.c_str());
    }
    ssl_config.cipher_suites = cipher_suites.data();
    ssl_config.cipher_suites_size = cipher_suites.size();
    std::vector<const char*> user_cas;
    for (const auto& user_ca : config.user_cas) {
      user_cas.push_back(user_ca.c_str());
    }
    ssl_config.user_cas = user_cas.data();
    ssl_config.user_cas_size = user_cas.size();
    ssl_config.verify_host =
      config.verify_host.empty() ? nullptr : config.verify_host.c_str();
    ssl_config.on_engine_read =
      [](rte_ssl_engine_t* engine, const uint8_t* data, size_t size) {
        SslEngine* This = (SslEngine*)rte_ssl_engine_get_user_data(engine);
        if (data && size) {
          This->config_.on_app_data(data, size);
        }
      };
    ssl_config.io_write =
      [](rte_ssl_engine_t* engine, const uint8_t* buf, size_t size) {
        SslEngine* This = (SslEngine*)rte_ssl_engine_get_user_data(engine);
        if (buf && size) {
          return This->config_.transport_write(buf, size);
        }

        return (size_t)0;
      };
    ssl_config.handshake_done = [](rte_ssl_engine_t* engine, int success) {
      SslEngine* This = (SslEngine*)rte_ssl_engine_get_user_data(engine);
      This->config_.handshake_done(success != 0);
    };
    auto* engine = rte_ssl_engine_create(&ssl_config);
    if (!engine) {
      return nullptr;
    }

    return std::make_shared<SslEngine>(engine, config);
  }

 public:
  SslEngine(rte_ssl_engine_t* engine, const Config& config)
    : engine_(engine),
      config_(config) {
    rte_ssl_engine_set_user_data(engine_, this);
  }

  ~SslEngine() {
    rte_ssl_engine_destroy(engine_);
  }

  bool Handshake() {
    return rte_ssl_engine_handshake(engine_) != 0;
  }

  void OnTransportData(const uint8_t* buf, size_t size) {
    rte_ssl_engine_on_io_read(engine_, buf, size);
  }

  size_t WriteApplicationData(const uint8_t* buf, size_t size) {
    return rte_ssl_engine_engine_write(engine_, buf, size);
  }

 private:
  rte_ssl_engine_t* engine_;
  Config config_;
};

}  // namespace crypto
}  // namespace utils
}  // namespace agora

#endif