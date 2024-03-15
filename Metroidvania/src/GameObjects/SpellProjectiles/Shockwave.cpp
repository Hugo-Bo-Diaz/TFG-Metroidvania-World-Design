#include "Shockwave.h"
#include "Application.h"
#include "Modules/Input.h"
#include "Modules/Render.h"
#include "Modules/Particles.h"
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

Shockwave::Shockwave()
{

}

Shockwave::~Shockwave()
{
	Engine->GetModule<Particles>().AddParticleEmitter(&rockblockexplosion, collider->x + collider->w / 2, collider->y + collider->h / 2, 300);
	Engine->GetModule<Particles>().RemoveParticleEmitter(p);
}

void Shockwave::Init()
{
	nextpos = new RXRect();
	nextpos->x = collider->x;
	nextpos->y = collider->y;
	nextpos->w = collider->w;
	nextpos->h = collider->h;

	particles = Engine->GetModule<Textures>().Load_Texture("Assets/Sprites/particles.png");
	spells = Engine->GetModule<Textures>().Load_Texture("Assets/Sprites/spells.png");

	mSFXGroundHit = Engine->GetModule<Audio>().LoadSFX("Assets/SFX/hit_floor.wav");

	shockwave_left.AddFrame({ 0,160,32,32 });
	shockwave_right.AddFrame({ 32,160,32,32 });

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

	p = Engine->GetModule<Particles>().AddParticleEmitter(&groundcontact, collider->x, collider->y + collider->h);
}

bool Shockwave::Loop(float dt)
{
	bool ret = true;

	collider->x += x_speed;
	nextpos->x += x_speed;

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

	Engine->GetModule<ObjectManager>().ClearCollisionArray(collisions);

	floor_check.x = x_speed + nextpos->x + nextpos->w/2;

	p->position_x = collider->x+collider->w/2;
	p->position_y = collider->y+collider->h/2;

	std::vector<RXRect*> colliders;
	
	bool should_delete = true;

	Engine->GetModule<ObjectManager>().GetNearbyWalls(collider->x + collider->w / 2, collider->y + collider->h / 2, 50, colliders);

	for (int i = 0; i < colliders.size(); ++i)
	{
		RXRect result;

		if (RXRectCollision(colliders[i], nextpos, &result) == true)// he goin crash!
		{
			Engine->GetModule<ObjectManager>().DeleteObject(this);
			//Engine->GetModule<Particles>().to_delete.push_back(p);
			Engine->GetModule<Audio>().PlaySFX(mSFXGroundHit);
			Engine->GetModule<Particles>().RemoveParticleEmitter(p);
		}
		if (RXPointInRect(&floor_check, colliders[i]) == true)
		{
			should_delete = false;
		}
	}

	if (should_delete)
	{
		Engine->GetModule<ObjectManager>().DeleteObject(this);
		//Engine->GetModule<Particles>().to_delete.push_back(p);
		Engine->GetModule<Audio>().PlaySFX(mSFXGroundHit);
		Engine->GetModule<Particles>().RemoveParticleEmitter(p);
	}

	return ret;
}

bool Shockwave::Render()
{
	if (x_speed > 0)
	{
		Engine->GetModule<::Render>().Blit(spells, collider->x, collider->y, *shockwave_right.GetCurrentFrame(), -2);
	}
	else
	{
		Engine->GetModule<::Render>().Blit(spells, collider->x, collider->y, *shockwave_left.GetCurrentFrame(), -2);
	}
	return true;
}

void Shockwave::Fire(bool left_dir, float speed)
{
	//angle = _angle;

	x_speed = speed;
	direction = -1;

	if (!left_dir)
	{
		x_speed = -x_speed;
		direction = 1;
	}

	floor_check.y = nextpos->y + nextpos->h + 8;
}