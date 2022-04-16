/**
 *
 * Agora Real Time Engagement
 * Created by Wei Hu in 2021-08.
 * Copyright (c) 2021 Agora IO. All rights reserved.
 *
 */
#pragma once

#include "rte_config.h"

#include "io/runloop.h"
#include "lib/atomic.h"
#include "lib/string.h"

#define RTE_ASYNC_SIGNATURE 0xD4CD6DEDB7906C26U

typedef struct rte_async_t {
  rte_atomic_t signature;
  rte_string_t name;

  rte_runloop_t* loop;
  rte_runloop_async_t* async;
  rte_runloop_async_t* async_for_close;

  void (*async_cb)(struct rte_async_t*, void*);
  void* async_cb_data;

  rte_atomic_t close;
  void (*close_cb)(struct rte_async_t*, void*);
  void* close_cb_data;
} rte_async_t;

AGORA_RTE_API rte_async_t* rte_async_create(const char* name,
                                            rte_runloop_t* loop,
                                            void* async_cb,
                                            void* data);

AGORA_RTE_API void rte_async_set_on_close(rte_async_t* self,
                                          void* close_cb,
                                          void* data);

AGORA_RTE_API void rte_async_trigger(rte_async_t* self);

AGORA_RTE_API void rte_async_close(rte_async_t* self);
