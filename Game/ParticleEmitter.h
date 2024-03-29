#ifndef PARTICLE_EMITTER__H
#define PARTICLE_EMITTER__H

#include <vector>
#include "SDL/include/SDL.h"
#include "Timer.h"

#include "Application.h"
#include "Render.h"

#define MAX_PARTICLES 2000



struct SDL_Texture;

class particle_preset
{
public:

	std::string name;
	std::string texture_name;

	std::vector<SDL_Rect*> area_in_texture;

	std::pair<float, float> minmax_angle = std::make_pair(0.0f, 0.0f);
	std::pair<float, float> minmax_angle_speed = std::make_pair(0.0f, 0.0f);

	std::pair<float, float> minmax_scale = std::make_pair(1.0f, 1.0f);
	std::pair<float, float> minmax_scale_speed = std::make_pair(0.0f, 0.0f);

	std::pair<float, float> minmax_lifespan = std::make_pair(1000.0f, 2000.0f);

	std::pair<float, float> minmax_speed_x = std::make_pair(0.0f, 0.0f);
	std::pair<float, float> minmax_speed_y = std::make_pair(0.0f, 0.0f);

	std::pair<float, float> minmax_acc_x = std::make_pair(0.0f, 0.0f);
	std::pair<float, float> minmax_acc_y = std::make_pair(0.0f, 0.0f);

	std::pair<float, float> minmax_x_offset = std::make_pair(-50.0f, 50.0f);
	std::pair<float, float> minmax_y_offset = std::make_pair(-50.0f, 50.0f);

	std::pair<float, float> minmax_frequency = std::make_pair(100.0f, 200.0f);

	~particle_preset()
	{
		for (std::vector<SDL_Rect*>::iterator it = area_in_texture.begin(); it != area_in_texture.end(); ++it)
		{
			//if (*it != nullptr)
				//delete *it;
		}
	}
};

struct particle
{
	SDL_Texture* texture;
	SDL_Rect* area_in_texture;

	float angle;
	float angle_speed;
	SDL_Rect target_on_screen;
	float scale_speed;
	float current_scale;
	Timer life;
	float lifespan;
	float speed_x;
	float speed_y;

	float acc_x;
	float acc_y;

	particle() {
		life.Start();
		life.Reset();
	}

	bool Update(float dt)
	{
		bool ret = true;

		speed_x += acc_x;
		speed_y += acc_y;

		target_on_screen.x += speed_x;
		target_on_screen.y += speed_y;
		current_scale += scale_speed;
		if(current_scale<=0)
		{
			current_scale = 0;
		}
		angle += angle_speed;

		//App->ren->DrawRect(&target_on_screen, 255, 0, 0, 255, true);
		//if it's dead, return false :D
		if (life.Read() > lifespan)
		{
			ret = false;
		}
		return ret;
	}
};


class ParticleEmitter
{
public:
	Timer life_emitter;
	float total_lifespan;
	Timer particles_spawn;
	particle_preset* preset_for_emitter;
	bool active = true;
	bool alive = true;
	float position_x;
	float position_y;

	ParticleEmitter() {};
	ParticleEmitter(particle_preset*, float lifespan,float p_x, float p_y);

	bool Loop(float dt);
	void Stop();
	void Resume();
	void Destroy();
	void Stop_and_Destroy();

	particle* particles [MAX_PARTICLES];

	SDL_Rect r = { 0,0,100,100 };

};



#endif