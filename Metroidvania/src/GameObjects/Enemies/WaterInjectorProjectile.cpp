#include "WaterInjectorProjectile.h"
#include "Application.h"
#include "Modules/Input.h"
#include "Modules/Render.h"
#include "Modules/Audio.h"

#include "../Player.h"
#include "Modules/Camera.h"


WaterInjectorProjectile::WaterInjectorProjectile()
{
}

void WaterInjectorProjectile::Destroy()
{
	Engine->GetModule<::Render>().AddParticleEmitter(&water, collider.x, collider.y, 300);
}

void WaterInjectorProjectile::Init()
{
	Engine->GetModule<::Render>().LoadTexture("Assets/Sprites/enemies/Injector.png", injector);
	Engine->GetModule<::Render>().LoadTexture("Assets/Sprites/particles.png", particles);

	r18waterfirst = { 0,48,12,12 };
	r19watersecond = { 12,48,12,12 };
	r20waterthird = { 24,48,12,12 };

	water.area_in_texture.push_back(&r18waterfirst);
	water.area_in_texture.push_back(&r19watersecond);
	water.area_in_texture.push_back(&r20waterthird);
	water.name = "water_death";
	water.minmax_x_offset = std::make_pair(0, 48);
	water.minmax_y_offset = std::make_pair(0, 48);
	water.minmax_speed_x = std::make_pair(-0.6, 0.6);
	water.minmax_speed_y = std::make_pair(-1, -2);
	water.minmax_scale = std::make_pair(1, 1);
	water.minmax_angle = std::make_pair(0, 360);
	water.minmax_lifespan = std::make_pair(200, 500);
	water.minmax_frequency = std::make_pair(7, 15);
	water.minmax_acc_y = std::make_pair(0.1, 0.3);
	water.texture_name = particles;

	projectile.AddFrame({ 144,48,48,48 });
	projectile.mTexture = injector;
}

bool WaterInjectorProjectile::Loop(float dt)
{
	bool ret = true;

	speed_y += accel_y;

	collider.y += speed_y;

	std::vector<RXRect*> colliders;
	Engine->GetModule<SceneController>().GetNearbyWalls(collider.x + collider.w / 2, collider.y + collider.h / 2, 50, colliders);

	for (int i = 0; i < colliders.size(); ++i)
	{
		RXRect result;
		if (RXRectCollision(colliders[i], &collider, &result) == true)// he goin crash!
		{
			Engine->GetModule<SceneController>().DeleteObject(this);
		}
	}

	std::vector<collision> collisions;
	Engine->GetModule<SceneController>().GetCollisions(&collider, collisions);

	for (std::vector<collision>::iterator it = collisions.begin(); it != collisions.end(); it++)
	{
		if ((*it).object != this)
		{
			if ((*it).object->IsSameTypeAs<Player>())
			{

			}
		}
	}

	return ret;
}

bool WaterInjectorProjectile::Render()
{
		Engine->GetModule<::Render>().RenderAnimation(projectile, collider.x, collider.y, -2);

	return true;
}

void WaterInjectorProjectile::Fire(float _speed_x, float _speed_y)
{
	speed_y = _speed_y;
}
