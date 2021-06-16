#ifndef GROUND__SPELL__PICKUP__H
#define GROUND__SPELL__PICKUP__H

#include "Physics.h"
#include "Animation.h"

class ParticleEmitter;

class GroundSpellPickup : public physobj
{
public:

	GroundSpellPickup();
	~GroundSpellPickup();

	bool Loop(float dt);
	bool Render();

	SDL_Rect groundspellbook;

	ParticleEmitter* p;
	ParticleEmitter* q;
};
#endif