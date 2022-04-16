/**
 *
 * Agora Real Time Engagement
 * Created by Zheng, Ender in 2021-08.
 * Copyright (c) 2021 Agora IO. All rights reserved.
 *
 */
#pragma once

#if defined(_WIN32) && !defined(__clang__)

#if !defined(UNUSED)
#define UNUSED
#endif  // !defined(UNUSED)

#if !defined(PURE)
#define PURE
#endif  // !defined(PURE)

#ifndef LIKELY
#define LIKELY(x) (x)
#endif  // !LIKELY

#ifndef UNLIKELY
#define UNLIKELY(x) (x)
#endif  // !UNLIKELY

#else  //  !defined(_WIN32)

#if !defined(UNUSED)
#define UNUSED __attribute__((unused))
#endif  // !defined(UNUSED)

#if !defined(PURE)
#define PURE __attribute__((const))
#endif  // !defined(PURE)

#ifndef LIKELY
#define LIKELY(x) __builtin_expect(!!(x), 1)
#endif  // !LIKELY

#ifndef UNLIKELY
#define UNLIKELY(x) __builtin_expect(!!(x), 0)
#endif  // !UNLIKELY

#if defined(__has_warning)
#define RTE_HAS_WARNING(x) __has_warning(x)
#else
#define RTE_HAS_WARNING(x) 0
#endif

#endif  // defined(_WIN32)
