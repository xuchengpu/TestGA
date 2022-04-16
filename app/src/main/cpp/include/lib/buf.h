/**
 *
 * Agora Real Time Engagement
 * Created by Wei Hu in 2021-07.
 * Copyright (c) 2021 Agora IO. All rights reserved.
 *
 */
#pragma once

#include <stddef.h>
#include <string.h>

#include "lib/alloc.h"

/** \addtogroup container
 *  @{
 */

#define RTE_BUF_STATIC_INIT \
  (rte_buf_t) {             \
    NULL, 0, 0              \
  }

typedef struct rte_buf_t {
  void* buf;
  size_t buf_size;
  int copy;
} rte_buf_t;

/**
 * @brief De-init a rte_buf_t struct
 *
 * @param self The buf.
 */
static inline void rte_buf_deinit(rte_buf_t* self) {
  if (!self) {
    return;
  }

  if (self->copy && self->buf) {
    rte_free(self->buf);
  }

  self->buf = NULL;
  self->copy = 0;
  self->buf_size = 0;
}

/**
 * @brief Init a buffer.
 * @param size The size of buffer.
 */
static inline void rte_buf_init(rte_buf_t* self, size_t size) {
  if (!self) {
    return;
  }

  if (size != 0) {
    self->buf = rte_malloc(size);
    self->buf_size = size;
    self->copy = 1;
  } else {
    self->buf = NULL;
    self->buf_size = 0;
    self->copy = 0;
  }
}

/**
 * @brief Init a rte_buf_t struct with other data.
 *
 * @param buf The pointer of buffer.
 * @param size The size of buffer.
 * @param copy Create a new buffer and copy origin if 1
 */
static inline void rte_buf_init_with_buffer(rte_buf_t* self,
                                            void* buf,
                                            size_t size,
                                            int copy) {
  if (!buf || size == 0) {
    goto error;
  }

  if (copy) {
    self->buf = rte_malloc(size);
    if (!self->buf) {
      goto error;
    }

    memcpy(self->buf, buf, size);
    self->buf_size = size;
    self->copy = 1;
  } else {
    self->buf = buf;
    self->buf_size = size;
    self->copy = 0;
  }

  return;

error:
  self->buf = NULL;
  self->buf_size = 0;
  self->copy = 0;
}

/** @}*/