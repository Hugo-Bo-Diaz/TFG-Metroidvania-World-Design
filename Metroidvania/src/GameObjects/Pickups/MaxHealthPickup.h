#ifndef MAX__HEALTH__PICKUP
#define MAX__HEALTH__PICKUP

#include "Modules/ObjectManager.h"
#include "EngineElements/ParticleEmitter.h"
#include "Modules/Textures.h"

class ParticleEmitter;

class MaxHealthPickup : public GameObject
{
public:
	MaxHealthPickup();
	MaxHealthPickup(std::list<ObjectProperty*>& aList);
	~MaxHealthPickup() {};

	void Init();
	bool Loop(float dt);
	bool Render();
	void Destroy();

	RXRect maxhealthplus;
	int pickup_id;

	ParticleEmitter* p;

	std::string text;
	int lore_unlock = -1;

	particle_preset magic;
	RXRect r13magic;

	TextureID particles;
	TextureID items;
};
#endif
