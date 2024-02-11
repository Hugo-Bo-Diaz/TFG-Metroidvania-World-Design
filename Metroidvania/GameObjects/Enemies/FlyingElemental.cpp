#include "FlyingElemental.h"
#include "Particles.h"
#include "Application.h"
#include "Audio.h"
#include "../EntityIDs.h"

FlyingElemental::FlyingElemental()
{
}

FlyingElemental::FlyingElemental(float _initial_y)
{
	initial_y = _initial_y;
}


FlyingElemental::~FlyingElemental()
{
	App->par->AddParticleEmitter(&App->par->explosion, collider->x, collider->y, 300);
}



bool FlyingElemental::Loop(float dt)
{
	//STEP 1
	collider->x = nextpos->x;
	collider->y = nextpos->y;

	//STEP 2
	speed_y += acceleration_y;

	nextpos->x += speed_x;
	nextpos->y += speed_y;

	std::vector<SDL_Rect*> colliders;
	App->phy->GetNearbyWalls(collider->x + collider->w / 2, collider->y + collider->h / 2, 100, colliders);

	//IF IT HAS BEEN IN THIS STATE FOR MORE THAN 3 SECS GO BACKWARDS
	for (int i = 0; i < colliders.size(); ++i)
	{
		SDL_Rect result;
		if (SDL_IntersectRect(colliders[i], nextpos, &result) == SDL_TRUE)
		{
			if (result.h < result.w)
			{

				if (collider->y < colliders[i]->y)// he goin crash!
				{
					//speed_y = 0;
					nextpos->y -= result.h;
				}

				if (collider->y > colliders[i]->y)// he goin crash!
				{
					//speed_y = 0;
					nextpos->y += result.h;
					speed_y = 0;
				}
			}
			else
			{
				if (collider->x > colliders[i]->x)
				{
					nextpos->x += result.w;
				}

				if (collider->x < colliders[i]->x)
				{
					nextpos->x -= result.w;
				}
			}
		}

	}


	switch (state)
	{
	case FE_PATROL:
	{
		if (speed_x < 0)
		{
			aggro.x = collider->x - aggro.w;
			aggro.y = collider->y;
		}
		else
		{
			aggro.x = collider->x + collider->w;
			aggro.y = collider->y;
		}

		//detect player
		std::vector<collision*> collisions;
		App->phy->GetCollisions(&aggro, collisions);

		for (std::vector<collision*>::iterator it = collisions.begin(); it != collisions.end(); it++)
		{
			if ((*it)->object != this)
			{
				if ((*it)->type == PLAYER_ID)
				{
					state = FE_STARTING_CHARGE;
					starting.Reset();
					starting.Start();

				}
			}
		}

		//App->ren->DrawRect(&aggro, 0, 255, 0, 50,true);

		current_distance += speed_x;

		if (abs(current_distance) > patrol_width)
		{
			speed_x = -speed_x;
			current_distance = 0;
		}

		if (collider->y < initial_y)
		{
			speed_y = bounce_strength;
		}

		last_state = FE_PATROL;
	}
		break;
	case FE_STARTING_CHARGE:
	{
		acceleration_y = 0;
		speed_y = 0;

		if (starting.Read() > time_starting)
		{
			state = FE_CHARGE;
		}

		if (last_state == FE_PATROL)
		{
			if (speed_x > 0)
			{
				speed_x = -speed_starting;
			}
			else
			{
				speed_x = speed_starting;
			}
		}
		last_state = FE_STARTING_CHARGE;
	}
		break;
	case FE_CHARGE:
	{
		if (last_state == FE_STARTING_CHARGE)
		{
			charged = false;
			speed_y = charge_speed_y;

			charge_timer.Reset();

			if (speed_x > 0)
				speed_x = -charge_speed_x;
			else
				speed_x = charge_speed_x;
		}
		acceleration_y = charge_accel_y;

		if (speed_y < 0)
		{
			charged = true;
		}

		if (charge_timer.Read() > 5000)
		{
			speed_x = -speed_x;
			charge_timer.Reset();
		}

		if(collider->y<initial_y && charged)
		{
			state = FE_PATROL;
			if(speed_x<0)
				speed_x = patrol_speed;
			else
				speed_x = -patrol_speed;
		}

		last_state = FE_CHARGE;
	}
		break;
	default:
		break;
	}

	return true;
}

bool FlyingElemental::Render()
{
	if (flying_left.amount_of_frames == 0 || flying_right.amount_of_frames == 0)
		SetAnimations(RED_FLYINGELEMENTAL);

	if (state == FE_PATROL || state == FE_CHARGE)
	{
		if (speed_x < 0)
		{
			flying_left.NextFrame();
			App->ren->Blit(App->tex->Get_Texture("flyingelemental"), collider->x, collider->y, flying_left.GetCurrentFrame(), 0);
		}
		else
		{
			flying_right.NextFrame();
			App->ren->Blit(App->tex->Get_Texture("flyingelemental"), collider->x, collider->y, flying_right.GetCurrentFrame(), 0);
		}
	}

	if (state == FE_STARTING_CHARGE)
	{
		if (speed_x > 0)
		{
			flying_left.NextFrame();
			App->ren->Blit(App->tex->Get_Texture("flyingelemental"), collider->x, collider->y, flying_left.GetCurrentFrame(), 0);
		}
		else
		{
			flying_right.NextFrame();
			App->ren->Blit(App->tex->Get_Texture("flyingelemental"), collider->x, collider->y, flying_right.GetCurrentFrame(), 0);
		}
	}

	return true;
}

void FlyingElemental::RecieveDamage(int dmg, int direction)
{
	App->aud->PlaySFX(SFX_ENEMY_HIT);
	health -= dmg;
	if (health <= 0)
	{
		App->phy->DeleteObject(this);
	}

	speed_x = -direction * speed_x;
	speed_y = -10;

}

void FlyingElemental::SetAnimations(FlyingElementalColor _c)
{
	c = _c;

	if (flying_left.amount_of_frames == 0)
	{
		flying_left.AddFrame({ 0  ,c * 56,56,56 });
		flying_left.AddFrame({ 56 ,c * 56,56,56 });
		flying_left.AddFrame({ 112,c * 56,56,56 });
		flying_left.AddFrame({ 168,c * 56,56,56 });
	}
	if (flying_right.amount_of_frames == 0)
	{
		flying_right.AddFrame({ 224,c * 56,56,56 });
		flying_right.AddFrame({ 280,c * 56,56,56 });
		flying_right.AddFrame({ 336,c * 56,56,56 });
		flying_right.AddFrame({ 392,c * 56,56,56 });
	}
}
