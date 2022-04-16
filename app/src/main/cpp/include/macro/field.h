/**
 *
 * Agora Real Time Engagement
 * Created by Zheng, Ender in 2021-08.
 * Copyright (c) 2021 Agora IO. All rights reserved.
 *
 */
#pragma once

#ifndef OFFSET_OF
#define OFFSET_OF(x, y) ((char*)(&(((x*)0)->y)))
#endif

#ifndef MEMBER_SIZE
#define MEMBER_SIZE(type, member) sizeof(((type*)0)->member)
#endif

#ifndef CONTAINER_OF_FROM_OFFSET
#define CONTAINER_OF_FROM_OFFSET(field_ptr, offset) \
  ((void*)((char*)(field_ptr) - (offset)))
#endif

#ifndef CONTAINER_OF_FROM_FIELD
#define CONTAINER_OF_FROM_FIELD(field_ptr, base_type, field_name) \
  CONTAINER_OF_FROM_OFFSET(field_ptr, OFFSET_OF(base_type, field_name))
#endif

#ifndef FIELD_OF_FROM_OFFSET
#define FIELD_OF_FROM_OFFSET(base_ptr, offset) \
  ((void*)((char*)(base_ptr) + (offset)))
#endif
