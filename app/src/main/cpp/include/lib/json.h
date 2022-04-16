/**
 *
 * Agora Real Time Engagement
 * Created by Wei Hu in 2021-07.
 * Copyright (c) 2022 Agora IO. All rights reserved.
 *
 */
#pragma once

#include "rte_config.h"

#include <stdbool.h>
#include <stdint.h>

typedef struct json_t json_t;

AGORA_RTE_API bool rte_json_check_integrity(json_t* json);

/**
 * @brief delete the item from json object specified by key.
 * @param json json object
 * @param field key name
 * @return true if key exists and delete successful, false otherwise
 */
AGORA_RTE_API bool rte_json_del_key(json_t* json, const char* field);

/**
 * @brief get string value from json object
 * @param json json object
 * @param field key
 * @return value if exists, NULL otherwise
 */
AGORA_RTE_API const char* rte_json_get_string(json_t* json, const char* field);

/**
 * @brief get int value from json object
 * @param json json object
 * @param field key
 * @return value if exists, -1 otherwise
 */
AGORA_RTE_API int64_t rte_json_get_integer(json_t* json, const char* field);

/**
 * @brief get floating-point value from json object
 * @param json json object
 * @param field key
 * @return value if exists, 0 otherwise
 */
AGORA_RTE_API double rte_json_get_real(json_t* json, const char* field);

/**
 * @brief get boolean value from json object
 * @param json json object
 * @param field key
 * @return value if exists, false otherwise
 */
AGORA_RTE_API bool rte_json_get_boolean(json_t* json, const char* field);

/**
 * @brief check if json object contains a field
 * @param json json object
 * @param field key
 * @return true if exists, false otherwise
 */
AGORA_RTE_API bool rte_json_is_key_exist(json_t* json, const char* field);

/**
 * @brief Get value of field from json object in string format, if the type
 *        of field is not string, dumps the value in string format.
 * @param json The json object.
 * @param field The field name.
 * @param must_free have to free after use.
 * @return The json string of field value if exists, or NULL.
 */
AGORA_RTE_API const char* rte_json_to_string(json_t* json,
                                             const char* field,
                                             bool* must_free);

/**
 * @brief Get value of field from json object in string format, if the type
 *        of field is not string, dumps the value in string format.
 * @param json The json object.
 * @param field The field name.
 * @param must_free have to free after use.
 * @return The json string of field value if exists, or NULL.
 */
AGORA_RTE_API json_t* rte_json_from_string(const char* msg);

/**
 * @brief Destroy a json object
 *
 * @param json The json object
 */
AGORA_RTE_API void rte_json_destroy(json_t* json);
