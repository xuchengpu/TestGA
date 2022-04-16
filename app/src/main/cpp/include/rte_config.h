/**
 *
 * Agora Real Time Engagement
 * Created by Wei Hu in 2021-07.
 * Copyright (c) 2021 Agora IO. All rights reserved.
 *
 */
#pragma once

#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C extern
#endif

#if !defined(ANYSIZE_ARRAY)
#if defined(_WIN32)
#define ANYSIZE_ARRAY 1
#else  // !defined(_WIN32)
#define ANYSIZE_ARRAY 0
#endif  // defined(_WIN32)
#endif  // !defined(ANYSIZE_ARRAY)

#if defined(_WIN32)

#if !defined(WIN32_LEAN_AND_MEAN)
#define WIN32_LEAN_AND_MEAN
#endif  // WIN32_LEAN_AND_MEAN

#if !defined(AGORA_CALL)
#define AGORA_CALL __cdecl
#endif  // defined(AGORA_CALL)

#if defined(AGORARTE_EXPORT)
#if !defined(AGORA_RTE_API)
#define AGORA_RTE_API EXTERN_C __declspec(dllexport)
#endif  // defined(AGORA_RTE_API)
#else  // !defined(AGORARTE_EXPORT)
#if !defined(AGORA_RTE_API)
#define AGORA_RTE_API EXTERN_C __declspec(dllimport)
#endif  // defined(AGORA_RTE_API)
#endif  // defined(AGORARTE_EXPORT)

#if !defined(AGORA_RTE_PRIVATE_API)
#define AGORA_RTE_PRIVATE_API EXTERN_C
#endif  // defined(AGORA_RTE_PRIVATE_API)

#elif defined(__APPLE__)
#include <TargetConditionals.h>
#if !defined(AGORA_RTE_API)
#define AGORA_RTE_API __attribute__((visibility("default"))) EXTERN_C
#endif  // defined(AGORA_RTE_API)
#if !defined(AGORA_RTE_PRIVATE_API)
#define AGORA_RTE_PRIVATE_API __attribute__((visibility("hidden"))) EXTERN_C
#endif  // defined(AGORA_RTE_PRIVATE_API)
#if !defined(AGORA_CALL)
#define AGORA_CALL
#endif  // defined(AGORA_CALL)
#elif defined(__ANDROID__) || defined(__linux__)
#if !defined(AGORA_RTE_API)
#define AGORA_RTE_API EXTERN_C __attribute__((visibility("default")))
#endif  // defined(AGORA_RTE_API)
#if !defined(AGORA_RTE_PRIVATE_API)
#define AGORA_RTE_PRIVATE_API EXTERN_C __attribute__((visibility("hidden")))
#endif  // defined(AGORA_RTE_PRIVATE_API)
#if !defined(AGORA_CALL)
#define AGORA_CALL
#endif  // defined(AGORA_CALL)
#else  // !defined(__APPLE__) && !defined(__ANDROID__) && !defined(__linux__)
#if !defined(AGORA_RTE_API)
#define AGORA_RTE_API EXTERN_C
#endif  // defined(AGORA_RTE_API)
#if !defined(AGORA_RTE_PRIVATE_API)
#define AGORA_RTE_PRIVATE_API EXTERN_C
#endif  // defined(AGORA_RTE_PRIVATE_API)
#if !defined(AGORA_CALL)
#define AGORA_CALL
#endif  // defined(AGORA_CALL)
#endif  // defined(__APPLE__)

#ifndef INCLUDE_PRIVATE
#define INCLUDE_PRIVATE(x) "src/" x
#endif

#ifndef INCLUDE_PUBLIC
#define INCLUDE_PUBLIC(x) x
#endif

#include <stddef.h>
#include <stdint.h>

#if !defined(ssize_t)
typedef intptr_t ssize_t;
#endif

#if defined(_WIN32) && defined(__clang__) && !defined(__cplusplus)
#define static_assert(x, y)
#endif

