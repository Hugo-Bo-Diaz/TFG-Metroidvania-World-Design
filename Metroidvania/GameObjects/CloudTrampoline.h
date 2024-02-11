#ifndef CLOUD__TRAMPOLINE__H
#define CLOUD__TRAMPOLINE__H

#include "Physics.h"
#include "Timer.h"

class CloudTrampoline :
	public GameObject
{
public:
	CloudTrampoline();

	bool Loop(float dt);
	bool Render();

	SDL_Rect cloudLeft;
	SDL_Rect cloudLoop;
	SDL_Rect cloudRight;
};

#endif
