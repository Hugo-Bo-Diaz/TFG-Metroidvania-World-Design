#ifndef FIRE__SPELL__PICKUP__H
#define FIRE__SPELL__PICKUP__H

#include "Modules/ObjectManager.h"
#include "EngineElements/Animation.h"
#include "EngineElements/ParticleEmitter.h"
#include "Modules/Textures.h"

class ParticleEmitter;

class FireSpellPickup : public GameObject
{
public:

	FireSpellPickup();
	FireSpellPickup(std::list<ObjectProperty*>& aList) { new (this) FireSpellPickup; };
	~FireSpellPickup() {};

	void Init();
	bool Loop(float dt);
	bool Render();
	void Destroy();

	SDL_Rect firespellbook;

	particle_preset fireshield;
	SDL_Rect r2shield;
	SDL_Rect r12shield;
	ParticleEmitter* p;

	TextureID particles;
	TextureID spell_books;
};
#endif