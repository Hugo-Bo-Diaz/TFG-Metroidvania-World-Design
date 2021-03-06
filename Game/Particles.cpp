#include "Particles.h"

Particles::Particles()
{
	name = "Particles";
}

bool Particles::Init()
{
	//declare/load particle presets

	r1 = { 6,12,12,12 };
	r2 = { 6,24,12,12 };
	fire.area_in_texture.push_back(&r1);
	fire.area_in_texture.push_back(&r2);
	fire.name = "fire";
	fire.minmax_speed_y = std::make_pair(-1,-3);
	fire.texture_name = "fireball";

	return true;
}

bool Particles::Loop(float dt)
{
	//printf("%d", particles.size());
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