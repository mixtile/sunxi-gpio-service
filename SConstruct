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

import os

trunk_path = '#'
inc_path = '#'

env = Environment()

libs = ['pthread']

gpio_srcs = [
    "gpio_ctrl.c",
    "gpio_srv.c",
    "main.c",
]

gpio_client = [
    "test_gpio_service_client.c",
]

env.Program("gpio-service-client", gpio_client, LIBS=libs, CPATH=inc_path)

env.Program("gpio-service", gpio_srcs, LIBS=libs, CPATH=inc_path)
