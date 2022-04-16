/**
 *
 * Agora Real Time Engagement
 * Created by Zheng, Ender in 2021-08.
 * Copyright (c) 2021 Agora IO. All rights reserved.
 *
 */
#pragma once

#include "rte_config.h"

typedef struct rte_event_t rte_event_t;

/**
 * @brief Create an event object.
 * @param init_state The initial state of the event object.
 * @param auto_reset Whether the event object will be automatically reset to
 *                   non-signaled state after it is waked up by another thread.
 * @return The event object.
 */
AGORA_RTE_API rte_event_t* rte_event_create(int init_state, int auto_reset);

/**
 * @beied Wait for the event object to be signaled.
 * @param event The event object.
 * @param wait_ms The timeout in milliseconds.
 * @return 0 if the event object is signaled; otherwise, -1.
 */
AGORA_RTE_API int rte_event_wait(rte_event_t* event, int wait_ms);

/**
 * @brief Set the event object to signaled state.
 * @param event The event object.
 */
AGORA_RTE_API void rte_event_set(rte_event_t* event);

/**
 * @brief Reset the event object to non-signaled state.
 * @param event The event object.
 */
AGORA_RTE_API void rte_event_reset(rte_event_t* event);

/**
 * @brief Destroy the event object.
 */
AGORA_RTE_API void rte_event_destroy(rte_event_t* event);
