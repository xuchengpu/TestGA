/**
 *
 * Agora Real Time Engagement
 * Created by Zheng, Ender in 2021-08.
 * Copyright (c) 2021 Agora IO. All rights reserved.
 *
 */
#pragma once

#if defined(_WIN32)
#define rte_alignof __alignof
#else
#include <stdalign.h>
#define rte_alignof alignof
#endif
