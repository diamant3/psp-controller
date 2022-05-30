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

#define print 	    pspDebugScreenPrintf
#define MODULE_NAME "psp_controller"
#define PORT        80
#define IP          "192.168.4.1"

PSP_MODULE_INFO(MODULE_NAME, 0, 0, 1);
PSP_MAIN_THREAD_ATTR(0);

int done = 0;

int connect_ap(int connection) {
	SetupExitCallback();

	int err; 
	int last_state = -1;

	err = sceNetApctlConnect(connection);
	if (err != 0) {
		print(MODULE_NAME ": sceNetApctlConnect returns %08x\n", err);
		return -1;
	}

	print(MODULE_NAME ": Connecting...\n");
	while (1) {
		int state;

		err = sceNetApctlGetState(&state);
		if (err != 0) {
			print(MODULE_NAME ": sceNetApctlGetState returns $%x\n", err);
			break;
		}

		if (state != last_state) {
			print("Connection state %d of 4\n", state);
			last_state = state;
		}

		if (state == PSP_NET_APCTL_STATE_DISCONNECTED) {
			continue;
		}

		if (state == PSP_NET_APCTL_STATE_GOT_IP) {
			print(MODULE_NAME ": Connected!");
			break;
		}

		sceKernelDelayThread(50 * 1000); // 50ms
	}

	return 0;
}

int make_socket(uint16_t port) 
{
	int sock;
	struct sockaddr_in name;

	sock = sceNetInetSocket(PF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		return -1;
	}

	memset(&name, 0, sizeof(struct sockaddr_in));
	name.sin_family = AF_INET;
	name.sin_port = htons(port);
	inet_pton(AF_INET, IP, &name.sin_addr.s_addr);

	sceNetInetConnect(sock, (struct sockaddr *) &name, sizeof(struct sockaddr_in));

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
    int cbid; 
    cbid = sceKernelCreateCallback("Exit Callback", ExitCallback, NULL); 
    sceKernelRegisterExitCallback(cbid); 
    sceKernelSleepThreadCB(); 
    return 0; 
} 
 
int SetupExitCallback()
{ 
    int thid = 0;
    thid = sceKernelCreateThread("Callback Update Thread", CallbackThread, 0x11, 0xFA0, 0, 0); 
    if(thid >= 0) { 
        sceKernelStartThread(thid, 0, 0); 
    } 
    return thid; 
}

int main(void) 
{
	int sock;
	SceCtrlData pad;
	char led_on[] = "GET /LEDOn HTTP/1.1\r\nHost: " IP "\r\nContent-Type: text/plain\r\n\r\n";
	char led_off[] = "GET /LEDOff HTTP/1.1\r\nHost: " IP "\r\nContent-Type: text/plain\r\n\r\n";


	pspDebugScreenInit();
	SetupExitCallback();

	sceUtilityLoadNetModule(PSP_NET_MODULE_COMMON);
    sceUtilityLoadNetModule(PSP_NET_MODULE_INET);

	sceNetInit(64 * 1024, 32, 2 * 1024, 32, 2 * 1024);
	sceNetInetInit();
	sceNetApctlInit(0x2000, 20);

	connect_ap(1);
	sock = make_socket(PORT);
	
	pspDebugScreenClear();
	print("PSP->ESP8266 Controller v0.1 - github.com/diamant3");
	while(!done) {
		pspDebugScreenSetXY(0, 2);
		sceCtrlReadBufferPositive(&pad, 1); 

		if (pad.Buttons != 0){
			if (pad.Buttons & PSP_CTRL_CIRCLE) {
				print("LED ON! \n");
				sceNetInetSend(sock, &led_on, sizeof(led_on) + 1, 0);
			} 
			if (pad.Buttons & PSP_CTRL_CROSS) {
				print("LED OFF! \n");
				sceNetInetSend(sock, &led_off, sizeof(led_off) + 1, 0);
			}         
		}
	}

	sceNetInetClose(sock);
	cleanup();
	sceKernelExitGame();
	return 0;
}


