//  Agora Media SDK
//
//  Created by Yaqi Li in 2022-02.
//  Copyright (c) 2022 Agora IO. All rights reserved.
//

#pragma once

#include "rte_config.h"

#define RTE_MAX_PCM_SAMPLES 3840
#define RTE_MAX_PCM_SIZE_IN_BYTE  RTE_MAX_PCM_SAMPLES * sizeof(int16_t)

typedef enum RTE_AUDIO_BYTES_PER_SAMPLE {
  RTE_AUDIO_TWO_TYPES_PER_SAMPLE = 2,
} RTE_AUDIO_BYTES_PER_SAMPLE;

typedef struct rte_audio_pcm_frame_t {
  uint32_t capture_timestamp_ms;
  uint32_t samples_per_channel;
  uint32_t sample_rate_hz;
  uint32_t number_of_channels;
  RTE_AUDIO_BYTES_PER_SAMPLE bytes_per_sample;
  int16_t* samples;
  uint8_t interleaved; // 0 for non-interleaved frame
} rte_audio_pcm_frame_t;

// Public interfaces
// AGORA_RTE_API rte_audio_pcm_frame_t* rte_audio_pcm_frame_create();
// AGORA_RTE_API rte_audio_pcm_frame_t* rte_audio_pcm_frame_copy(const rte_audio_pcm_frame_t* src);
// AGORA_RTE_API int rte_audio_pcm_frame_destroy(rte_audio_pcm_frame_t* self);
