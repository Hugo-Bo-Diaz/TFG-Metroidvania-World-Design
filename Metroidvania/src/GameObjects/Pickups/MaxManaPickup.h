#ifndef MAX__MANA__PICKUP
#define MAX__MANA__PICKUP

#include "Modules/ObjectManager.h"
#include "EngineElements/ParticleEmitter.h"
#include "Modules/Textures.h"

class ParticleEmitter;

class MaxManaPickup : public GameObject
{
public:
	MaxManaPickup();
	MaxManaPickup(std::list<ObjectProperty*>& aProperties);
	~MaxManaPickup() {};

	void Init();
	bool Loop(float dt);
	bool Render();
	void Destroy();

	RXRect maxmanaplus;
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
