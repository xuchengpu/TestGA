/**
 *
 * Agora Real Time Engagement
 * Created by Wei Hu in 2021-07.
 * Copyright (c) 2021 Agora IO. All rights reserved.
 *
 */
#pragma once
#include "rte_config.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>

#include "io/runloop.h"
#include "lib/atomic.h"

#define RTE_STREAM_SIGNATURE 0xDE552052E7F8EE10U
#define RTE_STREAM_DEFAULT_BUF_SIZE (64 * 1024)

typedef struct rte_stream_t rte_stream_t;
typedef struct rte_transport_t rte_transport_t;
typedef struct rte_streambackend_t rte_streambackend_t;

struct rte_stream_t {
  rte_atomic_t signature;
  rte_atomic_t close;

  rte_transport_t* transport;
  rte_streambackend_t* backend;

  void* user_data;

  void (*on_message_read)(rte_stream_t* stream, void* msg, int size);
  void (*on_message_sent)(rte_stream_t* stream, int status, void* args);
  void (*on_message_free)(rte_stream_t* stream, int status, void* args);

  void (*on_close)(void* on_close_data);
  void* on_close_data;
};

// Public interface
/**
 * @brief Begin read from stream.
 * @param self The stream to read from.
 * @return 0 if success, otherwise -1.
 */
AGORA_RTE_API int rte_stream_start_read(rte_stream_t* self);

/**
 * @brief Stop read from stream.
 * @param self The stream to read from.
 * @return 0 if success, otherwise -1.
 */
AGORA_RTE_API int rte_stream_stop_read(rte_stream_t* self);

/**
 * @brief Send a message to stream.
 * @param self The stream to send to.
 * @param msg The message to send.
 * @param size The size of message.
 * @return 0 if success, otherwise -1.
 */
AGORA_RTE_API int rte_stream_send(rte_stream_t* self,
                                  const char* msg,
                                  uint32_t size,
                                  void* args);

/**
 * @brief Close the stream.
 * @param self The stream to close.
 */
AGORA_RTE_API void rte_stream_close(rte_stream_t* self);

/**
 * @brief Set close callback for stream.
 * @param self The stream to set close callback for.
 * @param close_cb The callback to set.
 * @param close_cb_data The args of |close_cb| when it's been called
 */
AGORA_RTE_API void rte_stream_set_close_cb(rte_stream_t* self,
                                           void* close_cb,
                                           void* close_cb_data);

/**
 * @brief Migrate a stream from one runloop to another
 * @param self The stream to migrate
 * @param from The runloop to migrate from
 * @param to The runloop to migrate to
 * @param user_data The user data to be passed to the callback
 * @param cb The callback to be called when the migration is done
 * @return 0 on success, -1 on failure
 * @note 1. |cb| will be called in |to| loop thread if success
 *       2. |from| and |to| have to be the same implementation type
 *          (e.g.event2, uv, etc.)
 *       3. |self| will be removed from |from| loop and no more data
 *          will be read from it
 */
AGORA_RTE_API int rte_stream_migrate(rte_stream_t* self,
                                     rte_runloop_t* from,
                                     rte_runloop_t* to,
                                     void** user_data,
                                     void (*cb)(rte_stream_t* new_stream,
                                                void** user_data));

AGORA_RTE_API bool rte_stream_check_integrity(rte_stream_t* self);

AGORA_RTE_API void rte_stream_init(rte_stream_t* self);

AGORA_RTE_API void rte_stream_on_close(rte_stream_t* self);