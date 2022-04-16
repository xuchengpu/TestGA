/**
 *
 * Agora Real Time Engagement
 * Created by Zheng, Ender in 2021-08.
 * Copyright (c) 2021 Agora IO. All rights reserved.
 *
 */
#pragma once
#include "rte_config.h"

#include "common/config.h"
#include "common/protocol.h"

/** \addtogroup ap
 *  @{
 */

typedef struct rte_ap_client_factory_t rte_ap_client_factory_t;
typedef struct rte_ap_client_t rte_ap_client_t;

/**
 * @brief General response from ap server
 */
typedef struct rte_ap_client_generic_resp_t {
  /**
   * @brief Wan ip of the ap client instance that send request.
   */
  const char* wan_ip;
  /**
   * @brief Service ID of ap server.
   */
  uint32_t service_id;
  /**
   * @brief Timestamp of ap server.
   */
  uint64_t server_ts;
  /**
   * @brief URI of ap server.
   */
  uint16_t uri;
  /**
   * @brief The payload of response.
   */
  const char* payload;
  /**
   * @brief The size of payload.
   */
  size_t payload_size;
  /**
   * @brief Type of ap server.
   */
  int service_type;
  /**
   * @brief The error code of response, 0 means no error.
   */
  int error;
  /**
   * @brief The ip of ap server.
   */
  const char* ap_ip;
  /**
   * @brief Whether it's an embeded ip or not.
   */
  int is_embedded_ip;
} rte_ap_client_general_resp_t;

/**
 * @brief Callbacks that ap client will trigger.
 */
typedef struct rte_ap_client_callback_t {
  /**
   * @brief Triggered when the ap client instance needs re-init
   * @param self The ap client instance.
   * @param arg The argument take from |rte_ap_client_create|
   */
  void (*on_need_reinit)(rte_ap_client_t* self, void* arg);

  /**
   * @brief Triggered when the ap client connect timeout.
   * @param self The ap client instance.
   * @param arg The argument take from |rte_ap_client_create|
   */
  void (*on_timeout)(rte_ap_client_t* self, void* arg);

  /**
   * @brief Triggered when the ap client receive response.
   * @param self The ap client instance.
   * @param resp The response.
   * @param need_terminate If the callback set |need_terminate| to 1, the ap
   *                       client install will do terminate procedure, otherwise
   *                       will retry connect.
   * @param arg The argument take from |rte_ap_client_create|
   */
  void (*on_packet)(rte_ap_client_t* self,
                    const rte_ap_client_general_resp_t* resp,
                    int* need_terminate,
                    void* arg);
} rte_ap_client_callback_t;

/**
 * @brief Config used when create ap client factory instance.
 */
typedef struct rte_ap_client_config_t {
  /**
   * @brief The embeded ip list.
   */
  const char** default_ip_list;
  /**
   * @brief The count of embeded ip list.
   */
  size_t default_ip_list_count;
  /**
   * @brief The ports of embeded ip list.
   */
  const uint16_t* default_ports;
  /**
   * @brief The count of port list.
   */
  size_t default_ports_count;
  /**
   * @brief The type of server.
   */
  rte_ap_server_type_t default_server_type;
} rte_ap_client_config_t;

/**
 * @brief Create ap client factory instance.
 *
 * @param facility_config The io delegate used for ap client and underlying
 *                        transport.
 * @param af_family The address family, same as the one in `socket`
 * @param ap_config The config used when creating ap client factory.
 * @return The instance of ap client facotory if success, NULL otherwise.
 * @note Take care of the life cycle of io delegate, make sure it alive until
 *       all ap client from the same factory destroied.
 */
AGORA_RTE_API rte_ap_client_factory_t* rte_ap_client_factory_create(
  const rte_facility_delegate_t* facility_config,
  int af_family,
  const rte_ap_client_config_t* ap_config);

/**
 * @brief Destroy ap client factory instance.
 *
 * @param factory The ap client factory.
 */
AGORA_RTE_API void rte_ap_client_factory_destroy(
  rte_ap_client_factory_t* factory);

/**
 * @brief Clear the embeded ip list.
 *
 * @param factory The ap client factory.
 */
AGORA_RTE_API void rte_ap_client_factory_clear_server_list(
  rte_ap_client_factory_t* factory);

/**
 * @brief Append ip and port to embeded ip list.
 *
 * @param factory The ap client factory.
 * @param server_list The embeded ip list.
 * @param server_list_count The count of embeded ip list.
 * @param port_list The ports of embeded ip list.
 * @param port_list_count The count of port list.
 * @param server_type The type of server.
 * @note If |port_list_count| == 1, it will apply to all ips in |server_list|.
 */
AGORA_RTE_API void rte_ap_client_factory_append_server_list(
  rte_ap_client_factory_t* factory,
  const char** server_list,
  size_t server_list_count,
  const uint16_t* port_list,
  size_t port_list_count,
  rte_ap_server_type_t server_type);

/**
 * @brief Append dns result to ap server list.
 *
 * @param factory The ap client factory.
 * @param ip_list The ip list.
 * @param ip_list_count The count of ip list.
 * @param port_list The ports of ip list.
 * @param port_list_count The count of port list.
 * @param server_type The type of server.
 * @note If |port_list_count| == 1, it will apply to all ips in |server_list|.
 */
AGORA_RTE_API void rte_ap_client_factory_set_dns_result(
  rte_ap_client_factory_t* factory,
  const char** ip_list,
  size_t ip_list_count,
  const uint16_t* port_list,
  size_t port_list_count,
  rte_ap_server_type_t server_type);

/**
 * @brief Enable crypto or not.
 *
 * @param factory The ap client factory.
 * @param enable 1 to enable, 0 to disable.
 */
AGORA_RTE_API void rte_ap_client_factory_set_crypto_access(
  rte_ap_client_factory_t* factory,
  int enable);

/**
 * @brief Check whether ap client factory is initialized or not.
 *
 * @param factory The ap client factory.
 * @return 1 if initialized, 0 otherwise.
 */
AGORA_RTE_API int rte_ap_client_factory_is_initialized(
  const rte_ap_client_factory_t* factory);

/**
 * @brief Get underlying io delegate of an ap client factory.
 *
 * @param factory The ap client factory.
 * @return The io delegate.
 */
AGORA_RTE_API const rte_facility_delegate_t* rte_ap_client_factory_get_shim(
  const rte_ap_client_factory_t* factory);

/**
 * @brief Create an ap client instance from factory.
 *
 * @param factory The ap client factory.
 * @param callback Callbacks of ap client.
 * @param appid The token of ap server.
 * @param sid The session id of ap server.
 * @param arg The arg that will be taken in |callback|.
 * @return The ap client instance if success, NULL othwerwise.
 */
AGORA_RTE_API rte_ap_client_t* rte_ap_client_create(
  rte_ap_client_factory_t* factory,
  rte_ap_client_callback_t* callback,
  const char* appid,
  const char* sid,
  void* arg);

/**
 * @brief Destroy an ap client instance.
 *
 * @param client The ap client.
 */
AGORA_RTE_API void rte_ap_client_destroy(rte_ap_client_t* client);

/**
 * @brief Set timeout time of ap request.
 *
 * @param client The ap client.
 * @param timeout The timeout time in ms.
 */
AGORA_RTE_API void rte_ap_client_set_timeout(rte_ap_client_t* client,
                                             int timeout);

/**
 * @brief Set a server list that will connect directly, ignore embeded ip list
 *        and dns result.
 *
 * @param client The ap client.
 * @param ip_list The ip list.
 * @param ip_list_count The count of ip list.
 * @param port_list The port of ip list.
 */
AGORA_RTE_API void rte_ap_client_set_designated_servers(rte_ap_client_t* client,
                                                        const char** ip_list,
                                                        size_t ip_list_count,
                                                        uint16_t port);

/**
 * @brief Stop an inflight request.
 *
 * @param client The ap client.
 */
AGORA_RTE_API void rte_ap_client_stop_request(rte_ap_client_t* client);

/**
 * @brief Force use tcp link and ignore other link type.
 *
 * @param client The ap client.
 * @param force_tcp 1 to use tcp, 0 to pick automatically.
 */
AGORA_RTE_API void rte_ap_client_set_force_tcp_transport(
  rte_ap_client_t* client,
  int force_tcp);

/**
 * @brief Force disable proxy.
 *
 * @param client The ap client.
 * @param direct 1 to disable proxy, 0 hornor configs.
 */
AGORA_RTE_API void rte_ap_client_set_direct(rte_ap_client_t* client,
                                            int direct);

/**
 * @brief Set sni type when doing ssl handshake.
 *
 * @param client The ap client.
 * @param sni_type The sni type.
 */
AGORA_RTE_API void rte_ap_client_set_sni_type(rte_ap_client_t* client,
                                              rte_sni_type_t sni_type);

/**
 * @brief Set request strategy when doing ap request.
 *
 * @param client The ap client.
 * @param request_type The strategy type.
 */
AGORA_RTE_API void rte_ap_client_set_request_strategy(
  rte_ap_client_t* client,
  rte_request_strategy_type_t request_type);

/**
 * @brief Send request to ap.
 *
 * @param client The ap client.
 * @param uri The uri of ap server.
 * @param service_id The service ID if ap server.
 * @param payload The payload of request.
 * @param payload_size The size of payload.
 * @param network_type The type of underlying transport.
 * @param append_request Thether this is a appended request or not.
 */
AGORA_RTE_API void rte_ap_client_send_request(
  rte_ap_client_t* client,
  uint16_t uri,
  uint32_t service_id,
  const char* payload,
  size_t payload_size,
  rte_tp_network_type_t network_type,
  int append_request);

/**
 * @brief Notify ap client that network type changed.
 *
 * @param client The ap client.
 */
AGORA_RTE_API void rte_ap_client_trigger_connection_changed(
  rte_ap_client_t* client);

/**
 * @brief Get underlying io delegate of an ap client.
 *
 * @param factory The ap client.
 * @return The io delegate.
 */
AGORA_RTE_API const rte_facility_delegate_t* rte_ap_client_get_shim(
  const rte_ap_client_t* client);

/** @}*/