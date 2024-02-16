#ifndef RAIN__H
#define RAIN__H

#include "SDL/include/SDL.h"
#include "Textures.h"

class Cloud;

class Rain
{
public:
	SDL_Rect blitrect;
	SDL_Rect hitbox;

	float speed;
	Rain();
	void Loop();
	void Render();

	Cloud* parent;
	
	TextureID spells;
};

#endif