#include "WindSlash.h"
#include "Application.h"
#include "Modules/Input.h"
#include "Modules/Render.h"

#include "../Enemies/Enemy.h"


WindSlash::WindSlash()
{
	windslash.AddFrame({ 0,222,48,48 });
	windslash.AddFrame({ 48,222,48,48 });
	windslash.AddFrame({ 96,222,48,48 });
	windslash.AddFrame({ 144,222,48,48 });
}

void WindSlash::Init()
{
	Engine->GetModule<::Render>().LoadTexture("Assets/Sprites/particles.png", particles);
	Engine->GetModule<::Render>().LoadTexture("Assets/Sprites/spells.png", spells);
	windslash.mTexture = spells;

	r5slash = { 0,12,12,12 };
	windslash_part.area_in_texture.push_back(&r5slash);
	windslash_part.name = "windslash";
	windslash_part.minmax_speed_y = std::make_pair(-0.1, 0.1);
	windslash_part.minmax_speed_x = std::make_pair(-0.1, 0.1);
	windslash_part.minmax_lifespan = std::make_pair(75, 150);
	windslash_part.minmax_angle_speed = std::make_pair(5, 15);
	windslash_part.minmax_angle = std::make_pair(0, 360);
	windslash_part.minmax_x_offset = std::make_pair(0, 48);
	windslash_part.minmax_y_offset = std::make_pair(0, 48);
	windslash_part.minmax_frequency = std::make_pair(5, 20);
	windslash_part.texture_name = particles;

	p = Engine->GetModule<::Render>().AddParticleEmitter(&windslash_part, collider.x, collider.h);

}

bool WindSlash::Loop(float dt)
{
	bool ret = true;

	collider.x += direction * speed;
	windslash.NextFrame();

	p->position_x = collider.x;
	p->position_y = collider.y;

	std::vector<RXRect*> colliders;
	Engine->GetModule<SceneController>().GetNearbyWalls(collider.x + collider.w / 2, collider.y + collider.h / 2, 50, colliders);

	for (int i = 0; i < colliders.size(); ++i)
	{
		RXRect result;
		if (RXRectCollision(colliders[i], &collider, &result) == true)// he goin crash!
		{
			Engine->GetModule<SceneController>().DeleteObject(this);
			Engine->GetModule<::Render>().RemoveParticleEmitter(p);
			//Engine->GetModule<::Render>().to_delete.push_back(p);
			Engine->GetModule<::Render>().AddParticleEmitter(&windslash_part, collider.x, collider.y, 500);
		}
	}

	std::vector<collision> collisions;
	Engine->GetModule<SceneController>().GetCollisions(&collider, collisions);

	for (std::vector<collision>::iterator it = collisions.begin(); it != collisions.end(); it++)
	{
		if ((*it).object != this && !(std::find(mObjectsPassed.begin(), mObjectsPassed.end(), (*it).object) != mObjectsPassed.end()))
		{
			if ((*it).object->IsSameTypeAs<Enemy>())
			{
				health -= 1;
				if (health <= 0)
				{
					Engine->GetModule<SceneController>().DeleteObject(this);
					Engine->GetModule<::Render>().RemoveParticleEmitter(p);
				}
				mObjectsPassed.push_back((*it).object);
				((Enemy*)(*it).object)->RecieveDamage(damage, direction);
				Engine->GetModule<::Render>().AddParticleEmitter(&windslash_part, collider.x, collider.y, 500);
			}
		}
	}

	return ret;
}

bool WindSlash::Render()
{
	Engine->GetModule<::Render>().RenderAnimation(windslash, collider.x, collider.y, -2);
	return true;
}

void WindSlash::Fire(bool left_dir)
{
	if (left_dir)
		direction = 1;
	else
		direction = -1;
}