#include "Shockwave.h"
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

Shockwave::Shockwave()
{
	shockwave_left.AddFrame({0,160,32,32});
	shockwave_right.AddFrame({32,160,32,32});

	p = App->par->AddParticleEmitter(&App->par->groundcontact, collider->x, collider->y + collider->h);
}

Shockwave::~Shockwave()
{
	App->par->AddParticleEmitter(&App->par->rockblockexplosion, collider->x + collider->w / 2, collider->y + collider->h / 2, 300);
	App->par->RemoveParticleEmitter(p);
}

bool Shockwave::Loop(float dt)
{
	bool ret = true;

	collider->x += x_speed;
	nextpos->x += x_speed;

	std::vector<collision*> collisions;
	App->phy->GetCollisions(collider, collisions);

	for (std::vector<collision*>::iterator it = collisions.begin(); it != collisions.end(); it++)
	{
		if ((*it)->object != this)
		{
			if ((*it)->type == HAZARDS_ROCK_BLOCK_ID)
			{
				//delete this object :^)
				App->phy->DeleteObject(this);
				App->phy->DeleteObject((*it)->object);
			}

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

	floor_check.x = x_speed + nextpos->x + nextpos->w/2;

	p->position_x = collider->x+collider->w/2;
	p->position_y = collider->y+collider->h/2;

	std::vector<SDL_Rect*> colliders;
	
	bool should_delete = true;

	App->phy->GetNearbyWalls(collider->x + collider->w / 2, collider->y + collider->h / 2, 50, colliders);

	for (int i = 0; i < colliders.size(); ++i)
	{
		SDL_Rect result;

		if (SDL_IntersectRect(colliders[i], nextpos, &result) == SDL_TRUE)// he goin crash!
		{
			App->phy->DeleteObject(this);
			//App->par->to_delete.push_back(p);
			App->aud->PlaySFX(SFX_GROUND_HIT);
			App->par->RemoveParticleEmitter(p);
		}
		if (SDL_PointInRect(&floor_check, colliders[i]) == SDL_TRUE)
		{
			should_delete = false;
		}
	}

	if (should_delete)
	{
		App->phy->DeleteObject(this);
		//App->par->to_delete.push_back(p);
		App->aud->PlaySFX(SFX_GROUND_HIT);
		App->par->RemoveParticleEmitter(p);
	}

	return ret;
}

bool Shockwave::Render()
{
	if (x_speed > 0)
	{
		App->ren->Blit(App->tex->Get_Texture("spells"), collider->x, collider->y, shockwave_right.GetCurrentFrame(), -2);
	}
	else
	{
		App->ren->Blit(App->tex->Get_Texture("spells"), collider->x, collider->y, shockwave_left.GetCurrentFrame(), -2);
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