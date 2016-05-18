#include "multi_platform.h"
#include "DRTEngine2.h"
#include <time.h>
#include "SDL_gfx/SDL_gfxPrimitives.h"
#include "SDL_gfx/SDL_gfxPrimitives.c"


int width=640;
int height=480;


SDL_Surface * OpenScreen(const int width, const int height, const int bpp, const int flags)
{
	SDL_Surface * screen = SDL_SetVideoMode(width, height, bpp, flags);
	if(!screen)
	{
		//Cout() << Format("Couldn't set display mode: %s\n", SDL_GetError());
		return NULL;
	}
	return screen;
}

void Paint(SDL_Surface * surface)
{
	SDL_LockSurface(surface);
	
	SDL_UnlockSurface(surface);
}


#ifdef PLATFORM_WIN32
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
#else
int main(int argc, char *argv[])
#endif
{
	#ifdef PLATFORM_WII
	//void *xfb = NULL;
	//GXRModeObj *rmode = NULL;
	//VIDEO_Init();
	WPAD_Init();
	/*
	rmode = VIDEO_GetPreferredMode(NULL);
	xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
	console_init(xfb,20,20,rmode->fbWidth,rmode->xfbHeight,rmode->fbWidth*VI_DISPLAY_PIX_SZ);
	VIDEO_Configure(rmode);
	VIDEO_SetNextFramebuffer(xfb);
	VIDEO_SetBlack(FALSE);
	VIDEO_Flush();
	VIDEO_WaitVSync();
	if(rmode->viTVMode&VI_NON_INTERLACE) VIDEO_WaitVSync();
	*/
	
    //if (!fatInitDefault()) printf("Unable to enable FAT \n");

	#endif
	
	DRTE::DRTEScene scene;
	//scene=new DRTE::DRTEScene();
	//scene.Demo();
	//scene.LoadScene("/home/niofis/Enrique/Tesis/Modelos/luces5.ms3d");
	//scene.LoadScene("/home/niofis/Enrique/Tesis/Modelos/sphere.ms3d");
	//scene.LoadScene("/home/niofis/Enrique/Tesis/Modelos/room.ms3d");
	//scene.LoadScene("/home/niofis/Enrique/Tesis/Modelos/water_box.ms3d");
	//scene.LoadScene("/home/niofis/Enrique/Tesis/Modelos/luces5.ms3d");
	//scene.LoadScene("D:/Enrique/Tesis/Modelos/esferas_simple.ms3d");
	scene.LoadScene("/media/USB FLASH/Tesis/Modelos/esferas_simple.ms3d");
	//scene.LoadScene("sd:/cubo_simple.ms3d");
	//scene.LoadScene("/home/niofis/Enrique/Tesis/Modelos/esferas_simple.ms3d");
	//scene.LoadScene("/mnt/enrique/Enrique/Tesis/Modelos/esferas_simple.ms3d");
	//scene.LoadScene("modelo.ms3d");
	if(!scene.num_objects)
		scene.Demo();
	DRTE::DRTEngine engine;
	engine.SetScene(&scene);
	engine.SetDimentions(width,height);
	engine.SetThreads(1);
	
	char title[256];
	
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
		return 0;

	int videoflags =SDL_HWACCEL | SDL_HWACCEL | SDL_DOUBLEBUF;
	SDL_Surface * screen = OpenScreen(width, height, 24, videoflags);
	
	if(!screen)
		return 0;
		
	SDL_ShowCursor(0);

	SDL_Event event;
	bool done = false;
	
	/**********Variables para calcular el tiempo de renderizado***********/
	#ifdef PLATFORM_WIN32
		LARGE_INTEGER inicio;
		LARGE_INTEGER fin;
		LARGE_INTEGER freq;
		long long  i,f,q;
	#else
		int inicio,fin,freq;//clock_t
		int  i,f,q;
		
	#endif

	/*char keys[256];
	#ifdef PLATFORM_WIN32
		ZeroMemory(keys,256);	//Y se pone todo en ceros
	#else
		memset(keys,0,256);
	#endif*/
	
	/**********Variables para calcular el tiempo de renderizado***********/
	while(!done)
	{
		#ifdef PLATFORM_WIN32
		QueryPerformanceCounter(&inicio);
		#elif defined(PLATFORM_WII)
			i=inicio=gettime();
		#else
			i=inicio=clock();
		
		#endif
		
		if(SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_KEYDOWN:
					if(event.key.keysym.sym == SDLK_LALT || event.key.keysym.sym == SDLK_TAB)
						break;
					if(event.key.keysym.sym == SDLK_RETURN)
					{
						videoflags ^= SDL_FULLSCREEN;
						screen = OpenScreen(screen->w, screen->h, screen->format->BitsPerPixel, videoflags);
						if(!screen)
							done = true;
						break;
					}
					if(event.key.keysym.sym == SDLK_ESCAPE)
						done=true;
					//keys[event.key.keysym.sym]=1;
					break;
				case SDL_KEYUP:
					//keys[event.key.keysym.sym]=0;
					break;
				case SDL_QUIT:
					done = true;
					break;
				default:
					break;
			}
		}
		else
		{
			//scene.cameras[0]->Rotate(0.017452 * 2.5,2);
			
			Uint8 * keys;
			keys=SDL_GetKeyState(NULL); 
			scene.cameras[0]->Rotate(0.017452 * 2.5,2);
			if(keys[SDLK_UP])
				scene.cameras[0]->Rotate(0.017452 * -2.5,1);
			if(keys[SDLK_DOWN])
				scene.cameras[0]->Rotate(0.017452 * 2.5,1);
			if(keys[SDLK_LEFT])
				scene.cameras[0]->Rotate(0.017452 * -2.5,2);
			if(keys[SDLK_RIGHT])
				scene.cameras[0]->Rotate(0.017452 * 2.5,2);
			
			if(keys[SDLK_w])
				scene.cameras[0]->Move(0.0f,0.0f,1.0f);
			if(keys[SDLK_s])
				scene.cameras[0]->Move(0.0f,0.0f,-1.0f);
			if(keys[SDLK_a])
				scene.cameras[0]->Move(-1.0f,0.0f,0.0f);
			if(keys[SDLK_d])
				scene.cameras[0]->Move(1.0f,0.0f,0.0f);
			if(keys[SDLK_q])
				scene.cameras[0]->Move(0.0f,1.0f,0.0f);
			if(keys[SDLK_e])
				scene.cameras[0]->Move(0.0f,-1.0f,0.0f);
			
			
			//scene.cameras->At(0)->Rotate();
			SDL_LockSurface(screen);
			engine.Render((char*) screen->pixels);
			SDL_UnlockSurface(screen);
			
			/***********Calculos del tiempo de renderizado************************/
			#ifdef PLATFORM_WIN32		
				QueryPerformanceCounter(&fin);
				QueryPerformanceFrequency(&freq);
				i=inicio.HighPart;
				i<<=32;
				i+=inicio.LowPart;
				f=fin.HighPart;
				f<<=32;
				f+=fin.LowPart;
				q=freq.HighPart;
				q<<=32;
				q+=freq.LowPart;
			#elif defined(PLATFORM_WII)
			
				f=fin=gettime();
				q=freq=TB_TIMER_CLOCK;
			#else
				f=fin=clock();
				q=freq=CLOCKS_PER_SEC;
			
				
			#endif
			//std::cout<<"Tiempo de renderizado: "<<(double)((double)(f-i)/(double)q)*1000<<" ms\n";
			//std::cout<<1/((double)((double)(f-i)/(double)q))<<" fps\r";
			#ifdef PLATFORM_WIN32
			double res=(double)((double)(f-i)/(double)q);
			#else
			double res=(double)((double)(f-i)/(double)q);
			#endif
			sprintf(title,"%f fps - %f s",1/res,res);
			
			//SDL_WM_SetCaption(title,NULL);
			stringColor(screen,0,0,title,0xFFFFFF);
		
			SDL_Flip(screen);
			
		}
		
		
	}
	SDL_FreeSurface(screen);
	SDL_Quit();
	#ifdef PLATFORM_WII
	exit(0);
	#endif
	return 0;
}

