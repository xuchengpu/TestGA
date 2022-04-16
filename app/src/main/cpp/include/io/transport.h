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

#include "io/runloop.h"
#include "lib/atomic.h"
#include "lib/mutex.h"

typedef struct rte_transportbackend_t rte_transportbackend_t;
typedef struct rte_string_t rte_string_t;
typedef struct rte_stream_t rte_stream_t;

typedef enum RTE_TRANSPORT_DROP_TYPE {
  /* Drop oldest data when transport channel is full, only available when
     transport type is shm or raw pointer */
  RTE_TRANSPORT_DROP_OLD,

  /* Drop current data when transport channel is full */
  RTE_TRANSPORT_DROP_NEW,

  /* Drop current data if no reader is waiting */
  RTE_TRANSPORT_DROP_IF_NO_READER,

  /* Drop current data by asking, only available when
     transport type is shm or raw pointer .
     Useful if user wan't to drop by bussiness logic (e.g. never drop I frame)
   */
  RTE_TRANSPORT_DROP_ASK
} RTE_TRANSPORT_DROP_TYPE;

typedef struct rte_transport_t rte_transport_t;

struct rte_transport_t {
  /**
   * uv loop that attached to current thread
   */
  rte_runloop_t* loop;

  void* user_data;

  rte_transportbackend_t* backend;
  rte_atomic_t close;

  rte_mutex_t* lock;
  int drop_when_full;
  RTE_TRANSPORT_DROP_TYPE drop_type;

  /**
   * Callback when a new stream is created and trying to connect
   */
  void (*on_server_connecting)(rte_transport_t* transport,
                               rte_stream_t* stream);

  /**
   * Callback when a new rx stream is connected
   */
  void (*on_server_connected)(rte_transport_t* transport,
                              rte_stream_t* stream,
                              int status);

  /**
   * Callback when a new rx stream is created
   */
  void (*on_client_accepted)(rte_transport_t* transport,
                             rte_stream_t* stream,
                             int status);

  /**
   * Callback when transport closed
   */
  void (*on_close)(void* on_close_data);
  void* on_close_data;
};

// Public interface
AGORA_RTE_API rte_transport_t* rte_transport_create(rte_runloop_t* loop);

AGORA_RTE_API int rte_transport_close(rte_transport_t* self);

AGORA_RTE_API void rte_transport_set_close_cb(rte_transport_t* self,
                                              void* close_cb,
                                              void* close_cb_data);

AGORA_RTE_API int rte_transport_listen(rte_transport_t* self,
                                       const rte_string_t* my_uri);

AGORA_RTE_API int rte_transport_connect(rte_transport_t* self,
                                        rte_string_t* dest);

AGORA_RTE_API int rte_transport_connect_with_timeout(rte_transport_t* self,
                                                     rte_string_t* dest,
                                                     int timeout);
