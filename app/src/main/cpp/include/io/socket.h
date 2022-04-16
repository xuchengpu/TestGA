/**
 *
 * Agora Real Time Engagement
 * Created by Wei Hu in 2021-08.
 * Copyright (c) 2021 Agora IO. All rights reserved.
 *
 */
#pragma once

#include "rte_config.h"

#include <stdint.h>
#if defined(_WIN32)
// clang-format off
#include <Windows.h>
#include <In6addr.h>
#include <WinSock2.h>
// clang-format on
#else
#include <netinet/in.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <sys/types.h>
#endif

#include "lib/string.h"

typedef enum RTE_SOCKET_FAMILY {
  RTE_SOCKET_FAMILY_INET = AF_INET,  // IPv4
  RTE_SOCKET_FAMILY_INET6 = AF_INET6  // IPv6
} RTE_SOCKET_FAMILY;

typedef enum RTE_SOCKET_TYPE {
  RTE_SOCKET_TYPE_STREAM = 1,  // TCP
  RTE_SOCKET_TYPE_DATAGRAM = 2,  // UDP
} RTE_SOCKET_TYPE;

typedef enum RTE_SOCKET_PROTOCOL {
  RTE_SOCKET_PROTOCOL_TCP = 6,
  RTE_SOCKET_PROTOCOL_UDP = 17,
} RTE_SOCKET_PROTOCOL;

typedef struct rte_addr_port_t {
  rte_string_t* addr;
  uint16_t port;
} rte_addr_port_t;

typedef struct rte_socket_addr_t {
  RTE_SOCKET_FAMILY family;
  union {
    struct in_addr sin_addr;
    struct in6_addr sin6_addr;
  } addr;
  uint16_t port;
} rte_socket_addr_t;

typedef struct rte_socket_t {
  RTE_SOCKET_FAMILY family;
  RTE_SOCKET_PROTOCOL protocol;
  RTE_SOCKET_TYPE type;
  int fd;
} rte_socket_t;

// Socket address
AGORA_RTE_API rte_socket_addr_t* rte_socket_addr_create(const char* address,
                                                        uint16_t port);

AGORA_RTE_API void rte_socket_addr_destroy(rte_socket_addr_t* self);

// Socket
AGORA_RTE_API rte_socket_t* rte_socket_create(RTE_SOCKET_FAMILY family,
                                              RTE_SOCKET_TYPE type,
                                              RTE_SOCKET_PROTOCOL protocol);

AGORA_RTE_API void rte_socket_destroy(rte_socket_t* self);

AGORA_RTE_API bool rte_socket_connect(rte_socket_t* socket,
                                      rte_socket_addr_t* address);

AGORA_RTE_API ssize_t rte_socket_send(const rte_socket_t* self,
                                      void* buf,
                                      size_t buf_size);

AGORA_RTE_API ssize_t rte_socket_recv(const rte_socket_t* socket,
                                      void* buf,
                                      size_t buf_size);

AGORA_RTE_API rte_addr_port_t
rte_socket_peer_addr_port(const rte_socket_t* self);

AGORA_RTE_API int rte_socket_error();
