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
