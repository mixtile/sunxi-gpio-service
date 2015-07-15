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

int _send_cmd(int client, struct sockaddr* server_addr, socklen_t length, char head, u_int16_t gpio, u_int8_t value)
{
    int ret = 0;
    char rcv_buf[2] = {0,0};
    char buf[4] = {head, gpio>>8&0xFF, gpio&0xFF, value};

    connect(client, server_addr, length);

    write(client, buf, sizeof(buf));
    read(client, rcv_buf, 2);
    printf("recv %c, ret %d \n", rcv_buf[0], rcv_buf[1]);

    write(client, buf, sizeof(buf));
    read(client, rcv_buf, 2);
    printf("recv %c, ret %d \n", rcv_buf[0], rcv_buf[1]);

    return 0;
}

int main(void)
{
    socklen_t length = 0;

    #ifdef ILK_ANDROID
    char ser_name[] = "/dev/socket/mixtile.gpio_srv_port";
    #else
    char ser_name[] = "./mixtile.gpio_srv_port";
    #endif

    struct sockaddr_un server_addr;

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, ser_name);
    length = strlen(server_addr.sun_path)+sizeof(server_addr.sun_family);

    int client = socket(PF_UNIX, SOCK_STREAM, 0);
    if(client < 0)
    {
        printf("srv create socket failed !\n");
        return -1;
    }

    printf("srv create socket success !\n");

    _send_cmd(client, (struct sockaddr *)&server_addr, length, 'E', 12, 0);

    _send_cmd(client, (struct sockaddr *)&server_addr, length, 'U', 12, 0);

    _send_cmd(client, (struct sockaddr *)&server_addr, length, 'U', 12, 0);

    _send_cmd(client, (struct sockaddr *)&server_addr, length, 'U', 12, 0);

    return 0;
}
