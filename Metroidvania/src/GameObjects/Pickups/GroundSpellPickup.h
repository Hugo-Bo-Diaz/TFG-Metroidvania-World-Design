#ifndef GROUND__SPELL__PICKUP__H
#define GROUND__SPELL__PICKUP__H

#include "Modules/ObjectManager.h"
#include "EngineElements/Animation.h"
#include "EngineElements/ParticleEmitter.h"
#include "Modules/Textures.h"

class ParticleEmitter;

class GroundSpellPickup : public GameObject
{
public:

	GroundSpellPickup();
	GroundSpellPickup(std::list<ObjectProperty*>& aList) { new (this) GroundSpellPickup; };
	~GroundSpellPickup() {};

	void Init();
	bool Loop(float dt);
	bool Render();
	void Destroy();

	RXRect groundspellbook;

	ParticleEmitter* p;
	ParticleEmitter* q;
	particle_preset sand_left;
	particle_preset sand_right;
	RXRect r16sandfirst;
	RXRect r17sandsecond;

	TextureID particles;
	TextureID spell_books;
};
#endif