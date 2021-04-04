#ifndef MAX__MANA__PICKUP
#define MAX__MANA__PICKUP

#include "Physics.h"

class ParticleEmitter;

class MaxManaPickup : public physobj
{
public:
	MaxManaPickup();
	~MaxManaPickup();

	bool Loop(float dt);
	bool Render();

	SDL_Rect maxmanaplus;
	int pickup_id;

	ParticleEmitter* p;

};
#endif
