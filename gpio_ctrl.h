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
