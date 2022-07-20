#include <pspkernel.h>
#include <pspdebug.h>
#include <pspctrl.h>
#include <psputility.h>
#include <pspnet.h>
#include <pspnet_inet.h>
#include <pspnet_apctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#include "callback.h"

#define print pspDebugScreenPrintf
#define MODULE_NAME "psp-controller"
#define PORT 80
#define IP "192.168.4.1"

PSP_MODULE_INFO(MODULE_NAME, 0, 0, 2);
PSP_MAIN_THREAD_ATTR(0);

int sock = 0;

int connect_ap(int access_point) {
    int connection_state = 0;
    int connection_state_previous = 0;

    if (sceUtilityLoadNetModule(PSP_NET_MODULE_COMMON) < 0) {
        return -1;
    }

    if (sceUtilityLoadNetModule(PSP_NET_MODULE_INET) < 0) {
        return -1;
    }

    if (sceNetInit(65536, 32, 2048, 32, 2048) < 0) {
        return -1;
    }

    if (sceNetInetInit() < 0) {
        return -1;
    }

    if (sceNetApctlInit(0x2000, 20) < 0) {
        return -1;
    }

    int connection_status = sceNetApctlConnect(access_point);
    if (connection_status < 0) {
        print(MODULE_NAME ": sceNetApctlConnect status %08x\n", connection_status);
        return connection_status;
    }

    while (!connection_state) {
        int status = sceNetApctlGetState(&connection_state);
        if (status < 0) {
            print(MODULE_NAME ": sceNetApctlGetState status %x\n", status);
            break;
        }

        if (connection_state != connection_state_previous) {
            switch (connection_state) {
                case PSP_NET_APCTL_STATE_DISCONNECTED:
                    print(MODULE_NAME ": DISCONNECTED!\n");
                break;

                case PSP_NET_APCTL_STATE_SCANNING:
                    print(MODULE_NAME ": SCANNING...\n");
                break;

                case PSP_NET_APCTL_STATE_JOINING:
                    print(MODULE_NAME ": JOINING...\n");
                break;

                case PSP_NET_APCTL_STATE_GETTING_IP:
                    print(MODULE_NAME ": OBTAINING IP ADDRESS...\n");
                break;

                case PSP_NET_APCTL_STATE_GOT_IP:
                    print(MODULE_NAME ": CONNECTED!\n");
                    connection_state = 1;
                break;

                case PSP_NET_APCTL_STATE_EAP_AUTH:
                    print(MODULE_NAME ": AUTHENTICATING...\n");;
                break;

                case PSP_NET_APCTL_STATE_KEY_EXCHANGE:
                    print(MODULE_NAME ": EXCHANGING KEYS...\n");
                break;
            }

            connection_state_previous = connection_state;
        }

        sceKernelDelayThread(50 * 1000); // 50ms
    }

    return 0;
}

int make_socket(unsigned short port) {
    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    inet_pton(AF_INET, IP, &server.sin_addr.s_addr);

    sock = sceNetInetSocket(PF_INET, SOCK_STREAM, 0);
    if (sock > 0) {
        sceNetInetConnect(sock, (struct sockaddr *) &server, sizeof(server));
    }

    return sock;
}

int main(int argc, char *argv[]) {
    done = 0;
    SceCtrlData pad;

    pspDebugScreenInit();
    SetupExitCallback();

    int res = connect_ap(1);
    if (res < 0) {
        return res;
    }

    sock = make_socket(PORT);
    if (sock < 0) {
        return sock;
    }

    pspDebugScreenClear();
    print("github.com/diamant3/" MODULE_NAME);
    while(!done) {
        pspDebugScreenSetXY(0, 2);
        sceCtrlReadBufferPositive(&pad, 1);
        sceCtrlSetSamplingCycle(0);
        sceCtrlGetSamplingMode(PSP_CTRL_MODE_DIGITAL);

        if (pad.Buttons != 0) {
            if (pad.Buttons & PSP_CTRL_CIRCLE) {
                const char led_on[] = "1\n";
                print("LED ON! \n");
                sceNetInetSend(sock, &led_on, sizeof(led_on), 0);
            }

            if (pad.Buttons & PSP_CTRL_CROSS) {
                const char led_off[] = "0\n";
                print("LED OFF! \n");
                sceNetInetSend(sock, &led_off, sizeof(led_off), 0);
            }
        }
    }

    sceNetInetClose(sock);
    sceNetApctlTerm();
    sceNetInetTerm();
    sceNetTerm();
    sceUtilityUnloadNetModule(PSP_NET_MODULE_COMMON);
    sceUtilityUnloadNetModule(PSP_NET_MODULE_INET);
    sceKernelExitGame();

    return 0;
}