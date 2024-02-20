#include "Particles.h"
#include "Logger.h"

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
			to_delete.push_back(*it);
		}
	}


	for (std::list<ParticleEmitter*>::iterator it = to_delete.begin(); it != to_delete.end(); it++)
	{
		delete(*it);
		particles.erase(std::find(particles.begin(), particles.end(), *it));

	}
	to_delete.clear();

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

ParticleEmitter* Particles::AddParticleEmitter(particle_preset * particle_preset, float x, float y, float lifespan)
{
	ParticleEmitter* emit = new ParticleEmitter(particle_preset,lifespan,x,y);
	particles.push_back(emit);
	return emit;
}

void Particles::RemoveParticleEmitter(ParticleEmitter * _to_delete)
{
	bool is_in_array = false;

	for (std::list<ParticleEmitter*>::iterator it = to_delete.begin(); it != to_delete.end(); it++)
	{
		if ((*it) == _to_delete)
			is_in_array = true;
	}

	if (!is_in_array)
		to_delete.push_back(_to_delete);
}

void Particles::ClearParticles()
{
	Logger::Console_log(LogLevel::LOG_INFO, "Clearing Particles");
	for (std::list<ParticleEmitter*>::iterator it = particles.begin(); it != particles.end(); it++)
	{
		//to_delete.push_back(*it);
		RemoveParticleEmitter(*it);
	}
}
