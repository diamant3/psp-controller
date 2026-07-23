#include <pspuser.h>
#include <pspdebug.h>
#include <pspctrl.h>

#include "callback.h"
#include "network.h"

PSP_MODULE_INFO("psp-controller", PSP_MODULE_USER, 0, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER);

int main(void) {
    setup_callback();
    pspDebugScreenInit();

    sceCtrlSetSamplingCycle(0);
    sceCtrlSetSamplingMode(PSP_CTRL_MODE_DIGITAL);

    SceCtrlData pad;
    int socket = init_network();
    while (1) {
        pspDebugScreenSetXY(0, 4);
        sceCtrlReadBufferPositive(&pad, 1);

        if (pad.Buttons != 0) {
            if (pad.Buttons & PSP_CTRL_CIRCLE) {
                pspDebugScreenPrintf("LED Status: On ");
                send_network(socket, 1);
            } 
            
            if (pad.Buttons & PSP_CTRL_CROSS) {
                pspDebugScreenPrintf("LED Status: Off");
                send_network(socket, 0);
            }
        }

        sceKernelDelayThread(50 * 1000); // 50ms
    }

    destroy_network();

    return 0;
}
