#ifndef FIRE__SPELL__PICKUP__H
#define FIRE__SPELL__PICKUP__H

#include "Physics.h"
#include "Animation.h"

class ParticleEmitter;

class FireSpellPickup : public physobj
{
public:

	FireSpellPickup();
	~FireSpellPickup();

	bool Loop(float dt);
	bool Render();

	SDL_Rect firespellbook;

	ParticleEmitter* p;
};
#endif