/**
 *
 * Agora Real Time Engagement
 * Created by Zheng, Ender in 2021-08.
 * Copyright (c) 2021 Agora IO. All rights reserved.
 *
 */
#pragma once

#include "rte_config.h"

#include <stdarg.h>
#include <stdint.h>

/** \addtogroup delegate
 *  @{
 */

#define RTE_FACILITY_VERSION 1

typedef enum rte_tp_network_type_t {
  RTE_TP_NETWORK_TYPE_IPV4 = 1,
  RTE_TP_NETWORK_TYPE_NAT_IPV6 = 1 << 1,
  RTE_TP_NETWORK_TYPE_PURE_IPV6 = 1 << 2,
  RTE_TP_NETWORK_TYPE_IPV6 =
    RTE_TP_NETWORK_TYPE_NAT_IPV6 | RTE_TP_NETWORK_TYPE_PURE_IPV6,
} rte_tp_network_type_t;

typedef enum rte_sni_type_t {
  RTE_SNI_WITH_CONFIGURATION = -1,
  RTE_SNI_WITH_DEFAULT_VALUE,
  RTE_SNI_WITH_SELF_SIGNED_DOMAIN,
  RTE_SNI_WITH_REALTIME_LOG_SERVICE,
  RTE_SNI_WITH_ON_PREMISES,
} rte_sni_type_t;

typedef enum rte_request_strategy_type_t {
  RTE_REQUESTSTRATEGY_DEFAULT = 0,
  RTE_REQUESTSTRATEGY_BESTEFFORT = 1,
  RTE_REQUESTSTRATEGY_AGGRESSIVE = 2,
  RTE_REQUESTSTRATEGY_LITE = 3,
} rte_request_strategy_type_t;

typedef struct rte_facility_delegate_t rte_facility_delegate_t;

/**
 * @brief Common object header of all transport delegate object
 */
typedef struct rte_tp_object_t {
  /**
   * @brief Version of this object
   * @note |version| will and must be guranteed as the first 32 bit of _all_
   *       delegate object.
   */
  uint32_t version;

  /**
   * @brief Dealloc resource of this object.
   * @param obj The object.
   */
  void (*destroy)(struct rte_tp_object_t* obj);

  /**
   * @brief User data that caller can set.
   */
  void* user_data;
} rte_tp_object_t;

typedef struct rte_tp_timer_t rte_tp_timer_t;

/**
 * @brief Callback when timer triggered.
 * @param self The timer.
 * @param arg The arg when calling |start| of rte_tp_timer_t. Be careful that it
 *            is _not_ |user_data| in rte_tp_object_t
 */
typedef void (*rte_tp_timer_callback_t)(rte_tp_timer_t* self, void* arg);

/**
 * @brief Creator of transport timer delegate.
 * @param delegate The delegate object.
 * @param callback The callback when timer triggered.
 * @param interval Interval of timer.
 * @param persist Whether the timer is peridical or not.
 * @return The timer object if success, NULL otherwise
 */
typedef rte_tp_timer_t* (*rte_tp_timer_creator_t)(
  rte_facility_delegate_t* delegate,
  rte_tp_timer_callback_t callback,
  uint32_t interval,
  int persist);

/**
 * @brief Header of timer object.
 */
struct rte_tp_timer_t {
  /**
   * @brief Common object header.
   */
  rte_tp_object_t base;
  /**
   * @brief Start a timer.
   * @param timer The timer object create from |rte_tp_timer_creator_t|.
   * @param arg The arg used in |callback|
   */
  void (*start)(rte_tp_timer_t* timer, void* arg);
  /**
   * @brief Stop a timer.
   * @param timer The timer object create from |rte_tp_timer_creator_t|.
   */
  void (*stop)(rte_tp_timer_t* timer);
};

typedef enum rte_tp_log_level_t {
  RTE_TP_LOG_LEVEL_DEBUG,
  RTE_TP_LOG_LEVEL_INFO,
  RTE_TP_LOG_LEVEL_WARNING,
  RTE_TP_LOG_LEVEL_ERROR,
} rte_tp_log_level_t;

/**
 * @brief Log delegate.
 */
typedef struct rte_tp_log_t {
  /**
   * @brief Enable logger
   * @param delegate The delegate object.
   * @param enable Whether enable logger or not.
   */
  void (*enable)(rte_facility_delegate_t* delegate, int enable);
  /**
   * @brief Write log.
   * @param delegate The delegate object.
   * @param level The log level.
   * @param fmt The log format, exactly same as the one in "vprintf"
   * @param ap The arg list, exactly same as the one in "vprintf"
   */
  void (*vwrite)(rte_facility_delegate_t* delegate,
                 rte_tp_log_level_t level,
                 const char* fmt,
                 va_list ap);
} rte_tp_log_t;

/**
 * @brief The callback that will be triggered when task scheduled.
 * @param from The |from| argument in |post|
 * @param arg The |arg| argument in |arg|
 */
typedef void (*rte_tp_task_func_t)(void* from, void* arg);

/**
 * @brief Task delegate.
 */
typedef struct rte_tp_task_t {
  /**
   * @brief Async perfrom an action.
   * @param delegate The delegate object.
   * @param task The task function.
   * @param from The invoker, useful if want to track the call sequence.
   * @param arg The argument that will appears in |task| when invoked.
   */
  void (*post)(const rte_facility_delegate_t* delegate,
               rte_tp_task_func_t task,
               void* from,
               void* arg);
} rte_tp_task_t;

typedef struct rte_tp_env_t {
  /**
   * @brief Get sdk version.
   * @param delegate The delegate object.
   */
  const char* (*get_sdk_version)(rte_facility_delegate_t* delegate);
  /**
   * @brief Get application id
   * @param delegate The delegate object.
   */
  const char* (*get_app_id)(rte_facility_delegate_t* delegate);
  /**
   * @brief Get sdk install id.
   * @param delegate The delegate object.
   */
  const char* (*get_install_id)(rte_facility_delegate_t* delegate);
  /**
   * @brief Get current area name.
   * @param delegate The delegate object.
   */
  const char* (*get_area_name)(rte_facility_delegate_t* delegate);
  /**
   * @brief Get current network type.
   * @param delegate The delegate object.
   */
  rte_tp_network_type_t (*get_network_type)(rte_facility_delegate_t* delegate);
  /**
   * @brief Get current socket error.
   * @param delegate The delegate object.
   */
  int (*get_sock_error)(rte_facility_delegate_t* delegate);
  /**
   * @brief Get whether crypto is supported or not
   * @param delegate The delegate object.
   */
  int (*is_crypto_enabled)(rte_facility_delegate_t* delegate);
} rte_tp_env_t;

typedef struct rte_tp_sock5_client_t rte_tp_sock5_client_t;

struct rte_tp_sock5_client_t {
  /**
   * @brief Common object header.
   */
  rte_tp_object_t base;
  /**
   * @brief Get wan ip through proxy.
   * @param self The sock5 client object.
   */
  const char* (*get_wan_ip)(const rte_tp_sock5_client_t* self);
  /**
   * @brief Set wan ip through proxy.
   * @param self The sock5 client object.
   * @param ip The ip address.
   */
  void (*set_wan_ip)(rte_tp_sock5_client_t* self, const char* ip);
  /**
   * @brief Get connection ID.
   * @param self The sock5 client object.
   */
  uint32_t (*get_connection_id)(const rte_tp_sock5_client_t* self);
  /**
   * @brief Set connection ID.
   * @param self The sock5 client object.
   * @param id The ID.
   */
  void (*set_connection_id)(rte_tp_sock5_client_t* self, uint32_t id);
  /**
   * @brief Start ignore last sent ts record.
   * @param self The sock5 client object.
   */
  void (*start_ignore_last_ts)(rte_tp_sock5_client_t* self);
  /**
   * @brief Stop ignore last sent ts record.
   * @param self The sock5 client object.
   */
  void (*stop_ignore_last_ts)(rte_tp_sock5_client_t* self);
  /**
   * @brief Get last sent ts record.
   * @param self The sock5 client object.
   * @return Last sent ts.
   */
  uint32_t (*get_last_transport_send_ts)(const rte_tp_sock5_client_t* self);
  /**
   * @brief Get local ip.
   * @param self The sock5 client object.
   * @return Local ip string, do _not_ try to free it.
   */
  const char* (*get_local_ip)(const rte_tp_sock5_client_t* self);
  /**
   * @brief Get local port.
   * @param self The sock5 client object.
   * @return Local port.
   */
  uint16_t (*get_local_port)(const rte_tp_sock5_client_t* self);
  /**
   * @brief Get pointer of implementation object.
   * @param self The sock5 client object.
   * @return Implementation object. This function is useful in interface like
   *         |set_proxy|.
   */
  void* (*get_impl)(const rte_tp_sock5_client_t* self);
};

/**
 * @brief Creator of transport sock5 proxy delegate object.
 * @param delegate The delegate object.
 * @param ip The ip of proxy server.
 * @param port The port of proxy server.
 * @note Currently not support authentacation.
 */
typedef rte_tp_sock5_client_t* (*rte_tp_sock5_creator_t)(
  rte_facility_delegate_t* delegate,
  const char* ip,
  uint16_t port);

typedef struct rte_tp_udp_t rte_tp_udp_t;

/**
 * @brief The callback when udp data received.
 * @param self The udp object.
 * @param ip The remote ip.
 * @param port The remote port.
 * @param data The buffer of received data.
 * @param length The length of buffer.
 * @param arg The argument in |rte_tp_udp_creator_t|. Be careful that it is
 *            _not_ |user_data| of rte_tp_object_t.
 */
typedef void (*rte_tp_udp_on_data_callback_t)(rte_tp_udp_t* self,
                                              const char* ip,
                                              uint16_t port,
                                              const char* data,
                                              size_t length,
                                              void* arg);

/**
 * @brief The callback when udp error happens.
 * @param self The udp object.
 * @param error The error code.
 * @param arg The argument in |rte_tp_udp_creator_t|. Be careful that it is
 *            _not_ |user_data| of rte_tp_object_t.
 */
typedef void (*rte_tp_udp_on_error_callback_t)(rte_tp_udp_t* self,
                                               int error,
                                               void* arg);

struct rte_tp_udp_t {
  /**
   * @brief Common object header.
   */
  rte_tp_object_t base;

  /**
   * @brief Set socket iptos option.
   * @param self The udp object.
   * @param enable Enable iptos or not.
   */
  void (*set_socket_ip_tos)(rte_tp_udp_t* self, int enable);

  /**
   * @brief Get local ip address.
   * @param self The udp object.
   * @return The local ip address. Do _not_ try to free it.
   */
  const char* (*get_local_ip)(const rte_tp_udp_t* self);

  /**
   * @brief Get local port.
   * @param self The udp object.
   * @return The port. Will return 0 if not binded.
   */
  uint16_t (*get_local_port)(const rte_tp_udp_t* self);

  /**
   * @brief Bind udp object to a randomly selected port.
   * @param self The udp object.
   * @param family The socket address family.
   * @return 1 if success, 0 otherwise.
   */
  int (*bind)(rte_tp_udp_t* self, int family);

  /**
   * @brief Bind udp object to an IP with a randomly selected port.
   * @param self The udp object.
   * @param family The socket address family.
   * @param ip The ip address to bind.
   * @return 1 if success, 0 otherwise.
   */
  int (*bind_ip)(rte_tp_udp_t* self, int family, const char* ip);

  /**
   * @brief Bind udp object to an specific device with a randomly selected port.
   * @param self The udp object.
   * @param family The socket address family.
   * @param ifid The interface id to bind.
   * @return 1 if success, 0 otherwise.
   */
  int (*bind_device)(rte_tp_udp_t* self, int family, const char* ifid);

  /**
   * @brief Check whether it is binded or not.
   * @param self The udp object.
   * @return 1 if binded, 0 otherwise.
   */
  int (*binded)(const rte_tp_udp_t* self);

  /**
   * @brief Set buffer size of underlying socket.
   * @param self The udp object.
   * @param size The buffer size.
   * @return 1 if success, 0 otherwise.
   */
  int (*set_socket_buffer_size)(rte_tp_udp_t* self, size_t size);

  /**
   * @brief Set proxy to a udp object.
   * @param self The udp object.
   * @param proxy The socks5 client object.
   * @return 1 if success, 0 otherwise.
   */
  int (*set_proxy_server)(rte_tp_udp_t* self,
                          const rte_tp_sock5_client_t* proxy);

  /**
   * @brief Get fd underlying socket.
   * @param self The udp object.
   * @return fd number if success, 0 otherwise.
   */
  int (*get_socket_fd)(const rte_tp_udp_t* self);

  /**
   * @brief Send out data through udp object.
   * @param self The udp object.
   * @param ip The remote ip address.
   * @param port The remote port.
   * @param data The buffer of data.
   * @param length The size of buffer.
   * @return Size of sent data.
   */
  size_t (*send_buffer)(rte_tp_udp_t* self,
                        const char* ip,
                        uint16_t port,
                        const char* data,
                        size_t length);
};
/**
 * @brief Creator of transport udp delegate.
 * @param delegate The delegate object.
 * @param on_data The callback that will be triggered when data received.
 * @param on_error The callback that will be triggered when error happens.
 * @param arg The argument that used in |on_data| and |on_error|.
 */
typedef rte_tp_udp_t* (*rte_tp_udp_creator_t)(
  rte_facility_delegate_t* delegate,
  rte_tp_udp_on_data_callback_t on_data,
  rte_tp_udp_on_error_callback_t on_error,
  void* arg);

typedef struct rte_tp_tcp_t rte_tp_tcp_t;

/**
 * @brief The callback when tcp connect done.
 * @param self The tcp object.
 * @param succ The connect result.
 * @param arg The argument in |rte_tp_udp_creator_t|. Be careful that it is
 *            _not_ |user_data| of rte_tp_object_t.
 */
typedef void (*rte_tp_tcp_on_connect_callback_t)(rte_tp_tcp_t* self,
                                                 int succ,
                                                 void* arg);

/**
 * @brief The callback when tcp data received.
 * @param self The tcp object.
 * @param buf The buffer of received data.
 * @param size The length of buffer.
 * @param arg The argument in |rte_tp_udp_creator_t|. Be careful that it is
 *            _not_ |user_data| of rte_tp_object_t.
 */
typedef int (*rte_tp_tcp_on_data_callback_t)(rte_tp_tcp_t* self,
                                             const char* buf,
                                             size_t size,
                                             void* arg);

/**
 * @brief The callback when tcp error happens.
 * @param self The tcp object.
 * @param arg The argument in |rte_tp_udp_creator_t|. Be careful that it is
 *            _not_ |user_data| of rte_tp_object_t.
 */
typedef void (*rte_tp_tcp_on_error_callback_t)(rte_tp_tcp_t* self, void* arg);

struct rte_tp_tcp_t {
  /**
   * @brief Common object header.
   */
  rte_tp_object_t base;
  /**
   * @brief Start connect.
   * @param self The tcp object.
   * @return 1 if success, 0 otherwise.
   */
  int (*connect)(rte_tp_tcp_t* self);
  /**
   * @brief Set connect timeout time.
   * @param self The tcp object.
   * @param ms The timeout time in ms
   * @return 1 if success, 0 otherwise.
   */
  int (*set_timeout)(rte_tp_tcp_t* self, uint32_t ms);
  /**
   * @brief Check whether connected or not.
   * @param self The tcp object.
   * @return 1 if connected, 0 otherwise.
   */
  int (*is_connected)(const rte_tp_tcp_t* self);
  /**
   * @brief Send out data through tcp object.
   * @param self The tcp object.
   * @param data The buffer of data.
   * @param length The size of buffer.
   * @return Size of sent data.
   */
  size_t (*send_buffer)(rte_tp_tcp_t* self, const char* data, size_t length);
  /**
   * @brief Start disconnect.
   * @param self The tcp object.
   * @param delete_later Delete self after disconnect done.
   * @return 1 if success, 0 otherwise.
   */
  int (*disconnect)(rte_tp_tcp_t* self, int delete_later);
};

/**
 * @brief Creator of transport tcp delegate.
 * @param delegate The delegate object.
 * @param ip The remote ip.
 * @param port The remote port.
 * @param on_connect The callback that will be triggered when connect done.
 * @param on_error The callback that will be triggered when error happens.
 * @param on_data The callback that will be triggered when data received.
 * @param on_error The callback that will be triggered when error happens.
 * @param arg The argument that used in |on_connect|, |on_data| and |on_error|.
 */
typedef rte_tp_tcp_t* (*rte_tp_tcp_creator_t)(
  rte_facility_delegate_t* delegate,
  const char* ip,
  uint16_t port,
  rte_tp_tcp_on_connect_callback_t on_connect,
  rte_tp_tcp_on_data_callback_t on_data,
  rte_tp_tcp_on_error_callback_t on_error,
  void* arg);

struct rte_facility_delegate_t {
  /**
   * @brief The creator of timer delegate.
   */
  rte_tp_timer_creator_t timer_creator;
  /**
   * @brief The creator of udp delegate.
   */
  rte_tp_udp_creator_t udp_creator;
  /**
   * @brief The creator of socks5 delegate.
   */
  rte_tp_sock5_creator_t sock5_client_creator;
  /**
   * @brief The creator of tcp delegate.
   */
  rte_tp_tcp_creator_t tcp_creator;
  /**
   * @brief The delegate of async call.
   */
  rte_tp_task_t task;
  /**
   * @brief The delegate of logger.
   */
  rte_tp_log_t logger;
  /**
   * @brief The delegate of sdk information.
   */
  rte_tp_env_t sdk;
};

/** @}*/