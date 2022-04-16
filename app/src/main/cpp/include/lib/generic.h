/**
 *
 * Agora Real Time Engagement
 * Created by Zheng, Ender in 2021-08.
 * Copyright (c) 2021 Agora IO. All rights reserved.
 *
 */
#pragma once
#include "rte_config.h"

#include "lib/list.h"
#include "lib/rbtree.h"

typedef enum RTE_GENERIC_TYPE {
  RTE_TYPE_PRIMITIVE_FIRST,
  RTE_TYPE_PRIMITIVE_I8 = RTE_TYPE_PRIMITIVE_FIRST,
  RTE_TYPE_PRIMITIVE_U8,
  RTE_TYPE_PRIMITIVE_I16,
  RTE_TYPE_PRIMITIVE_U16,
  RTE_TYPE_PRIMITIVE_I32,
  RTE_TYPE_PRIMITIVE_U32,
  RTE_TYPE_PRIMITIVE_I64,
  RTE_TYPE_PRIMITIVE_U64,
  RTE_TYPE_PRIMITIVE_POINTER,
  RTE_TYPE_PRIMITIVE_FLOAT,
  RTE_TYPE_PRIMITIVE_DOUBLE,
  RTE_TYPE_PRIMITIVE_BOOL,
  RTE_TYPE_PRIMITIVE_LAST = RTE_TYPE_PRIMITIVE_BOOL,
  RTE_TYPE_GENERIC_FIRST,
  RTE_TYPE_GENERIC_LIST = RTE_TYPE_GENERIC_FIRST,
  RTE_TYPE_GENERIC_MAP,
  RTE_TYPE_GENERIC_BUFFER,
  RTE_TYPE_GENERIC_ARRAY,
  RTE_TYPE_GENERIC_TUPLE,
  RTE_TYPE_GENERIC_STRING,
  RTE_TYPE_GENERIC_LAST = RTE_TYPE_GENERIC_STRING,
  RTE_TYPE_STRUCT,
  RTE_TYPE_MAX,
} RTE_GENERIC_TYPE;

typedef struct rte_runtime_type_t {
  RTE_GENERIC_TYPE type;
  size_t subtype_count;
  size_t array_size;
  struct rte_runtime_type_t** subtypes;
} rte_runtime_type_t;

#define RTE_TYPE_IS_VALID(type)                    \
  ((int)(type) >= (int)RTE_TYPE_PRIMITIVE_FIRST && \
   (int)(type) < (int)RTE_TYPE_MAX)

#define RTE_TYPE_IS_PRIMITIVE(type) \
  ((type) >= RTE_TYPE_PRIMITIVE_FIRST && (type) <= RTE_TYPE_PRIMITIVE_LAST)

#define RTE_TYPE_IS_GENERIC(type) \
  ((type) >= RTE_TYPE_GENERIC_FIRST && (type) <= RTE_TYPE_GENERIC_LAST)

#define RTE_TYPE_IS_STRUCT(type) ((type) == RTE_TYPE_STRUCT)

AGORA_RTE_API rte_runtime_type_t* rte_runtime_type_create(
  RTE_GENERIC_TYPE primary,
  ...);

AGORA_RTE_API void rte_runtime_type_destroy(rte_runtime_type_t* t);

AGORA_RTE_API rte_runtime_type_t* rte_runtime_type_clone(
  const rte_runtime_type_t* t);

AGORA_RTE_API size_t rte_runtime_type_get_primitive_size(RTE_GENERIC_TYPE type);

AGORA_RTE_API size_t rte_runtime_type_get_generic_size(RTE_GENERIC_TYPE type);

AGORA_RTE_API void rte_runtime_type_primitive_write(RTE_GENERIC_TYPE type,
                                                    uintptr_t val,
                                                    const uint8_t* buf);

AGORA_RTE_API uintptr_t rte_runtime_type_primitive_read(RTE_GENERIC_TYPE type,
                                                        const uint8_t* buf);

// clang-format off
/**
 * C macro does not support recursive definition
 * We have to use different macro "name" in nested macro parameter,
 * that's why there're RTE_TYPE_DEF_ macro
 * Usage:
 *
 * - primitive types:
 *    Example: RTE_TYPE_DEF(RTE_TYPE_PRIMITIVE_XXX)
 *
 * - list: represent a rte_list_t struct.
 *    Example: RTE_TYPE_DEF(RTE_TYPE_GENERIC_LIST, RTE_TYPE_DEF(xxx, ...))
 *    Note:    Second param is the type of list item, it can be primitive or generic
 *
 * - map: represent a rte_rbtree_t struct
 *    Example: RTE_TYPE_DEF(RTE_TYPE_GENERIC_MAP, RTE_TYPE_DEF(xxx, ...), RTE_TYPE_DEF(xxx, ...))
 *    Note:    Second param is the type of key, third param is the type of value, both of them
 *             can be primitive or generic.
 *
 * - buffer: represent a rte_buf_t struct.
 *     Example: RTE_TYPE_DEF(RTE_TYPE_GENERIC_BUFFER)
 *     Note:    No additional types needed because we assume it is array of uint8_t.
 *
 * - array: represent a rte_array_t struct.
 *     Example: RTE_TYPE_DEF(RTE_TYPE_GENERIC_ARRAY, n, RTE_TYPE_DEF(xxx, ...))
 *     Note:    n is the size of array (item count, not memory size), third param is the type of item,
 *     Note:    Item type _must_ be primitive type, otherwise crash happens.
 *
 * - tuple: represent a rte_tuple_t struct.
 *     Example: RTE_TYPE_DEF(RTE_TYPE_GENERIC_TUPLE, n, RTE_TYPE_DEF(xxx, ...), RTE_TYPE_DEF(xxx, ...), ...)
 *     Note:    n is the field count, third to last param are types for each field.
 *     Note:    If field type is generic, it _must_ be a pointer allocated in heap, for example
 *              ```c
 *                 RTE_TYPE_DEF(RTE_TYPE_GENERIC_TUPLE, 1, RTE_TYPE_DEF(RTE_TYPE_GENERIC_BUFFER))
 *              ```
 *              is a type similar with std::tuple<rte_buffer_t*>, _not_ std::tuple<rte_buffer_t>
 *
 * - string: represent a rte_string_t struct.
 *     Example: RTE_TYPE_DEF(RTE_TYPE_GENERIC_STRING)
 *     Note:    No additional param needed because we assume it is array of char with \0 ends.
 */
#define RTE_TYPE_DEF(...) RTE_TYPE_DEF_(__VA_ARGS__)

#define RTE_TYPE_DEF_(primary, ...) \
  rte_runtime_type_create((primary), ##__VA_ARGS__)

// clang-format on
