#include <pspkernel.h>
#include <pspdebug.h>
#include <pspctrl.h>
#include <pspnet_inet.h>

#include <string.h>

#include "callback.h"
#include "connection.h"
#include "psp_modules.h"
#include "main.h"

PSP_MODULE_INFO(MODULE_NAME, PSP_MODULE_USER, 1, 0);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER);

int main(void) 
{
    SetupExitCallback();
    pspDebugScreenInit();
    load_netmodule();

    int socket = create_sock();
    if (socket < 0) 
    {
        pspDebugScreenPrintf("create_sock() - Error: %d", socket);
        return -1;
    }

    int ret = connect_ap(socket, IP, PORT);
    if (ret < 0) 
    {
        pspDebugScreenPrintf("connect_ap() - Error: %d", ret);
        return -1;
    }

    pspDebugScreenPrintf("==================================\n");
    pspDebugScreenPrintf("psp-controller-client\n");
    pspDebugScreenPrintf("GitHub: https://github.com/diamant3/" MODULE_NAME "\n");
    pspDebugScreenPrintf("==================================\n");

    SceCtrlData pad;
    sceCtrlSetSamplingCycle(0);
    sceCtrlSetSamplingMode(PSP_CTRL_MODE_DIGITAL);

    const char ON[32] = "GET /led_on HTTP/1.1\r\n";
    const char OFF[32] = "GET /led_off HTTP/1.1\r\n";

    while (1) 
    {
        pspDebugScreenSetXY(0, 5);
        sceCtrlReadBufferPositive(&pad, 1);

        if (pad.Buttons != 0) 
        {
            if (pad.Buttons & PSP_CTRL_CIRCLE) 
            {
                pspDebugScreenPrintf("LED Status: On ");
                sceNetInetSend(socket, &ON, strlen(ON), 0);
            }

            if (pad.Buttons & PSP_CTRL_CROSS) 
            {
                pspDebugScreenPrintf("LED Status: Off");
                sceNetInetSend(socket, &OFF, strlen(OFF), 0);
            }
        }

        sceKernelDelayThread(1000);
    }

    unload_netmodule(socket);
    return 0;
}