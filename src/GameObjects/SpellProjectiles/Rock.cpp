#include "Rock.h"
#include "Application.h"
#include "Modules/Input.h"
#include "Modules/Render.h"
#include "Modules/Particles.h"
#include "Modules/Camera.h"
#include "Modules/Audio.h"

#include "../Enemies/CoalJumper.h"
#include "../Enemies/GroundedElemental.h"
#include "../Enemies/FlyingElemental.h"
#include "../Enemies/ArmorTrap.h"
#include "../Enemies/ShieldMonster.h"
#include "../Enemies/ClingingCreature.h"
#include "../Enemies/FlyingAxe.h"
#include "../Enemies/FlyingShield.h"

#include "../Hazards/HazardsRockBlock.h"

Rock::Rock()
{
}

Rock::~Rock()
{
	Engine->GetModule<Particles>().AddParticleEmitter(&rockblockexplosion, collider->x + collider->w / 2, collider->y + collider->h / 2, 300);
}

void Rock::Init()
{
	particles = Engine->GetModule<Textures>().Load_Texture("Assets/Sprites/particles.png");
	spells = Engine->GetModule<Textures>().Load_Texture("Assets/Sprites/spells.png");

	mSFXGroundHit = Engine->GetModule<Audio>().LoadSFX("Assets/SFX/hit_floor.wav");

	r8ground = { 0,24,12,12 };
	r9ground = { 12,24,12,12 };
	rockblockexplosion.area_in_texture.push_back(&r8ground);
	rockblockexplosion.area_in_texture.push_back(&r9ground);
	rockblockexplosion.name = "ground";
	rockblockexplosion.minmax_speed_y = std::make_pair(-6, -4);
	rockblockexplosion.minmax_speed_x = std::make_pair(-3, 3);
	rockblockexplosion.minmax_lifespan = std::make_pair(75, 150);
	rockblockexplosion.minmax_angle_speed = std::make_pair(5, 15);
	rockblockexplosion.minmax_angle = std::make_pair(0, 360);
	rockblockexplosion.minmax_x_offset = std::make_pair(-25, 25);
	rockblockexplosion.minmax_y_offset = std::make_pair(-25, 25);
	rockblockexplosion.minmax_acc_y = std::make_pair(1, 2);
	rockblockexplosion.minmax_frequency = std::make_pair(10, 25);
	rockblockexplosion.texture_name = particles;

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
	groundcontact.texture_name = particles;

	rock_sprite.AddFrame({ 96,160,32,32 });
}

bool Rock::Loop(float dt)
{
	bool ret = true;

	collider->x += x_speed;

	collider->y += y_speed;

	y_speed += gravity;

	std::vector<collision*> collisions;
	Engine->GetModule<ObjectManager>().GetCollisions(collider, collisions);

	for (std::vector<collision*>::iterator it = collisions.begin(); it != collisions.end(); it++)
	{
		if ((*it)->object != this)
		{
			if ((*it)->object->IsSameTypeAs<HazardRockBlock>())
			{
				//delete this object :^)
				Engine->GetModule<ObjectManager>().DeleteObject(this);
				Engine->GetModule<ObjectManager>().DeleteObject((*it)->object);
			}

			if ((*it)->object->IsSameTypeAs<Enemy>())
			{
				((Enemy*)(*it)->object)->RecieveDamage(damage, direction);
				Engine->GetModule<ObjectManager>().DeleteObject(this);
			}

		}
	}


	std::vector<SDL_Rect*> colliders;
	Engine->GetModule<ObjectManager>().GetNearbyWalls(collider->x + collider->w / 2, collider->y + collider->h / 2, 50, colliders);

	for (int i = 0; i < colliders.size(); ++i)
	{
		SDL_Rect result;
		if (SDL_IntersectRect(colliders[i], collider, &result) == SDL_TRUE)// he goin crash!
		{
			Engine->GetModule<Audio>().PlaySFX(mSFXGroundHit);
			Engine->GetModule<ObjectManager>().DeleteObject(this);
			Engine->GetModule<Particles>().AddParticleEmitter(&groundcontact, collider->x + collider->w/2, collider->y + collider->h/2, 100);
			Engine->GetModule<Camera>().CameraShake(15, 60);
		}
	}

	return ret;
}

bool Rock::Render()
{
	Engine->GetModule<::Render>().Blit(spells, collider->x, collider->y, rock_sprite.GetCurrentFrame(), -2);

	return true;
}

void Rock::Fire(bool left_dir, float angle, float speed, float _gravity)
{
	//angle = _angle;

	x_speed = speed * cos(angle*(3.1428 / 180));
	y_speed = -speed * sin(angle*(3.1428 / 180));
	
	direction = 1;

	if (!left_dir)
	{
		x_speed = -x_speed;
		direction = -1;
	}
	
	gravity = _gravity;
}