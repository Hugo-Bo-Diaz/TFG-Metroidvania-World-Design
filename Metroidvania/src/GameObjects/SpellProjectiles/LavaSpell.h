#ifndef LAVA__SPELL__H
#define LAVA__SPELL__H

#include "Utils/Timer.h"
#include "SDL/include/SDL.h"
#include "Modules/Textures.h"

class EngineAPI;
class Fire;

class LavaSpell
{
public:
	SDL_Rect blitrect;
	SDL_Rect hitbox;

	float speed;
	float lifespan = 500;
	Timer life;

	LavaSpell(TextureID tex, EngineAPI* aAPI) : spells(tex),Engine(aAPI) {};
	void Loop();
	void Render();

	Fire* parent;
	EngineAPI* Engine;

	int damage = 1;
	TextureID spells;
};

#endif