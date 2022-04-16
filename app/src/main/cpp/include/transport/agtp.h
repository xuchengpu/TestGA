/**
 *
 * Agora Real Time Engagement
 * Created by Zheng, Ender in 2021-08.
 * Copyright (c) 2021 Agora IO. All rights reserved.
 *
 */

#pragma once

#include "rte_config.h"

#include <stdint.h>

#include "common/config.h"
#include "lib/packer.h"

/** \addtogroup transport
 *  @{
 */

typedef struct rte_agtp_factory_t rte_agtp_factory_t;
typedef struct rte_agtp_t rte_agtp_t;

typedef enum rte_agtp_proxy_type_t {
  RTE_AGTP_PROXY_UDPRELAY = 0,
  RTE_AGTP_PROXY_UDPWITHAPDNS = 1,
  RTE_AGTP_PROXY_UDPWITHAPIPS = 2,
  RTE_AGTP_PROXY_UDPWITHPROXYIPS = 3,
  RTE_AGTP_PROXY_NOPROXY = 4,
  RTE_AGTP_PROXY_TCPWITHAPDNS = 10,
  RTE_AGTP_PROXY_TCPWITHAPIPS = 11,
  RTE_AGTP_PROXY_TCPWITHPROXYIPS = 12,
  RTE_AGTP_PROXY_TCPTLSWITHAPDNS = 13,
  RTE_AGTP_PROXY_TCPTLSWITHAPIPS = 14,
  RTE_AGTP_PROXY_TCPTLSWITHPROXYIPS = 15,
  RTE_AGTP_PROXY_AUTOFALLBACKTCPTLS = 16,
} rte_agtp_proxy_type_t;

typedef enum rte_proxy_env_t {
  RTE_PROXY_ENV_WHITELIST = 0,
  RTE_PROXY_ENV_NONWHITELIST = 1,
} rte_proxy_env_t;

typedef struct rte_ap_default_config_t {
  const char** domains;
  size_t domains_count;
  const char** tls_domains;
  size_t tls_domains_count;
  const char** ips;
  size_t ips_count;
  const char** tls_ips;
  size_t tls_ips_count;
  uint16_t* ports;
  size_t ports_count;
  uint16_t* aut_ports;
  size_t aut_ports_count;
  uint16_t* tls_ports;
  size_t tls_ports_count;
} rte_ap_default_config_t;
typedef struct rte_proxy_config_ap_t {
  rte_ap_default_config_t udp_ap_config;
  rte_ap_default_config_t tcp_ap_config;
  rte_ap_default_config_t tcp_tls_ap_config;
} rte_proxy_config_ap_t;

typedef struct rte_agtp_proxy_config_t {
  rte_agtp_proxy_type_t type;
  const char** param1;
  size_t param1_count;
  uint16_t param2;
  rte_proxy_config_ap_t config_ap;
  rte_proxy_env_t proxy_env;
  int force_proxy;
} rte_agtp_proxy_config_t;

typedef enum rte_auto_fallback_type_t {
  RTE_AUTO_FALLBACK_TYPE_NONEFALLBACK = 0,
  RTE_AUTO_FALLBACK_TYPE_TOTCPIFUDPBLOCK = 1,
  RTE_AUTO_FALLBACK_TYPE_TOWHITELISTTCPIFUDPBLOCK = 2,
} rte_auto_fallback_type_t;

typedef struct rte_agtp_auto_fallback_config_t {
  rte_auto_fallback_type_t type;
  const char* config;
  rte_proxy_config_ap_t config_ap;
} rte_agtp_auto_fallback_config_t;

typedef struct rte_agtp_observer_t {
  void (*on_connect)(rte_agtp_t* transport, int connected, void* args);
  void (*on_error)(rte_agtp_t* transport, int error_type, void* args);
  int (*on_data)(rte_agtp_t* transport,
                 const char* data,
                 size_t length,
                 void* args);
} rte_agtp_observer_t;

typedef struct rte_agtp_server_listener_t {
  rte_agtp_observer_t* (*on_accept)(void* self,
                                    const char* ip,
                                    uint16_t port,
                                    const char* data,
                                    size_t length,
                                    void* args);
  void (*on_accepted)(void* self,
                      rte_agtp_t* tp,
                      const char* early_data,
                      void* args);
} rte_agtp_server_listener_t;

typedef struct rte_agtp_create_observer_t {
  void (*on_creating)(void* self, void* args);
} rte_agtp_create_observer_t;

typedef struct rte_agtp_udp_link_allocator_visitor_t {
  void (*on_udp_data_received)(void* self, void* args);
} rte_agtp_udp_link_allocator_visitor_t;

AGORA_RTE_API rte_agtp_factory_t* rte_agtp_factory_create(
  const rte_facility_delegate_t* config);

AGORA_RTE_API int rte_agtp_factory_set_proxy_config(
  rte_agtp_factory_t* factory,
  const rte_agtp_proxy_config_t* config);

AGORA_RTE_API void rte_agtp_factory_set_auto_fallback_config(
  rte_agtp_factory_t* factory,
  const rte_agtp_auto_fallback_config_t* config);

AGORA_RTE_API void rte_agtp_factory_set_local_ip(rte_agtp_factory_t* factory,
                                                 const char* local_ip);

AGORA_RTE_API void rte_agtp_factory_set_local_network_interface(
  rte_agtp_factory_t* factory,
  const char* network_interface);

AGORA_RTE_API void rte_agtp_factory_set_udp_iptos(rte_agtp_factory_t* factory,
                                                  int enable);

AGORA_RTE_API int rte_agtp_factory_get_proxy_type(rte_agtp_factory_t* factory);

AGORA_RTE_API void rte_agtp_factory_destroy(rte_agtp_factory_t* factory);

AGORA_RTE_API rte_agtp_t* rte_agtp_create_udp(
  rte_agtp_factory_t* factory,
  const rte_agtp_observer_t* observer,
  int direct,
  rte_tp_sock5_client_t* proxy,
  int reuse_link,
  void* args);

AGORA_RTE_API rte_agtp_t* rte_agtp_create_tcp(
  rte_agtp_factory_t* factory,
  const rte_agtp_observer_t* observer,
  int direct,
  int encryption,
  const char* verify_domain,
  const char** custom_cas,
  size_t custom_ca_count,
  void* args);

AGORA_RTE_API rte_agtp_t* rte_agtp_create_aut(
  rte_agtp_factory_t* factory,
  const rte_agtp_observer_t* observer,
  int direct,
  int reuse_link,
  void* args);

AGORA_RTE_API rte_agtp_t* rte_agtp_create_udp_server(
  rte_agtp_factory_t* factory,
  const rte_agtp_observer_t* observer,
  void* listener_obj,
  const rte_agtp_server_listener_t* listener,
  void* args);

AGORA_RTE_API uint16_t rte_agtp_get_binding_port(const rte_agtp_t* tp);

AGORA_RTE_API void rte_agtp_destroy(rte_agtp_t* tp);

AGORA_RTE_API int rte_agtp_factory_is_cloud_proxy(rte_agtp_factory_t* factory);

AGORA_RTE_API void rte_agtp_factory_set_local_proxy(rte_agtp_factory_t* factory,
                                                    const char* verify_domain);

AGORA_RTE_API void rte_agtp_factory_set_creating_observer(
  rte_agtp_factory_t* factory,
  void* obj,
  const rte_agtp_create_observer_t* observer,
  void* args);

AGORA_RTE_API void rte_agtp_factory_set_udp_link_visitor(
  rte_agtp_factory_t* factory,
  void* obj,
  const rte_agtp_udp_link_allocator_visitor_t* visitor,
  void* args);

AGORA_RTE_API void rte_agtp_factory_set_udp_proxy(
  rte_agtp_factory_t* factory,
  const rte_tp_sock5_client_t* sock5_client);

AGORA_RTE_API int rte_agtp_connect(rte_agtp_t* tp,
                                   const char* ip,
                                   uint16_t port);

AGORA_RTE_API
int rte_agtp_connect_with_early_data(rte_agtp_t* tp,
                                     const char* ip,
                                     uint16_t port,
                                     const uint8_t* early_data,
                                     size_t early_data_count);

AGORA_RTE_API int rte_agtp_send_packet(rte_agtp_t* tp,
                                       const rte_unpacked_data_t* packet);

AGORA_RTE_API
int rte_agtp_send_buffer(rte_agtp_t* tp, const char* data, size_t length);

AGORA_RTE_API
int rte_agtp_use_early_data(const rte_agtp_t* tp);

AGORA_RTE_API void rte_agtp_set_timeout(rte_agtp_t* tp, uint32_t timeout);

AGORA_RTE_API
int rte_agtp_is_connected(const rte_agtp_t* tp);

AGORA_RTE_API const char* rte_agtp_remote_ip(const rte_agtp_t* tp);

AGORA_RTE_API uint16_t rte_agtp_remote_port(const rte_agtp_t* tp);

AGORA_RTE_API int rte_agtp_get_type(const rte_agtp_t* tp);

AGORA_RTE_API
int rte_agtp_get_fd(const rte_agtp_t* tp);

AGORA_RTE_API
void rte_agtp_set_observer(rte_agtp_t* tp,
                           const rte_agtp_observer_t* observer,
                           void* arg);

AGORA_RTE_API
void rte_agtp_set_iptos(rte_agtp_t* tp, int enable);

AGORA_RTE_API const char* rte_agtp_get_wan_ip(const rte_agtp_t* tp);

const char* rte_agtp_get_type_name(int type);

AGORA_RTE_API int rte_agtp_type_is_udp(int type);

AGORA_RTE_API int rte_agtp_type_is_tcp(int type);

AGORA_RTE_API int rte_agtp_type_is_aut(int type);

/** @}*/

/* C api for C++ */
#ifdef __cplusplus
#include <functional>
#include <memory>

namespace agora {
namespace transport {
class INetworkTransportHelper;
}  // namespace transport
}  // namespace agora

AGORA_RTE_API agora::transport::INetworkTransportHelper*
rte_agtp_create_transport_helper(const rte_facility_delegate_t* config);

AGORA_RTE_API void rte_agtp_destroy_transport_helper(
  agora::transport::INetworkTransportHelper* helper);

namespace agora {
namespace transport {

using TransportPtr = std::unique_ptr<
  agora::transport::INetworkTransportHelper,
  std::function<void(agora::transport::INetworkTransportHelper*)>>;

static TransportPtr CreateTransport(const rte_facility_delegate_t* config) {
  return TransportPtr(rte_agtp_create_transport_helper(config),
                      [](agora::transport::INetworkTransportHelper* helper) {
                        rte_agtp_destroy_transport_helper(helper);
                      });
}

}  // namespace transport
}  // namespace agora
#endif
