#ifndef LAVA__SPELL__H
#define LAVA__SPELL__H

#include "Timer.h"
#include "SDL/include/SDL.h"
class Fire;

class LavaSpell
{
public:
	SDL_Rect blitrect;
	SDL_Rect hitbox;

	float speed;
	float lifespan = 500;
	Timer life;

	void Loop();
	void Render();

	Fire* parent;

};

#endif