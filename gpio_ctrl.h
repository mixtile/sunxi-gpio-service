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
 
#ifndef GPIO_CTRL_H
#define GPIO_CTRL_H
#include <sys/types.h>

int gpio_ctrl_init();
int8_t gpio_ctrl_export(int io);
int8_t gpio_ctrl_unexport(int io);
int8_t gpio_ctrl_enable_out(int io);
int8_t gpio_ctrl_enable_in(int io);
int8_t gpio_ctrl_set_out(int io, int bon);
int8_t gpio_ctrl_get_in(int io);
int gpio_ctrl_fini();

#endif // GPIO_CTRL_H
