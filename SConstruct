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
