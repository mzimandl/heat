#include "SDL/SDL.h"
#include "SDL/SDL_gfxPrimitives.h"

#include <stdlib.h>
#include <stdio.h>
#include <string>

#include "ctimer.h"

#define width 800
#define height 600
#define bpp 0

#define bin 1
#define pocet 400
float teplota[pocet];
float pocatecni[pocet];

#define Tick 20

SDL_Surface *screen = NULL;
SDL_Event event;

cTimer Timer;

Uint32 bila;

bool init()
{
	int i;
	
	printf("Inicializuji SDL ...");
	
	if ( SDL_Init(SDL_INIT_EVERYTHING) == -1 )
	{
		printf("\n Nemohu inicializovat SDL: %s\n", SDL_GetError());
		return false;
	}
	
	screen = SDL_SetVideoMode(width, height, bpp, SDL_HWSURFACE|SDL_DOUBLEBUF);
	if ( screen == NULL )
	{
		printf("\n Nemohu nastavit obraz: %s\n", SDL_GetError());
		return false;
	}
	
	printf(" hotovo\n");
	
	printf("Nacitam data ...");
	printf(" hotovo\n");
	
	printf("Nastavuji zbytek ...");
	srand(time(0));
	for (i=1;i<pocet;i++) {
		teplota[i]=0;
		pocatecni[i]=0;
	}
	bila = SDL_MapRGB(screen->format, 255, 255, 255);
	printf(" hotovo\n");
	
	return true;
}

void final()
{
	printf("Ukoncuji ...");
	SDL_Quit();
	printf(" hotovo\n");
}

void kresli_teplotu(float T)
{
	int i;
	
	if (T<1) for (i=0;i<pocet;i++) boxRGBA(screen,i*width/pocet,height,(i+1)*width/pocet,height-(int)pocatecni[i],100+155*(int)pocatecni[i]/height,0,100-100*(int)pocatecni[i]/height,255);
	for (i=0;i<pocet;i++) boxRGBA(screen,i*width/pocet,height,(i+1)*width/pocet,height-(int)teplota[i],100+155*(int)teplota[i]/height,0,100-100*(int)teplota[i]/height,255);
}

void vyvin_teplotu(float T)
{
	#define a 1
	int i,j,k;
	
	for (i=0;i<pocet;i++) {
		teplota[i]=0;
		for (j=0;j<pocet;j++) for (k=0;k<bin;k++) teplota[i] += pocatecni[j]*exp(-(i*bin+bin/2-j*bin-k)*(i*bin+bin/2-j*bin-k)/(4*a*a*T))/(2*a*sqrt(3.14*T));
		teplota[i]/bin;
	}
}

int main(int argc, char **argv)
{	
	if ( !init() ) return 1;
	
	int x,y;
		
	bool cycling = true;
	bool run = false;
	
	float cas = 0;
		
	while ( cycling )
	{
		Timer.Start();
		
		while ( SDL_PollEvent(&event) )
		{
			switch ( event.type )
			{
				case SDL_KEYDOWN:
					switch ( event.key.keysym.sym )
					{
						case SDLK_ESCAPE:
							cycling = false;
							break;
						case SDLK_SPACE:
							run = !run;
							break;
							
						case SDLK_BACKSPACE:
							run = false;
							cas = 0;
							break;
						case SDLK_TAB:
							break;
					}
					break;				
				case SDL_QUIT:
					cycling = false;
					break;
			}
		}
		
		if (!run && cas == 0) {
			if (SDL_GetMouseState(&x, &y) & SDL_BUTTON(1)) pocatecni[x*pocet/width] = height-y;		
			else if (SDL_GetMouseState(&x, &y) & SDL_BUTTON(3)) pocatecni[x*pocet/width] = 0;
		}
		
		vyvin_teplotu(cas);
		if (run) cas+=1;
	
		SDL_FillRect(screen,NULL,bila);
		kresli_teplotu(cas);
		SDL_Flip(screen);
		
		if (Timer.GetTicks() < Tick) SDL_Delay(Tick-Timer.GetTicks());
		//while (Timer.GetTicks() < Tick) {}
	}
	
	final();
	
	return 0;
}
