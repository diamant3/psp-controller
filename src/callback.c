#include <pspkernel.h>

#include "callback.h"

int done = 0;

int ExitCallback(int arg1, int arg2, void *common) {
    done = 1;
    return 0;
}

int CallbackThread(SceSize args, void *argp) {
    int cbid = 0;

    cbid = sceKernelCreateCallback("Exit Callback", ExitCallback, NULL);
    sceKernelRegisterExitCallback(cbid);
    sceKernelSleepThreadCB();

    return 0;
}

int SetupExitCallback(void) {
    int thid = 0;
    
    thid = sceKernelCreateThread("Callback Update Thread", CallbackThread, 0x11, 0xFA0, 0, 0);
    if(thid >= 0) {
        sceKernelStartThread(thid, 0, 0);
    }

    return thid;
}