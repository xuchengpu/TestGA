//
//  EMAPManager.hpp
//  ApTest
//
//  Created by lixiaoming on 2022/3/16.
//

#ifndef EMAPManager_hpp
#define EMAPManager_hpp

#include <stdio.h>
#include <string>
#include <memory>
#include "logging.h"
// 定义tag
#define LOG_TAG "C_TAG"

class EMAPClientManager
{
public:
    EMAPClientManager(const std::string&workPath = "");
    ~EMAPClientManager();
private:
    void clear();
    void initAP();
    void clearAP();
    void initArgus();
    void clearArgus();
    void initFacility();
    bool parse(const std::string& conf);
public:
    //  初始化
    void init();
    int sendDataReport(const std::string& report,int seq);

    int sendConfRequest(bool forceToServer = false);
    // 给ap/arugs回调函数调用的接口
    void onReceiveConf(const std::string& conf);
    void onArgusConnect();
    void onArgusResp(int seq);
    void onArgusDisconnect();
    const std::string appkey() const{return mAppkey;}
    void setVid(const int& vid) {mVid = vid;}
    void setAppId(const std::string& appId) {mAppId = appId;}
    // 重连argus
    void reconnectArgus();
private:
    // 环信appkey
    std::string mAppkey;
    // 置换得到的appId
    std::string mAppId;
    // 区域字段
    std::string mArea;
    // 得到的dnsconfig信息
    std::string mDnsConf;
    // 持久化存储路径，暂时用不到
    std::string mWorkPath;
    // 请求得到的vid
    int32_t mVid;
    // 数据上报开关
    int mEnable;
};

#endif /* EMAPManager_hpp */
