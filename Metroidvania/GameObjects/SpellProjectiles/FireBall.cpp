#include "FireBall.h"
#include "Application.h"
#include "Input.h"
#include "Render.h"
#include "Particles.h"
#include "Audio.h"

#include "../Enemies/CoalJumper.h"
#include "../Enemies/GroundedElemental.h"
#include "../Enemies/FlyingElemental.h"
#include "../Enemies/ArmorTrap.h"
#include "../Enemies/ShieldMonster.h"
#include "../Enemies/ClingingCreature.h"
#include "../Enemies/FlyingAxe.h"
#include "../Enemies/FlyingShield.h"

#include "../EntityIDs.h"

FireBall::FireBall()
{
	particles = App->tex->Load_Texture("Assets/Sprites/particles.png");
	spells = App->tex->Load_Texture("Assets/Sprites/spells.png");

	fireball_big.AddFrame({0,0,64,64});
	fireball_small.AddFrame({96,0,32,32});

	r1exp = { 0,0,12,12 };
	explosion.area_in_texture.push_back(&r1exp);
	explosion.name = "explosion";
	explosion.minmax_x_offset = std::make_pair(-20, 20);
	explosion.minmax_y_offset = std::make_pair(-20, 20);
	explosion.minmax_speed_y = std::make_pair(-2, -3);
	explosion.minmax_speed_x = std::make_pair(-0.6, 0.6);
	explosion.minmax_scale_speed = std::make_pair(0.01, 0.02);
	explosion.minmax_lifespan = std::make_pair(200, 500);
	explosion.minmax_frequency = std::make_pair(10, 50);
	explosion.minmax_acc_y = std::make_pair(0.05, 0.2);
	explosion.texture_name = particles;
}

void FireBall::Destroy()
{
	App->par->AddParticleEmitter(&explosion, collider->x, collider->y, 300);
}

bool FireBall::Loop(float dt)
{
	bool ret = true;

	collider->x += direction * speed;
	nextpos->x += direction * speed;

	std::vector<SDL_Rect*> colliders;
	App->phy->GetNearbyWalls(collider->x + collider->w / 2, collider->y + collider->h / 2, 50, colliders);

	for (int i = 0; i < colliders.size(); ++i)
	{
		SDL_Rect result;
		if (SDL_IntersectRect(colliders[i], nextpos, &result) == SDL_TRUE)// he goin crash!
		{
			App->phy->DeleteObject(this);
			App->aud->PlaySFX(SFX_GROUND_HIT);
		}
	}


	std::vector<collision*> collisions;
	App->phy->GetCollisions(collider, collisions);

	for (std::vector<collision*>::iterator it = collisions.begin(); it != collisions.end(); it++)
	{
		if ((*it)->object != this)
		{
			objectId t = (*it)->type;
			if (t == COAL_JUMPER_ID || t == GROUNDED_ELEMENTAL_ID || t == FLYING_ELEMENTAL_ID ||
				t == ARMOR_TRAP_ID || t == SHIELD_MONSTER_ID || t == CLING_CREATURE_ID ||
				t == FLYING_AXE_ID || t == FLYING_SHIELD_ID)
			{
				((Enemy*)(*it)->object)->RecieveDamage(damage, direction);
				App->phy->DeleteObject(this);
			}

		}
	}

	App->phy->ClearCollisionArray(collisions);


	return ret;
}

bool FireBall::Render()
{
	if (is_big)
		App->ren->Blit(spells, collider->x, collider->y, fireball_big.GetCurrentFrame(), -2);
	else
		App->ren->Blit(spells, collider->x, collider->y, fireball_small.GetCurrentFrame(), -2);

	return true;
}

void FireBall::Fire(bool left_dir, bool _is_big)
{
	if (left_dir)
		direction = 1;
	else
		direction = -1;

	is_big = _is_big;
	
	if (!is_big)
	{
		collider->y += 16;
		collider->w = 16;
		collider->h = 16;

		nextpos->y += 16;
		nextpos->w = 16;
		nextpos->h = 16;

	}
	else
	{
	damage += 3;
	speed += 10;
	}

}
