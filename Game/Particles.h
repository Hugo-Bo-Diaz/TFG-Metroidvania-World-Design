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

	std::list<ParticleEmitter*> to_delete;

	SDL_Rect r1;
	SDL_Rect r2;
	particle_preset fire;


};

#endif // !PHYSICS__H