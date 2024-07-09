#include <pspkernel.h>
#include <pspdebug.h>
#include <pspctrl.h>
#include <pspsdk.h>
#include <pspnet_apctl.h>
#include <psputility.h>
#include <arpa/inet.h>

#include <string.h>

#define MODULE_NAME "PSP-CONTROLLER"
#define IP_ADDRESS "192.168.4.1"
#define PORT 80

PSP_MODULE_INFO(MODULE_NAME, PSP_MODULE_USER, 0, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);

PSP_HEAP_THRESHOLD_SIZE_KB(4096);
PSP_HEAP_SIZE_KB(-4096);
PSP_MAIN_THREAD_STACK_SIZE_KB(4096);

int exit_cb(int arg0, int arg1, void *common) {
    sceKernelExitGame();
    return 0;
}

int thread_cb(SceSize args, void *argp) {
    int cbid = sceKernelCreateCallback("exit_cb", exit_cb, NULL);

    sceKernelRegisterExitCallback(cbid);
    sceKernelSleepThreadCB();
    return 0;
}

int setup_cb(void) {
    SceUID thid = sceKernelCreateThread("setup_cb", thread_cb, 0x10, 0x1000, PSP_THREAD_ATTR_USER, NULL);
    sceKernelStartThread(thid, 0, NULL);
    return thid;
}

int connect_apctl(int config) {
    int state;
    int last_state = -1;
    int error;

    /* Connect to first wifi profile */
    error = sceNetApctlConnect(config);
    if (error < 0) {
        pspDebugScreenPrintf(MODULE_NAME ": sceNetApctlConnect returns %08x\n", error);
        return 0;
    }

    pspDebugScreenPrintf(MODULE_NAME ": Connecting...\n");
    while (1) {
        error = sceNetApctlGetState(&state);
        if (error < 0) {
            pspDebugScreenPrintf(MODULE_NAME ": sceNetApctlGetState returns $%08x\n", error);
			break;
        }

        if (state > last_state) {
            pspDebugScreenPrintf("connection state %d of 4\n", state);
            last_state = state;
        }

        if (state == PSP_NET_APCTL_STATE_GOT_IP) { break; }
        sceKernelDelayThread(50 * 1000); // 50ms
    }
}

int main(void) {
    setup_cb();
    sceUtilityLoadNetModule(PSP_NET_MODULE_COMMON);
    sceUtilityLoadNetModule(PSP_NET_MODULE_INET);
    pspDebugScreenInit();

    int sock;
    int error;
    SceCtrlData pad;
    struct sockaddr_in name;
    const char ON[64] = "GET /on HTTP/1.1\r\nHost: " IP_ADDRESS "\r\nConnection: close\r\n\r\n";
    const char OFF[64] = "GET /off HTTP/1.1\r\nHost: " IP_ADDRESS "\r\nConnection: close\r\n\r\n";

    memset(&name, 0, sizeof(name));
    sceCtrlSetSamplingCycle(0);
    sceCtrlSetSamplingMode(PSP_CTRL_MODE_DIGITAL);

    do {
        error = pspSdkInetInit();
        if (error) {
            pspDebugScreenPrintf(MODULE_NAME ": pspSdkInetInit returns %08x\n", error);
            break;
        }

        connect_apctl(1);
        pspDebugScreenClear();
        pspDebugScreenPrintf("==================================\n");
        pspDebugScreenPrintf("psp-controller-client\n");
        pspDebugScreenPrintf("GitHub: https://github.com/diamant3/psp-controller\n");
        pspDebugScreenPrintf("==================================\n");
        while (1) {
            pspDebugScreenSetXY(0, 4);
            sceCtrlReadBufferPositive(&pad, 1);

            if (pad.Buttons != 0) {
                sock = socket(PF_INET, SOCK_STREAM, 0);
                if (sock < 0) { return -1; }

                name.sin_family = AF_INET;
                name.sin_port = htons(PORT);
                inet_pton(AF_INET, IP_ADDRESS, &(name.sin_addr));

                connect(sock, (struct sockaddr *)&name, sizeof(name));

                if (pad.Buttons & PSP_CTRL_CIRCLE) {
                    pspDebugScreenPrintf("LED Status: On ");
                    send(sock, &ON, strlen(ON), 0);
                } 
                
                if (pad.Buttons & PSP_CTRL_CROSS) {
                    pspDebugScreenPrintf("LED Status: Off");
                    send(sock, &OFF, strlen(OFF), 0);
                }
            }

            sceKernelDelayThread(50 * 1000); // 50ms
        }
    } while (0);

    pspSdkInetTerm();
    sceUtilityUnloadNetModule(PSP_NET_MODULE_COMMON);
    sceUtilityUnloadNetModule(PSP_NET_MODULE_INET);
    return 0;
}
