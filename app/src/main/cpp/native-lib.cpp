#include <jni.h>
#include <string>
#include "emapclientmanager.hpp"
#include "iostream"
#include <android/log.h>
#include "logging.h"
#include "openssl/crypto.h"
#include <env.h>
#define LOG_TAG "C_TAG"

extern "C" JNIEXPORT jstring JNICALL
Java_com_xcp_testga_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {

    std::string hello = "Hello from C++";

    EMAPClientManager* gEMAPClient=new EMAPClientManager();
    gEMAPClient->init();

    LOGI(LOG_TAG,"Hello from C++");

    LOGI(LOG_TAG,OPENSSL_VERSION_TEXT);

    return env->NewStringUTF(hello.c_str());
}

extern "C" JNIEXPORT
jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
    LOGI(LOG_TAG,"JNI_OnLoad");
    rte_jni_enable(vm,JNI_VERSION_1_2);
    return JNI_VERSION_1_2;
}