#ifndef PARTICLE__H
#define PARTICLE__H

#include "PartsDef.h"
#include "Textures.h"
#include "Application.h"
#include "Render.h"
#include "Part.h"
#include "EngineElements/ParticleEmitter.h"

#include <list>
#include <vector>
#include <utility>
#include <unordered_set>


class DLL_EXPORT Particles : public Part
{
public:
	Particles(EngineAPI& aAPI);
	
	ParticleEmitter* AddParticleEmitter(particle_preset* particle_preset, float x, float y, float lifespan = -1, int depth = 0);
	void RemoveParticleEmitter(ParticleEmitter* _to_delete);

	class ParticlesImpl;
};

#endif // !PHYSICS__H