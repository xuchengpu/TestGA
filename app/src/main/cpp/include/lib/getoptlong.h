/**
 *
 * Agora Real Time Engagement
 * Created by Zheng, Ender in 2021-08.
 * Copyright (c) 2021 Agora IO. All rights reserved.
 *
 */
#pragma once

#include "rte_config.h"

typedef struct rte_opt_long_t {
  const int short_name;
  const char* long_name;
  const int has_param;
  const char* helpe_msg;
} rte_opt_long_t;

/**
 * @brief Print usage message
 * @param exec_name program name
 * @param opts option longs
 */
AGORA_RTE_API void rte_print_help(const char* exec_name,
                                  const rte_opt_long_t* opts);

/**
 * @brief Parse command line arguments
 * @param argc argument count
 * @param argv argument values
 * @param opts option longs
 * @param p pointer to store parsed arguments
 * @return short name of the option, -1 if error
 */
AGORA_RTE_API int rte_getopt_long(int argc,
                                  const char** argv,
                                  const rte_opt_long_t* opts,
                                  char** p);
