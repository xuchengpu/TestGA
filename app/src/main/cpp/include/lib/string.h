/**
 *
 * Agora Real Time Engagement
 * Created by Wei Hu in 2021-07.
 * Copyright (c) 2021 Agora IO. All rights reserved.
 *
 */
#pragma once

#include "rte_config.h"

#include <assert.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "lib/atomic.h"
#include "lib/buf.h"

#define RTE_STRING_SIGNATURE 0x178445C0402E320DU

#define RTE_STRING_PRE_BUF_SIZE 256

typedef struct rte_list_t rte_list_t;

typedef struct rte_string_t {
  rte_atomic_t signature;
  char* buf;  // Pointer to allocated buffer.
  char pre_buf[RTE_STRING_PRE_BUF_SIZE];
  size_t buf_size;  // Allocated capacity.
  size_t first_unused_idx;  // Index of first unused byte.
} rte_string_t;

/**
 * @brief Create a string object.
 * @return A pointer to the string object.
 */
AGORA_RTE_API rte_string_t* rte_string_create(void);

/**
 * @brief Create a string object from c string.
 * @param fmt The c string.
 * @return A pointer to the string object.
 */
AGORA_RTE_API rte_string_t* rte_string_create_with_value(const char* fmt, ...);

/**
 * @brief Create a string object from another string object.
 * @param other The other string object.
 * @return A pointer to the string object.
 */
AGORA_RTE_API rte_string_t* rte_string_clone(const rte_string_t* other);

/**
 * @brief Initialize a string object from existing memory.
 * @param self The string object.
 */
AGORA_RTE_API void rte_string_init(rte_string_t* self);

/**
 * @brief Initialize a string object from existing memory, and set the value.
 * @param self The string object.
 * @param fmt The c string.
 */
AGORA_RTE_API void rte_string_init_with_value(rte_string_t* self,
                                              const char* fmt,
                                              ...);

/**
 * @brief Initialize a string object from another string object.
 * @param self The string object.
 * @param other The other string object.
 */
AGORA_RTE_API void rte_string_copy(rte_string_t* self, rte_string_t* other);

/**
 * @brief Initialize a string object from another string object.
 * @param self The string object.
 * @param other The other string object.
 * @param size the max size, copy all if size <= 0
 */
AGORA_RTE_API void rte_string_copy_c_str(rte_string_t* self,
                                         const char* other,
                                         size_t size);

/**
 * @brief Destroy a string object and release the memory.
 * @param self The string object.
 */
AGORA_RTE_API void rte_string_destroy(rte_string_t* self);

/**
 * @brief Destroy a string object, left the memory.
 * @param self The string object.
 */
AGORA_RTE_API void rte_string_deinit(rte_string_t* self);

/**
 * @brief Set the string object as empty.
 * @param self The string object.
 */
AGORA_RTE_API void rte_string_clear(rte_string_t* self);

/**
 * @brief Reserve memory for the string object.
 * @param self The string object.
 * @param extra The size of the memory to be reserved.
 */
AGORA_RTE_API void rte_string_reserve(rte_string_t* self, size_t extra);

AGORA_RTE_API void rte_string_vset(rte_string_t* self,
                                   const char* fmt,
                                   va_list ap);

/**
 * @brief Set the string object with a c string.
 * @param self The string object.
 * @param fmt The c string.
 */
AGORA_RTE_API void rte_string_set(rte_string_t* self, const char* fmt, ...);

/**
 * @brief Prepend a c string to the string object.
 * @param self The string object.
 * @param fmt The c string.
 */
AGORA_RTE_API void rte_string_prepend(rte_string_t* self, const char* fmt, ...);

AGORA_RTE_API void rte_string_vprepend(rte_string_t* self,
                                       const char* fmt,
                                       va_list ap);

/**
 * @brief Append a c string to the string object.
 * @param self The string object.
 * @param fmt The c string.
 */
AGORA_RTE_API void rte_string_concat(rte_string_t* self, const char* fmt, ...);

/**
 * @brief Check if the string object is empty.
 * @param self The string object.
 * @return true if the string object is empty, otherwise false.
 */
AGORA_RTE_API bool rte_string_is_empty(const rte_string_t* self);

/**
 * @brief Check if the string object starts with the specified substring.
 * @param self The string object.
 * @param start The substring to be compared.
 * @return true if the string object starts with the specified substring,
 * otherwise false.
 */
AGORA_RTE_API bool rte_string_starts_with(const rte_string_t* self,
                                          const char* start);

/**
 * @brief Check if the string object is equal to another string object.
 * @param self The string object.
 * @param other The other string object.
 * @return true if the string object is equal to the other string object,
 *         otherwise false.
 */
AGORA_RTE_API bool rte_string_is_equal(const rte_string_t* self,
                                       const rte_string_t* other);

/**
 * @brief Check if the string object is equal to a c string.
 * @param self The string object.
 * @param other The c string.
 * @return true if the string object is equal to the c string, otherwise false.
 */
AGORA_RTE_API bool rte_string_is_equal_c_str(rte_string_t* self,
                                             const char* other);

/**
 * @brief Check if the string is equal to a c string in case-insensitive flavor.
 * @param self The string object.
 * @param other The c string.
 * @return true if the string object is equal to the c string in
 *         case-insensitive flavor, otherwise false.
 */
AGORA_RTE_API bool rte_string_is_equal_c_str_case_insensitive(
  rte_string_t* self,
  const char* other);

/**
 * @brief Check if the string contains a c string.
 * @param self The string object.
 * @param b The c string.
 * @return true if the string object contains the c string, otherwise false.
 */
AGORA_RTE_API bool rte_string_contains(rte_string_t* self, const char* b);

/**
 * @brief Convert the string object to lowercase.
 * @param self The string object.
 */
AGORA_RTE_API void rte_string_to_lower(rte_string_t* self);

/**
 * @brief Convert the string object to uppercase.
 * @param self The string object.
 */
AGORA_RTE_API void rte_string_to_upper(rte_string_t* self);

/**
 * @brief Get c string from the string object.
 * @param self The string object.
 * @return A pointer to the c string.
 */
inline static const char* rte_string_c_str(const rte_string_t* self) {
  // It's possible that the return value of this function is used by "%s", and
  // pass NULL as the value of "%s" is an undefined behavior, so we ensure that
  // the return value of this function is not NULL.
  assert(self);

  return self ? self->buf : NULL;
}

/**
 * @brief Get the length of the string object.
 * @param self The string object.
 * @return The length of the string object.
 */
inline static size_t rte_string_len(const rte_string_t* self) {
  return self ? strlen(self->buf) : 0;
}

/**
 * @brief Convert the buffer content to a hexadecimal string.
 * @param self The string object.
 * @param buf The buffer.
 */
AGORA_RTE_API void rte_string_hex_from_buf(rte_string_t* self, rte_buf_t buf);

/**
 * @brief Check if the c string is equal to another c string object.
 * @param a The c string object.
 * @param b The other c string object.
 * @return true if the c string a is equal to the other c string b,
 *         otherwise false.
 */
AGORA_RTE_API bool rte_c_string_is_equal(const char* a, const char* b);

AGORA_RTE_API bool rte_c_string_is_equal_with_size(const char* a,
                                                   const char* b,
                                                   size_t num);

/**
 * @brief Check if the c string is equal to a c string in case-insensitive
 * flavor.
 * @param a The c string.
 * @param b The c string.
 * @return true if the c string a is equal to the c string b in case-insensitive
 * flavor, otherwise false.
 */
AGORA_RTE_API bool rte_c_string_is_equal_case_insensitive(const char* a,
                                                          const char* b);

AGORA_RTE_API bool rte_c_string_is_equal_with_size_case_insensitive(
  const char* a,
  const char* b,
  size_t num);

/**
 * @brief Check if the c string object is empty.
 * @param self The c string object.
 * @return true if the c string object is empty, otherwise false.
 */
AGORA_RTE_API bool rte_c_string_is_empty(const char* str);

/**
 * @brief Check if the c string starts with another c string.
 * @param self The c string object.
 * @param prefix The prefix c string object.
 * @return true if the c string starts with another c string, otherwise false.
 */
AGORA_RTE_API bool rte_c_string_starts_with(const char* str,
                                            const char* prefix);

/**
 * @brief Check if c string 'a' is smaller than 'b'. The definitions of
 * 'smaller' is as follows.
 *   - The length is smaller.
 *   - If the length is equal, then the first unequal character is smaller.
 */
AGORA_RTE_API
bool rte_c_string_is_equal_or_smaller(const char* a, const char* b);

/**
 * @brief find position of a string.
 * @param src
 * @param search string to locate
 * @return the position 'serach' is first found in 'src'; -1 if not found.
 */
AGORA_RTE_API int rte_c_string_index_of(const char* src, const char* search);

/**
 * @brief split string by delimiter.
 * @param src The source string to be splitted.
 * @param delimiter
 * @return the splitted substring list.
 */
AGORA_RTE_API void rte_c_string_split(const char* src,
                                      const char* delimiter,
                                      rte_list_t* result);

/**
 * @brief Check if the c string contains a c string.
 * @param self The c string.
 * @param b The c string.
 * @return true if the c string object contains the c string, otherwise false.
 */
AGORA_RTE_API bool rte_c_string_contains(const char* a, const char* b);

/**
 * @brief Convert a c string to a URI encoded string.
 * @param src The source c string.
 * @param len The length of the source c string.
 * @param result The result string object.
 */
AGORA_RTE_API void rte_c_string_uri_encode(const char* src,
                                           size_t len,
                                           rte_string_t* result);

/**
 * @brief Convert a c string to a URI decoded string.
 * @param src The source c string.
 * @param len The length of the source c string.
 * @param result The result string object.
 */
AGORA_RTE_API void rte_c_string_uri_decode(const char* src,
                                           size_t len,
                                           rte_string_t* result);

/**
 * @brief Escape a string by replacing certain special characters by a sequence
 * of an escape character (backslash) and another character and other control
 * characters by a sequence of "\u" followed by a four-digit hex representation.
 * @param src The source string to escape.
 * @param result The output string.
 */
AGORA_RTE_API void rte_c_string_escaped(const char* src, rte_string_t* result);
