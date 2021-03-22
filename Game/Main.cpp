#include <stdlib.h>
#include <stdio.h>
#include "Application.h"

//#include "network/stdafx.h"
//#include "network/ServerGame.h"
//#include "network/ClientGame.h"
//// used for multi-threading
//#include <process.h>


#include "SDL/include/SDL.h"
#pragma comment( lib, "SDL/libx86/SDL2.lib" )
#pragma comment( lib, "SDL/libx86/SDL2main.lib" )

enum ApplicationState
{
	CREATE,
	LOOP,
	CLEAN,
	QUIT,
	EXIT
};

Application* App = nullptr;

int main(int argc, char* args[])
{
	SDL_Init(SDL_INIT_VIDEO);
	
	ApplicationState state = CREATE;
	App = new Application();

	if (!App->Init())
	{
		state = EXIT;
		printf("EXITING: with errors (init)\n");
	}
	else
	{
		state = LOOP;
	}

	while (state != EXIT)
	{
		switch (state)
		{
			case LOOP:
			{			
				if (!App->Loop())
				{
					state = QUIT;
					printf("EXITING: with errors (loop)\n");
				}
			}
			break;
			case QUIT:
			{
				if (!App->CleanUp())
				{
					printf("EXITING: with errors (cleanup)\n");
				}
				else
				{
					delete App;
					App = nullptr;
					printf("EXITING: no errors found");
				}
				state = EXIT;
			}
			break;
		default:
			printf("dafuq\n");
			break;
		}
	}
	SDL_Quit();
	return 1;
}