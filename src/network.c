#ifndef __NETWORK__
#define __NETWORK__

#include <pspnet_apctl.h>
#include <psputility.h>
#include <pspsdk.h>
#include <arpa/inet.h>

#include <string.h>

#define IP_ADDRESS "192.168.4.1"
#define PORT 80
#define ON  "GET /on HTTP/1.1\r\nHost: " IP_ADDRESS "\r\nConnection: close\r\n\r\n"
#define OFF "GET /off HTTP/1.1\r\nHost: " IP_ADDRESS "\r\nConnection: close\r\n\r\n"

void send_network(int sock, int num) {
    // const char ON[64] = "GET /on HTTP/1.1\r\nHost: " IP_ADDRESS "\r\nConnection: close\r\n\r\n";
    // const char OFF[64] = "GET /off HTTP/1.1\r\nHost: " IP_ADDRESS "\r\nConnection: close\r\n\r\n";

    if (num == 1) {
        send(sock, &ON, strlen(ON), 0);
    } else if (num == 0) {
        send(sock, &OFF, strlen(OFF), 0);
    } else {
        return;
    }
}

static int connect_network(int index) {
    int state, prev_state = -1;

    int ret = sceNetApctlConnect(index);
    if (ret < 0) { return -1; }

    while (1) {
        int ret = sceNetApctlGetState(&state);
        if (ret < 0) { return -2; }

        if (state > prev_state) { prev_state = state; }

        if (state == PSP_NET_APCTL_STATE_GOT_IP) { break; }
        sceKernelDelayThread(50 * 1000); // 50ms
    }

    return 0;
}

int init_network(void) {
    sceUtilityLoadNetModule(PSP_NET_MODULE_COMMON);
    sceUtilityLoadNetModule(PSP_NET_MODULE_INET);

    int ret = -1;

    ret = pspSdkInetInit();
    if (ret != 0) {
        return -1;
    }

    connect_network(1);

    struct sockaddr_in name;
    memset(&name, 0, sizeof name);

    int s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) { return -2; }

    name.sin_family = AF_INET;
    name.sin_port = htons(PORT);

    inet_pton(AF_INET, IP_ADDRESS, &(name.sin_addr));

    ret = connect(s, (struct sockaddr *)&name, sizeof name);
    if (ret < 0) { return -3; }

    return s;
}

void destroy_network(void) {
    pspSdkInetTerm();
    sceUtilityUnloadNetModule(PSP_NET_MODULE_COMMON);
    sceUtilityUnloadNetModule(PSP_NET_MODULE_INET);
}

#endif