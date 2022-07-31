#include <pspkernel.h>
#include <pspdebug.h>
#include <pspctrl.h>
#include <pspnet_inet.h>
#include <pspnet_apctl.h>
#include <pspnet.h>
#include <psputility.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#include "callback.h"

#define MODULE_NAME "psp-controller"
#define PORT 80
#define IP "192.168.4.1"

PSP_MODULE_INFO(MODULE_NAME, PSP_MODULE_USER, 1, 0);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER);

void net_load(void) {
    sceUtilityLoadNetModule(PSP_NET_MODULE_COMMON);
    sceUtilityLoadNetModule(PSP_NET_MODULE_INET);
    sceNetInit(0x10000, 0x20, 0x1000, 0x20, 0x1000);
    sceNetInetInit();
    sceNetApctlInit(0x2000, 0x14);
}

void net_unload(int sock) {
    sceNetInetClose(sock);
    sceNetApctlTerm();
    sceNetInetTerm();
    sceNetTerm();
    sceUtilityUnloadNetModule(PSP_NET_MODULE_COMMON);
    sceUtilityUnloadNetModule(PSP_NET_MODULE_INET);
}

void net_connect_ap(int ap) {
    int connected = 0;
    int prev_conn_state = 0;
    int err = -1;

    err = sceNetApctlConnect(ap);
    if (err < 0) {
        pspDebugScreenPrintf("sceNetApctlConnect Error: 0x%8x", err);
    }

    while (!connected) {
        int conn_state = 0;

        err = sceNetApctlGetState(&conn_state);
        if (err < 0) {
            pspDebugScreenPrintf("sceNetApctlGetState Error: %8x", err);
            break;
        }

        if (conn_state > prev_conn_state) {
            pspDebugScreenPrintf("Connection state: %d/100%.\n", (conn_state * 20));
            switch (conn_state) {
                case PSP_NET_APCTL_STATE_GOT_IP:
                    connected = 1;
                break;
            }
            prev_conn_state = conn_state;
        }

        sceKernelDelayThread(1000 * 1000); // 1s
        pspDebugScreenClear();
    }
}

int net_create_sock(unsigned short port) {
    int sock = 0;
    struct sockaddr_in server;

    sock = sceNetInetSocket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        return sock;
    }

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    inet_pton(AF_INET, IP, &server.sin_addr.s_addr);

    sceNetInetConnect(sock, (struct sockaddr *) &server, sizeof(server));
    
    return sock;
}

void net_get_ip(void) {
    union SceNetApctlInfo ip_info;
    // Get IP address
    if (sceNetApctlGetInfo(PSP_NET_APCTL_INFO_IP, &ip_info) < 0) {
        pspDebugScreenPrintf("Status: NOT CONNECTED! \n\n");
    } else {
        pspDebugScreenPrintf("Status: CONNECTED!\n\n");
    }
}

int main(void) {
    pspDebugScreenInit();
    SetupExitCallback();
    net_load();

    int socket = 0;
    SceCtrlData pad;

    net_connect_ap(1);
    socket = net_create_sock(PORT);
    net_get_ip();

    char ON[] = "1\r\n\r\n";
    char OFF[] = "0\r\n\r\n";

    pspDebugScreenPrintf("==================================\n");
    pspDebugScreenPrintf("psp-controller client\n");
    pspDebugScreenPrintf("github.com/diamant3/" MODULE_NAME "\n");
    pspDebugScreenPrintf("==================================\n");

    while(!done) {
        pspDebugScreenSetXY(0, 9);
        sceCtrlReadBufferPositive(&pad, 1);

        if (pad.Buttons != 0) {
            if (pad.Buttons & PSP_CTRL_CIRCLE) {
                pspDebugScreenPrintf("LED Status:  ON\n");
                sceNetInetSend(socket, &ON, sizeof(ON), 0);
            }

            if (pad.Buttons & PSP_CTRL_CROSS) {
                pspDebugScreenPrintf("LED Status: OFF\n");
                sceNetInetSend(socket, &OFF, sizeof(OFF), 0);
            }
        }
    }

    net_unload(socket);
    sceKernelExitGame();
    return 0;
}