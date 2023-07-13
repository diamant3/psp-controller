#include <pspkernel.h>
#include <pspnet_inet.h>
#include <pspnet_apctl.h>
#include <psputility.h>
#include <arpa/inet.h>
#include <string.h>

#include "connection.h"
#include "main.h"

// get connection state
int get_conn_state(int index) 
{
    int conn_state = 0;

    // connect to first network on the list
    int ret = sceNetApctlConnect(index);
    if (ret < 0) { return ret; }

    while (1) 
    {
        // get connection state
        ret = sceNetApctlGetState(&conn_state);
        if (ret < 0) { return ret; }

        // got ip state means succesfull
        if (conn_state == PSP_NET_APCTL_STATE_GOT_IP) { break; }

        sceKernelDelayThread(HALF_DELAY);
    }

    // Display connection status
    pspDebugScreenPrintf(MODULE_NAME ": Connected!\n\n");
    sceKernelDelayThread(FULL_DELAY);
    pspDebugScreenClear();

    return ret;
}

// create a socket
int create_sock(void) 
{
    // create a socket
    int sock = sceNetInetSocket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) { return sock; }

    return sock;
}

// connect to server
int connect_ap(int sock, const char *ip, unsigned short port)
{
    int ret = get_conn_state(1);
    if (ret < 0) { return ret; }

    // set up server struct
    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));

    // set up the server address
    server.sin_addr.s_addr = inet_addr(ip);
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    // connect
    int con = sceNetInetConnect(sock, (struct sockaddr *)&server, sizeof(server));
    if (con < 0) { return con; }

    return con;
}