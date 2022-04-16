/**
 *
 * Agora Real Time Engagement
 * Created by Zheng, Ender in 2021-08.
 * Copyright (c) 2021 Agora IO. All rights reserved.
 *
 */
#pragma once
#include "rte_config.h"

#include <stddef.h>

#include "ap/client.h"

/** \addtogroup tds
 *  @{
 */

typedef struct rte_tds_task_t rte_tds_task_t;

/**
 * @brief Parameters when fetching tds config.
 */
typedef struct rte_tds_task_param_t {
  /**
   * @brief Cipher method of payload. Always set to 1.
   */
  int cipher_method;
  /**
   * @brief The device ID.
   */
  const char* device;
  /**
   * @brief The operation system name.
   */
  const char* system;
  /**
   * @brief The version of SDK.
   */
  const char* version;
  /**
   * @brief The token of ap server.
   */
  const char* appid;
  /**
   * @brief The device information.
   */
  const char* detail;
  /**
   * @brief The install ID of APP.
   */
  const char* install_id;
  /**
   * @brief The session ID.
   */
  const char* session_id;
  /**
   * @brief Use ipv6 (if possible)
   */
  int use_ipv6;
} rte_tds_task_param_t;

/**
 * @brief Callback that will be triggered when tds config received.
 * @param result 1 if success, 0 otherwise.
 * @param keys Array of tds keys.
 * @param values Array of tds values.
 * @param count The count of key-value pairs.
 * @param arg The argument taken from |rte_tds_client_fetch|
 */
typedef void (*rte_tds_client_on_message_cb)(int result,
                                             const char** keys,
                                             const char** values,
                                             size_t count,
                                             void* arg);

/**
 * @brief Fetch tds config from ap server.
 *
 * @param factory The ap client factory.
 * @param param The parameters.
 * @param cb The callbacks.
 * @param arg The argument that will be taken in callback
 * @return A tds task instance.
 * @note The tds task instance must be destroied manually after fetching done,
 *       otherwise memory leaks.
 */
AGORA_RTE_API rte_tds_task_t* rte_tds_client_fetch(
  rte_ap_client_factory_t* factory,
  const rte_tds_task_param_t* param,
  rte_tds_client_on_message_cb cb,
  void* arg);

/**
 * @brief Stop a fetching request.
 *
 * @param task The tds task instance from |rte_tds_client_fetch|
 */
AGORA_RTE_API void rte_tds_client_task_stop(rte_tds_task_t* task);

/**
 * @brief Destroy a tds task instance.
 *
 * @param task The tds task instance from |rte_tds_client_fetch|
 * @note |rte_tds_task_t| will use a ref_count to protect it's own life cycle,
 *       the memory will dealloced _after_ all reference decreased.
 */
AGORA_RTE_API void rte_tds_client_task_destroy(rte_tds_task_t* task);

/** @}*/