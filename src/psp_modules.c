#include <pspnet.h>
#include <pspnet_inet.h>
#include <pspnet_apctl.h>
#include <psputility.h>
#include <arpa/inet.h>

#include "psp_modules.h"

void load_netmodule(void)
{
    sceUtilityLoadNetModule(PSP_NET_MODULE_COMMON);
    sceUtilityLoadNetModule(PSP_NET_MODULE_INET);
    sceNetInit(0x10000, 0x20, 0x1000, 0x20, 0x1000);
    sceNetInetInit();
    sceNetApctlInit(0x2000, 0x14);
}


void unload_netmodule(int sock) 
{
    sceNetInetClose(sock);
    sceNetApctlTerm();
    sceNetInetTerm();
    sceNetTerm();
    sceUtilityUnloadNetModule(PSP_NET_MODULE_COMMON);
    sceUtilityUnloadNetModule(PSP_NET_MODULE_INET);
}