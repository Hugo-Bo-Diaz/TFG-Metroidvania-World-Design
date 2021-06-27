#ifndef PARTICLE__H
#define PARTICLE__H

#include "PartsDef.h"
#include "Textures.h"
#include "Application.h"
#include "Render.h"

#include "ParticleEmitter.h"

#include <list>
#include <vector>
#include <utility>

#include "SDL/include/SDL.h"



class Particles : public Part
{
public:

	Particles();

	bool Init();
	bool Loop(float dt);
	bool CleanUp();
	
	std::list<ParticleEmitter*> particles;
	std::list<ParticleEmitter*>* GetParticleList();

	ParticleEmitter* AddParticleEmitter(particle_preset* particle_preset, float x, float y, float lifespan = -1);
	void RemoveParticleEmitter(ParticleEmitter* _to_delete);

	std::list<ParticleEmitter*> to_delete;
	void ClearParticles();


	SDL_Rect r1exp;
	SDL_Rect r2shield;
	SDL_Rect r3lava;
	SDL_Rect r4ice;
	SDL_Rect r5slash;
	SDL_Rect r6buff;
	SDL_Rect r7buff;
	SDL_Rect r8ground;
	SDL_Rect r9ground;
	SDL_Rect r10grass;
	SDL_Rect r11grass;
	SDL_Rect r12shield;

	SDL_Rect r13magic;

	SDL_Rect r14firegedeath;
	SDL_Rect r15firegedeath;

	SDL_Rect r16sandfirst;
	SDL_Rect r17sandsecond;

	SDL_Rect r18metalfirst;
	SDL_Rect r19metalsecond;

	particle_preset explosion;
	particle_preset fireshield;
	particle_preset lava;
	particle_preset ice;
	particle_preset windslash;
	particle_preset windbuff;
	particle_preset groundcontact;
	particle_preset rockblockexplosion;
	particle_preset grass;
	particle_preset magic;
	particle_preset fire_ge_death;
	particle_preset sand_left;
	particle_preset sand_right;
	particle_preset shield_monster_hit;
	particle_preset stone_death;
	particle_preset shield_monster_death;
	particle_preset metal;
	particle_preset metal_and_sand;
	particle_preset smoke;
};

#endif // !PHYSICS__H