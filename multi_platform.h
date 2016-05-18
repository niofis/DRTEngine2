

#ifndef _DRTECore_multi_platform_h_
#define _DRTECore_multi_platform_h_

//#define PLATFORM_WIN32
//#define PLATFORM_WII
#define PLATFORM_LINUX

	#ifdef PLATFORM_WIN32
		//#include <windows.h>
		#include <windows.h>
		#define THREAD DWORD __stdcall
		#define DRTECreateThread(f,p) CreateThread(NULL,0,f,p,0,NULL)       
	#elif defined(PLATFORM_LINUX)
		#include <stdio.h>
		#include <string.h>
		#include <sys/types.h>
		
				
		#define THREAD void*
		#define DRTECreateThread(fc,pc) {pthread_t  handle; pthread_create(&handle,0,fc,pc);}
		
		//#include <sys/un.h>
		#include <stdlib.h>
		//#include <linux/time.h>
		
		#include <unistd.h>
		
		#define HANDLE int
		#define DWORD unsigned short
		#define Sleep usleep
		#define CopyMemory memcpy
	#elif defined(PLATFORM_WII)
		#define PLATFORM_LINUX

		#include <stdio.h>
		#include <string.h>
		#include <sys/types.h>
		
		#include <ogcsys.h>
		#include <errno.h>
		#include <stdlib.h>
		#include <gccore.h>
		#include <wiiuse/wpad.h>

		#include <ogc/lwp.h>
		#include <fat.h>
		#include <sys/dir.h>
		#include <sdcard/wiisd_io.h>
		#include <ogc/lwp_watchdog.h>

		
		#define THREAD void*
		#define DRTECreateThread(fc,pc) {lwp_t  handle; LWP_CreateThread(&handle,fc,pc,NULL,0,40);}
		
		//#include <sys/un.h>
		#include <stdlib.h>
		//#include <linux/time.h>
		
		#include <unistd.h>
		
		#define HANDLE int
		#define DWORD unsigned short
		#define Sleep usleep
		#define CopyMemory memcpy
	#endif
#endif
