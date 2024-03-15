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

	SDL_Rect groundspellbook;

	ParticleEmitter* p;
	ParticleEmitter* q;
	particle_preset sand_left;
	particle_preset sand_right;
	SDL_Rect r16sandfirst;
	SDL_Rect r17sandsecond;

	TextureID particles;
	TextureID spell_books;
};
#endif