//  Agora Media SDK
//
//  Created by Yaqi Li in 2022-02.
//  Copyright (c) 2022 Agora IO. All rights reserved.
//

#pragma once

#include "video_base.h"
#include "audio_base.h"

#include "utils/inc/lib/string.h"
#include "utils/inc/lib/generic.h"

typedef struct rte_runloop_t rte_runloop_t;
typedef struct rte_media_ipc_sink_panel_t rte_media_ipc_sink_panel_t;
typedef struct rte_media_ipc_source_panel_t rte_media_ipc_source_panel_t;

typedef enum RTE_MEDIA_IPC_STATE {
  RTE_MEDIA_IPC_STATE_DISCONNECTED,
  RTE_MEDIA_IPC_STATE_CONNECTING,
  RTE_MEDIA_IPC_STATE_CONNECTED,
  RTE_MEDIA_IPC_STATE_ERRORED,
} RTE_MEDIA_IPC_STATE;

typedef struct rte_media_ipc_event_t {
  rte_string_t* key;
  RTE_GENERIC_TYPE value_type;
  void* value;
} rte_media_ipc_event_t;

typedef struct rte_media_ipc_sink_panel_t {
  rte_string_t* uri;
  rte_runloop_t* loop;
  void* user_data;

  void (*on_conn_state_changed)(rte_media_ipc_sink_panel_t* panel, int64_t handle, RTE_MEDIA_IPC_STATE state, void* user_data);
  void (*on_audio_data_received)(rte_media_ipc_sink_panel_t* panel, int64_t handle, rte_audio_pcm_frame_t* data, void* user_data);
  void (*on_video_data_received)(rte_media_ipc_sink_panel_t* panel, int64_t handle, rte_video_frame_t* data, void* user_data);
  void (*on_media_event_received)(rte_media_ipc_sink_panel_t* panel, int64_t handle, rte_media_ipc_event_t* event, void* user_data);
} rte_media_ipc_sink_panel_t;

typedef struct rte_media_ipc_source_panel_t {
  rte_string_t* uri;
  rte_runloop_t* loop;
  void* user_data;

  void (*on_conn_state_changed)(rte_media_ipc_source_panel_t* panel, RTE_MEDIA_IPC_STATE state, void* user_data);
  void (*on_media_event_received)(rte_media_ipc_source_panel_t* panel, rte_media_ipc_event_t* event, void* user_data);
  void (*on_video_frame_sent)(rte_media_ipc_source_panel_t* panel, rte_video_frame_t* frame, void* user_data);
} rte_media_ipc_source_panel_t;

// public interface

AGORA_RTE_API rte_media_ipc_event_t* rte_media_ipc_event_create(const char* key, RTE_GENERIC_TYPE type, const void* value);
AGORA_RTE_API int rte_media_ipc_event_destroy(rte_media_ipc_event_t* self);
AGORA_RTE_API rte_string_t* rte_media_ipc_uri_create(const char* protocol, const char* host, uint16_t port);

AGORA_RTE_API rte_media_ipc_sink_panel_t* rte_media_ipc_sink_panel_create(rte_string_t* uri);
// sink panel will automatically create a loop if it is null, otherwise it will reuse the loop
AGORA_RTE_API int rte_media_ipc_sink_panel_start(rte_media_ipc_sink_panel_t* self, void* raw_loop);
AGORA_RTE_API int rte_media_ipc_sink_panel_push_event(rte_media_ipc_sink_panel_t* self, rte_media_ipc_event_t* event);
AGORA_RTE_API int rte_media_ipc_sink_panel_stop(rte_media_ipc_sink_panel_t* self);
AGORA_RTE_API int rte_media_ipc_sink_panel_destroy(rte_media_ipc_sink_panel_t* self);

AGORA_RTE_API rte_media_ipc_source_panel_t* rte_media_ipc_source_panel_create(rte_string_t* uri);
// source panel will automatically create a loop if it is null, otherwise it will reuse the loop
AGORA_RTE_API int rte_media_ipc_source_panel_start(rte_media_ipc_source_panel_t* self, void* raw_loop);
AGORA_RTE_API int rte_media_ipc_source_panel_push_audio(rte_media_ipc_source_panel_t* self, rte_audio_pcm_frame_t* frame);
AGORA_RTE_API int rte_media_ipc_source_panel_push_video(rte_media_ipc_source_panel_t* self, rte_video_frame_t* frame);
AGORA_RTE_API int rte_media_ipc_source_panel_push_event(rte_media_ipc_source_panel_t* self, rte_media_ipc_event_t* event);
AGORA_RTE_API int rte_media_ipc_source_panel_stop(rte_media_ipc_source_panel_t* self);
AGORA_RTE_API int rte_media_ipc_source_panel_destroy(rte_media_ipc_source_panel_t* self);
