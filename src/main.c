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


void net_connect_ap(int index) {
    int ret = sceNetApctlConnect(index);
    if (ret < 0) {
        pspDebugScreenPrintf("sceNetApctlConnect Error: 0x%x", ret);
        return;
    }

    int conn_state;
    while (1) {
        ret = sceNetApctlGetState(&conn_state);
        if (ret < 0) {
            pspDebugScreenPrintf("sceNetApctlGetState Error: %x", ret);
            return;
        }

        if (conn_state == PSP_NET_APCTL_STATE_GOT_IP) {
            break;
        }

        sceKernelDelayThread(500 * 1000); // 500ms delay
    }

    sceKernelDelayThread(500 * 1000); // 500ms delay
    pspDebugScreenPrintf(MODULE_NAME ": Connected!\n\n");
}

int net_create_sock(const char *ip, unsigned short port) {
    struct sockaddr_in server;
    int sock = sceNetInetSocket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        pspDebugScreenPrintf("sceNetInetSocket Error.");
        return -1;
    }

    memset(&server, 0, sizeof(server));
    server.sin_addr.s_addr = inet_addr(ip);
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    int ret = sceNetInetConnect(sock, (struct sockaddr *) &server, sizeof(server));
    if (ret < 0) {
        pspDebugScreenPrintf("sceNetInetConnect Error.");
        sceNetInetClose(sock);
        return -1;
    }

    return sock;
}

int main(void) {
    SceCtrlData pad;
    SetupExitCallback();
    net_load();
    pspDebugScreenInit();

    net_connect_ap(1);
    int socket = net_create_sock(IP, PORT);
    if (socket < 0) {
        return 1;
    }

    char ON[30] = "GET /led_on HTTP/1.0\r\n\r\n";
    char OFF[30] = "GET /led_off HTTP/1.0\r\n\r\n";

    pspDebugScreenPrintf("==================================\n");
    pspDebugScreenPrintf("psp-controller-client\n");
    pspDebugScreenPrintf("github.com/diamant3/" MODULE_NAME "\n");
    pspDebugScreenPrintf("==================================\n");

    sceCtrlSetSamplingCycle(0);
    sceCtrlSetSamplingMode(PSP_CTRL_MODE_DIGITAL);

    while (1) {
        pspDebugScreenSetXY(0, 15);
        int ret = sceCtrlReadBufferPositive(&pad, 1);
        if (ret < 0) {
            pspDebugScreenPrintf("sceCtrlReadBufferPositive Error: %x", ret);
            break;
        }

        if (pad.Buttons != 0) {
            if (pad.Buttons & PSP_CTRL_CIRCLE) {
                pspDebugScreenPrintf("LED Status:  ON\n");
                sceNetInetSend(socket, ON, strlen(ON), 0);
            }

            if (pad.Buttons & PSP_CTRL_CROSS) {
                pspDebugScreenPrintf("LED Status: OFF\n");
                sceNetInetSend(socket, OFF, strlen(OFF), 0);
            }
        }
    }

    net_unload(socket);
    return 0;
}