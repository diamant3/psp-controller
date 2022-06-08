#include <pspkernel.h>
#include <pspsdk.h>
#include <pspdebug.h>
#include <pspctrl.h>
#include <psputility.h>
#include <pspnet.h>
#include <pspnet_inet.h>
#include <pspnet_apctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>

#define psp_print 	pspDebugScreenPrintf
#define MODULE_NAME "psp-controller"
#define PORT        80
#define IP          "192.168.4.1"

PSP_MODULE_INFO(MODULE_NAME, 0, 0, 2);
PSP_MAIN_THREAD_ATTR(0);

int done = 0;

int connect_ap(int access_point) {
	int connection_state = 0;
	int connection_state_previous = -1;
	int running = 0;
	int err = 0;

	err = sceUtilityLoadNetModule(PSP_NET_MODULE_COMMON);
	if (err != 0)
	{
		return err;
	}

	err = sceUtilityLoadNetModule(PSP_NET_MODULE_INET);
    if (err != 0)
	{
		return err;
	}

	err = sceNetInit(65536, 32, 2048, 32, 2048);
	if (err != 0)
	{
		return err;
	}

	err = sceNetInetInit();
	if (err != 0)
	{
		return err;
	}

	err = sceNetApctlInit(0x2000, 20);
	if (err != 0) 
	{
		return err;
	}

	do 
	{
		int conn_status = sceNetApctlConnect(access_point);
		if (conn_status < 0)
		{
			psp_print(MODULE_NAME ": sceNetApctlConnect status %08x\n", conn_status);
			running = 0;
			break;
		} 
		else 
		{
			running = 1;
			break;
		}

		psp_print(MODULE_NAME ": Connecting... | \n");
		pspDebugScreenClear();
		psp_print(MODULE_NAME ": Connecting... / \n");
		pspDebugScreenClear();
		psp_print(MODULE_NAME ": Connecting... -- \n");
		pspDebugScreenClear();
		psp_print(MODULE_NAME ": Connecting... \\ \n");
		pspDebugScreenClear();
	} while(1);

	while (running) 
	{

		int status = sceNetApctlGetState(&connection_state);
		if (status < 0) 
		{
			psp_print(MODULE_NAME ": sceNetApctlGetState status $%x\n", status);
			break;
		}

		if (connection_state != connection_state_previous) 
		{
			switch (connection_state) 
			{
				case PSP_NET_APCTL_STATE_DISCONNECTED:
					psp_print(MODULE_NAME ": DISCONNECTED!\n");
				break;

				case PSP_NET_APCTL_STATE_SCANNING:
                	psp_print(MODULE_NAME ": SCANNING...\n"); 
				break;

            	case PSP_NET_APCTL_STATE_JOINING:
                	psp_print(MODULE_NAME ": JOINING...\n"); 
				break;

            	case PSP_NET_APCTL_STATE_GETTING_IP:
                	psp_print(MODULE_NAME ": OBTAINING IP ADDRESS...\n"); 
				break;

            	case PSP_NET_APCTL_STATE_GOT_IP:
                	psp_print(MODULE_NAME ": CONNECTED!\n");
					return 1;

            	case PSP_NET_APCTL_STATE_EAP_AUTH:
                	psp_print(MODULE_NAME ": AUTHENTICATING...\n");; 
				break;

            	case PSP_NET_APCTL_STATE_KEY_EXCHANGE:
                	psp_print(MODULE_NAME ": EXCHANGING KEYS...\n"); 
				break;
			}

			connection_state_previous = connection_state;
		}

		sceKernelDelayThread(500 * 1000); // 500ms
	}

	return 0;
}

int make_socket(unsigned short port) 
{
	struct sockaddr_in server;
	memset(&server, 0, sizeof (struct sockaddr_in));
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	inet_pton(AF_INET, IP, &server.sin_addr.s_addr);

	int sock = sceNetInetSocket(PF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		return sock;
	}

	sceNetInetConnect(sock, (struct sockaddr *) &server, sizeof server);

	return sock;
}

void cleanup()
{
    sceNetApctlTerm();
    sceNetInetTerm();
    sceNetTerm();
    sceUtilityUnloadNetModule(PSP_NET_MODULE_COMMON);
    sceUtilityUnloadNetModule(PSP_NET_MODULE_INET);
}

int ExitCallback(int arg1, int arg2, void *common)
{ 
	done = 1;

    return 0; 
} 

int CallbackThread(SceSize args, void *argp)
{ 
    int cbid = sceKernelCreateCallback("Exit Callback", ExitCallback, NULL); 
	if (cbid < 0)
	{
		return cbid;
	}
    sceKernelRegisterExitCallback(cbid); 
    sceKernelSleepThreadCB();

    return 0; 
} 
 
int SetupExitCallback()
{ 

    int thid = sceKernelCreateThread("Callback Update Thread", CallbackThread, 0x11, 0xFA0, 0, 0); 
    if(thid <= 0) 
	{ 
        return thid;
    } 

	sceKernelStartThread(thid, 0, 0); 
    return thid; 
}

int main(int argc, char *argv[]) 
{
	int sock = 0;
	SceCtrlData pad;

	pspDebugScreenInit();
	SetupExitCallback();

	connect_ap(1);
	sock = make_socket(PORT);
	
	pspDebugScreenClear();
	psp_print(MODULE_NAME ": github.com/diamant3");
	while(!done) 
	{
		pspDebugScreenSetXY(0, 2);
		sceCtrlReadBufferPositive(&pad, 1); 

		if (pad.Buttons != 0)
		{
			if (pad.Buttons & PSP_CTRL_CIRCLE) 
			{
				const char led_on[] = "GET /LEDOn\r\n";
				psp_print("LED ON! \n");
				sceNetInetSend(sock, led_on, strlen(led_on), 0);
			} 

			if (pad.Buttons & PSP_CTRL_CROSS) 
			{
				const char led_off[] = "GET /LEDOff\r\n";
				psp_print("LED OFF! \n");
				sceNetInetSend(sock, led_off, strlen(led_off), 0);
			}         
		}
	}

	sceNetInetClose(sock);
	cleanup();
	sceKernelExitGame();

	return 0;
}


