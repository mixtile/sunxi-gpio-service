/*
 * Copyright (C) 2015 Focalcrest, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
#ifndef GPIO_DEFS_H
#define GPIO_DEFS_H

#include <sys/types.h>

#define GPIO_DEBUG_ENABLE 1

/* service macros */
#ifdef GPIO_ANDROID
#define GPIO_SRV_SOCKET_NAME         "/dev/socket/mixtile.gpio_srv_port"
#else
#define GPIO_SRV_SOCKET_NAME         "./mixtile.gpio_srv_port"
#endif

#define GPIO_CMD_EXPORT         'E'
#define GPIO_CMD_UNEXPORT       'U'
#define GPIO_CMD_ENABLE_OUT     'O'
#define GPIO_CMD_ENABLE_IN      'I'
#define GPIO_CMD_WRITE          'W'
#define GPIO_CMD_READ           'R'

typedef struct
{
    u_int8_t cmd;
    u_int16_t gpio;
}stGPIO_CMD_T, *pstGPIO_CMD_T;

typedef int (*gpio_func_t)(void* para, void* data);

#endif // GPIO_DEFS_H
