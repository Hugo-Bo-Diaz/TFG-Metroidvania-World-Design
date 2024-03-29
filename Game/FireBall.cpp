#include "FireBall.h"
#include "Application.h"
#include "Input.h"
#include "Console.h"
#include "Render.h"
#include "Particles.h"
#include "Audio.h"

#include "CoalJumper.h"
#include "GroundedElemental.h"
#include "FlyingElemental.h"
#include "ArmorTrap.h"
#include "ShieldMonster.h"
#include "ClingingCreature.h"

FireBall::FireBall()
{
	fireball_big.AddFrame({0,0,64,64});
	fireball_small.AddFrame({96,0,32,32});
}

FireBall::~FireBall()
{
	App->par->AddParticleEmitter(&App->par->explosion, collider->x, collider->y, 300);
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
			if ((*it)->type == COAL_JUMPER)
			{
				((CoalJumper*)(*it)->object)->RecieveDamage(damage, direction);
				App->phy->DeleteObject(this);
			}
			if ((*it)->type == GROUNDED_ELEMENTAL)
			{
				((GroundedElemental*)(*it)->object)->RecieveDamage(damage, direction);
				App->phy->DeleteObject(this);
			}
			if ((*it)->type == FLYING_ELEMENTAL)
			{
				((FlyingElemental*)(*it)->object)->RecieveDamage(damage, direction);
				App->phy->DeleteObject(this);
			}
			if ((*it)->type == ARMOR_TRAP)
			{
				((ArmorTrap*)(*it)->object)->RecieveDamage(damage, direction);
				App->phy->DeleteObject(this);
			}
			if ((*it)->type == SHIELD_MONSTER)
			{
				((ShieldMonster*)(*it)->object)->RecieveDamage(damage, direction);
				App->phy->DeleteObject(this);
			}
			if ((*it)->type == CLING_CREATURE)
			{
				((ClingCreature*)(*it)->object)->RecieveDamage(damage, direction);
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
		App->ren->Blit(App->tex->Get_Texture("spells"), collider->x, collider->y, fireball_big.GetCurrentFrame(), -2);
	else
		App->ren->Blit(App->tex->Get_Texture("spells"), collider->x, collider->y, fireball_small.GetCurrentFrame(), -2);

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
