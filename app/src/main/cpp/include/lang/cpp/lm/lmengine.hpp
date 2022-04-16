/**
 *
 * Agora Real Time Engagement
 * Created by Zheng, Ender in 2021-08.
 * Copyright (c) 2021 Agora IO. All rights reserved.
 *
 */
#pragma once
#include <algorithm>
#include <cstdarg>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <memory>
#include <new>
#include <string>
#include <utility>
#include <vector>

#include "lm/lmengine.h"

namespace agora {
namespace utils {

class LastMileEngine {
 public:
  template <typename ChildClass>
  using LastMileEngineType =
    std::unique_ptr<ChildClass, std::function<void(ChildClass* claz)>>;

  using ConnectionCallback = std::function<void(int succ)>;

  using ConnectionDataCallback = std::function<void()>;

  using ConnectionErrorCallback = std::function<void(int error_code)>;

  using TimerCallback = std::function<void()>;

  using DnsCallback =
    std::function<void(const std::string& if_id,
                       const std::string& host,
                       const std::vector<rte_lm_addr_t>& addresses)>;

  using EdgeListCallback =
    std::function<void(const std::vector<rte_lm_addr_t>& edges)>;

  struct Config {
    bool auto_reconn_from_scratch = false;
    uint32_t conn_from_scratch_times_quota = 1;

    std::string if_id;

    struct dns_cache {
      uint32_t expire_time = -1;
    } dns_cache;

    struct conn {
      bool auto_reconn = true;
      bool fail_at_once = false;
      bool always_select_new = true;
      uint32_t retry_times = 3;
      uint32_t init_timeout_time = 500;
    } conn;

    struct lbes {
      int fetch_timeout = 300;
    } lbes;

    struct edge {
      bool hb_enabled = true;
      int hb_interval = 2000;
      int hb_timeout = 20000;
    } edge;

    std::vector<rte_lm_addr_t> predefined_lbeses;
    std::vector<rte_lm_addr_t> predefined_edges;
  };

 public:  // NOLINT
  template <typename ChildClass, typename... Args>
  static LastMileEngineType<ChildClass> Create(const Config& config,
                                               Args&&... args) {
    return LastMileEngineType<ChildClass>(
      new (std::nothrow) ChildClass(config, std::forward<Args>(args)...),
      [](ChildClass* claz) {
        // make sure engine_ destroy in child class
        // so that all entries in vtable are still valid
        claz->Cleanup();
        delete claz;
      });
  }

 public:  // NOLINT
  virtual ~LastMileEngine() = default;

  rte_lm_result_t Start(const std::string& host, bool reuse_dns_cache) {
    if (engine_ == nullptr) {
      return RTE_LMRESULT_INVALID_PARAMETER;
    }

    return rte_lmengine_start(engine_, host.c_str(), reuse_dns_cache ? 1 : 0);
  }

  rte_lm_result_t Stop() {
    if (engine_ == nullptr) {
      return RTE_LMRESULT_INVALID_PARAMETER;
    }

    return rte_lmengine_stop(engine_);
  }

  rte_lm_result_t GetCurrentState(int* state, const char** desc) const {
    if (engine_ == nullptr) {
      return RTE_LMRESULT_INVALID_PARAMETER;
    }

    return rte_lmengine_current_state(engine_, state, desc);
  }

  bool IsAlive() {
    if (engine_ == nullptr) {
      return false;
    }

    return rte_lmengine_is_alive(engine_) != 0;
  }

 protected:
  explicit LastMileEngine(const Config& c) {
    rte_lm_config_t config;
    config.auto_reconn_from_scratch = c.auto_reconn_from_scratch ? 1 : 0;
    config.conn_from_scratch_times_quota = c.conn_from_scratch_times_quota;
    config.if_id = c.if_id.c_str();
    config.dns_cache.expire_time = c.dns_cache.expire_time;
    config.conn.auto_reconn = c.conn.auto_reconn ? 1 : 0;
    config.conn.always_select_new = c.conn.always_select_new ? 1 : 0;
    config.conn.fail_at_once = c.conn.fail_at_once ? 1 : 0;
    config.conn.init_timeout_time = c.conn.init_timeout_time;
    config.conn.retry_times = c.conn.retry_times;
    config.lbes.fetch_timeout = c.lbes.fetch_timeout;
    config.edge.hb_enabled = c.edge.hb_enabled ? 1 : 0;
    config.edge.hb_timeout = c.edge.hb_timeout;
    config.edge.hb_interval = c.edge.hb_interval;
    config.predefined_lbeses =
      c.predefined_lbeses.empty() ? nullptr : &c.predefined_lbeses[0];
    config.predefined_lbes_count = c.predefined_lbeses.size();
    config.predefined_edges =
      c.predefined_edges.empty() ? nullptr : &c.predefined_edges[0];
    config.predefined_edge_count = c.predefined_edges.size();

    config.delegate.do_create_connection = InteropCreateConnection;
    config.delegate.do_destroy_connection = InteropDestroyConnection;
    config.delegate.do_create_timer = InteropCreateTimer;
    config.delegate.do_destroy_timer = InteropDestroyTimer;
    config.delegate.do_name_resolve = InteropNameResolve;
    config.delegate.do_select_address = InteropSelectAddress;
    config.delegate.do_connect = InteropConnect;
    config.delegate.do_disconnect = InteropDisconnect;
    config.delegate.do_send_heartbeat = InteropSendHeartBeat;
    config.delegate.do_fetch_edge_list = InteropFetchEdgeList;
    config.delegate.do_update_dns = InteropUpdateDnsCache;
    config.delegate.do_count_dns = InteropCountDnsCache;
    config.delegate.do_get_dns = InteropGetDnsCache;
    config.delegate.do_write_log = InteropWriteLog;

    engine_ =
      rte_lmengine_create(&config, reinterpret_cast<rte_lm_user_data_t*>(this));
  }

  virtual rte_lm_connection_t* DoCreateConnection(
    rte_lm_remote_type_t type,
    ConnectionCallback&& conn_callback,
    ConnectionDataCallback&& data_callback,
    ConnectionErrorCallback&& error_callback) {
    (void)type;
    (void)conn_callback;
    (void)data_callback;
    (void)error_callback;
    return nullptr;
  }

  virtual void DoDestroyConnection(rte_lm_connection_t* conn) {
    (void)conn;
  }

  virtual rte_lm_timer_t* DoCreateTimer(int timeout,
                                        bool persist,
                                        TimerCallback&& callback) {
    (void)timeout;
    (void)persist;
    (void)callback;
    return nullptr;
  }

  virtual void DoDestroyTimer(rte_lm_timer_t* timer) {
    (void)timer;
  }

  virtual void DoDnsRequest(const std::string& ifid,
                            const std::string& host,
                            DnsCallback&& callback) {
    (void)ifid;
    (void)host;
    (void)callback;
  }

  virtual void DoSelectAddress(rte_lm_remote_type_t type,
                               const std::vector<rte_lm_addr_t*>& addrs) {
    (void)type;
    (void)addrs;
  }

  virtual void DoConnect(rte_lm_connection_t* conn, const rte_lm_addr_t* addr) {
    (void)conn;
    (void)addr;
  }

  virtual void DoDisconnect(rte_lm_connection_t* conn) {
    (void)conn;
  }

  virtual void DoSendHeartBeat(rte_lm_connection_t* conn) {
    (void)conn;
  }

  virtual void DoFetchEdgeList(rte_lm_connection_t* conn,
                               EdgeListCallback&& callback) {
    (void)conn;
    (void)callback;
  }

  virtual void DoUpdateDnsCache(const std::string& if_id,
                                const std::string& host,
                                const std::vector<rte_lm_addr_t>& addrs,
                                uint32_t expire_time) {
    (void)if_id;
    (void)host;
    (void)addrs;
    (void)expire_time;
  }

  virtual size_t DoCountDnsCache(const std::string& if_id,
                                 const std::string& host) {
    (void)if_id;
    (void)host;
    return 0;
  }

  virtual std::vector<rte_lm_addr_t> DoGetDnsCache(const std::string& if_id,
                                                   const std::string& host) {
    (void)if_id;
    (void)host;
    return std::vector<rte_lm_addr_t>();
  }

  virtual void DoWriteLog(rte_lm_log_level_t level, const std::string& log) {
    (void)level;
    (void)log;
  }

  void Cleanup() {
    if (engine_ != nullptr) {
      rte_lmengine_destroy(engine_);
    }

    engine_ = nullptr;
  }

 private:
  static rte_lm_connection_t* InteropCreateConnection(
    rte_lm_user_data_t* ctx,
    rte_lm_remote_type_t type,
    rte_lm_connection_callback_t c_conn_callback,
    rte_lm_connection_data_callback_t c_data_callback,
    rte_lm_connection_error_callback_t c_error_callback,
    void* arg) {
    auto* engine = reinterpret_cast<LastMileEngine*>(ctx);
    rte_lm_connection_t* ret = nullptr;
    auto conn_callback = [c_conn_callback, &ret, arg](int succ) {
      if (c_conn_callback != nullptr) {
        c_conn_callback(ret, succ, arg);
      }
    };
    auto data_callback = [c_data_callback, &ret, arg]() {
      if (c_data_callback != nullptr) {
        c_data_callback(ret, arg);
      }
    };
    auto error_callback = [c_error_callback, &ret, arg](int error_code) {
      if (c_error_callback != nullptr) {
        c_error_callback(ret, error_code, arg);
      }
    };

    ret = engine->DoCreateConnection(type,
                                     std::move(conn_callback),
                                     std::move(data_callback),
                                     std::move(error_callback));
    return ret;
  }

  static void InteropDestroyConnection(rte_lm_user_data_t* ctx,
                                       rte_lm_connection_t* conn) {
    auto* engine = reinterpret_cast<LastMileEngine*>(ctx);
    engine->DoDestroyConnection(conn);
  }

  static rte_lm_timer_t* InteropCreateTimer(rte_lm_user_data_t* ctx,
                                            int timeout,
                                            int persist,
                                            rte_lm_timer_callback_t c_callback,
                                            void* arg) {
    auto* engine = reinterpret_cast<LastMileEngine*>(ctx);
    rte_lm_timer_t* ret = nullptr;
    auto callback = [c_callback, &ret, arg]() {
      if (c_callback != nullptr) {
        c_callback(ret, arg);
      }
    };
    ret = engine->DoCreateTimer(timeout, persist != 0, std::move(callback));
    return ret;
  }

  static void InteropDestroyTimer(rte_lm_user_data_t* ctx,
                                  rte_lm_timer_t* timer) {
    auto* engine = reinterpret_cast<LastMileEngine*>(ctx);
    engine->DoDestroyTimer(timer);
  }

  static void InteropNameResolve(rte_lm_user_data_t* ctx,
                                 const char* if_id,
                                 const char* host,
                                 rte_lm_name_resove_callback_t c_callback,
                                 void* arg) {
    auto* engine = reinterpret_cast<LastMileEngine*>(ctx);
    auto callback = [c_callback, ctx, arg](
                      const std::string& id,
                      const std::string& h,
                      const std::vector<rte_lm_addr_t>& addresses) {
      if (c_callback != nullptr) {
        c_callback(ctx,
                   id.c_str(),
                   h.c_str(),
                   addresses.empty() ? nullptr : &addresses[0],
                   addresses.size(),
                   arg);
      }
    };
    engine->DoDnsRequest(if_id, host, std::move(callback));
  }

  static void InteropSelectAddress(rte_lm_user_data_t* ctx,
                                   rte_lm_remote_type_t type,
                                   rte_lm_addr_t* addresses,
                                   size_t count) {
    auto* engine = reinterpret_cast<LastMileEngine*>(ctx);
    std::vector<rte_lm_addr_t*> addrs;
    if (addresses != nullptr && count != 0) {
      addrs.reserve(count);
      for (auto i = 0; i < count; i++) {
        addrs.emplace_back(&addresses[i]);
      }
    }
    engine->DoSelectAddress(type, addrs);
  }

  static void InteropConnect(rte_lm_user_data_t* ctx,
                             rte_lm_connection_t* conn,
                             const rte_lm_addr_t* addr) {
    auto* engine = reinterpret_cast<LastMileEngine*>(ctx);
    engine->DoConnect(conn, addr);
  }

  static void InteropDisconnect(rte_lm_user_data_t* ctx,
                                rte_lm_connection_t* conn) {
    auto* engine = reinterpret_cast<LastMileEngine*>(ctx);
    engine->DoDisconnect(conn);
  }

  static void InteropSendHeartBeat(rte_lm_user_data_t* ctx,
                                   rte_lm_connection_t* conn) {
    auto* engine = reinterpret_cast<LastMileEngine*>(ctx);
    engine->DoSendHeartBeat(conn);
  }

  static void InteropFetchEdgeList(rte_lm_user_data_t* ctx,
                                   rte_lm_connection_t* conn,
                                   rte_lm_fetch_edge_list_callback_t c_callback,
                                   void* arg) {
    auto* engine = reinterpret_cast<LastMileEngine*>(ctx);
    auto callback =
      [conn, c_callback, arg](const std::vector<rte_lm_addr_t>& edges) {
        if (c_callback != nullptr) {
          c_callback(conn,
                     edges.empty() ? nullptr : &edges[0],
                     edges.size(),
                     arg);
        }
      };
    engine->DoFetchEdgeList(conn, std::move(callback));
  }

  static void InteropUpdateDnsCache(rte_lm_user_data_t* ctx,
                                    const char* if_id,
                                    const char* host,
                                    const rte_lm_addr_t* addresses,
                                    size_t count,
                                    uint32_t expire_time) {
    auto* engine = reinterpret_cast<LastMileEngine*>(ctx);
    std::vector<rte_lm_addr_t> addrs;
    if (addresses != nullptr && count != 0) {
      addrs.reserve(count);
      for (auto i = 0; i < count; i++) {
        addrs.emplace_back(addresses[i]);
      }
    }
    engine->DoUpdateDnsCache(if_id, host, addrs, expire_time);
  }

  static size_t InteropCountDnsCache(rte_lm_user_data_t* ctx,
                                     const char* if_id,
                                     const char* host) {
    auto* engine = reinterpret_cast<LastMileEngine*>(ctx);
    return engine->DoCountDnsCache(if_id, host);
  }

  static size_t InteropGetDnsCache(rte_lm_user_data_t* ctx,
                                   const char* if_id,
                                   const char* host,
                                   rte_lm_addr_t* addrs,
                                   size_t count) {
    auto* engine = reinterpret_cast<LastMileEngine*>(ctx);
    if (addrs == nullptr || count == 0) {
      return 0;
    }

    auto ret = engine->DoGetDnsCache(if_id, host);
    auto copy_count = std::min(ret.size(), count);
    if (copy_count == 0) {
      return 0;
    }

    memcpy(addrs, &ret[0], sizeof(*addrs) * copy_count);
    return copy_count;
  }

  static void InteropWriteLog(rte_lm_user_data_t* ctx,
                              rte_lm_log_level_t level,
                              const char* fmt,
                              ...) {
    va_list ap;
    va_start(ap, fmt);
    auto size = vsnprintf(nullptr, 0, fmt, ap);
    va_end(ap);
    if (size == 0) {
      return;
    }

    std::string msg;
    msg.resize(size + 1);
    va_start(ap, fmt);
    vsnprintf(const_cast<char*>(msg.c_str()), size + 1, fmt, ap);
    va_end(ap);

    auto* engine = reinterpret_cast<LastMileEngine*>(ctx);
    engine->DoWriteLog(level, msg);
  }

 private:  // NOLINT
  rte_lm_engine_t* engine_;
};

}  // namespace utils
}  // namespace agora