/**
 *
 * Agora Real Time Engagement
 * Created by Zheng, Ender in 2021-08.
 * Copyright (c) 2021 Agora IO. All rights reserved.
 *
 */
#pragma once

#include "rte_config.h"

#include "lib/string.h"

/**
 * @brief Dynamicly load a module.
 * @param name The name of the module to load.
 * @param as_local If true, the module will be loaded as RTLD_LOCAL, RTLD_GLOBAL
 *                 otherwise
 * @return The handle of the loaded module, or NULL on failure.
 * @note On iOS and Android, this function do nothing and will assert your app
 *       in debug mode.
 */
AGORA_RTE_API void* rte_module_load(const rte_string_t* name, int as_local);

/**
 * @brief Unload a module.
 * @param handle The handle of the module to unload.
 * @return 0 on success, or -1 on failure.
 */
AGORA_RTE_API int rte_module_close(void* handle);