#ifndef MAX__MANA__PICKUP
#define MAX__MANA__PICKUP

#include "Physics.h"
#include "ParticleEmitter.h"
#include "Textures.h"

class ParticleEmitter;

class MaxManaPickup : public GameObject
{
public:
	MaxManaPickup();
	~MaxManaPickup();

	bool Loop(float dt);
	bool Render();

	SDL_Rect maxmanaplus;
	int pickup_id;

	ParticleEmitter* p;

	std::string text;
	int lore_unlock = -1;

	particle_preset magic;
	SDL_Rect r13magic;

	TextureID particles;
	TextureID items;

	static GameObject* Factory(std::list<ObjectProperty*>&);
	std::type_index GetTypeInfo() { return std::type_index(typeid(*this)); }
};
#endif
