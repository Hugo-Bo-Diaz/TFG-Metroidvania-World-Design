#include "HazardWaterfall.h"

#include "../Player.h"
#include "Modules/Render.h"
#include "Application.h"
#include "../../Spells/Fire.h"

HazardWaterfall::HazardWaterfall()
{
}

void HazardWaterfall::Init()
{
	Engine->GetModule<::Render>().LoadTexture("Assets/Sprites/hazards.png", hazards);
	Engine->GetModule<::Render>().LoadTexture("Assets/Sprites/particles.png", particles);

	anim.AddFrame({ 192,0,48,48 });
	anim.AddFrame({ 240,0,48,48 });
	anim.AddFrame({ 192,48,48,48 });
	anim.AddFrame({ 240,48,48,48 });
	anim.mTexture = hazards;

	r18waterfirst = { 0,48,12,12 };
	r19watersecond = { 12,48,12,12 };
	r20waterthird = { 24,48,12,12 };
	water_drip.area_in_texture.push_back(&r18waterfirst);
	water_drip.area_in_texture.push_back(&r19watersecond);
	water_drip.area_in_texture.push_back(&r20waterthird);
	water_drip.name = "water_drip";
	water_drip.minmax_x_offset = std::make_pair(12, 36);
	water_drip.minmax_y_offset = std::make_pair(0, 5);
	water_drip.minmax_speed_x = std::make_pair(-2.0, 2.0);
	water_drip.minmax_speed_y = std::make_pair(0, 0);
	water_drip.minmax_scale = std::make_pair(1, 1);
	water_drip.minmax_lifespan = std::make_pair(200, 500);
	water_drip.minmax_frequency = std::make_pair(10, 20);
	water_drip.minmax_acc_y = std::make_pair(0.1, 0.2);
	water_drip.texture_name = particles;

	mEmitter = Engine->GetModule<::Render>().AddParticleEmitter(&water_drip, collider.x, collider.y, -1.0f, -50);
	mEmitter->Stop();
}

bool HazardWaterfall::Loop(float dt)
{
	std::vector<collision> collisions;
	Engine->GetModule<SceneController>().GetCollisions(&collider, collisions);

	bool isCollidingWithPlayer = false;
	for (std::vector<collision>::iterator it = collisions.begin(); it != collisions.end(); it++)
	{
		if ((*it).object != this)
		{
			if ((*it).object->IsSameTypeAs<Player>())
			{
				Player* lPlayer = (Player*)(*it).object;
				lPlayer->speed_y += 1;
				isCollidingWithPlayer = true;
				mEmitter->position_x = lPlayer->collider.x + 12;
				mEmitter->position_y = lPlayer->collider.y;
			}
		}
	}

	if (isCollidingWithPlayer)
	{
		if (!mEmitter->active)
		{
			mEmitter->Resume();
		}
	}
	else
	{
		if (mEmitter->active)
		{
			mEmitter->Stop();
		}
	}

	if (water_animation.Read() > iteration_time)
	{
		anim.NextFrame();
		water_animation.Reset();
	}
	return true;
}

bool HazardWaterfall::Render()
{
	int tilesx = collider.w / 48;
	int tilesy = collider.h / 48;

	for (int i = 0; i < tilesx; ++i)
	{
		for (int j = 0; j < tilesy; ++j)
		{
			Engine->GetModule<::Render>().RenderAnimation(anim, collider.x + 48 * i, collider.y + 48 * j, -50);
		}
	}
	return true;
}