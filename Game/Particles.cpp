#include "Particles.h"
#include "Logger.h"


#include <sstream>

Particles::Particles()
{
	name = "Particles";
}

bool Particles::Init()
{

	return true;
}

bool Particles::Loop(float dt)
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
		App->ren->BlitParticleEmitter(*it, RenderQueue::RENDER_GAME);
	}

	return true;
}

bool Particles::CleanUp()
{
	for (std::list<ParticleEmitter*>::iterator it = particles.begin(); it != particles.end(); it++)
	{
		if(*std::find(particles.begin(), particles.end(), *it))
			delete(*it);
	}
	particles.clear();
	return true;
}

std::list<ParticleEmitter*>* Particles::GetParticleList()
{
	return &particles;
}

ParticleEmitter* Particles::AddParticleEmitter(particle_preset * particle_preset, float x, float y, float lifespan,int depth)
{
	ParticleEmitter* emit = new ParticleEmitter(particle_preset, lifespan, x, y, depth);
	particles.push_back(emit);
	return emit;
}

void Particles::RemoveParticleEmitter(ParticleEmitter * _to_delete)
{
	to_delete.insert(_to_delete);
}

void Particles::ClearParticles()
{
	Logger::Console_log(LogLevel::LOG_INFO, "Clearing Particles");
	for (std::list<ParticleEmitter*>::iterator it = particles.begin(); it != particles.end(); it++)
	{
		to_delete.insert(*it);
	}
}
