
#include "emapclientmanager.hpp"

#include "ap/client.h"
#include "argus/client.h"
#include "shim/default.h"

//utils
#include "lang/cpp/io/runloop.h"
#include "lib/packer.h"
#include "lib/rbtree.h"
#include "lib/string.h"
#include "lib/thread.h"
#include "lib/tuple.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#ifdef _WIN32
#include <winsock.h>
#else
#include <sys/socket.h>
#endif

#define STAGING_ENV 1

RTE_PACKER_DECLARE_STRUCT(5,
                          EasemobConfRequest,
                          uint16_t,
                          RTE_TYPE_DEF(RTE_TYPE_PRIMITIVE_U16),
                          type,
                          uint16_t,
                          RTE_TYPE_DEF(RTE_TYPE_PRIMITIVE_U16),
                          uri,
                          rte_string_t,
                          RTE_TYPE_DEF(RTE_TYPE_GENERIC_STRING),
                          appkey,
                          rte_string_t,
                          RTE_TYPE_DEF(RTE_TYPE_GENERIC_STRING),
                          token,
                          rte_rbtree_t,
                          RTE_TYPE_DEF(RTE_TYPE_GENERIC_MAP,
                          RTE_TYPE_DEF(RTE_TYPE_PRIMITIVE_U32),
                          RTE_TYPE_DEF(RTE_TYPE_GENERIC_STRING)),
                          detail);
RTE_PACKER_DECLARE_STRUCT(5,
                          EasemobConfResponse,
                          uint16_t,
                          RTE_TYPE_DEF(RTE_TYPE_PRIMITIVE_U16),
                          type,
                          uint16_t,
                          RTE_TYPE_DEF(RTE_TYPE_PRIMITIVE_U16),
                          uri,
                          uint32_t,
                          RTE_TYPE_DEF(RTE_TYPE_PRIMITIVE_U32),
                          code,
                          rte_string_t,
                          RTE_TYPE_DEF(RTE_TYPE_GENERIC_STRING),
                          conf,
                          rte_rbtree_t,
                          RTE_TYPE_DEF(RTE_TYPE_GENERIC_MAP,
                          RTE_TYPE_DEF(RTE_TYPE_PRIMITIVE_U32),
                          RTE_TYPE_DEF(RTE_TYPE_GENERIC_STRING)),
                          detail);

enum SubProtocolUri {
    kEasemobConfRequest = 31,
    kEasemobConfResponse = 32,
};

enum {kAccessPointServerType = 0};
enum {
    kVid = 1,
    kAppleId = 2,
};

// 声明apClient的回调函数
static void __on_need_reinit(rte_ap_client_t* self, void* arg);

static void __on_timeout(rte_ap_client_t* self, void* arg);

static void __on_packet(rte_ap_client_t* self,
                        const rte_ap_client_general_resp_t* resp,
                        int* need_terminate,
                        void* arg);

// 声明argus的回调函数
static void __argus_on_connect(rte_argus_t* argus,bool result,void* arg);

static void __argus_on_disconnect(rte_argus_t* argus, void* arg);

static void __argus_on_resp(rte_argus_t* argus,int result, uint32_t seq,uint32_t rtt,void* arg);

static void __argus_on_error(rte_argus_t* argus,int error,void* arg);

//static const char* kAppId = "aab8b8f5a8cd4469a63042fcfafe7063";
static const char* kSessionId = "1234567890";
static bool kEnableLog = true;

// apClient的回调函数
static rte_ap_client_callback_t ap_client_callback = {
  .on_need_reinit = __on_need_reinit,
  .on_timeout = __on_timeout,
  .on_packet = __on_packet,
};

// argus的回调函数
static rte_argus_callback_t argus_callback = {
    .on_connect_result = __argus_on_connect,
    .on_disconnected = __argus_on_disconnect,
    .on_response = __argus_on_resp,
    .on_error = __argus_on_error,
};

enum ArgusConnectionState
{
    ArgusConnected = 0,
    ArggusDisconnected
};

static rte_ap_client_t* gApClient = nullptr;
static std::unique_ptr<rte::Runloop> gLoop = nullptr;
static rte_facility_delegate_t* gDefaultShim = nullptr;
static rte_ap_client_factory_t* gApFactory = nullptr;
static rte_agtp_factory_t* gAgtpFactory = nullptr;
static rte_argus_t* gArgus = nullptr;
static ArgusConnectionState gArgusConnectState = ArggusDisconnected;
static EMAPClientManager* gEMAPClient = nullptr;
static std::mutex gMutex;

// callback functions
void* RunLoop(void *arg) {
     LOGI(LOG_TAG, "start run loop");
    reinterpret_cast<rte::Runloop*>(arg)->Run();
    return nullptr;
}

static void __on_packet(rte_ap_client_t* self,
                        const rte_ap_client_general_resp_t* resp,
                        int* need_terminate,
                        void* arg) {
    LOGI(LOG_TAG, "__on_packet");
     LOGI(LOG_TAG, __func__ ," uri:" , resp->uri);
    if(resp && resp->uri == kEasemobConfResponse) {
        rte_EasemobConfResponse_struct_t easemobResp;
        rte_EasemobConfResponse_struct_init(&easemobResp);
        auto packed = rte_packed_data_create((const uint8_t*)resp->payload,
                                             resp->payload_size,
                                             RTE_PACKED_SIZE_U16);
        rte_packer_unpack(packed, &easemobResp.base, RTE_PACKER_LITTLE_ENDIAN);
         LOGI(LOG_TAG, "EasemobConfResponse code:", easemobResp.code);
        std::string conf(easemobResp.conf.buf);
         LOGI(LOG_TAG, "EasemobConfResponse conf:" , easemobResp.conf.buf);
        // response 中的detail定义
        rte_rbtree_foreach(&easemobResp.detail, itor) {
            void* k = rte_rbnode_key(itor);
            void* v = rte_rbnode_value(itor);
            if(k && v) {
                rte_string_t* str = (rte_string_t*)v;
                std::string buf(str->buf);
                if(k == (void*)kVid) {
                    int tmp = atoi(buf.c_str());
                    gEMAPClient->setVid(tmp);
                }
                if(k == (void*)kAppleId) {
                    // appId
                    gEMAPClient->setAppId(buf);
                }
            }
            
        }
        rte_EasemobConfResponse_struct_deinit(&easemobResp);
        std::lock_guard<std::mutex> lock(gMutex);
        if(gEMAPClient) {
            gEMAPClient->onReceiveConf(conf);
        }
    }
  
  if (need_terminate) {
    *need_terminate = 1;
  }
}

static void LogEnable(rte_facility_delegate_t* d, int enabled) {
  kEnableLog = enabled != 0;
}

static void LogWriteV(rte_facility_delegate_t* d,
                                rte_tp_log_level_t level,
                                const char* fmt,
                                va_list ap) {
    if(kEnableLog) {
        int len = vsnprintf(nullptr, 0, fmt, ap);
        std::string str(len+1,0);
        vsnprintf((char*)str.c_str(), len+1, fmt, ap);
//         LOGI(LOG_TAG, str);
    }
}

static void __on_need_reinit(rte_ap_client_t* self, void* arg) {
     LOGI(LOG_TAG, __func__);
    std::lock_guard<std::mutex> lock(gMutex);
    if(gEMAPClient)
    {
        gEMAPClient->init();
    }
    
}

static void __on_timeout(rte_ap_client_t* self, void* arg) {
     LOGI(LOG_TAG, __func__);
}

static void __argus_on_connect(rte_argus_t* argus,bool result,void* arg)
{
     LOGI(LOG_TAG, "__argus_on_connect");
    gArgusConnectState = ArgusConnected;
    std::lock_guard<std::mutex> lock(gMutex);
    if(gEMAPClient) {
        gEMAPClient->onArgusConnect();
    }
}

static void __argus_on_disconnect(rte_argus_t* argus, void* arg)
{
     LOGI(LOG_TAG, __func__);
    gArgusConnectState = ArggusDisconnected;
    std::lock_guard<std::mutex> lock(gMutex);
    if(gEMAPClient) {
        gEMAPClient->onArgusDisconnect();
    }
}

static void __argus_on_resp(rte_argus_t* argus,int result, uint32_t seq,uint32_t rtt,void* arg)
{
     LOGI(LOG_TAG, __func__ ," seq:" , seq , " rtt:" , rtt);
    std::lock_guard<std::mutex> lock(gMutex);
    if(gEMAPClient) {
        gEMAPClient->onArgusResp(seq);
    }
}

static void __argus_on_error(rte_argus_t* argus,int error,void* arg)
{
     LOGI(LOG_TAG, __func__ , " error:" , error);
    if(gEMAPClient){
        gEMAPClient->reconnectArgus();
    }
}

EMAPClientManager::EMAPClientManager(const std::string&workPath):
  mAppkey(""),
  mArea(""),
  mWorkPath(workPath),
  mAppId(""),
  mVid(0)
{
    if(!gLoop) {
        gLoop = rte::Runloop::Create(RTE_RUNLOOP_EVENT2);
        
    }
    //loadConfFromLocal();
}

EMAPClientManager::~EMAPClientManager()
{

    clear();
    if(gDefaultShim) {
        rte_facility_delegate_destroy(gDefaultShim);
        gDefaultShim = nullptr;
    }
    if(gLoop) {
        gLoop->PostTaskTail([] {
            gLoop->Stop();
        });
    }
    std::lock_guard<std::mutex> lock(gMutex);
    gEMAPClient = nullptr;
}

void EMAPClientManager::init()
{
    std::string appId="easemob-demo#easeim";
    std::string area="CN";
    // 存储变量
    mAppkey = appId;
    mArea = area;
    // 初始化
    {
        std::lock_guard<std::mutex> lock(gMutex);
        gEMAPClient = this;
    }
    initFacility();
    initAP();
    sendConfRequest();
}

void EMAPClientManager::initFacility()
{
    rte_facility_config_t config;
    config.appid = mAppkey.c_str();
    config.install_id = "5678";
    config.area = mArea.c_str();
    config.version = "3.7.200";
    config.logger.enable = LogEnable;
    //config.logger.enable = [](rte_facility_delegate_t* d, int enabled){};
    config.logger.vwrite = LogWriteV;
    config.port_rang_start = 8086;
    config.port_rang_end = 20086;
    config.port_list = nullptr;
    config.port_list_size = 0;
    if(!gDefaultShim)
    {
        gDefaultShim = rte_facility_delegate_create_default(gLoop->get_c_loop(), &config);
        rte_thread_t* major_ = rte_thread_create(nullptr, RunLoop, gLoop.get());
    }
}

void EMAPClientManager::initAP() {
    // 初始化ap
    if(gApClient)
        clearAP();
    gLoop->PostTaskTail([&](){
        if(!gApFactory) {
            gApFactory = rte_ap_client_factory_create(gDefaultShim, AF_INET, nullptr);
        }
        if(!gApClient) {
            gApClient = rte_ap_client_create(gApFactory,
                                                &ap_client_callback,
                                                mAppkey.c_str(),
                                                kSessionId,
                                                nullptr);
    #ifdef STAGING_ENV
            const char *iplist[1] = {"114.236.137.40"};
            rte_ap_client_set_designated_servers(gApClient, iplist, 1, 8443);
    #endif
            rte_ap_client_set_request_strategy(gApClient, RTE_REQUESTSTRATEGY_DEFAULT);
        }
    });
}

int EMAPClientManager::sendConfRequest(bool forceToServer) {

    gLoop->PostTaskTail([&] {
        rte_EasemobConfRequest_struct_t req;
        rte_EasemobConfRequest_struct_init(&req);
        req.uri = kEasemobConfRequest;
        req.type = kAccessPointServerType;
        rte_string_init_with_value(&req.appkey, mAppkey.c_str());
        rte_string_init_with_value(&req.token, mAppkey.c_str());
        rte_packed_data_t* packed = rte_packed_data_create_empty(RTE_PACKED_SIZE_U16);
        rte_packer_pack(&req.base, packed, RTE_PACKER_LITTLE_ENDIAN);
        if(gApClient) {
            const char* data = (const char*)rte_packed_data_get_data(packed);
            size_t len = rte_packed_data_get_length(packed);
            rte_ap_client_send_request(gApClient,
                                       kEasemobConfRequest,
                                       0,
                                       data,
                                       len,
                                       RTE_TP_NETWORK_TYPE_IPV4,
                                       0);
            LOGI(LOG_TAG, "send rte_ap_client_send_request");
            
        }
      rte_EasemobConfRequest_struct_deinit(&req);
    });
    LOGI(LOG_TAG, "send EasemobConfRequest");
    return 0;
}

void EMAPClientManager::initArgus()
{
     LOGI(LOG_TAG, "initArgus");
    gLoop->PostTaskTail([&](){
        if(!gAgtpFactory) {
            gAgtpFactory = rte_agtp_factory_create(gDefaultShim);
        }
        if(!gArgus) {
    //        rte_argus_link_config_t link_config;
    //        rte_argus_set_link_config(gArgus, &link_config);
            gArgus = rte_argus_create(gApFactory, gAgtpFactory, &argus_callback, mAppId.c_str(), kSessionId, nullptr);
            rte_argus_config_t argusConfig;
            argusConfig.area_code = mArea.c_str();
            argusConfig.use_aut = true;
            int ret = rte_argus_connect(gArgus, &argusConfig);
        }
    });
}

void EMAPClientManager::clear()
{
    clearArgus();
    clearAP();
}

void EMAPClientManager::reconnectArgus()
{
    // 启动定时器
//    if(!mTimerPtr->isRunning()) {
//        EMTimerCallback timerCB = [&](EMTimer::Result result) {
//            if(result != EMTimer::CANCEL) {
//                if(gArgusConnectState == ArgusConnected) {
//                    return mTimerPtr->cancel();
//                }
//                gLoop->PostTaskTail([&]{
//                     LOGI(LOG_TAG, "reconnect argus...");
//                    rte_argus_config_t argusConfig;
//                    argusConfig.area_code = mArea.c_str();
//                    argusConfig.use_aut = true;
//                    int ret = rte_argus_connect(gArgus, &argusConfig);
//                });
//
//            }
//        };
//        mTimerPtr->start(10*1000, timerCB,true);
//    }
}

void EMAPClientManager::clearArgus()
{
    if(gLoop) {
        gLoop->PostTaskTail([]{
            if(gArgus) {
                rte_argus_destroy(gArgus);
                gArgusConnectState = ArggusDisconnected;
                gArgus = nullptr;
            }
            if(gAgtpFactory) {
                rte_agtp_factory_destroy(gAgtpFactory);
                gAgtpFactory = nullptr;
            }
        });
    }
}
void EMAPClientManager::clearAP()
{
    if(gLoop) {
        gLoop->PostTaskTail([]{
            if(gApClient) {
                rte_ap_client_destroy(gApClient);
                gApClient = nullptr;
            }
            if(gApFactory) {
                rte_ap_client_factory_destroy(gApFactory);
                gApFactory = nullptr;
            }
        });
    }
}

int EMAPClientManager::sendDataReport(const std::string &report,int seq) {
    if(gArgusConnectState != ArgusConnected) {
        return -1;
    }
     LOGI(LOG_TAG, __func__ ," seq:" ,seq);
    if(gLoop) {
        gLoop->PostTaskTail([report,seq] {
            rte_packed_data_t* packed = rte_packed_data_create((const uint8_t*)report.c_str(),report.size(),RTE_PACKED_SIZE_U16);
            const uint8_t* data = (const uint8_t*)rte_packed_data_get_data(packed);
            size_t lenth = rte_packed_data_get_length(packed);
            rte_argus_qos_level_t level = RTE_ARGUS_QOS_HIGH;
#ifdef STAGING_ENV
            level = static_cast<rte_argus_qos_level_t>(50);
#endif
            rte_argus_send(gArgus, RTE_ARGUS_FORMAT_PROTOBUF, RTE_ARGUS_SUB_FORMAT_DEFAULT, level, seq, 0,data,lenth);
        });
    }
    
    return 0;
}

void EMAPClientManager::onReceiveConf(const std::string &conf)
{
//    if(!conf.empty()) {
//        mDnsConf = conf;
//        saveConf();
//    }
    bool ret = parse(conf);
    if(ret) {
        
    }
    // 收到argus需要的appid，开始初始化
    initArgus();
}

void EMAPClientManager::onArgusConnect()
{
}

void EMAPClientManager::onArgusResp(int seq)
{
}

void EMAPClientManager::onArgusDisconnect()
{
}

bool EMAPClientManager::parse(const std::string &conf)
{
    // TODO 需要从这里拿到数据上报开关
    return false;
}
