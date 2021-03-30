#include "ParticleEmitter.h"
#include "Application.h"
#include "Textures.h"
#include "Render.h"
ParticleEmitter::ParticleEmitter(particle_preset * preset, float lifespan, float p_x,float p_y)
{
	position_x = p_x;
	position_y = p_y;
	preset_for_emitter = preset;
	total_lifespan = lifespan;
	
	for (int i = 0; i < MAX_PARTICLES; ++i)
	{
		particles[i] == NULL;
	}

}
float RandomFloat(float a, float b) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}

bool ParticleEmitter::Loop(float dt)
{
	bool ret = true;
	if (active)//spawn more particles
	{
		if (RandomFloat(preset_for_emitter->minmax_frequency.first, preset_for_emitter->minmax_frequency.second) < particles_spawn.Read())//check if it should spawn em
		{
			particle* p = new particle();
			p->angle = RandomFloat(preset_for_emitter->minmax_angle.first, preset_for_emitter->minmax_angle.second);
			p->angle_speed = RandomFloat(preset_for_emitter->minmax_angle_speed.first, preset_for_emitter->minmax_angle_speed.second);

			p->lifespan = RandomFloat(preset_for_emitter->minmax_lifespan.first, preset_for_emitter->minmax_lifespan.second);

			p->texture = App->tex->Get_Texture(preset_for_emitter->texture_name.c_str());
			p->area_in_texture = preset_for_emitter->area_in_texture[(rand() % static_cast<int>(preset_for_emitter->area_in_texture.size()))];

			p->target_on_screen.x = RandomFloat(preset_for_emitter->minmax_x_offset.first, preset_for_emitter->minmax_x_offset.second) + position_x;
			p->target_on_screen.y = RandomFloat(preset_for_emitter->minmax_y_offset.first, preset_for_emitter->minmax_y_offset.second) + position_y;

			p->current_scale = RandomFloat(preset_for_emitter->minmax_scale.first, preset_for_emitter->minmax_scale.second);
			p->target_on_screen.w = (float)p->area_in_texture->w * (float)p->current_scale;
			p->target_on_screen.h = (float)p->area_in_texture->h * (float)p->current_scale;
			
			p->speed_x = RandomFloat(preset_for_emitter->minmax_speed_x.first, preset_for_emitter->minmax_speed_x.second);
			p->speed_y = RandomFloat(preset_for_emitter->minmax_speed_y.first, preset_for_emitter->minmax_speed_y.second);
			
			p->acc_x = RandomFloat(preset_for_emitter->minmax_acc_x.first, preset_for_emitter->minmax_acc_x.second);
			p->acc_y = RandomFloat(preset_for_emitter->minmax_acc_y.first, preset_for_emitter->minmax_acc_y.second);

			p->scale_speed = RandomFloat(preset_for_emitter->minmax_scale_speed.first, preset_for_emitter->minmax_scale_speed.second);

			for (int i = 0; i < MAX_PARTICLES; ++i)
			{
				if (particles[i] == NULL)
				{
					particles[i] = p;
					break;
				}
			}

			particles_spawn.Reset();
		}
	}

	//update the rest of the particles
	for (int i = 0; i < MAX_PARTICLES; ++i)
	{
		if (particles[i] != NULL)
		{
			if(!particles[i]->Update(dt))
			{
				delete particles[i];
				particles[i] = nullptr;
			}
		}
	}

	if(life_emitter.Read() > total_lifespan&&total_lifespan!=-1)
	{
		Stop_and_Destroy();
	}
	if(!active && !alive)
	{
		bool destroy = true;
		for (int i = 0; i < MAX_PARTICLES; ++i)
		{
			if (particles[i] != NULL)
			{
				destroy = false;
				break;
			}
		}
		if(destroy)
		{
			Destroy();
			ret = false;
		}
	}
	r.x = position_x;
	r.y = position_y;
	//App->ren->DrawRect(&r, 255, 255, 255, 255, true);
	return ret;
}

void ParticleEmitter::Resume()
{
	active = true;
}

void ParticleEmitter::Stop()
{
	active = false;
}

void ParticleEmitter::Destroy()
{
	for (int i = 0; i < MAX_PARTICLES; ++i)
	{
		if (particles[i] != NULL)
		{
			delete particles[i];
		}
	}
	
}

void ParticleEmitter::Stop_and_Destroy()
{
	Stop();
	alive = false;
}
