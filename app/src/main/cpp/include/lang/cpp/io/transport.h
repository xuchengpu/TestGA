/**
 *
 * Agora Real Time Engagement
 * Created by Zheng, Ender in 2021-08.
 * Copyright (c) 2022 Agora IO. All rights reserved.
 *
 */
#pragma once

#include <cstddef>
#include <functional>
#include <memory>

#include "io/stream.h"
#include "io/transport.h"
#include "lang/cpp/io/runloop.h"
#include "lang/cpp/lib/string.h"

namespace rte {

class Transport;
class Stream;
using RteTransport = std::shared_ptr<Transport>;
using RteStream = std::shared_ptr<Stream>;

class Stream : public std::enable_shared_from_this<Stream> {
 public:
  using CloseCallback = std::function<void()>;
  using MessageReadCallback = std::function<void(const void*, int)>;
  using MessageSentCallback = std::function<void(int, void*)>;
  using MessageFreeCallback = std::function<void(int, void*)>;

  Stream() = delete;

  Stream(const Stream& rhs) = delete;
  Stream& operator=(const Stream& rhs) = delete;

  Stream(Stream&& rhs) = delete;

  Stream& operator=(Stream&& rhs) = delete;

 public:
  void OnRead(MessageReadCallback&& read_cb) {
    message_read_cb_ = std::move(read_cb);
  }

  void OnWriteDone(MessageSentCallback&& sent_cb) {
    message_sent_cb_ = std::move(sent_cb);
  }

  void OnBufferFree(MessageFreeCallback&& free_cb) {
    message_free_cb_ = std::move(free_cb);
  }

  int StartRead() {
    if (!stream_) {
      return -1;
    }

    return rte_stream_start_read(stream_);
  }

  int StopRead() {
    if (!stream_) {
      return -1;
    }

    return rte_stream_stop_read(stream_);
  }

  int Send(const char* msg, uint32_t size, void* user_data) {
    if (!stream_) {
      return -1;
    }
    int rc = rte_stream_send(stream_, msg, size, user_data);
    return rc;
  }

  int Close(CloseCallback&& cb) {
    if (!stream_) {
      return -1;
    }

    close_cb_ = std::move(cb);
    CloseCallbackData* data = std::make_unique<CloseCallbackData>().release();
    if (!data) {
      return -1;
    }

    message_free_cb_ = nullptr;
    message_sent_cb_ = nullptr;
    message_read_cb_ = nullptr;
    data->self = shared_from_this();
    rte_stream_set_close_cb(stream_, (void*)OnClose, data);
    rte_stream_close(stream_);
    return 0;
  }

 private:
  explicit Stream(rte_stream_t* stream) : stream_(stream) {
    if (!stream_) {
      return;
    }

    stream_->user_data = this;
    stream_->on_message_free = OnMessageFree;
    stream_->on_message_read = OnMessageRead;
    stream_->on_message_sent = OnMessageSent;
  }

 private:
  struct CloseCallbackData {
    RteStream self;
  };

  static void OnMessageRead(rte_stream_t* stream, void* msg, int size) {
    if (!stream || !stream->user_data) {
      return;
    }

    Stream* This = reinterpret_cast<Stream*>(stream->user_data);
    if (!This->message_read_cb_) {
      return;
    }

    This->message_read_cb_(msg, size);
  }

  static void OnMessageSent(rte_stream_t* stream, int status, void* user_data) {
    if (!stream || !stream->user_data) {
      return;
    }

    Stream* This = reinterpret_cast<Stream*>(stream->user_data);
    if (!This->message_sent_cb_) {
      return;
    }

    This->message_sent_cb_(status, user_data);
  }

  static void OnMessageFree(rte_stream_t* stream, int status, void* user_data) {
    if (!stream || !stream->user_data) {
      return;
    }

    Stream* This = reinterpret_cast<Stream*>(stream->user_data);
    if (!This->message_free_cb_) {
      return;
    }

    This->message_free_cb_(status, user_data);
  }

  static void OnClose(void* arg) {
    CloseCallbackData* data = reinterpret_cast<CloseCallbackData*>(arg);
    if (!data || !data->self) {
      return;
    }

    if (data->self->close_cb_) {
      data->self->close_cb_();
      data->self->close_cb_ = nullptr;
    }

    data->self = nullptr;
    delete data;
  }

 private:
  ::rte_stream_t* stream_ = nullptr;
  CloseCallback close_cb_;
  MessageReadCallback message_read_cb_;
  MessageSentCallback message_sent_cb_;
  MessageFreeCallback message_free_cb_;

  friend Transport;
};

class Transport : public std::enable_shared_from_this<Transport> {
 public:
  using ClientAcceptCallback = std::function<void(RteStream, int)>;
  using ServerConnectCallback = std::function<void(RteStream, int)>;
  using CloseCallback = std::function<void()>;

  static RteTransport Create(const RteRunloop& loop) {
    return Create(loop->get_c_loop());
  }

  static RteTransport Create() {
    return Create(rte_runloop_current());
  }

  Transport() = delete;

  Transport(const Transport& rhs) = delete;
  Transport& operator=(const Transport& rhs) = delete;

  Transport(Transport&& rhs) = delete;

  Transport& operator=(Transport&& rhs) = delete;

 public:
  struct CloseCallbackData {
    RteTransport self;
  };

  int Close(CloseCallback&& cb) {
    if (!tp_) {
      return -1;
    }

    close_cb_ = std::move(cb);
    CloseCallbackData* data = std::make_unique<CloseCallbackData>().release();
    if (!data) {
      return -1;
    }

    client_accept_cb_ = nullptr;
    server_connect_cb_ = nullptr;
    data->self = shared_from_this();
    rte_transport_set_close_cb(tp_, (void*)OnClose, data);
    return rte_transport_close(tp_);
  }

  int Listen(const RteString& uri, ClientAcceptCallback&& cb) {
    if (!tp_) {
      return -1;
    }

    client_accept_cb_ = std::move(cb);
    return rte_transport_listen(tp_, uri);
  }

  int Connect(const RteString& uri, ServerConnectCallback&& cb) {
    if (!tp_) {
      return -1;
    }

    server_connect_cb_ = std::move(cb);
    return rte_transport_connect(
      tp_,
      const_cast<rte_string_t*>((const rte_string_t*)uri));
  }

 private:
  static RteTransport Create(::rte_runloop_t* loop) {
    if (!loop) {
      return nullptr;
    }

    auto tp = rte_transport_create(loop);
    if (!tp) {
      return nullptr;
    }

    return std::shared_ptr<Transport>(new (std::nothrow) Transport(tp));
  }

  static void OnClientAccepted(rte_transport_t* transport,
                               rte_stream_t* stream,
                               int status) {
    if (status != 0 || !transport || !transport->user_data || !stream) {
      return;
    }

    Transport* This = reinterpret_cast<Transport*>(transport->user_data);
    if (!This->client_accept_cb_) {
      return;
    }

    RteStream s = std::shared_ptr<Stream>(new (std::nothrow) Stream(stream));
    if (!s) {
      return;
    }

    This->client_accept_cb_(s, status);
  }

  static void OnServerConnected(rte_transport_t* transport,
                                rte_stream_t* stream,
                                int status) {
    if (status != 0 || !transport || !transport->user_data || !stream) {
      return;
    }

    Transport* This = reinterpret_cast<Transport*>(transport->user_data);
    if (!This->server_connect_cb_) {
      return;
    }

    RteStream s = std::shared_ptr<Stream>(new (std::nothrow) Stream(stream));
    if (!s) {
      return;
    }

    This->server_connect_cb_(s, status);
  }

  static void OnClose(void* on_close_data) {
    CloseCallbackData* data =
      reinterpret_cast<CloseCallbackData*>(on_close_data);
    if (!data || !data->self) {
      return;
    }

    if (data->self->close_cb_) {
      data->self->close_cb_();
      data->self->close_cb_ = nullptr;
    }

    data->self = nullptr;
    delete data;
  }

  explicit Transport(::rte_transport_t* tp) : tp_(tp) {
    if (!tp_) {
      return;
    }

    tp_->on_client_accepted = OnClientAccepted;
    tp_->on_server_connected = OnServerConnected;
    tp_->user_data = this;
  }

 private:
  CloseCallback close_cb_;
  ClientAcceptCallback client_accept_cb_;
  ServerConnectCallback server_connect_cb_;
  ::rte_transport_t* tp_ = nullptr;
};

}  // namespace rte