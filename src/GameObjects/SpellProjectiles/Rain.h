#ifndef RAIN__H
#define RAIN__H

#include "SDL/include/SDL.h"
#include "Modules/Textures.h"

class EngineAPI;
class Cloud;

class Rain
{
public:
	SDL_Rect blitrect;
	SDL_Rect hitbox;

	float speed;
	Rain(EngineAPI* aAPI) ;
	void Loop();
	void Render();

	Cloud* parent;
	
	TextureID spells;
	EngineAPI* Engine;
};

#endif