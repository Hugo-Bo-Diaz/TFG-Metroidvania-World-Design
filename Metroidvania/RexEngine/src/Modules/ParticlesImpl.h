#ifndef PARTICLES_IMPL__H
#define PARTICLES_IMPL__H

#include "../include/Modules/Particles.h"
#include "PartImpl.h"
#include <unordered_set>

class Particles::ParticlesImpl : public Part::Part_Impl
{
public:
	Particles::ParticlesImpl(Particles* aParticles):mPartInst(aParticles){};

	void SetParticlesInst(Particles* aParticles)
	{
		mPartInst = aParticles;
	}
	void ClearParticles();

protected:
	bool Init();
	bool Loop(float dt);
	bool CleanUp();

private:
	std::unordered_set<ParticleEmitter*> to_delete;
	std::list<ParticleEmitter*> particles;
	friend class Particles;

	Particles* mPartInst;
};

#endif