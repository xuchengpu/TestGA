/**
 *
 * Agora Real Time Engagement
 * Created by Zheng, Ender in 2021-09.
 * Copyright (c) 2021 Agora IO. All rights reserved.
 *
 */
#pragma once

#include "rte_config.h"

#include <stddef.h>

#define RTE_RUNLOOP_EVENT2 "event2"
#define RTE_RUNLOOP_UV "uv"
#define RTE_RUNLOOP_AGORA "agora"

typedef struct rte_runloop_t rte_runloop_t;

typedef struct rte_runloop_async_t rte_runloop_async_t;

typedef struct rte_runloop_timer_t rte_runloop_timer_t;

/**
 * @brief Create a runloop.
 * @param type The implementation of the runloop.
 *             Create from a default one if |type| == NULL
 * @return The runloop. NULL if failed.
 */
AGORA_RTE_API rte_runloop_t* rte_runloop_create(const char* type);

/**
 * @brief Attach to an existing runloop with "raw loop pointer"
 *
 * @param type The implementation of the runloop. Must _not_ be "NULL".
 * @param raw The "raw loop pointer", for example ev_base* if type == "event2"
 * @param on_attached The "callback" invoked when the loop is attached successfully
 * @param user_data The "customized data" as the 2nd argument when "on_attached" is invoked
 * @return The runloop.
 * @note Be careful that |raw| must has currect |type| otherwise crash happens.
 */
AGORA_RTE_API rte_runloop_t* rte_runloop_attach(const char* type,
                                                void* raw,
                                                void(*on_attached)(rte_runloop_t*, void*),
                                                void* user_data);

/**
 * @brief Detach a runloop that is previous attached
 *
 * @param loop The runloop to detach
 * @param on_detached The "callback" invoked when the loop is detached successfully
 * @param user_data The "customized data" as the argument when "on_detached" is invoked
 * @note This method only stops the underlying task queue. Timers and
 *       events created from |loop| may still be triggered unless you close them
 *       all before detaching.
 */
AGORA_RTE_API void rte_runloop_detach(rte_runloop_t* loop,
                                      void(*on_detached)(void*),
                                      void* user_data);

/**
 * @brief Destroy a runloop.
 * @param loop The runloop to destroy.
 * @note For loop from |rte_runloop_attach|, only call this method within "on_detached" callback
 */
AGORA_RTE_API void rte_runloop_destroy(rte_runloop_t* loop);

/**
 * @brief Get backend type of a runloop
 *
 * @param loop The runloop
 * @return The name of backend type, nullptr if fail
 */
AGORA_RTE_API const char* rte_runloop_get_type(rte_runloop_t* loop);

/**
 * @brief Get current loop.
 * @return The current loop. NULL if no loop running.
 */
AGORA_RTE_API rte_runloop_t* rte_runloop_current();

/**
 * @brief Get underlying loop object.
 * @param loop The runloop.
 * @return The underlying loop object. NULL if failed.
 *         For example, libevent loop will return event_base*
 */
AGORA_RTE_API void* rte_runloop_get_raw(rte_runloop_t* loop);

/**
 * @brief Run the loop.
 * @param loop The runloop.
 * @note This function will block until the loop is stopped.
 *       If loop is coming from |rte_runloop_attach|, nothing would happen.
 */
AGORA_RTE_API void rte_runloop_run(rte_runloop_t* loop);

/**
 * @brief Stop the loop.
 * @param loop The runloop.
 * @note Stop a loop from |rte_runloop_create| will stop task queue as well as
 *       the underlying io loop. This method does not work for loop from 
 *       |rte_runloop_attach|. Invoke |rte_runloop_detach| instead.
 */
AGORA_RTE_API void rte_runloop_stop(rte_runloop_t* loop);

/**
 * @brief Check whether the loop is running.
 * @param loop The runloop.
 * @return 1 if the loop is running, 0 otherwise.
 */
AGORA_RTE_API int rte_runloop_alive(rte_runloop_t* loop);

/**
 * @brief Create an async signal
 * @param type The implementation of signal.
 *             Create from a default one if |type| == NULL
 * @return The signal. NULL if failed.
 */
AGORA_RTE_API rte_runloop_async_t* rte_runloop_async_create(const char* type);

/**
 * @brief Close an async signal.
 * @param async The signal.
 * @param close_cb The callback to be called when the signal is closed.
 */
AGORA_RTE_API void rte_runloop_async_close(
  rte_runloop_async_t* async,
  void (*close_cb)(rte_runloop_async_t*));

/**
 * @brief Destroy an async signal.
 * @param async The signal.
 */
AGORA_RTE_API void rte_runloop_async_destroy(rte_runloop_async_t* async);

/**
 * @brief Notify the signal.
 * @param async The signal.
 * @return 0 if success, -1 otherwise.
 * @note The signal callback will be called in the thread of |loop| that inited
 */
AGORA_RTE_API int rte_runloop_async_notify(rte_runloop_async_t* async);

/**
 * @brief Bind an async signal to a runloop.
 * @param async The signal.
 * @param loop The runloop.
 * @param callback The callback to be called when the signal is notified.
 * @return 0 if success, -1 otherwise.
 * @note The implementation of |loop| should be the same as the implementation
 *       of |async|, otherwise the behavior is undefined.
 */
AGORA_RTE_API int rte_runloop_async_init(
  rte_runloop_async_t* async,
  rte_runloop_t* loop,
  void (*callback)(rte_runloop_async_t*));

/**
 * @brief Create an async task and insert it in front.
 * @param loop The runloop.
 * @param task_cb The callback to be called when the task is executed.
 * @param from The pointer of poster.
 * @param arg The argument to be passed to the callback.
 * @return 0 if success, -1 otherwise.
 */
AGORA_RTE_API int rte_runloop_post_task_front(rte_runloop_t* loop,
                                              void (*task_cb)(void*, void*),
                                              void* from,
                                              void* arg);

/**
 * @brief Create an async task and insert it in back.
 * @param loop The runloop.
 * @param task_cb The callback to be called when the task is executed.
 * @param from The pointer of poster.
 * @param arg The argument to be passed to the callback.
 * @return 0 if success, -1 otherwise.
 */
AGORA_RTE_API int rte_runloop_post_task_tail(rte_runloop_t* loop,
                                             void (*task_cb)(void*, void*),
                                             void* from,
                                             void* arg);

/**
 * @brief Get pending task size
 * @param loop The runloop.
 * @return The pending task size.
 */
AGORA_RTE_API size_t rte_runloop_task_queue_size(rte_runloop_t* loop);

/**
 * @brief Create a timer in of a runloop
 * @param type The implementation of timer.
 *             Create from a default one if |type| == NULL
 * @param timeout Timeout time in ms.
 * @param periodic Whether the timer is notified periodicly
 * @return The timer. NULL if failed.
 */
AGORA_RTE_API rte_runloop_timer_t* rte_runloop_timer_create(const char* type,
                                                            int timeout,
                                                            int periodic);

/**
 * @brief Set timeout and callback style
 * @param timer The timer.
 * @param timeout Timeout time in ms.
 * @param periodic Whether the timer is notified periodicly.
 * @return 0 if success, -1 if the timer not valid.
 * @note Will not take effert immediately if already started.
 */
AGORA_RTE_API int rte_runloop_timer_set_timeout(rte_runloop_timer_t* timer,
                                                int timeout,
                                                int periodic);

/**
 * @brief Bind an timer to a runloop and start.
 * @param timer The timer.
 * @param loop The runloop.
 * @param callback The callback to be called when timer is notified.
 * @return 0 if success, -1 otherwise.
 * @note The implementation of |loop| should be the same as the implementation
 *       of |timer|, otherwise the behavior is undefined.
 */
AGORA_RTE_API int rte_runloop_timer_start(rte_runloop_timer_t* timer,
                                          rte_runloop_t* loop,
                                          void (*callback)(rte_runloop_timer_t*,
                                                           void*),
                                          void* arg);

/**
 * @brief Stop an timer.
 * @param timer The timer.
 * @param stop_cb The callback to be called when the timer is stopped.
 */
AGORA_RTE_API void rte_runloop_timer_stop(rte_runloop_timer_t* timer,
                                          void (*stop_cb)(rte_runloop_timer_t*,
                                                          void*),
                                          void* arg);

/**
 * @brief Stop an timer.
 * @param timer The timer.
 * @param close_cb The callback to be called when the timer is stopped.
 */
AGORA_RTE_API void rte_runloop_timer_close(
  rte_runloop_timer_t* timer,
  void (*close_cb)(rte_runloop_timer_t*, void*),
  void* arg);

/**
 * @brief Destroy an timer.
 * @param timer The timer.
 */
AGORA_RTE_API void rte_runloop_timer_destroy(rte_runloop_timer_t* timer);
