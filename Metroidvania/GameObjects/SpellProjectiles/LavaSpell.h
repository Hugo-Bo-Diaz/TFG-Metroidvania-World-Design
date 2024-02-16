#ifndef LAVA__SPELL__H
#define LAVA__SPELL__H

#include "Timer.h"
#include "SDL/include/SDL.h"
#include "Textures.h"

class Fire;

class LavaSpell
{
public:
	SDL_Rect blitrect;
	SDL_Rect hitbox;

	float speed;
	float lifespan = 500;
	Timer life;

	LavaSpell(TextureID tex);
	void Loop();
	void Render();

	Fire* parent;

	int damage = 1;
	TextureID spells;
};

#endif