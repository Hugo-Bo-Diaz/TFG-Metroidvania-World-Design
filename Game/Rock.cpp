#include "Rock.h"
#include "Application.h"
#include "Input.h"
#include "Console.h"
#include "Render.h"
#include "Particles.h"
#include "Camera.h"
#include "Audio.h"

#include "CoalJumper.h"
#include "GroundedElemental.h"
#include "FlyingElemental.h"
#include "ArmorTrap.h"
#include "ShieldMonster.h"
#include "ClingingCreature.h"

Rock::Rock()
{
	rock_sprite.AddFrame({ 96,160,32,32 });
}

Rock::~Rock()
{
	App->par->AddParticleEmitter(&App->par->rockblockexplosion, collider->x + collider->w / 2, collider->y + collider->h / 2, 300);
}

bool Rock::Loop(float dt)
{
	bool ret = true;

	collider->x += x_speed;
	nextpos->x += x_speed;

	collider->y += y_speed;
	nextpos->y += y_speed;

	y_speed += gravity;

	std::vector<collision*> collisions;
	App->phy->GetCollisions(collider, collisions);

	for (std::vector<collision*>::iterator it = collisions.begin(); it != collisions.end(); it++)
	{
		if ((*it)->object != this)
		{
			if ((*it)->type == HAZARDS_ROCK_BLOCK)
			{
				//delete this object :^)
				App->phy->DeleteObject(this);
				App->phy->DeleteObject((*it)->object);
			}

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


	std::vector<SDL_Rect*> colliders;
	App->phy->GetNearbyWalls(collider->x + collider->w / 2, collider->y + collider->h / 2, 50, colliders);

	for (int i = 0; i < colliders.size(); ++i)
	{
		SDL_Rect result;
		if (SDL_IntersectRect(colliders[i], nextpos, &result) == SDL_TRUE)// he goin crash!
		{
			App->aud->PlaySFX(SFX_GROUND_HIT);
			App->phy->DeleteObject(this);
			App->par->AddParticleEmitter(&App->par->groundcontact, collider->x + collider->w/2, collider->y + collider->h/2, 100);
			App->cam->CameraShake(15, 60);
		}
	}

	return ret;
}

bool Rock::Render()
{
	App->ren->Blit(App->tex->Get_Texture("spells"), collider->x, collider->y, rock_sprite.GetCurrentFrame(), -2);

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