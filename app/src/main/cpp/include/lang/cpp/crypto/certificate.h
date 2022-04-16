/**
 *
 * Agora Real Time Engagement
 * Created by Zheng, Ender in 2021-08.
 * Copyright (c) 2021 Agora IO. All rights reserved.
 *
 */
#pragma once

#include "crypto/certificate.h"

#if defined(__cplusplus)

#include <cstdint>
#include <ctime>
#include <memory>
#include <string>
#include <vector>

namespace agora {
namespace utils {
namespace crypto {

class Certificate {
 public:
  static std::shared_ptr<Certificate> CreateByName(
    const std::string& alias_name) {
    auto* c = rte_cert_create_by_name(alias_name.c_str());
    if (!c) {
      return nullptr;
    }

    return std::make_shared<Certificate>(c);
  }

  static std::shared_ptr<Certificate> CreateBySystemName(
    const std::string& alias_name) {
    auto* c = rte_cert_create_by_system_name(alias_name.c_str());
    if (!c) {
      return nullptr;
    }

    return std::make_shared<Certificate>(c);
  }

  static std::shared_ptr<Certificate> CreateFromFile(
    const std::string& pem_file) {
    auto* c = rte_cert_import_pem_file(pem_file.c_str());
    if (!c) {
      return nullptr;
    }

    return std::make_shared<Certificate>(c);
  }

  static std::shared_ptr<Certificate> CreateFromPemContent(
    const std::string& pem_content) {
    auto* c = rte_cert_import_pem_content(pem_content.c_str());
    if (!c) {
      return nullptr;
    }

    return std::make_shared<Certificate>(c);
  }

  static std::shared_ptr<Certificate> CreateFromData(const uint8_t* data,
                                                     size_t size) {
    auto* c = rte_cert_import_x509((const char*)data, size);
    if (!c) {
      return nullptr;
    }

    return std::make_shared<Certificate>(c);
  }

  static std::vector<std::string> GetAllCerts() {
    // this function is dangerout, don't call it again
    return std::vector<std::string>();
  }

  static std::string GetUserCertByName(const std::string& alias) {
    auto* cert = rte_cert_get_embeded_cert(alias.c_str());
    if (!cert) {
      return "";
    }

    auto size = rte_cert_get_x509_size(cert);
    if (size == 0) {
      return "";
    }

    std::unique_ptr<char[]> buf = std::make_unique<char[]>(size);
    if (!buf) {
      return "";
    }

    rte_cert_export_x509(cert, (void*)buf.get(), size);
    return buf.get();
  }

 public:
  std::string IssuerName() {
    auto size = rte_cert_get_issuer_name_size(cert_);
    if (size == 0) {
      return "";
    }

    std::unique_ptr<char[]> buf = std::make_unique<char[]>(size);
    if (!buf) {
      return "";
    }

    rte_cert_get_issuer_name(cert_, (char*)buf.get(), size);
    return buf.get();
  }

  bool VerifyHost(const std::string& host) {
    return rte_cert_verify_host(cert_, host.c_str());
  }

  bool VerifySignature(const std::string& content,
                       const std::string& signature) {
    return rte_cert_verify_signature(cert_,
                                     content.c_str(),
                                     content.length(),
                                     signature.c_str(),
                                     signature.length());
  }

  std::vector<uint8_t> Export() {
    std::vector<uint8_t> ret;

    auto size = rte_cert_get_x509_size(cert_);
    if (size == 0) {
      return ret;
    }

    ret.resize(size);
    rte_cert_export_x509(cert_, (void*)ret.data(), size);
    return ret;
  }

  ~Certificate() {
    rte_cert_destroy(cert_);
  }

  explicit Certificate(rte_cert_t* c) : cert_(c) {
  }

 private:
  rte_cert_t* cert_;
};

class TrustManager {
 public:
  static std::shared_ptr<TrustManager> CreateFromCerts(
    const std::vector<std::string>& pem_certs) {
    std::vector<const char*> certs;
    certs.reserve(pem_certs.size());
    for (const auto& cert : pem_certs) {
      certs.push_back(cert.c_str());
    }

    auto tm = rte_trust_create(certs.data(), certs.size());
    if (!tm) {
      return nullptr;
    }

    return std::make_shared<TrustManager>(tm);
  }

 public:
  explicit TrustManager(rte_trust_t* tm) : tm_(tm) {
  }

  bool Evaluate() {
    return rte_trust_evaluate(tm_);
  }

  ~TrustManager() {
    rte_trust_destroy(tm_);
  }

 private:
  rte_trust_t* tm_;
};

}  // namespace crypto
}  // namespace utils
}  // namespace agora

#endif