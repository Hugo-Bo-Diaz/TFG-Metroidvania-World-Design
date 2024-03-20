#ifndef PARTICLE__H
#define PARTICLE__H

#include "PartsDef.h"
#include "Textures.h"
#include "Application.h"
#include "Render.h"
#include "Part.h"
#include "EngineElements/ParticleEmitter.h"

//module that handles particles
class DLL_EXPORT Particles : public Part
{
public:
	Particles(EngineAPI& aAPI);
	
	//creates a particle emitter and adds it to the engine
	ParticleEmitter* AddParticleEmitter(particle_preset* particle_preset, float x, float y, float lifespan = -1, int depth = 0);
	//removes a particle emitter and deletes its memory
	void RemoveParticleEmitter(ParticleEmitter* _to_delete);

	class ParticlesImpl;
};

#endif // !PHYSICS__H