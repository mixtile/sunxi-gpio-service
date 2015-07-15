#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>
#include <stddef.h>
#include <pthread.h>
#include <string.h>

#include "gpio_srv.h"
#include "gpio_defs.h"
#include "gpio_ctrl.h"

#if(GPIO_DEBUG_ENABLE == 1)
#define GPIO_SRV_DEBUG printf
#else
#define GPIO_SRV_DEBUG
#endif

#define GPIO_SRV_P pthread_mutex_lock(&gpdc->mutex)
#define GPIO_SRV_V pthread_mutex_unlock(&gpdc->mutex)

#define GPIO_SRV_BUFFER_SIZE 2048

typedef struct
{
    int init_flag;
    int active_flag;

    gpio_srv_para_t para;
    int server_socket;
    socklen_t server_len;
    struct sockaddr_un server_addr;
    char buffer[GPIO_SRV_BUFFER_SIZE];
    char send_buffer[GPIO_SRV_BUFFER_SIZE];

    // client params
    int client_socket;
    socklen_t client_len;
    struct sockaddr_un client_addr;

    pthread_t task;
    pthread_mutex_t mutex;
}gpio_srv_t, *pgpio_srv_t;

static gpio_srv_t gdc;
static pgpio_srv_t gpdc = &gdc;

int util_debug_raw_hex(u_int8_t* buffer, size_t size)
{
    unsigned int ii = 0;
    printf("\n");
    for(ii=0; ii<size; ii++)
    {
        printf("%02x", buffer[ii]);
        if(0 == (ii+1)%16)
        {
            printf("\n");
        }
    }
    printf("\n");

    return 0;
}

static void gpio_srv_destroy(void)
{
    if(0 == gpdc->init_flag)
    {
        return;
    }

    if(0 != gpdc->task)
    {
        pthread_join(gpdc->task, NULL);
    }

    pthread_mutex_destroy(&gpdc->mutex);

    if(0 != gpdc->server_socket)
    {
        close(gpdc->server_socket);
    }

    memset(gpdc, 0, sizeof(gpio_srv_t));
}

static void* _gpio_srv_func(void *arg)
{
    int active = 0;
    int recv_size = 0;
    struct sockaddr_un client_addr;
    int client_len = 0;

    while(1)
    {
        GPIO_SRV_P;
        active = gpdc->active_flag;
        GPIO_SRV_V;

        if(active)
        {
            memset(gpdc->buffer, 0, sizeof(gpdc->buffer));

            recv_size = recvfrom(gpdc->server_socket, gpdc->buffer, sizeof(gpdc->buffer), 0, (struct sockaddr *)&client_addr, &client_len);

            if(-1 == recv_size)
            {
                GPIO_SRV_DEBUG("srv socket recvfrom failed!");
            }
            else
            {
                if(NULL != gpdc->para.msg_func)
                {
                    gpdc->para.msg_func(gpdc->buffer, &recv_size);
                }

                util_debug_raw_hex(gpdc->buffer, recv_size);
            }
        }
        else
        {
            sleep(10);
        }
    }

    return NULL;
}

int gpio_srv_init(pgpio_srv_para_t ppara)
{
    int ret = 0;
    int length = 0;

    if(1 == gpdc->init_flag || NULL == ppara)
    {
        return -1;
    }

    memset(gpdc, 0, sizeof(gpio_srv_t));

    gpdc->para = *ppara;

    gpdc->server_socket = socket(PF_UNIX, SOCK_STREAM, 0);
    if(gpdc->server_socket < 0)
    {
        GPIO_SRV_DEBUG("srv create socket failed !\n");
        return -1;
    }

    GPIO_SRV_DEBUG("srv create socket success !\n");

    gpdc->server_addr.sun_family = AF_UNIX;

    strcpy(gpdc->server_addr.sun_path, ppara->name);
    unlink(gpdc->server_addr.sun_path);
    length = strlen(gpdc->server_addr.sun_path)+sizeof(gpdc->server_addr.sun_family);

    ret = bind(gpdc->server_socket, (struct sockaddr*)&gpdc->server_addr,
        length);

    listen(gpdc->server_socket, 5);

    chmod(ppara->name, 0777);

    if(ret < 0)
    {
        close(gpdc->server_socket);
        GPIO_SRV_DEBUG("srv bind socket failed !\n");
        return -1;
    }

    GPIO_SRV_DEBUG("srv bind socket success !\n");

    pthread_mutex_init(&gpdc->mutex, NULL);

    if(0 == ppara->mode)
    {
        //create thread
        ret = pthread_create(&gpdc->task, NULL, _gpio_srv_func, NULL);
        if(ret != 0)
        {
            close(gpdc->server_socket);
            pthread_mutex_destroy(&gpdc->mutex);
            GPIO_SRV_DEBUG("srv create thread error: %s\n",strerror(ret));
            return -1;
        }
        GPIO_SRV_DEBUG("srv create thread success !\n");
    }

    gpdc->init_flag = 1;
    gpdc->active_flag = 1;
    return 0;
}


int gpio_srv_process(void)
{
    ssize_t recv_size = 0;
    int active = 0;

    if(0 == gpdc->init_flag)
    {
        GPIO_SRV_DEBUG("srv not inited !\n");
        return -1;
    }

    while(1)
    {
        GPIO_SRV_P;
        active = gpdc->active_flag;
        GPIO_SRV_V;

        if(active)
        {
            memset(gpdc->buffer, 0, sizeof(gpdc->buffer));

            gpdc->client_socket = accept(gpdc->server_socket, (struct sockaddr *)&gpdc->client_addr, &gpdc->client_len);

            recv_size = read(gpdc->client_socket, gpdc->buffer, sizeof(gpdc->buffer));

            if(recv_size > 0)
            {
                util_debug_raw_hex(gpdc->buffer, recv_size);
                if(NULL != gpdc->para.msg_func)
                {
                    gpdc->para.msg_func(gpdc->buffer, &recv_size);
                }
            }

            close(gpdc->client_socket);
        }
    }
}

int gpio_srv_ctrl(int onoff)
{
    if(0 == gpdc->init_flag)
    {
        return -1;
    }
    GPIO_SRV_P;
    gpdc->active_flag = onoff;
    GPIO_SRV_V;
    return 0;
}

int gpio_srv_fini(void)
{
    int ret = 0;
    if(1 == gpdc->init_flag)
    {
        if(0 == gpdc->para.mode)
        {
            pthread_join(gpdc->task, NULL);
        }

        pthread_mutex_destroy(&gpdc->mutex);
        close(gpdc->server_socket);
        gpdc->init_flag = 0;
        gpdc->active_flag = 0;
    }
    return ret;
}



int gpio_srv_resp(char *buf, int len)
{
    int ii = 0;
    for (ii=0; ii < len && ii < GPIO_SRV_BUFFER_SIZE; ii++)
    {
        gpdc->send_buffer[ii] = buf[ii];
    }

    if(gpdc->active_flag)
    {
        util_debug_raw_hex(gpdc->send_buffer, ii);
        write(gpdc->client_socket, gpdc->send_buffer, ii);
    }

    return 0;
}
