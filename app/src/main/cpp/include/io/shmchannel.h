/**
 *
 * Agora Real Time Engagement
 * Created by Zheng, Ender in 2021-08.
 * Copyright (c) 2021 Agora IO. All rights reserved.
 *
 */
#pragma once

#include "rte_config.h"

#include "io/runloop.h"

typedef struct rte_shm_channel_t rte_shm_channel_t;

AGORA_RTE_API int rte_shm_channel_create(const char* name,
                                         rte_shm_channel_t* channel[2]);

AGORA_RTE_API void rte_shm_channel_close(rte_shm_channel_t* channel);

AGORA_RTE_API int rte_shm_channel_active(rte_shm_channel_t* channel, int read);

AGORA_RTE_API int rte_shm_channel_inactive(rte_shm_channel_t* channel,
                                           int read);

AGORA_RTE_API int rte_shm_channel_wait_remote(rte_shm_channel_t* channel,
                                              int wait_ms);

AGORA_RTE_API int rte_shm_channel_send(rte_shm_channel_t* channel,
                                       void* data,
                                       size_t size,
                                       int nonblock);

AGORA_RTE_API int rte_shm_channel_recv(rte_shm_channel_t* channel,
                                       void* data,
                                       size_t size,
                                       int nonblock);

AGORA_RTE_API int rte_shm_channel_get_capacity(rte_shm_channel_t* channel);

AGORA_RTE_API int rte_shm_channel_set_signal(rte_shm_channel_t* channel,
                                             rte_runloop_async_t* signal,
                                             int read);
