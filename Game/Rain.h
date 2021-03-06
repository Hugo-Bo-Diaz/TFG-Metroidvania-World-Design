#ifndef RAIN__H
#define RAIN__H

#include "SDL/include/SDL.h"
class Cloud;

class Rain
{
public:
	SDL_Rect blitrect;
	SDL_Rect hitbox;

	float speed;

	void Loop();

	Cloud* parent;
	
};

#endif