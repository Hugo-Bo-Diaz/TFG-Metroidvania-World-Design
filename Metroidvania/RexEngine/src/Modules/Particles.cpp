#include "RXpch.h"
#include "Modules/Particles.h"
#include "Utils/Logger.h"
#include "EngineAPI.h"

#include "ParticlesImpl.h"
#include "RenderImpl.h"

Particles::Particles(EngineAPI& aAPI) : Part("Particles",aAPI)
{
	mPartFuncts = new ParticlesImpl(this);
}

#pragma region IMPLEMENTATION

bool Particles::ParticlesImpl::Loop(float dt)
{
	for (std::list<ParticleEmitter*>::iterator it = particles.begin(); it != particles.end(); it++)
	{
		if (!(*it)->Loop(dt))
		{
			to_delete.insert(*it);
		}
	}

	for (std::unordered_set<ParticleEmitter*>::iterator it = to_delete.begin(); it != to_delete.end(); it++)
	{
		delete(*it);
		particles.erase(std::find(particles.begin(), particles.end(), *it));
	}
	to_delete.clear();

	for (std::list<ParticleEmitter*>::iterator it = particles.begin(); it != particles.end(); it++)
	{
		mPartInst->mApp.GetImplementation<Render,Render::RenderImpl>()->BlitParticleEmitter(*it, RenderQueue::RENDER_GAME);
	}

	return true;
}

bool Particles::ParticlesImpl::CleanUp()
{
	for (std::list<ParticleEmitter*>::iterator it = particles.begin(); it != particles.end(); it++)
	{
		if(*std::find(particles.begin(), particles.end(), *it))
			delete(*it);
	}
	particles.clear();
	return true;
}

void Particles::ParticlesImpl::ClearParticles()
{
	Logger::Console_log(LogLevel::LOG_INFO, "Clearing Particles");
	for (std::list<ParticleEmitter*>::iterator it = particles.begin(); it != particles.end(); it++)
	{
		to_delete.insert(*it);
	}
}

#pragma endregion

#pragma region PUBLIC API

ParticleEmitter* Particles::AddParticleEmitter(particle_preset * particle_preset, float x, float y, float lifespan,int depth)
{
	ParticlesImpl* lImpl = dynamic_cast<ParticlesImpl*>(mPartFuncts);
	if (!lImpl)
	{
		Logger::Console_log(LogLevel::LOG_ERROR, "Wrong format on the implementation class");
		return 0;
	}

	ParticleEmitter* emit = new ParticleEmitter(particle_preset, lifespan, x, y, depth);
	lImpl->particles.push_back(emit);
	return emit;
}

void Particles::RemoveParticleEmitter(ParticleEmitter * _to_delete)
{
	ParticlesImpl* lImpl = dynamic_cast<ParticlesImpl*>(mPartFuncts);
	if (!lImpl)
	{
		Logger::Console_log(LogLevel::LOG_ERROR, "Wrong format on the implementation class");
		return;
	}

	lImpl->to_delete.insert(_to_delete);
}

#pragma endregion