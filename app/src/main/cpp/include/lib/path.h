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
 * @brief Get current working directory.
 * @return Current working directory.
 */
AGORA_RTE_API rte_string_t* rte_path_get_cwd();

/**
 * @brief Get directory of current module
 * @param addr The address of which it belong to
 * @return Directory of current module
 */
AGORA_RTE_API rte_string_t* rte_path_get_module_path(const void* addr);

/**
 * @brief Get directory of executable binray
 * @return Directory of current executable binray
 */
AGORA_RTE_API rte_string_t* rte_path_get_executable_path();

/**
 * @brief Convert a path into system preferred format.
 *        For example on Windows the path "c:/aaa/bbb" will be
 *        converted to "C:\\aaa\\bbb".
 * @param path Path to be converted.
 * @return 0 if success, -1 if failed.
 */
AGORA_RTE_API int rte_path_to_system_flavor(rte_string_t* path);

/**
 * @brief Get dir of a path.
 *        For example /aaa/bbb -> /aaa
 * @param path Path of which dir is to be got.
 * @return Dir of the path.
 */
AGORA_RTE_API rte_string_t* rte_path_get_dir(const rte_string_t* path);

/**
 * @brief Get file name of a path.
 *        For example /aaa/bbb -> bbb
 * @param path Path of which file name is to be got.
 * @return File name of the path.
 */
AGORA_RTE_API rte_string_t* rte_path_get_file(const rte_string_t* path);

/**
 * @brief Get absolute path of a path.
 *        For example /aaa/./../aaa/bbb -> /aaa/bbb
 * @param path Path of which absolute path is to be got.
 * @return Absolute path of the path.
 */
AGORA_RTE_API rte_string_t* rte_path_realpath(const rte_string_t* path);

/**
 * @brief Check whether a path is a dir
 * @param path Path to be checked.
 * @return 1 if path is a dir, 0 if not.
 */
AGORA_RTE_API int rte_path_is_dir(const rte_string_t* path);

/**
 * @brief Check whether a path ends with "." or ".."
 * @param path Path to be checked.
 * @return 1 if path ends with  "." or "..", 0 if not.
 */
AGORA_RTE_API int rte_path_is_special_dir(const rte_string_t* path);

/**
 * @brief Check whether a path is a a shared library
 * @param path Path to be checked.
 * @return 1 if path is a shared library, 0 if not.
 */
AGORA_RTE_API int rte_path_is_shared_library(const rte_string_t* path);

/**
 * @brief Make dirs for a path
 * @param path Path to be made.
 * @param recursive Whether to make dirs recursively.
 * @return 0 if success, -1 if failed.
 */
AGORA_RTE_API int rte_path_mkdir(const rte_string_t* path, int recursive);

/**
 * @brief Check whether a path exists
 * @param path Path to be checked.
 * @return 1 if path exists, 0 if not.
 */
AGORA_RTE_API int rte_path_exists(const char* path);

typedef struct rte_path_itor_t rte_path_itor_t;

typedef struct rte_dir_fd_t rte_dir_fd_t;

/**
 * @brief Open a path for iteration
 * @param path Path to be opened.
 * @return Iterator of the path. NULL if error happens
 */
AGORA_RTE_API rte_dir_fd_t* rte_path_open_dir(const rte_string_t* path);

/**
 * @brief Close an iterator of a path
 * @param dir Iterator to be closed.
 * @return 0 if success, -1 if failed.
 */
AGORA_RTE_API int rte_path_close_dir(rte_dir_fd_t* dir);

/**
 * @brief Get the first entry of an iterator
 * @param dir Iterator to be iterated.
 * @return The first entry of the iterator. NULL if error happens
 */
AGORA_RTE_API rte_path_itor_t* rte_path_get_first(rte_dir_fd_t* dir);

/**
 * @brief Get the next entry of an iterator
 * @param dir Iterator to be iterated.
 * @return The next entry of the iterator. NULL if no more entries
 */
AGORA_RTE_API rte_path_itor_t* rte_path_get_next(rte_path_itor_t* itor);

/**
 * @brief Get the short name of an entry
 * @param itor Entry to be got.
 * @return The short name of the entry. NULL if error happens
 */
AGORA_RTE_API rte_string_t* rte_path_itor_get_name(rte_path_itor_t* itor);

/**
 * @brief Get the full name of an entry
 * @param itor Entry to be got.
 * @return The full name of the entry. NULL if error happens
 */
AGORA_RTE_API rte_string_t* rte_path_itor_get_full_name(rte_path_itor_t* itor);