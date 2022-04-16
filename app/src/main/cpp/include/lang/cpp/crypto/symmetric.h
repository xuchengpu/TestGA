//
//  Agora Real Time Engagement
//
//  Created by Ender Zheng in 2020-08.
//  Copyright (c) 2015 Agora IO. All rights reserved.
//
#pragma once
#include "rte_config.h"

#include "crypto/symmetric.h"

#if defined(__cplusplus)

#include <memory>
#include <set>
#include <stdint.h>
#include <string>

namespace agora {
namespace utils {
namespace crypto {

class SymmetricKey {
 public:
  static std::shared_ptr<SymmetricKey> Create(rte_symmetric_cipher_t cipher) {
    auto* key = rte_symmetric_key_create(cipher);
    if (!key) {
      return nullptr;
    }

    return std::make_shared<SymmetricKey>(key);
  }

  static std::shared_ptr<SymmetricKey> CreateEmpty() {
    auto* key = rte_symmetric_key_create_empty();
    if (!key) {
      return nullptr;
    }

    return std::make_shared<SymmetricKey>(key);
  }

 public:
  explicit SymmetricKey(rte_symmetric_key_t* key) : key_(key) {
  }

  ~SymmetricKey() {
    rte_symmetric_key_destroy(key_);
  }

  const uint8_t* Get() const {
    return rte_symmetric_key_get(key_);
  }

  bool Import(const uint8_t* buf, size_t buf_size) {
    return rte_symmetric_key_import(key_, buf, buf_size);
  }

  bool IsValid() const {
    return rte_symmetric_key_is_valid(key_);
  }

  size_t KeyLength() const {
    return rte_symmetric_key_get_length(key_);
  }

  const rte_symmetric_key_t* GetRaw() const {
    return key_;
  }

 private:
  rte_symmetric_key_t* key_;
};

class Symmetric {
 public:
  static std::shared_ptr<Symmetric> Create(
    rte_symmetric_cipher_t cipher,
    const std::shared_ptr<SymmetricKey> key = nullptr) {
    auto* s = rte_symmetric_create(
      cipher,
      (rte_symmetric_key_t*)(key ? key->GetRaw() : nullptr));
    if (!s) {
      return nullptr;
    }

    return std::make_shared<Symmetric>(s);
  }

 public:
  explicit Symmetric(rte_symmetric_t* symmetric) : symmetric_(symmetric) {
  }

  ~Symmetric() {
    rte_symmetric_destroy(symmetric_);
  }

  bool SetKey(std::shared_ptr<SymmetricKey> key) {
    return rte_symmetric_set_key(symmetric_,
                                 (rte_symmetric_key_t*)key->GetRaw());
  }

  size_t Encrypt(const void* input,
                 size_t input_size,
                 void* iv,
                 size_t iv_size,
                 void* aad,
                 size_t aad_size,
                 void* output,
                 size_t output_size,
                 void* auth_tag = nullptr,
                 size_t auth_tag_size = 0) {
    return rte_symmetric_encrypt(symmetric_,
                                 input,
                                 input_size,
                                 iv,
                                 iv_size,
                                 aad,
                                 aad_size,
                                 output,
                                 output_size,
                                 auth_tag,
                                 auth_tag_size);
  }

  size_t Decrypt(const void* input,
                 size_t input_size,
                 void* iv,
                 size_t iv_size,
                 void* aad,
                 size_t aad_size,
                 void* output,
                 size_t output_size,
                 void* auth_tag = nullptr,
                 size_t auth_tag_size = 0) {
    return rte_symmetric_decrypt(symmetric_,
                                 input,
                                 input_size,
                                 iv,
                                 iv_size,
                                 aad,
                                 aad_size,
                                 output,
                                 output_size,
                                 auth_tag,
                                 auth_tag_size);
  }

 private:
  rte_symmetric_t* symmetric_;
};

class Digest {
 public:
  static std::shared_ptr<Digest> Create(rte_digest_alg_t alg,
                                        bool hmac = false,
                                        const uint8_t* key = nullptr,
                                        size_t key_len = 0) {
    rte_digest_t* d = nullptr;
    if (hmac) {
      d = rte_digest_create_hmac(alg, key, key_len);
    } else {
      d = rte_digest_create(alg);
    }

    if (!d) {
      return nullptr;
    }

    return std::make_shared<Digest>(d);
  }

 public:
  explicit Digest(rte_digest_t* digest) : digest_(digest) {
  }

  ~Digest() {
    rte_digest_destroy(digest_);
  }

  void Reset() {
    rte_digest_reset(digest_);
  }

  void Update(const uint8_t* data, size_t length) {
    rte_digest_update(digest_, data, length);
  }

  void Final(uint8_t* data) {
    rte_digest_final(digest_, data);
  }

  size_t DigestLength() const {
    return rte_digest_get_digest_size(digest_);
  }

  size_t BlockSize() const {
    return rte_digest_get_block_size(digest_);
  }

 private:
  rte_digest_t* digest_;
};

static bool KeyDerivationPBKDF1(rte_symmetric_cipher_t cipher,
                                rte_digest_alg_t prf_alg,
                                const char* data,
                                size_t data_size,
                                const uint8_t* salt,
                                size_t round,
                                uint8_t* key,
                                uint8_t* iv) {
  return rte_key_derivation_pbkdf1(cipher,
                                   prf_alg,
                                   data,
                                   data_size,
                                   salt,
                                   round,
                                   key,
                                   iv);
}

static bool KeyDerivationPBKDF2(rte_digest_alg_t prf_alg,
                                const char* data,
                                size_t data_size,
                                const uint8_t* salt,
                                size_t salt_size,
                                size_t round,
                                uint8_t* result,
                                size_t result_size) {
  return rte_key_derivation_pbkdf2(prf_alg,
                                   data,
                                   data_size,
                                   salt,
                                   salt_size,
                                   round,
                                   result,
                                   result_size);
}

static size_t CalculateKeyLength(rte_symmetric_cipher_t cipher) {
  return rte_calculate_key_length(cipher);
}

static size_t CalculateIVLength(rte_symmetric_cipher_t cipher) {
  return rte_calculate_iv_length(cipher);
}

}  // namespace crypto
}  // namespace utils
}  // namespace agora

#endif