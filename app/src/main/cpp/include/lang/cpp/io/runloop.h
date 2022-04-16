/**
 *
 * Agora Real Time Engagement
 * Created by Zheng, Ender in 2021-08.
 * Copyright (c) 2021 Agora IO. All rights reserved.
 *
 */
#pragma once

#include <functional>
#include <memory>
#include <string>

#include "io/runloop.h"

namespace rte {

class Runloop;
using RteRunloop = std::unique_ptr<Runloop>;

using RteRunloopAttachedCallback = std::function<void()>;


class Runloop {
 public:
  static RteRunloop Create(const std::string& impl = "") {
    auto loop = rte_runloop_create(impl.empty() ? nullptr : impl.c_str());
    if (!loop) {
      return nullptr;
    }

    return std::unique_ptr<Runloop>(new (std::nothrow) Runloop(loop, true));
  }

  static RteRunloop Attach(const std::string& impl, void* raw, RteRunloopAttachedCallback&& cb) {
    auto result = std::unique_ptr<Runloop>(new (std::nothrow) Runloop(std::move(cb)));
    rte_runloop_t* attached = rte_runloop_attach(impl.c_str(),
                                                 raw,
                                                 Runloop::OnRunloopReady,
                                                 result.get());
    
    
    if (!attached) {
      return nullptr;
    }
    result->loop_ = attached;
    return result;
  }

  static RteRunloop Shadow(rte_runloop_t* loop) {
    if (!loop) {
      return nullptr;
    }

    return std::unique_ptr<Runloop>(new (std::nothrow) Runloop(loop, false));
  }

  Runloop() = delete;

  Runloop(const Runloop& rhs) = delete;
  Runloop& operator=(const Runloop& rhs) = delete;

  Runloop(Runloop&& rhs) = delete;

  Runloop& operator=(Runloop&& rhs) = delete;

  ~Runloop() {
    if (needs_destroy_) {
      rte_runloop_destroy(loop_);
    }
  }

 public:
  ::rte_runloop_t* get_c_loop() const {
    return loop_;
  }

  void Run() {
    rte_runloop_run(loop_);
  }

  void Stop() {
    rte_runloop_stop(loop_);
  }

  bool Alive() const {
    return rte_runloop_alive(loop_) == 1;
  }

  bool PostTaskTail(std::function<void()>&& callback) {
    return PostTask(std::move(callback), rte_runloop_post_task_tail);
  }

  bool PostTaskFront(std::function<void()>&& callback) {
    return PostTask(std::move(callback), rte_runloop_post_task_front);
  }

  void Detach() {
    rte_runloop_detach(loop_,
                       [](void* data) {
                         auto raw = (rte_runloop_t*) data;
                         rte_runloop_destroy(raw);
                       },
                       loop_);
  }

 private:
  struct Task {
    std::function<void()> callback;
  };

  static void OnRunloopReady(::rte_runloop_t* loop, void* user_data) {
    Runloop* self = static_cast<Runloop*>(user_data);
    if (self->attach_cb_) {
      self->attach_cb_();
    }
  }

  template <typename T>
  bool PostTask(std::function<void()>&& callback, T c_api) {
    if (!loop_) {
      return false;
    }

    //auto c = std::make_unique<Task>().release();
      auto c = std::unique_ptr<Task>(new Task()).release();
    if (!c) {
      return false;
    }

    c->callback = std::move(callback);

    auto ret = c_api(
      loop_,
      [](void* from, void* arg) {
        (void)from;
        auto* t = (Task*)arg;
        if (!t || !t->callback) {
          return;
        }

        t->callback();

        delete t;
      },
      this,
      c);
    if (ret != 0) {
      delete c;
    }

    return ret == 0;
  }

  explicit Runloop(::rte_runloop_t* loop, bool needs_destroy)
    : loop_(loop),
      needs_destroy_(needs_destroy) {
  }

  explicit Runloop( RteRunloopAttachedCallback&& cb)
    : loop_(NULL),
      needs_destroy_(false),
      attach_cb_(cb) {
  }

 private:
  ::rte_runloop_t* loop_;
  const bool needs_destroy_;
  RteRunloopAttachedCallback attach_cb_;
};

}  // namespace rte
