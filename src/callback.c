#ifndef __CALLBACK__
#define __CALLBACK__

#include <pspuser.h>

static int exit_callback(int arg1, int arg2, void *arg) {
    (void)arg1;
    (void)arg2;
    (void)arg;

    sceKernelExitGame(); 

    return 0;
}

static int thread_callback(SceSize args, void *argp) {
    (void)args;
    (void)argp;

    int id = sceKernelCreateCallback("exit_callback", exit_callback, NULL);
    if (id >= 0) {
        sceKernelRegisterExitCallback(id);
        sceKernelSleepThreadCB();
    }

    return id;
}

int setup_callback(void) {
    SceUID id = sceKernelCreateThread("update_thread", thread_callback, 0x18, 0x10000, PSP_THREAD_ATTR_USER, NULL);
    if (id >= 0) {
        sceKernelStartThread(id, 0, NULL);
    }

    return id;
}

#endif