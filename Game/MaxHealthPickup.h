#ifndef MAX__HEALTH__PICKUP
#define MAX__HEALTH__PICKUP

#include "Physics.h"

class ParticleEmitter;

class MaxHealthPickup : public physobj
{
public:
	MaxHealthPickup();
	~MaxHealthPickup();

	bool Loop(float dt);
	bool Render();

	SDL_Rect maxhealthplus;
	int pickup_id;

	ParticleEmitter* p;
	std::string text;
	int lore_unlock = -1;
};
#endif
