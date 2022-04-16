/**
 *
 * Agora Real Time Engagement
 * Created by Zheng, Ender in 2021-08.
 * Copyright (c) 2021 Agora IO. All rights reserved.
 *
 */
#pragma once

#include "rte_config.h"

#include <jni.h>

/**
 * @brief: Enable jni.
 * @param jvm: Java VM object.
 * @param jni_version: Version of jni.
 */
AGORA_RTE_API void rte_jni_enable(JavaVM* jvm, jint jni_version);

/**
 * @brief: Attach to current thread and fetch jni env
 * @return: jni env object of current thread, nullptr if jni not enabled
 * @note: You can assume this function always return valid jni env
 *        if |rte_jni_enable| already called.
 *        Will automatically detach when thread destroying
 */
AGORA_RTE_API JNIEnv* rte_jni_attach_current_thread();