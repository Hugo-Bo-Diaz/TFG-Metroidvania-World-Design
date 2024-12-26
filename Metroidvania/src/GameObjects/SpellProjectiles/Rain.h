#ifndef RAIN__H
#define RAIN__H

#include "RXRect.h"
#include "Modules/Render.h"

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
	
	RexTextureID spells;
	EngineAPI* Engine;
};

#endif