#ifndef FIRE__SPELL__PICKUP__H
#define FIRE__SPELL__PICKUP__H

#include "Physics.h"
#include "Animation.h"
#include "ParticleEmitter.h"

class ParticleEmitter;

class FireSpellPickup : public GameObject
{
public:

	FireSpellPickup();
	~FireSpellPickup();

	bool Loop(float dt);
	bool Render();

	SDL_Rect firespellbook;

	particle_preset fireshield;
	SDL_Rect r2shield;
	SDL_Rect r12shield;
	ParticleEmitter* p;
};
#endif