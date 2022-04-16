/**
 *
 * Agora Real Time Engagement
 * Created by Zheng, Ender in 2021-08.
 * Copyright (c) 2021 Agora IO. All rights reserved.
 *
 */
#pragma once

#include "rte_config.h"

#include <stdint.h>

#include "lib/alloc.h"
#include "lib/generic.h"
#include "macro/field.h"

typedef enum RTE_PACKER_ENDIAN {
  RTE_PACKER_BIG_ENDIAN,
  RTE_PACKER_LITTLE_ENDIAN
} RTE_PACKER_ENDIAN;

typedef enum RTE_PACKED_SIZE_WIDTH {
  RTE_PACKED_SIZE_U16 = RTE_TYPE_PRIMITIVE_U16,
  RTE_PACKED_SIZE_U32 = RTE_TYPE_PRIMITIVE_U32,
} RTE_PACKED_SIZE_WIDTH;

typedef struct rte_packed_data_t rte_packed_data_t;

typedef struct rte_packer_meta_t {
  rte_runtime_type_t* schema;
  void* address;
} rte_packer_meta_t;

typedef struct rte_unpacked_data_t {
  rte_packer_meta_t* meta;
  size_t field_count;
} rte_unpacked_data_t;

typedef struct rte_stream_unpacker_t rte_stream_unpacker_t;

/**
 * @brief Pack a struct into plain raw data.
 *
 * @param input The unpacked data, have to be packable. See
 *              |RTE_PACKER_DECLARE_STRUCT| for more details
 * @param output The packed data, have to be prepared by
 *               |rte_packed_data_create_empty|
 * @param endian The endianess of packed raw data.
 * @return 0 if success, -1 otherwise
 */
AGORA_RTE_API int rte_packer_pack(const rte_unpacked_data_t* input,
                                  rte_packed_data_t* output,
                                  RTE_PACKER_ENDIAN endian);

/**
 * @brief Unpack plain raw data into packable struct.
 *
 * @param input The packed data, have to be prepared by
 *              |rte_packed_data_create|
 * @param output The unpacked data, have to be packable. See
 *               |RTE_PACKER_DECLARE_STRUCT| for more details
 * @param endian The endianess of packed raw data.
 * @return 0 if success, -1 otherwise
 */
AGORA_RTE_API int rte_packer_unpack(rte_packed_data_t* input,
                                    rte_unpacked_data_t* output,
                                    RTE_PACKER_ENDIAN endian);

/**
 * @brief Create a stream unpacker. A stream unpacker can take received data
 *        streamly.
 *
 * @param endian The endianess of packed raw data.
 * @param size_width The byte width of the maximum packed data length
 * @return The unpacker
 */
AGORA_RTE_API rte_stream_unpacker_t* rte_stream_unpacker_create(
  RTE_PACKER_ENDIAN endian,
  RTE_PACKED_SIZE_WIDTH size_width);

/**
 * @brief Destroy a stream unpacker.
 *
 * @param unpacker The unpacker
 */
AGORA_RTE_API void rte_stream_unpacker_destroy(rte_stream_unpacker_t* unpacker);

/**
 * @brief Append data into unpacker.
 *
 * @param unpacker The unpacker.
 * @param data The data buffer.
 * @param size The size of buffer.
 * @return 0 if success, -1 otherwise
 */
AGORA_RTE_API int rte_stream_unpacker_append(rte_stream_unpacker_t* unpacker,
                                             const void* data,
                                             size_t size);

/**
 * @brief Check whether there's at least one packet received completly.
 *
 * @param unpacker The unpacker
 * @return 1 if have at least one complete packet, 0 otherwise.
 */
AGORA_RTE_API int rte_stream_unpacker_can_finalize(
  const rte_stream_unpacker_t* unpacker);

/**
 * @brief Finalize (unpack) a complete received packet.
 *
 * @param unpacker The unpacker.
 * @param output The unpacked data, have to be packable. See
 *               |RTE_PACKER_DECLARE_STRUCT| for more details
 * @return 0 if success, -1 otherwise
 */
AGORA_RTE_API int rte_stream_unpacker_finalize(rte_stream_unpacker_t* unpacker,
                                               rte_unpacked_data_t* output);

/**
 * @brief Create a packed data from buffer.
 *
 * @param buf The buffer.
 * @param size The size of buffer.
 * @return Pointer of |rte_packed_data_t| if success, NULL otherwise
 */
AGORA_RTE_API rte_packed_data_t* rte_packed_data_create(
  const uint8_t* buf,
  size_t size,
  RTE_PACKED_SIZE_WIDTH size_width);

/**
 * @brief Create an empty packed data.
 *
 * @param size_width The byte width of the maximum packed data length
 * @return Pointer of |rte_packed_data_t| if success, NULL otherwise
 */
AGORA_RTE_API rte_packed_data_t* rte_packed_data_create_empty(
  RTE_PACKED_SIZE_WIDTH size_width);

/**
 * @brief Append raw buffer to a packed data.
 *
 * @param data The data
 * @param buf The buffer.
 * @param len The length of buffer.
 * @param data_has_endian Whether buffer should be endian-converted.
 * @param endian The endianess of packed raw data.
 * @return 0 if success, -1 otherwise
 */
AGORA_RTE_API int rte_packed_data_append(rte_packed_data_t* data,
                                         const void* buf,
                                         size_t len,
                                         bool data_has_endian,
                                         RTE_PACKER_ENDIAN endian);

/**
 * @brief Destroy a packed data
 *
 * @param data The data
 */
AGORA_RTE_API void rte_packed_data_destroy(rte_packed_data_t* data);

/**
 * @brief Get buffer of plain raw packed data.
 *
 * @param data The data
 * @return The pointer of raw data if success, NULL otherwise
 */
AGORA_RTE_API void* rte_packed_data_get_data(rte_packed_data_t* data);

/**
 * @brief Get length of plain raw packed data.
 *
 * @param data The data
 * @return The length of raw data if success, 0 otherwise
 */
AGORA_RTE_API size_t rte_packed_data_get_length(rte_packed_data_t* data);

/**
 * @brief Get current reader index of data.
 *
 * @param data The data
 * @return The current reader index.
 */
AGORA_RTE_API size_t rte_packed_data_get_offset(rte_packed_data_t* data);

/**
 * @brief Reset data into empty.
 *
 * @param data The data
 * @return 0 if success, -1 otherwise
 */
AGORA_RTE_API int rte_packed_data_reset(rte_packed_data_t* data);

/**
 * @brief Reset data into not-read-yet.
 *
 * @param data The data
 * @return 0 if success, -1 otherwise
 */
AGORA_RTE_API int rte_packed_data_rewind(rte_packed_data_t* data);

/**
 * @brief Check whether data is complete.
 *
 * @param data The data
 * @return 1 if complete, 0 otherwise
 */
AGORA_RTE_API int rte_packed_data_check(rte_packed_data_t* data,
                                        RTE_PACKER_ENDIAN endian);

/**
 * @brief This function is used by |RTE_PACKER_DECLARE_STRUCT| macro.
 * Usually you shouldn't call this directly
 */
AGORA_RTE_API rte_unpacked_data_t* rte_unpacked_data_create(size_t size,
                                                            size_t field_count,
                                                            ...);

/**
 * @brief This function is used by |RTE_PACKER_DECLARE_STRUCT| macro.
 * Usually you shouldn't call this directly
 */
AGORA_RTE_API void rte_unpacked_data_destroy(rte_unpacked_data_t* data);

/**
 * @brief This function is used by |RTE_PACKER_DECLARE_STRUCT| macro.
 * Usually you shouldn't call this directly
 */
AGORA_RTE_API void rte_unpacked_data_init(rte_unpacked_data_t* self,
                                          size_t field_count,
                                          ...);

/**
 * @brief This function is used by |RTE_PACKER_DECLARE_STRUCT| macro.
 * Usually you shouldn't call this directly
 */
AGORA_RTE_API void rte_unpacked_data_deinit(rte_unpacked_data_t* data);

#define RTE_PACKER_STRUCT_FIELD_1(t, s, f, ...) t f;

#define RTE_PACKER_STRUCT_FIELD_2(t, s, f, ...) \
  t f;                                          \
  RTE_PACKER_STRUCT_FIELD_1(__VA_ARGS__)

#define RTE_PACKER_STRUCT_FIELD_3(t, s, f, ...) \
  t f;                                          \
  RTE_PACKER_STRUCT_FIELD_2(__VA_ARGS__)

#define RTE_PACKER_STRUCT_FIELD_4(t, s, f, ...) \
  t f;                                          \
  RTE_PACKER_STRUCT_FIELD_3(__VA_ARGS__)

#define RTE_PACKER_STRUCT_FIELD_5(t, s, f, ...) \
  t f;                                          \
  RTE_PACKER_STRUCT_FIELD_4(__VA_ARGS__)

#define RTE_PACKER_STRUCT_FIELD_6(t, s, f, ...) \
  t f;                                          \
  RTE_PACKER_STRUCT_FIELD_5(__VA_ARGS__)

#define RTE_PACKER_STRUCT_FIELD_7(t, s, f, ...) \
  t f;                                          \
  RTE_PACKER_STRUCT_FIELD_6(__VA_ARGS__)

#define RTE_PACKER_STRUCT_FIELD_8(t, s, f, ...) \
  t f;                                          \
  RTE_PACKER_STRUCT_FIELD_7(__VA_ARGS__)

#define RTE_PACKER_STRUCT_FIELD_9(t, s, f, ...) \
  t f;                                          \
  RTE_PACKER_STRUCT_FIELD_8(__VA_ARGS__)

#define RTE_PACKER_STRUCT_FIELD_10(t, s, f, ...) \
  t f;                                           \
  RTE_PACKER_STRUCT_FIELD_9(__VA_ARGS__)

#define RTE_PACKER_STRUCT_FIELD_11(t, s, f, ...) \
  t f;                                           \
  RTE_PACKER_STRUCT_FIELD_10(__VA_ARGS__)

#define RTE_PACKER_STRUCT_FIELD_12(t, s, f, ...) \
  t f;                                           \
  RTE_PACKER_STRUCT_FIELD_11(__VA_ARGS__)

#define RTE_PACKER_STRUCT_FIELD_13(t, s, f, ...) \
  t f;                                           \
  RTE_PACKER_STRUCT_FIELD_12(__VA_ARGS__)

#define RTE_PACKER_STRUCT_FIELD_14(t, s, f, ...) \
  t f;                                           \
  RTE_PACKER_STRUCT_FIELD_13(__VA_ARGS__)

#define RTE_PACKER_STRUCT_FIELD_15(t, s, f, ...) \
  t f;                                           \
  RTE_PACKER_STRUCT_FIELD_14(__VA_ARGS__)

#define RTE_PACKER_STRUCT_FIELD_16(t, s, f, ...) \
  t f;                                           \
  RTE_PACKER_STRUCT_FIELD_15(__VA_ARGS__)

#define RTE_PACKER_STRUCT_PARAM_1(name, t, s, f) \
  s, OFFSET_OF(rte_##name##_struct_t, f)

#define RTE_PACKER_STRUCT_PARAM_2(name, t, s, f, ...) \
  s, OFFSET_OF(rte_##name##_struct_t, f),             \
    RTE_PACKER_STRUCT_PARAM_1(name, __VA_ARGS__)

#define RTE_PACKER_STRUCT_PARAM_3(name, t, s, f, ...) \
  s, OFFSET_OF(rte_##name##_struct_t, f),             \
    RTE_PACKER_STRUCT_PARAM_2(name, __VA_ARGS__)

#define RTE_PACKER_STRUCT_PARAM_4(name, t, s, f, ...) \
  s, OFFSET_OF(rte_##name##_struct_t, f),             \
    RTE_PACKER_STRUCT_PARAM_3(name, __VA_ARGS__)

#define RTE_PACKER_STRUCT_PARAM_5(name, t, s, f, ...) \
  s, OFFSET_OF(rte_##name##_struct_t, f),             \
    RTE_PACKER_STRUCT_PARAM_4(name, __VA_ARGS__)

#define RTE_PACKER_STRUCT_PARAM_6(name, t, s, f, ...) \
  s, OFFSET_OF(rte_##name##_struct_t, f),             \
    RTE_PACKER_STRUCT_PARAM_5(name, __VA_ARGS__)

#define RTE_PACKER_STRUCT_PARAM_7(name, t, s, f, ...) \
  s, OFFSET_OF(rte_##name##_struct_t, f),             \
    RTE_PACKER_STRUCT_PARAM_6(name, __VA_ARGS__)

#define RTE_PACKER_STRUCT_PARAM_8(name, t, s, f, ...) \
  s, OFFSET_OF(rte_##name##_struct_t, f),             \
    RTE_PACKER_STRUCT_PARAM_7(name, __VA_ARGS__)

#define RTE_PACKER_STRUCT_PARAM_9(name, t, s, f, ...) \
  s, OFFSET_OF(rte_##name##_struct_t, f),             \
    RTE_PACKER_STRUCT_PARAM_8(name, __VA_ARGS__)

#define RTE_PACKER_STRUCT_PARAM_10(name, t, s, f, ...) \
  s, OFFSET_OF(rte_##name##_struct_t, f),              \
    RTE_PACKER_STRUCT_PARAM_9(name, __VA_ARGS__)

#define RTE_PACKER_STRUCT_PARAM_11(name, t, s, f, ...) \
  s, OFFSET_OF(rte_##name##_struct_t, f),              \
    RTE_PACKER_STRUCT_PARAM_10(name, __VA_ARGS__)

#define RTE_PACKER_STRUCT_PARAM_12(name, t, s, f, ...) \
  s, OFFSET_OF(rte_##name##_struct_t, f),              \
    RTE_PACKER_STRUCT_PARAM_11(name, __VA_ARGS__)

#define RTE_PACKER_STRUCT_PARAM_13(name, t, s, f, ...) \
  s, OFFSET_OF(rte_##name##_struct_t, f),              \
    RTE_PACKER_STRUCT_PARAM_12(name, __VA_ARGS__)

#define RTE_PACKER_STRUCT_PARAM_14(name, t, s, f, ...) \
  s, OFFSET_OF(rte_##name##_struct_t, f),              \
    RTE_PACKER_STRUCT_PARAM_13(name, __VA_ARGS__)

#define RTE_PACKER_STRUCT_PARAM_15(name, t, s, f, ...) \
  s, OFFSET_OF(rte_##name##_struct_t, f),              \
    RTE_PACKER_STRUCT_PARAM_14(name, __VA_ARGS__)

#define RTE_PACKER_STRUCT_PARAM_16(name, t, s, f, ...) \
  s, OFFSET_OF(rte_##name##_struct_t, f),              \
    RTE_PACKER_STRUCT_PARAM_15(name, __VA_ARGS__)

/**
 * @brief Declare a packable data struct in C.
 * @param field_count How many fields that this struct has.
 * @param name The name of the struct. Will append rte_ in prefix and _struct_t
 *              in appendix, for example rte_fake_struct_t if |name| is fake
 * @param type1 The type of first field, for example int, rte_string_t, etc
 * @param schema1 The type schema of first field, should define with
 *                RTE_TYPE_DEF macro. See details in lib/generic.h
 * @param field1 The name of first field.
 * @param ... Other fields. Each fields should have three properties like
 *            above.
 * @example
 *  If you declare a struct like this:
    ``` c
      RTE_PACKER_DECLARE_STRUCT(1,
                          my,
                          uint32_t,
                          RTE_TYPE_DEF(RTE_TYPE_PRIMITIVE_U32),
                          a,
                          long long,
                          RTE_TYPE_DEF(RTE_TYPE_PRIMITIVE_I64),
                          b)
    ```
    It will expand to
    ``` c
      typedef struct rte_my_struct_t {
        uint32_t a;
        long long b;
      };
    ```
    While treat a as U32 and b as I64 when packing / unpacking.
    You should take care of the type and its real size.
    Allowd types:
      - Primitive types: like int, long, float, etc
      - Generic types: currently only rte_list_t, rte_rbtree_t, rte_string_t
      - Other packable types declared by RTE_PACKER_DECLARE_STRUCT macro
    This macro also add four helper functions:
    ``` c
      static rte_my_struct_t* rte_my_struct_create();
      static void rte_my_struct_destroy(rte_my_struct_t* self);
      static void rte_my_struct_init(rte_my_struct_t* self);
      static void rte_my_struct_deinit(rte_my_struct_t* self);
    ```
    You have to make sure the struct is created by above xxx_create function,
    or initialized by above xxx_init function.
    Also remember to free the memory by using above xxx_destroy function,
    or de-initialize by above xxx_deinit function, otherwise memory leaks.
    When you declare a data struct that is combined by other packable
    structs, you may do like this:
    ``` c
      RTE_PACKER_DECLARE_STRUCT(1,
                          my2,
                          rte_my_struct_t,
                          RTE_TYPE_DEF(RTE_TYPE_GENERIC_STRUCT),
                          a,
                          rte_my_struct_t,
                          RTE_TYPE_DEF(RTE_TYPE_GENERIC_STRUCT),
                          b)
    ```
    Make sure all of the packable fields are initialized and de-initialized
    properly by its own helper function.
 */
#define RTE_PACKER_DECLARE_STRUCT(field_count, name, type, schema, field, ...) \
  typedef struct rte_##name##_struct_t rte_##name##_struct_t;                  \
  static void rte_##name##_struct_init(rte_##name##_struct_t* data);           \
  static void rte_##name##_struct_deinit(rte_##name##_struct_t* data);         \
  static const size_t k_##name##_struct_field_count = field_count;             \
  struct rte_##name##_struct_t {                                               \
    rte_unpacked_data_t base;                                                  \
    RTE_PACKER_STRUCT_FIELD_##field_count(type, schema, field, __VA_ARGS__);   \
  };                                                                           \
  static void rte_##name##_struct_destroy(rte_##name##_struct_t* data) {       \
    rte_unpacked_data_destroy(&data->base);                                    \
  }                                                                            \
  static rte_##name##_struct_t* rte_##name##_struct_create() {                 \
    return (rte_##name##_struct_t*)rte_unpacked_data_create(                   \
      sizeof(rte_##name##_struct_t),                                           \
      k_##name##_struct_field_count,                                           \
      RTE_PACKER_STRUCT_PARAM_##field_count(name,                              \
                                            type,                              \
                                            schema,                            \
                                            field,                             \
                                            ##__VA_ARGS__));                   \
  }                                                                            \
  static void rte_##name##_struct_deinit(rte_##name##_struct_t* data) {        \
    rte_unpacked_data_deinit(&data->base);                                     \
  }                                                                            \
  static void rte_##name##_struct_init(rte_##name##_struct_t* data) {          \
    rte_unpacked_data_init(                                                    \
      &data->base,                                                             \
      k_##name##_struct_field_count,                                           \
      RTE_PACKER_STRUCT_PARAM_##field_count(name,                              \
                                            type,                              \
                                            schema,                            \
                                            field,                             \
                                            ##__VA_ARGS__));                   \
  }
