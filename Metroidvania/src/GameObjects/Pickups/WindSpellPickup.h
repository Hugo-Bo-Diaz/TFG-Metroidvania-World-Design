#ifndef WIND__SPELL__PICKUP__H
#define WIND__SPELL__PICKUP__H

#include "Modules/SceneController.h"
#include "EngineElements/Animation.h"
#include "EngineElements/ParticleEmitter.h"
#include "Modules/Render.h"

class ParticleEmitter;

class WindSpellPickup : public GameObject
{
public:

	WindSpellPickup();
	WindSpellPickup(std::list<ObjectProperty*>& aList) { new (this) WindSpellPickup; };
	~WindSpellPickup() {};

	void Init();
	bool Loop(float dt);
	bool Render();
	void Destroy();

	RXRect windspellbook;

	particle_preset fireshield;
	RXRect r2shield;
	RXRect r12shield;
	ParticleEmitter* p;

	RexTextureID particles;
	RexTextureID spell_books;
};
#endif