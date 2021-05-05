#include "Particles.h"

Particles::Particles()
{
	name = "Particles";
}

bool Particles::Init()
{
	//declare/load particle presets

	r1exp = { 0,0,12,12 };
	explosion.area_in_texture.push_back(&r1exp);
	explosion.name = "explosion";
	explosion.minmax_x_offset = std::make_pair(-20,20);
	explosion.minmax_y_offset = std::make_pair(-20,20);
	explosion.minmax_speed_y = std::make_pair(-2,-3);
	explosion.minmax_speed_x = std::make_pair(-0.6, 0.6);
	explosion.minmax_scale_speed = std::make_pair(0.01,0.02);
	explosion.minmax_lifespan = std::make_pair(200,500);
	explosion.minmax_frequency = std::make_pair(10,50);
	explosion.minmax_acc_y = std::make_pair(0.05,0.2);
	explosion.texture_name = "particles";

	r2shield = { 12,0,12,12 };
	r12shield = { 24,0,12,12 };
	fireshield.area_in_texture.push_back(&r2shield);
	fireshield.area_in_texture.push_back(&r2shield);
	fireshield.area_in_texture.push_back(&r12shield);
	fireshield.name = "fireshield";
	fireshield.minmax_x_offset = std::make_pair(-5, 69);
	fireshield.minmax_y_offset = std::make_pair(0, 70);
	fireshield.minmax_speed_y = std::make_pair(-1.5, -2.5);
	fireshield.minmax_speed_x = std::make_pair(-0.1, 0.1);
	fireshield.minmax_scale_speed = std::make_pair(-0.03, -0.04);
	fireshield.minmax_scale = std::make_pair(1, 1.5);
	fireshield.minmax_acc_y = std::make_pair(0.04, 0.05);
	fireshield.minmax_lifespan = std::make_pair(300, 400);
	fireshield.minmax_frequency = std::make_pair(5, 20);
	fireshield.texture_name = "particles";

	r3lava = { 36,0,12,12 };
	lava.area_in_texture.push_back(&r3lava);
	lava.name = "lava";
	lava.minmax_speed_y = std::make_pair(-3, -4);
	lava.minmax_speed_x = std::make_pair(-2, 2);
	lava.minmax_lifespan = std::make_pair(200, 300);
	lava.minmax_x_offset = std::make_pair(0, 64);
	lava.minmax_y_offset = std::make_pair(60, 64);
	lava.minmax_acc_y = std::make_pair(0.4, 0.5);
	lava.minmax_frequency = std::make_pair(5, 20);
	lava.texture_name = "particles";

	r4ice = { 36,12,12,12 };
	ice.area_in_texture.push_back(&r4ice);
	ice.name = "ice";
	ice.minmax_speed_y = std::make_pair(-0.2, 0.2);
	ice.minmax_speed_x = std::make_pair(-0.4, 0.4);
	ice.minmax_lifespan = std::make_pair(75, 150);
	ice.minmax_x_offset = std::make_pair(22, 42);
	ice.minmax_y_offset = std::make_pair(4, 20);
	ice.minmax_frequency = std::make_pair(5, 20);
	ice.texture_name = "particles";

	r5slash = { 0,12,12,12 };
	windslash.area_in_texture.push_back(&r5slash);
	windslash.name = "windslash";
	windslash.minmax_speed_y = std::make_pair(-0.1, 0.1);
	windslash.minmax_speed_x = std::make_pair(-0.1, 0.1);
	windslash.minmax_lifespan = std::make_pair(75, 150);
	windslash.minmax_angle_speed = std::make_pair(5,15);
	windslash.minmax_angle = std::make_pair(0, 360);
	windslash.minmax_x_offset = std::make_pair(0, 48);
	windslash.minmax_y_offset = std::make_pair(0, 48);
	windslash.minmax_frequency = std::make_pair(5, 20);
	windslash.texture_name = "particles";


	r6buff = { 12,12,12,12 };
	r7buff = { 24,12,12,12 };
	windbuff.area_in_texture.push_back(&r6buff);
	windbuff.area_in_texture.push_back(&r7buff);
	windbuff.name = "windbuff";
	windbuff.minmax_speed_y = std::make_pair(-1, 1);
	windbuff.minmax_speed_x = std::make_pair(-1, 1);
	windbuff.minmax_lifespan = std::make_pair(75, 150);
	windbuff.minmax_x_offset = std::make_pair(0, 48);
	windbuff.minmax_y_offset = std::make_pair(40, 48);
	windbuff.minmax_frequency = std::make_pair(5, 10);
	windbuff.texture_name = "particles";

	r8ground = { 0,24,12,12 };
	r9ground = { 12,24,12,12 };
	groundcontact.area_in_texture.push_back(&r8ground);
	groundcontact.area_in_texture.push_back(&r9ground);
	groundcontact.name = "ground";
	groundcontact.minmax_speed_y = std::make_pair(-6, -4);
	groundcontact.minmax_speed_x = std::make_pair(-3, 3);
	groundcontact.minmax_lifespan = std::make_pair(75, 150);
	groundcontact.minmax_angle_speed = std::make_pair(5, 15);
	groundcontact.minmax_angle = std::make_pair(0, 360);
	groundcontact.minmax_x_offset = std::make_pair(-15, 15);
	groundcontact.minmax_y_offset = std::make_pair(-15, 15);
	groundcontact.minmax_acc_y = std::make_pair(1, 2);
	groundcontact.minmax_frequency = std::make_pair(10, 25);
	groundcontact.texture_name = "particles";

	r10grass = { 24,24,12,12 };
	r11grass = { 36,24,12,12 };
	grass.area_in_texture.push_back(&r10grass);
	grass.area_in_texture.push_back(&r11grass);
	grass.name = "grass";
	grass.minmax_x_offset = std::make_pair(-20, 20);
	grass.minmax_y_offset = std::make_pair(-20, 20);
	grass.minmax_angle_speed = std::make_pair(5, 15);
	grass.minmax_angle = std::make_pair(0, 360);
	grass.minmax_scale_speed = std::make_pair(0.03, 0.04);
	grass.minmax_scale = std::make_pair(1, 1.2);
	grass.minmax_lifespan = std::make_pair(100, 200);
	grass.minmax_frequency = std::make_pair(5, 20);
	grass.texture_name = "particles";



	r13magic = { 0,36,12,12 };
	magic.area_in_texture.push_back(&r13magic);
	magic.name = "magic";
	magic.minmax_x_offset = std::make_pair(-5, 53);
	magic.minmax_y_offset = std::make_pair(24, 55);
	magic.minmax_speed_y = std::make_pair(-0.3, -0.5);
	magic.minmax_scale = std::make_pair(0.5, 1);
	magic.minmax_angle_speed = std::make_pair(5, 15);
	magic.minmax_angle = std::make_pair(0, 360);
	magic.minmax_lifespan = std::make_pair(300, 400);
	magic.minmax_frequency = std::make_pair(20, 40);
	magic.texture_name = "particles";

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
	for (std::list<ParticleEmitter*>::iterator it = particles.begin(); it != particles.end(); it++)
	{
		//to_delete.push_back(*it);
		RemoveParticleEmitter(*it);
	}
}
