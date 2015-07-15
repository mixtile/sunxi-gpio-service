#ifndef GPIO_SRV_H
#define GPIO_SRV_H

#include "gpio_defs.h"

typedef struct
{
    char name[128];
    gpio_func_t msg_func;
    int mode;
}gpio_srv_para_t, *pgpio_srv_para_t;

int gpio_srv_init(pgpio_srv_para_t ppara);
int gpio_srv_ctrl(int onoff);
int gpio_srv_process(void);
int gpio_srv_resp(char* buf, int len);
int gpio_srv_fini(void);

#endif // GPIO_SRV_H
