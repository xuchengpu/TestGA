/**
 *
 * Agora Real Time Engagement
 * Created by Zheng, Ender in 2021-08.
 * Copyright (c) 2021 Agora IO. All rights reserved.
 *
 */
#pragma once
#include "rte_config.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "ap/client.h"
#include "common/config.h"
#include "transport/agtp.h"

/** \addtogroup argus
 *  @{
 */

typedef struct rte_argus_t rte_argus_t;

/**
 * @brief The callback when argus connect done.
 * @param argus The argus instance.
 * @param result true if connect success, false otherwise.
 * @param arg The argument taken from |rte_argus_create|
 */
typedef void (*rte_argus_on_connect_func_t)(rte_argus_t* argus,
                                            bool result,
                                            void* arg);
/**
 * @brief The callback when argus disconnect done.
 * @param argus The argus instance.
 */
typedef void (*rte_argus_on_disconnect_func_t)(rte_argus_t* argus, void* arg);

/**
 * @brief The callback when argus response received.
 * @param argus The argus instance.
 * @param result The resule ot send, 1 for success, 0 for fail (even with retry)
 * @param seq The sequence number of acked request.
 * @param rtt The diff of time in second between request send and response
 *            received.
 * @param arg The argument taken from |rte_argus_create|
 */
typedef void (*rte_argus_on_resp_func_t)(rte_argus_t* argus,
                                         int result,
                                         uint32_t seq,
                                         uint32_t rtt,
                                         void* arg);

/**
 * @brief The callback when argus connection has error.
 * @param argus The argus instance.
 * @param error The error number.
 * @param arg The argument taken from |rte_argus_create|
 */
typedef void (*rte_argus_on_error_func_t)(rte_argus_t* argus,
                                          int error,
                                          void* arg);

/**
 * @brief Callbacks of argus connection.
 */
typedef struct rte_argus_callback_t {
  rte_argus_on_connect_func_t on_connect_result;
  rte_argus_on_disconnect_func_t on_disconnected;
  rte_argus_on_resp_func_t on_response;
  rte_argus_on_error_func_t on_error;
} rte_argus_callback_t;

/**
 * @brief Configs of argus connection.
 */
typedef struct rte_argus_config_t {
  /**
   * @brief The area code, for example "CN", "JP", "US", etc..
   */
  const char* area_code;
  /**
   * @brief Use aut as underlying connection or not.
   */
  bool use_aut;
  /**
   * @brief Use ipv6 (if possible)
   */
  bool use_ipv6;
} rte_argus_config_t;

typedef enum rte_argus_data_format_t {
  RTE_ARGUS_FORMAT_AGORA = 0,
  RTE_ARGUS_FORMAT_THRIFT = 3,
  RTE_ARGUS_FORMAT_PROTOBUF = 4,
  RTE_ARGUS_FORMAT_PROTOBUF_IPV6 = 6
} rte_argus_data_format_t;

/**
 * @note Sub-types only apply to RTE_ARGUS_FORMAT_PROTOBUF_IPV6
 * All other format has RTE_ARGUS_SUB_FORMAT_DEFAULT which means
 * do _not_ contains payload length.
 */
typedef enum rte_argus_data_sub_format_t {
  RTE_ARGUS_SUB_FORMAT_DEFAULT,
  RTE_ARGUS_SUB_FORMAT_WITH_PAYLOAD_LEN,
} rte_argus_data_sub_format_t;

typedef enum rte_argus_qos_level_t {
  RTE_ARGUS_QOS_CRITICAL = 0,
  RTE_ARGUS_QOS_MOREHIGH = 5,
  RTE_ARGUS_QOS_HIGH = 10,
  RTE_ARGUS_QOS_NORMAL = 20,
  RTE_ARGUS_QOS_LOW = 30,
  RTE_ARGUS_QOS_DEBUG = 40,
  RTE_ARGUS_QOS_OBSOLETE = 100
} rte_argus_qos_level_t;

/**
 * @brief link configuration of argus connection.
 */
typedef struct rte_argus_link_config_t {
  /**
   * @brief Enable retry cache
   */
  int enable_cache;
  /**
   * @brief The retry interval in millisecond, only valid if |enable_cache| = 1
   */
  uint32_t interval;
  /**
   * @brief The retry times before abandom, only valid if |enable_cache| = 1
   */
  size_t retries;
} rte_argus_link_config_t;

/**
 * @brief Create an argus connection.
 *
 * @param ap_factory The ap factory.
 * @param agtp_factory The transport factory.
 * @param callback The callbacks.
 * @param appid The token used in ap server.
 * @param sessionid The session id used in ap server.
 * @param arg The argument that will be taken in callbacks.
 * @return Instance of argus connection if success, NULL otherwise.
 */
AGORA_RTE_API rte_argus_t* rte_argus_create(
  rte_ap_client_factory_t* ap_factory,
  rte_agtp_factory_t* agtp_factory,
  const rte_argus_callback_t* callback,
  const char* appid,
  const char* sessionid,
  void* arg);

/**
 * @brief Destroy an argus connection instance.
 *
 * @param argus The argus connection.
 * @note |rte_tds_task_t| will use a ref_count to protect it's own life cycle,
 *       the memory will dealloced _after_ all reference decreased.
 */
AGORA_RTE_API void rte_argus_destroy(rte_argus_t* argus);

/**
 * @brief Set callback of argus connection.
 *
 * @param argus The argus connection.
 * @param callback The callbacks.
 * @param arg The argument that will be taken in callbacks.
 * @note Will replace old callback if new callback is not NULL,
 *       remain untouched otherwise.
 *       For example if we have new callback like this:
 *          {.on_connect_result = NULL,
 *          .on_response = some_valid_function,}
 *       The |on_response| will be replaced but |on_connect_result| keeps old
 */
AGORA_RTE_API void rte_argus_set_callback(rte_argus_t* argus,
                                          const rte_argus_callback_t* callback,
                                          void* arg);

/**
 * @brief Connect to argus server.
 *
 * @param argus The argus connection.
 * @param config The configs used when connecting.
 * @return 0 if success, -1 otherwise.
 */
AGORA_RTE_API int rte_argus_connect(rte_argus_t* argus,
                                    const rte_argus_config_t* config);

/**
 * @brief Disconnect from a argus server.
 *
 * @param argus The argus connection.
 * @return 0 if success, -1 otherwise.
 */
AGORA_RTE_API int rte_argus_disconnect(rte_argus_t* argus);

/**
 * @brief Send request to argus server.
 *
 * @param argus The argus connection.
 * @param type The data type of payload.
 * @param subtype The subtype of playload.
 * @param qos The qos level of this request.
 * @param seq The sequence number of this request. Will echo in response.
 * @param cid The channel ID of this request, 0 if no channel.
 * @param data The buffer of payload.
 * @param size The size of payload.
 * @return 0 if success, -1 otherwise.
 * @note The format of |data| must align with |type| otherwise unknown behavior
 *       in server side.
 */
AGORA_RTE_API int rte_argus_send(rte_argus_t* argus,
                                 rte_argus_data_format_t type,
                                 rte_argus_data_sub_format_t subtype,
                                 rte_argus_qos_level_t qos,
                                 uint32_t seq,
                                 uint32_t cid,
                                 const uint8_t* data,
                                 size_t size);

/**
 * @brief Set link config of an argus connection
 *
 * @param argus The argus connection.
 * @param config The link config.
 * @note If |config| == NULL, will disable cache.
 * @note Will drop all remaining cache if re-config.
 */
AGORA_RTE_API void rte_argus_set_link_config(
  rte_argus_t* argus,
  const rte_argus_link_config_t* config);

/**
 * @brief Get underlying io delegate
 *
 * @param argus The argus connection.
 * @return The io delegate
 */
AGORA_RTE_API const rte_facility_delegate_t* rte_argus_get_shim(
  rte_argus_t* argus);

/** @}*/