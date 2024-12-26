#ifndef FIRE__SPELL__PICKUP__H
#define FIRE__SPELL__PICKUP__H

#include "Modules/SceneController.h"
#include "EngineElements/Animation.h"
#include "EngineElements/ParticleEmitter.h"
#include "Modules/Render.h"

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

	RXRect firespellbook;

	particle_preset fireshield;
	RXRect r2shield;
	RXRect r12shield;
	ParticleEmitter* p;

	RexTextureID particles;
	RexTextureID spell_books;
};
#endif