//
//  Agora Real Time Engagement
//
//  Created by Ender Zheng in 2020-08.
//  Copyright (c) 2015 Agora IO. All rights reserved.
//
#pragma once

#include "rte_config.h"

#include "crypto/asymmetric.h"

#if defined(__cplusplus)

#include <memory>
#include <string>

namespace agora {
namespace utils {
namespace crypto {

enum class ASYMMETRIC_ALGORITHM { RSA, ECDH, ECDSA, MAX };

class AsymmetricKeyPair {
 public:
  static std::shared_ptr<AsymmetricKeyPair> Create(ASYMMETRIC_ALGORITHM alg,
                                                   size_t key_len_bits) {
    rte_asymm_key_pair_t* pair = nullptr;

    if (alg == ASYMMETRIC_ALGORITHM::RSA) {
      pair = rte_asymm_key_pair_create_rsa(key_len_bits);
    } else if (alg == ASYMMETRIC_ALGORITHM::ECDH) {
      pair = rte_asymm_key_pair_create_ecdh(key_len_bits);
    } else if (alg == ASYMMETRIC_ALGORITHM::ECDSA) {
      pair = rte_asymm_key_pair_create_ecdsa(key_len_bits);
    } else {
      return nullptr;
    }

    if (pair == nullptr) {
      return nullptr;
    }

    return std::make_shared<AsymmetricKeyPair>(pair);
  }

  static std::shared_ptr<AsymmetricKeyPair> CreateEmpty(
    ASYMMETRIC_ALGORITHM alg,
    size_t key_len_bits = 0) {
    rte_asymm_key_pair_t* pair = nullptr;

    if (alg == ASYMMETRIC_ALGORITHM::RSA) {
      pair = rte_asymm_key_pair_create_rsa_empty(key_len_bits);
    } else if (alg == ASYMMETRIC_ALGORITHM::ECDH) {
      pair = rte_asymm_key_pair_create_ecdh_empty(key_len_bits);
    } else if (alg == ASYMMETRIC_ALGORITHM::ECDSA) {
      pair = rte_asymm_key_pair_create_ecdsa_empty(key_len_bits);
    } else {
      return nullptr;
    }

    if (pair == nullptr) {
      return nullptr;
    }

    return std::make_shared<AsymmetricKeyPair>(pair);
  }

 public:
  explicit AsymmetricKeyPair(rte_asymm_key_pair_t* pair) : pair_(pair) {
  }

  ~AsymmetricKeyPair() {
    if (pair_ != nullptr) {
      rte_asymm_key_pair_destroy(pair_);
    }
  }

  rte_asymm_key_t* PublicKey() const {
    return rte_asymm_key_pair_get_public(pair_);
  }

  rte_asymm_key_t* PrivateKey() const {
    return rte_asymm_key_pair_get_private(pair_);
  }

  size_t KeyLengthBits() const {
    return rte_asymm_key_pair_get_key_length(pair_);
  }

  bool ImportPublicKey(const std::string& key) {
    return rte_asymm_key_pair_import_public(pair_,
                                            (const uint8_t*)key.c_str(),
                                            key.length());
  }

  std::string ExportPublicKey() {
    size_t len = rte_asymm_key_pair_get_public_key_size(pair_);
    std::string key(len, '\0');
    rte_asymm_key_pair_export_public(pair_, (uint8_t*)key.c_str(), len);
    return key;
  }

  rte_asymm_key_pair_t* GetRaw() const {
    return pair_;
  }

 private:
  rte_asymm_key_pair_t* pair_;
};

class Encryptor {
 public:
  static std::shared_ptr<Encryptor> Create(
    ASYMMETRIC_ALGORITHM alg,
    std::shared_ptr<AsymmetricKeyPair> keys,
    rte_asymmetric_padding_t padding = RTE_ASYMMETRIC_PADDING_NONE) {
    rte_asymmetric_t* encryptor = nullptr;
    if (alg == ASYMMETRIC_ALGORITHM::RSA) {
      encryptor = rte_asymmetric_create_rsa(padding);
    } else if (alg == ASYMMETRIC_ALGORITHM::ECDSA) {
      encryptor = rte_asymmetric_create_ecdsa(padding);
    } else {
      return nullptr;
    }

    if (encryptor == nullptr) {
      return nullptr;
    }

    return std::make_shared<Encryptor>(encryptor, keys);
  }

 public:
  Encryptor(rte_asymmetric_t* encryptor,
            std::shared_ptr<AsymmetricKeyPair> keys)
    : encryptor_(encryptor),
      key_(keys) {
  }

  ~Encryptor() {
    if (encryptor_ != nullptr) {
      rte_asymmetric_destroy(encryptor_);
    }
  }

  size_t Encrypt(const uint8_t* input,
                 size_t input_len,
                 uint8_t* output,
                 size_t output_len) {
    if (!key_) {
      return 0;
    }

    return rte_asymmetric_encrypt(encryptor_,
                                  key_->GetRaw(),
                                  input,
                                  input_len,
                                  output,
                                  output_len);
  }

  size_t Decrypt(const uint8_t* input,
                 size_t input_len,
                 uint8_t* output,
                 size_t output_len) {
    if (!key_) {
      return 0;
    }

    return rte_asymmetric_decrypt(encryptor_,
                                  key_->GetRaw(),
                                  input,
                                  input_len,
                                  output,
                                  output_len);
  }

  void SetKey(std::shared_ptr<AsymmetricKeyPair> keys) {
    key_ = keys;
  }

 private:
  rte_asymmetric_t* encryptor_;
  std::shared_ptr<AsymmetricKeyPair> key_;
};

class SharedSecret {
 public:
  static std::shared_ptr<SharedSecret> Create(
    ASYMMETRIC_ALGORITHM alg,
    std::shared_ptr<AsymmetricKeyPair> local,
    std::shared_ptr<AsymmetricKeyPair> remote) {
    rte_asymmetric_t* shared_secret = nullptr;
    shared_secret = rte_asymmetric_create_ecdh();
    if (shared_secret == nullptr) {
      return nullptr;
    }

    return std::make_shared<SharedSecret>(shared_secret, local, remote);
  }

 public:
  SharedSecret(rte_asymmetric_t* shared_secret,
               std::shared_ptr<AsymmetricKeyPair> local,
               std::shared_ptr<AsymmetricKeyPair> remote)
    : shared_secret_(shared_secret),
      local_(local),
      remote_(remote) {
  }

  ~SharedSecret() {
    if (shared_secret_ != nullptr) {
      rte_asymmetric_destroy(shared_secret_);
    }
  }

  size_t GenerateSecret(uint8_t* output, size_t output_len) {
    return rte_asymmetric_share_secret(shared_secret_,
                                       local_->GetRaw(),
                                       remote_->GetRaw(),
                                       output,
                                       output_len);
  }

 private:
  rte_asymmetric_t* shared_secret_;
  std::shared_ptr<AsymmetricKeyPair> local_;
  std::shared_ptr<AsymmetricKeyPair> remote_;
};

std::string ECCPublicKeyDerToAsn(const std::string& ecc_pub) {
  auto size = rte_ecc_public_key_ansi_size((const uint8_t*)ecc_pub.c_str(),
                                           ecc_pub.length());
  std::string asn(size, '\0');
  rte_ecc_public_key_der_to_ansi((const uint8_t*)ecc_pub.c_str(),
                                 ecc_pub.length(),
                                 (uint8_t*)asn.c_str(),
                                 size);
  return asn;
}

std::string ECCPublicKeyAsnToDer(const std::string& ecc_pub) {
  auto size = rte_ecc_public_key_der_size((const uint8_t*)ecc_pub.c_str(),
                                          ecc_pub.length());
  std::string der(size, '\0');
  rte_ecc_public_key_ansi_to_der((const uint8_t*)ecc_pub.c_str(),
                                 ecc_pub.length(),
                                 (uint8_t*)der.c_str(),
                                 size);
  return der;
}

}  // namespace crypto
}  // namespace utils
}  // namespace agora

#endif