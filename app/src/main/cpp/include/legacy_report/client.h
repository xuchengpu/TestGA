/**
 *
 * Agora Real Time Engagement
 * Created by Zheng, Ender in 2021-08.
 * Copyright (c) 2021 Agora IO. All rights reserved.
 *
 */
#pragma once
#include "rte_config.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "argus/client.h"

/** \addtogroup legacy_reporter
 *  @{
 */

typedef struct rte_legacy_reporter_t rte_legacy_reporter_t;

/**
 * @brief The counter calculate method
 */
typedef enum rte_legacy_counter_calculator_t {
  /**
   * @brief Sum up all value in history
   */
  RTE_LEGACY_COUNTER_AS_SUM,
  /**
   * @brief Get minimal value in history
   */
  RTE_LEGACY_COUNTER_AS_MIN,
  /**
   * @brief Get maximal value in history
   */
  RTE_LEGACY_COUNTER_AS_MAX,
  /**
   * @brief Get average value in history
   */
  RTE_LEGACY_COUNTER_AS_AVERAGE,
  /**
   * @brief Get median value in history
   */
  RTE_LEGACY_COUNTER_AS_MEDIAN,
  /**
   * @brief Get earliest one in history
   */
  RTE_LEGACY_COUNTER_AS_EARLIEST,
  /**
   * @brief Get latest one in history
   */
  RTE_LEGACY_COUNTER_AS_LATEST,
  /**
   * @brief Invalid value, will ignore this counter
   */
  RTE_LEGACY_COUNTER_AS_UNKNOWN,
} rte_legacy_counter_calculator_t;

/**
 * @brief The custom rules for counter report
 */
typedef struct rte_legacy_reporter_counter_rule_t {
  /**
   * @brief The inverval in milli-second of a counter to report
   */
  uint32_t interval;
  /**
   * @brief Report only if the calculated value changed from last sent one.
   */
  bool send_only_changed;
  /**
   * @brief No limitation at all for this counter
   */
  bool ignore_limitation;
  /**
   * @brief Don't report this counter.
   */
  bool disabled;
} rte_legacy_reporter_counter_rule_t;

/**
 * @brief The custom rules for event report
 */
typedef struct rte_legacy_reporter_event_rule_t {
  /**
   * @brief The inverval in milli-second of an event to report
   */
  uint32_t interval;
  /**
   * @brief No limitation at all for this event
   */
  bool ignore_limitation;
  /**
   * @brief Don't report this event.
   */
  bool disabled;
} rte_legacy_reporter_event_rule_t;

static const rte_legacy_counter_calculator_t kLegacyReporterDefaultCalculator =
  RTE_LEGACY_COUNTER_AS_AVERAGE;

static const uint32_t kLegacyReporterDefaultInterval = 6000;  // 6 seconds

/**
 * @brief Create instance of argus reporter.
 *
 * @param argus The argus link.
 * @param calculator The default calculator of counters.
 * @param poll_interval The calculate & report interval in mill-second.
 * @param sid The session ID.
 * @return Instance of argus reporter if success, NULL otherwise.
 */
AGORA_RTE_API rte_legacy_reporter_t* rte_legacy_reporter_create(
  rte_argus_t* argus,
  rte_legacy_counter_calculator_t calculator,
  uint32_t poll_interval,
  const char* sid);

/**
 * @brief Destroy an argus reporter.
 *
 * @param reporter The argus reporter.
 */
AGORA_RTE_API void rte_legacy_reporter_destroy(rte_legacy_reporter_t* reporter);

/**
 * @brief Append en entry to counter history.
 *
 * @param reporter The argus reporter.
 * @param counter_id The id of counter.
 * @param counter_value The value of counter.
 * @param streamid The stream id of counter, 0 if counter is not for stream.
 * @return 0 if success, -1 otherwise.
 */
AGORA_RTE_API int rte_legacy_reporter_write_counter(
  rte_legacy_reporter_t* reporter,
  uint32_t counter_id,
  uint32_t counter_value,
  uint32_t streamid);

/**
 * @brief Send out an event.
 *
 * @param reporter The argus reporter.
 * @param event_id The id of event.
 * @param level The qos level of event.
 * @param buf The data buffer of event.
 * @param size The size of data.
 * @return 0 if success, -1 otherwise.
 * @note |buf| MUST BE THE SAME FORMAT AS in |protobuf_c_message_pack|
 *       because we will call protobuf_c_message_pack(buf, ...) directly without
 *       touching anything inside |buf|.
 * @note event will be send out immediatly if event rules allows.
 */
AGORA_RTE_API int rte_legacy_reporter_write_event(
  rte_legacy_reporter_t* reporter,
  uint32_t event_id,
  rte_argus_qos_level_t level,
  const void* buf,
  size_t size);

/**
 * @brief Set uid of current reporter.
 *
 * @param reporter The argus reporter.
 * @param uid The uid.
 * @return 0 if success, -1 otherwise.
 * @note All counters will take |uid| as one of it's field after this function
 *       being called. You can call this anytime you want your counter bind with
 *       an uid.
 */
AGORA_RTE_API int rte_legacy_reporter_set_uid(rte_legacy_reporter_t* reporter,
                                              uint32_t uid);

/**
 * @brief Set cid of current reporter.
 *
 * @param reporter The argus reporter.
 * @param cid The cid.
 * @return 0 if success, -1 otherwise.
 * @note All counters will take |cid| as one of it's field after this function
 *       being called. You can call this anytime you want your counter bind with
 *       an cid.
 */
AGORA_RTE_API int rte_legacy_reporter_set_cid(rte_legacy_reporter_t* reporter,
                                              uint32_t cid);

/**
 * @brief Set calculate method of a counter.
 *
 * @param reporter The argus reporter.
 * @param counter_id The id of counter.
 * @param calculator The calculate method.
 * @return 0 if success, -1 otherwise.
 */
AGORA_RTE_API int rte_legacy_reporter_set_counter_calculator(
  rte_legacy_reporter_t* reporter,
  uint32_t counter_id,
  rte_legacy_counter_calculator_t calculator);

/**
 * @brief Set qos level of a counter.
 *
 * @param reporter The argus reporter.
 * @param counter_id The id of counter.
 * @param level The qos level.
 * @return 0 if success, -1 otherwise.
 */
AGORA_RTE_API int rte_legacy_reporter_set_counter_qos_level(
  rte_legacy_reporter_t* reporter,
  uint32_t counter_id,
  rte_argus_qos_level_t level);

/**
 * @brief Set custom rules for a counter.
 *
 * @param reporter The argus reporter.
 * @param counter_id The id of counter.
 * @param rule The custom rule of counter.
 * @return 0 if success, -1 otherwise.
 * @note One counter May still dropped by default rule even custom rule allowd.
 *   The default rule is applied with following order:
 *   - QOS level is critical: allow
 *   - The first time one counter is ready to report: allow
 *   - External counters: allow
 *   - QOS level <= HIGH and value not changed: disallow
 *   - QOS level <= HIGH and value changed: allow
 *   - QOS level > HIGH: allow
 *   - Other: disallow
 */
AGORA_RTE_API int rte_legacy_reporter_set_custom_counter_rule(
  rte_legacy_reporter_t* reporter,
  uint32_t counter_id,
  const rte_legacy_reporter_counter_rule_t* rule);

/**
 * @brief Set custom rules for an event.
 *
 * @param reporter The argus reporter.
 * @param event_id The id of event.
 * @param rule The custom rule of event.
 * @return 0 if success, -1 otherwise.
 * @note There's no default rule for event.
 */
AGORA_RTE_API int rte_legacy_reporter_set_custom_event_rule(
  rte_legacy_reporter_t* reporter,
  uint32_t event_id,
  const rte_legacy_reporter_event_rule_t* rule);

/** @}*/