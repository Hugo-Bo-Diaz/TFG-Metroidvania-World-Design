#ifndef GROUND__SPELL__PICKUP__H
#define GROUND__SPELL__PICKUP__H

#include "Physics.h"
#include "Animation.h"
#include "ParticleEmitter.h"
#include "Textures.h"

class ParticleEmitter;

class GroundSpellPickup : public GameObject
{
public:

	GroundSpellPickup();
	~GroundSpellPickup();

	bool Loop(float dt);
	bool Render();

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