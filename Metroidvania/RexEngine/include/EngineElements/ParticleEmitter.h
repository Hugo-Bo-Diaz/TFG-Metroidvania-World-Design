#ifndef PARTICLE_EMITTER__H
#define PARTICLE_EMITTER__H

#include <vector>
#include "RXRect.h"
#include "Utils/Timer.h"

#include "Application.h"
#include "Modules/Render.h"
#include "Modules/Textures.h"

#define MAX_PARTICLES 2000
#define DLL_EXPORT __declspec(dllexport)


struct SDL_Texture;

//class that contains the definition for a particle emmitter
class DLL_EXPORT particle_preset
{
public:

	//name of the preset 
	std::string name;
	//texture 
	TextureID texture_name;

	//possible sections of the texture
	std::vector<RXRect*> area_in_texture;

	//spawn angle 
	std::pair<float, float> minmax_angle = std::make_pair(0.0f, 0.0f);
	//angle variation speed 
	std::pair<float, float> minmax_angle_speed = std::make_pair(0.0f, 0.0f);

	//inital scale
	std::pair<float, float> minmax_scale = std::make_pair(1.0f, 1.0f);
	//scale variation speed
	std::pair<float, float> minmax_scale_speed = std::make_pair(0.0f, 0.0f);

	//lifespan variation
	std::pair<float, float> minmax_lifespan = std::make_pair(1000.0f, 2000.0f);

	//speed on x
	std::pair<float, float> minmax_speed_x = std::make_pair(0.0f, 0.0f);
	//speed on y
	std::pair<float, float> minmax_speed_y = std::make_pair(0.0f, 0.0f);

	//acceleration on x
	std::pair<float, float> minmax_acc_x = std::make_pair(0.0f, 0.0f);
	//acceleration on y
	std::pair<float, float> minmax_acc_y = std::make_pair(0.0f, 0.0f);

	//variation on x coordinate
	std::pair<float, float> minmax_x_offset = std::make_pair(-50.0f, 50.0f);
	//variation on y coordinate
	std::pair<float, float> minmax_y_offset = std::make_pair(-50.0f, 50.0f);

	//spawn frequency
	std::pair<float, float> minmax_frequency = std::make_pair(100.0f, 200.0f);


	particle_preset(const particle_preset& old_obj)
	{
		minmax_angle = old_obj.minmax_angle;
		minmax_angle_speed = old_obj.minmax_angle_speed;

		minmax_scale = old_obj.minmax_scale;
		minmax_scale_speed = old_obj.minmax_scale_speed;

		minmax_lifespan = old_obj.minmax_lifespan;
		minmax_frequency = old_obj.minmax_frequency;

		minmax_speed_x = old_obj.minmax_speed_x;
		minmax_speed_y = old_obj.minmax_speed_y;

		minmax_x_offset = old_obj.minmax_x_offset;
		minmax_y_offset = old_obj.minmax_y_offset;

		minmax_acc_x = old_obj.minmax_acc_x;
		minmax_acc_y = old_obj.minmax_acc_y;
		
		name = old_obj.name;
		texture_name = old_obj.texture_name;


		for(int i = 0; i<old_obj.area_in_texture.size();++i)
		{
			RXRect* lRect = new RXRect();
			lRect->x = old_obj.area_in_texture[i]->x;
			lRect->y = old_obj.area_in_texture[i]->y;
			lRect->w = old_obj.area_in_texture[i]->w;
			lRect->h = old_obj.area_in_texture[i]->h;
			area_in_texture.push_back(lRect);
		}


	}
	particle_preset(){

	}

	~particle_preset()
	{
		for (std::vector<RXRect*>::iterator it = area_in_texture.begin(); it != area_in_texture.end(); ++it)
		{
			if (*it != nullptr)
				delete *it;
		}
	}
};

struct particle
{
	RXRect* area_in_texture;

	float angle;
	float angle_speed;
	RXRect target_on_screen;
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

/*class that processes a particle emmitter, as long as the engine is running and it has been registered they will be processed automatically
all of the properties will affect how it is presented
*/
class DLL_EXPORT ParticleEmitter
{
public:
	//if it has been set it will be destroyed once its lifespan reaches zero
	Timer life_emitter;
	//total time it will be alive
	float total_lifespan;
	//timer to keep track of spawning particles
	Timer particles_spawn;
	//preset that contains the parameters for the particles
	particle_preset* preset_for_emitter;
	//if active it will be processed
	bool active = true;
	//if not alive it will be eliminated next frame
	bool alive = true;
	//position on the x axis
	float position_x;
	//position on the y axis
	float position_y;
	//how far from the camera is it
	int depth;

	ParticleEmitter() {};
	ParticleEmitter(particle_preset*, float lifespan, float p_x, float p_y, int depth);

	bool Loop(float dt);
	void Stop();
	void Resume();
	void Destroy();
	void Stop_and_Destroy();

	particle* particles [MAX_PARTICLES];

	RXRect r = { 0,0,100,100 };
};



#endif