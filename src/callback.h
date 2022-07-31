#ifndef CALLBACK_H
#define CALLBACK_H

int ExitCallback(int arg1, int arg2, void *common);
int CallbackThread(SceSize args, void *argp);
int SetupExitCallback(void);

extern int done;

#endif