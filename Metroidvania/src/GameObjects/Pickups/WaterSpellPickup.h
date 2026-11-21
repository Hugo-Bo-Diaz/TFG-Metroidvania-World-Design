#ifndef WATER__SPELL__PICKUP__H
#define WATER__SPELL__PICKUP__H

#include "Modules/SceneController.h"
#include "EngineElements/Animation.h"
#include "EngineElements/ParticleEmitter.h"
#include "Modules/Render.h"

class ParticleEmitter;

class WaterSpellPickup : public GameObject
{
public:

	WaterSpellPickup();
	WaterSpellPickup(std::list<ObjectProperty*>& aList) { new (this) WaterSpellPickup; };
	~WaterSpellPickup() {};

	void Init();
	bool Loop(float dt);
	bool Render();
	void Destroy();

	RXRect waterspellbook;

	particle_preset water_drip;
	RXRect r18waterfirst;
	RXRect r19watersecond;
	RXRect r20waterthird;
	ParticleEmitter* p;

	RexTextureID particles;
	RexTextureID spell_books;
};
#endif