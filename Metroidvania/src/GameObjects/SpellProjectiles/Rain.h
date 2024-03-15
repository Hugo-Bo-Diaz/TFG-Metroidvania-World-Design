#ifndef RAIN__H
#define RAIN__H

#include "RXRect.h"
#include "Modules/Textures.h"

class EngineAPI;
class Cloud;

class Rain
{
public:
	RXRect blitrect;
	RXRect hitbox;

	float speed;
	Rain(EngineAPI* aAPI) ;
	void Loop();
	void Render();

	Cloud* parent;
	
	TextureID spells;
	EngineAPI* Engine;
};

#endif