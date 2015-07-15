#include "gpio_defs.h"
#include "gpio_ctrl.h"
#include "gpio_srv.h"
#include "stdio.h"

#if 0
#define GPIO_MAIN_DEBUG
#else
#define GPIO_MAIN_DEBUG printf
#endif

static int gpio_main_srv_handle(void* para, void* data)
{
    int ret = 0;
    static char resp[2] = {0, 0};
    char* msg = (char*)para;
    int len = *(int*)data;

    if(4 == len)
    {
        u_int16_t gpio = msg[1] << 8 | msg[2];
        u_int8_t set_value = msg[3];

        switch(msg[0])
        {
            case GPIO_CMD_EXPORT:
                GPIO_MAIN_DEBUG("export gpio %d", gpio);
                ret = gpio_ctrl_export(gpio);
                break;
            case GPIO_CMD_UNEXPORT:
                GPIO_MAIN_DEBUG("unexport gpio %d", gpio);
                ret = gpio_ctrl_unexport(gpio);
                break;
            case GPIO_CMD_ENABLE_OUT:
                GPIO_MAIN_DEBUG("enable gpio out %d", gpio);
                ret = gpio_ctrl_enable_out(gpio);
                break;
            case GPIO_CMD_ENABLE_IN:
                GPIO_MAIN_DEBUG("enable gpio in %d", gpio);
                ret = gpio_ctrl_enable_out(gpio);
                break;
            case GPIO_CMD_WRITE:
                GPIO_MAIN_DEBUG("write gpio %d to %d", gpio, set_value);
                ret = gpio_ctrl_set_out(gpio, (int)set_value);
                break;
            case GPIO_CMD_READ:
                GPIO_MAIN_DEBUG("read gpio %d", gpio);
                ret = gpio_ctrl_get_in(gpio);
                break;
            default:
                ret = -1;
                break;
        }

        resp[0] = msg[0];
        resp[1] = (char)ret;
        gpio_srv_resp(resp, 2);
    }

    return 0;
}

int serv_daemon()
{
    pid_t pid;

    pid = fork();

    if(pid == -1)
    {
        printf("fork error\n");
    }

    if(setsid() == -1)
    {
        printf("setsid error\n");
    }

    chdir("/");

    umask(0);

    return 0;
}

int main()
{
    int ret = 0;

    gpio_srv_para_t srv_para =
    {
        .name = GPIO_SRV_SOCKET_NAME,
        .msg_func = gpio_main_srv_handle,
        .mode = 1,
    };

    //serv_daemon();

    gpio_ctrl_init();

    ret = gpio_srv_init(&srv_para);
    if(ret != 0)
    {
        gpio_ctrl_fini();
    }

    gpio_srv_process();

    gpio_ctrl_fini();

    return 0;
}
