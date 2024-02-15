#ifndef MAX__HEALTH__PICKUP
#define MAX__HEALTH__PICKUP

#include "Physics.h"
#include "ParticleEmitter.h"

class ParticleEmitter;

class MaxHealthPickup : public GameObject
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

	particle_preset magic;
	SDL_Rect r13magic;
};
#endif
