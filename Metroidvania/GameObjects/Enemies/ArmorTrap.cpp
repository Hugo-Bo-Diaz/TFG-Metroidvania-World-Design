#include "ArmorTrap.h"
#include "Application.h"
#include "Render.h"
#include "Textures.h"
#include "Particles.h"
#include "Audio.h"
#include "../EntityIDs.h"

ArmorTrap::ArmorTrap()
{
	idle = {0,0,48,72};

	left.AddFrame({ 48,0,48,72 });
	left.AddFrame({ 96,0,48,72 });

	right.AddFrame({ 144,0,48,72 });
	right.AddFrame({ 192,0,48,72 });

	aggro.w = 300;
	aggro.h = 144;
}

ArmorTrap::~ArmorTrap()
{
	//ADD PARTICLES
	App->par->AddParticleEmitter(&App->par->fire_ge_death, collider->x, collider->y, 200);

}

bool ArmorTrap::Loop(float dt)
{
	//STEP 1
	collider->x = nextpos->x;
	collider->y = nextpos->y;

	//STEP 2
	speed_y += acceleration_y;

	nextpos->x += speed_x;
	nextpos->y += speed_y;

	aggro.x = collider->x + collider->w/2 - aggro.w/2;
	aggro.y = collider->y + collider->h - aggro.h;

	std::vector<collision*> collisions;
	App->phy->GetCollisions(&aggro, collisions);

	isplayernearby = false;
	for (std::vector<collision*>::iterator it = collisions.begin(); it != collisions.end(); it++)
	{
		if ((*it)->object != this)
		{
			if ((*it)->type == PLAYER_ID)
			{
				isplayernearby = true;
				target = (*it)->object;
			}
		}
	}
	if (!isplayernearby)
	{
	}

	switch (current_state)
	{
	case ArmorTrap_IDLE:
		{
			if (isplayernearby)
			{
				current_state = ArmorTrap_CHASE;
			}
			speed_x = 0;
		}
		break;
	case ArmorTrap_PATROL:
	{
		if (isplayernearby)
		{
			current_state = ArmorTrap_CHASE;
		}
	}
		break;
	case ArmorTrap_CHASE:
	{
		int direction = 0;
		int distance = target->collider->x - collider->x;
		if (distance > 0)
			direction = 1;
		else
			direction = -1;

		speed_x = direction * chase_speed;

		if (!isplayernearby)
		{
			current_state = ArmorTrap_PATROL;
			speed_x = direction * patrol_speed;
			target = nullptr;
		}
	}
		break;
	default:
		break;
	}


	std::vector<SDL_Rect*> colliders;
	App->phy->GetNearbyWalls(collider->x + collider->w / 2, collider->y + collider->h / 2, 100, colliders);

	bool change_direction = false;
	bool floor_below = false;
	if (speed_y >acceleration_y)
	{
		floor_below = true;
	}

	for (int i = 0; i < colliders.size(); ++i)
	{
		SDL_Point p1;//left(should not collide)
		SDL_Point p2;//down(should collide)
		if (speed_x < 0)//left
		{
			p1.x = collider->x - abs(speed_x);
			p1.y = collider->y + collider->h / 2;

			p2.x = collider->x - abs(speed_x);
			p2.y = collider->y + collider->h + 10;

		}
		else if (speed_x > 0)
		{
			p1.x = collider->x + collider->w + abs(speed_x);
			p1.y = collider->y + collider->h / 2;

			p2.x = collider->x + collider->w + abs(speed_x);
			p2.y = collider->y + collider->h + 10;
		}

		SDL_Rect result;
		if (SDL_IntersectRect(colliders[i], nextpos, &result) == SDL_TRUE && collider->y < colliders[i]->y)// he goin crash!
		{
			speed_y = 0;
			nextpos->y -= result.h;

			if (knocked_up)
			{
				speed_x = patrol_speed;
				knocked_up = false;
			}
		}

		if (SDL_PointInRect(&p2, colliders[i]) == SDL_TRUE)
		{
			floor_below = true;
		}

		if (SDL_PointInRect(&p1, colliders[i]) == SDL_TRUE)
		{
			change_direction = true;
		}

	}
	if ((change_direction || !floor_below) && (!knocked_up || change_direction))
	{
		speed_x = -speed_x;
	}

	return true;
}

bool ArmorTrap::Render()
{

	if (App->debug)
		App->ren->DrawRect(&aggro, 255, 255, 0, 100, true);

	if ((animation_timer.Read() > animation_interval_chase && current_state == ArmorTrap_CHASE)||(animation_timer.Read() > animation_interval_patrol && current_state == ArmorTrap_PATROL))
	{
		if (speed_x < 0)
			left.NextFrame();
		else
			right.NextFrame();

		animation_timer.Reset();
	}


	if (current_state == ArmorTrap_IDLE)
	{
		App->ren->Blit(App->tex->Get_Texture("armortrap"), collider->x, collider->y, &idle, 0);
	}
	else
	{
		if (speed_x < 0)
			App->ren->Blit(App->tex->Get_Texture("armortrap"), collider->x, collider->y, left.GetCurrentFrame(), 0);
		else
			App->ren->Blit(App->tex->Get_Texture("armortrap"), collider->x, collider->y, right.GetCurrentFrame(), 0);
	}


	return true;
}

void ArmorTrap::RecieveDamage(int dmg, int direction)
{
	App->aud->PlaySFX(SFX_ENEMY_HIT);

	health -= dmg;
	if (health <= 0)
	{
		App->phy->DeleteObject(this);
	}
	else
	{
		App->par->AddParticleEmitter(&App->par->metal, collider->x, collider->y, 300);
	}

	speed_x = direction * 6;
	speed_y = -10;
	knocked_up = true;
}