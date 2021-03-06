#
# Copyright (C) 2015 Focalcrest, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)
LOCAL_SRC_FILES:= gpio_ctrl.c gpio_srv.c main.c 
LOCAL_MODULE:= sunxi-gpio-service 
LOCAL_FORCE_STATIC_EXECUTABLE := true
LOCAL_LDLIB += -lpthread
LOCAL_CFLAGS    := -DGPIO_ANDROID -DSUNXI_A31
LOCAL_STATIC_LIBRARIES := libc 
LOCAL_MODULE_PATH := $(TARGET_OUT_EXECUTABLES)
LOCAL_MODULE_TAGS := debug
include $(BUILD_EXECUTABLE)

#include $(CLEAR_VARS)
#LOCAL_SRC_FILES:= test_gpio_service_client.c 
#LOCAL_MODULE:= sunxi-gpio-client-test 
#LOCAL_FORCE_STATIC_EXECUTABLE := true
#LOCAL_CFLAGS    := -DGPIO_ANDROID -DSUNXI_A31
#LOCAL_STATIC_LIBRARIES := libc 
#LOCAL_MODULE_PATH := $(TARGET_OUT_EXECUTABLES)
#LOCAL_MODULE_TAGS := debug
#include $(BUILD_EXECUTABLE)
