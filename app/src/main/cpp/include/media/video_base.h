//  Agora Media SDK
//
//  Created by Yaqi Li in 2022-02.
//  Copyright (c) 2022 Agora IO. All rights reserved.
//

#pragma once

#include "rte_config.h"

#define RTE_VIDEO_GL_TRANSFORM_MATRIX_SIZE 16

typedef enum RTE_VIDEO_FRAME_TYPE {
  RTE_VIDEO_FRAME_TYPE_UNSPECIFIED,
  RTE_VIDEO_FRAME_TYPE_RAW_MEMORY,
  RTE_VIDEO_FRAME_TYPE_GL_TEXTURE_OES, // Android Only
  RTE_VIDEO_FRAME_TYPE_GL_TEXTUER_2D,
  RTE_VIDEO_FRAME_TYPE_CVPIXELBUF, //iOS/Mac Only
  RTE_VIDEO_FRAME_TYPE_MTL_TEXTURE, //iOS/Mac Only
} RTE_VIDEO_FRAME_TYPE;

typedef enum RTE_EGL_CONTEXT_TYPE {
  RTE_VIDEO_EGL_CONTEXT_UNSPECIFIED,
  RTE_VIDEO_EGL_CONTEXT_10,
  RTE_VIDEO_EGL_CONTEXT_14,
} RTE_EGL_CONTEXT_TYPE; // Android Only

typedef enum RTE_VIDEO_FRAME_FORMAT_TYPE {
  RTE_VIDEO_FRAME_FORMAT_UNSPECIFIED,
  RTE_VIDEO_FRAME_FORMAT_YUV420,
  RTE_VIDEO_FRAME_FORMAT_YUV422,
  RTE_VIDEO_FRAME_FORMAT_NV21,
  RTE_VIDEO_FRAME_FORMAT_NV12,
  RTE_VIDEO_FRAME_FORMAT_RGBA,
  RTE_VIDEO_FRAME_FORMAT_ARGB,
  RTE_VIDEO_FRAME_FORMAT_BGRA,
} RTE_VIDEO_FRAME_FORMAT_TYPE;

typedef enum RTE_COLOR_PRIMARY_ID {
  // The indices are equal to the values specified in T-REC H.273 Table 2.
  RTE_COLOR_PRIMARY_ID_BT709 = 1,
  RTE_COLOR_PRIMARY_ID_UNSPECIFIED = 2,
  RTE_COLOR_PRIMARY_ID_BT470M = 4,
  RTE_COLOR_PRIMARY_ID_BT470BG = 5,
  RTE_COLOR_PRIMARY_ID_SMPTE170M = 6,  // Identical to BT601
  RTE_COLOR_PRIMARY_ID_SMPTE240M = 7,
  RTE_COLOR_PRIMARY_ID_FILM = 8,
  RTE_COLOR_PRIMARY_ID_BT2020 = 9,
  RTE_COLOR_PRIMARY_ID_SMPTEST428 = 10,
  RTE_COLOR_PRIMARY_ID_SMPTEST431 = 11,
  RTE_COLOR_PRIMARY_ID_SMPTEST432 = 12,
  RTE_COLOR_PRIMARY_ID_JEDECP22 = 22,  // Identical to EBU3213-E
} RTE_COLOR_PRIMARY_ID;

typedef enum RTE_COLOR_RANGE_ID {
  // The indices are equal to the values specified at
  // https://www.webmproject.org/docs/container/#colour for the element Range.
  RTE_COLOR_RANGE_ID_INVALID = 0,
  // Limited Rec. 709 color range with RGB values ranging from 16 to 235.
  RTE_COLOR_RANGE_ID_LIMITED = 1,
  // Full RGB color range with RGB valees from 0 to 255.
  RTE_COLOR_RANGE_ID_FULL = 2,
  // Range is defined by MatrixCoefficients/TransferCharacteristics.
  RTE_COLOR_RANGE_ID_DERIVED = 3,
} RTE_COLOR_RANGE_ID;

typedef enum RTE_COLOR_MATRIX_ID {
  // The indices are equal to the values specified in T-REC H.273 Table 4.
  RTE_COLOR_MATRIX_ID_RGB = 0,
  RTE_COLOR_MATRIX_ID_BT709 = 1,
  RTE_COLOR_MATRIX_ID_UNSPECIFIED = 2,
  RTE_COLOR_MATRIX_ID_FCC = 4,
  RTE_COLOR_MATRIX_ID_BT470BG = 5,
  RTE_COLOR_MATRIX_ID_SMPTE170M = 6,
  RTE_COLOR_MATRIX_ID_SMPTE240M = 7,
  RTE_COLOR_MATRIX_ID_YCOCG = 8,
  RTE_COLOR_MATRIX_ID_BT2020_NCL = 9,
  RTE_COLOR_MATRIX_ID_BT2020_CL = 10,
  RTE_COLOR_MATRIX_ID_SMPTE2085 = 11,
  RTE_COLOR_MATRIX_ID_CDNCLS = 12,
  RTE_COLOR_MATRIX_ID_CDCLS = 13,
  RTE_COLOR_MATRIX_ID_BT2100_ICTCP = 14,
} RTE_COLOR_MATRIX_ID;

typedef enum RTE_COLOR_TRANSFER_ID {
  // The indices are equal to the values specified in T-REC H.273 Table 3.
  RTE_COLOR_TRANSFER_ID_BT709 = 1,
  RTE_COLOR_TRANSFER_ID_UNSPECIFIED = 2,
  RTE_COLOR_TRANSFER_ID_GAMMA22 = 4,
  RTE_COLOR_TRANSFER_ID_GAMMA28 = 5,
  RTE_COLOR_TRANSFER_ID_SMPTE170M = 6,
  RTE_COLOR_TRANSFER_ID_SMPTE240M = 7,
  RTE_COLOR_TRANSFER_ID_LINEAR = 8,
  RTE_COLOR_TRANSFER_ID_LOG = 9,
  RTE_COLOR_TRANSFER_ID_LOG_SQRT = 10,
  RTE_COLOR_TRANSFER_ID_IEC61966_2_4 = 11,
  RTE_COLOR_TRANSFER_ID_BT1361_ECG = 12,
  RTE_COLOR_TRANSFER_ID_IEC61966_2_1 = 13,
  RTE_COLOR_TRANSFER_ID_BT2020_10 = 14,
  RTE_COLOR_TRANSFER_ID_BT2020_12 = 15,
  RTE_COLOR_TRANSFER_ID_SMPTEST2084 = 16,
  RTE_COLOR_TRANSFER_ID_SMPTEST428 = 17,
  RTE_COLOR_TRANSFER_ID_ARIB_STD_B67 = 18,
} RTE_COLOR_TRANSFER_ID;

typedef struct rte_video_color_sapce_t {
  RTE_COLOR_PRIMARY_ID primary_id;
  RTE_COLOR_RANGE_ID range_id;
  RTE_COLOR_MATRIX_ID matrix_id;
  RTE_COLOR_TRANSFER_ID transfer_id;
} rte_video_color_sapce_t;

typedef struct rte_video_frame_t {
  RTE_VIDEO_FRAME_TYPE frame_type;
  RTE_VIDEO_FRAME_FORMAT_TYPE format_type;
  uint32_t width;
  uint32_t height;
  uint32_t rotation;
  int64_t timespace_ms;
  rte_video_color_sapce_t color_space;
} rte_video_frame_t;

typedef struct rte_video_frame_raw_memory_t {
  rte_video_frame_t base;
  uint8_t planar_number;
  uint32_t stride_1;
  uint8_t* planar_data_1;
  uint32_t stride_2;
  uint8_t* planar_data_2;
  uint32_t stride_3;
  uint8_t* planar_data_3;
} rte_video_frame_raw_memory_t;

typedef struct rte_video_frame_gl_texture_t {
  rte_video_frame_t base;
  RTE_EGL_CONTEXT_TYPE context_type; // valid only for Andoird
  void* shared_context;
  int tex_id;
  float transform_matrix[RTE_VIDEO_GL_TRANSFORM_MATRIX_SIZE];
} rte_video_frame_gl_texture_t;

typedef struct rte_video_frame_native_t {
  rte_video_frame_t base;
  void* native_handle;
} rte_video_frame_native_t;

// Public interfaces
AGORA_RTE_API rte_video_frame_t* rte_video_frame_create_raw_memory(RTE_VIDEO_FRAME_FORMAT_TYPE format, int width, int height);
// AGORA_RTE_API rte_video_frame_t* rte_video_frame_create_gl_tex(int width, int height);
// AGORA_RTE_API rte_video_frame_t* rte_video_frame_create_native(int width, int height);
AGORA_RTE_API rte_video_frame_t* rte_video_frame_copy(const rte_video_frame_t* src);
AGORA_RTE_API int rte_video_frame_destroy(rte_video_frame_t* self);
