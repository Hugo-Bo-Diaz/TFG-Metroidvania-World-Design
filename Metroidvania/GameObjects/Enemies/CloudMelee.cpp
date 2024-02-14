#include "CloudMelee.h"
#include "Particles.h"
#include "Application.h"
#include "Audio.h"
#include "../EntityIDs.h"

CloudMelee::CloudMelee()
{
	//64*48

	facing_right.AddFrame({ 64,0,64,48 });
	facing_left.AddFrame({ 128,0,64,48 });
	facing_front.AddFrame({ 0,0,64,48 });
}

CloudMelee::CloudMelee(float _initial_x, float _initial_y)
{
	initial_y = _initial_y;
	initial_x = _initial_x;

	facing_right.AddFrame({ 64,0,64,48 });
	facing_left.AddFrame({ 128,0,64,48 });
	facing_front.AddFrame({ 0,0,64,48 });
}


CloudMelee::~CloudMelee()
{
	App->par->AddParticleEmitter(&App->par->explosion, collider->x, collider->y, 300);
}



bool CloudMelee::Loop(float dt)
{
	//STEP 1
	collider->x = nextpos->x;
	collider->y = nextpos->y;

	nextpos->x += speed_x;
	nextpos->y += speed_y;

	std::vector<SDL_Rect*> colliders;
	App->phy->GetNearbyWalls(collider->x + collider->w / 2, collider->y + collider->h / 2, 100, colliders);


	bool has_crashed = false;

	//IF IT HAS BEEN IN THIS STATE FOR MORE THAN 3 SECS GO BACKWARDS
	for (int i = 0; i < colliders.size(); ++i)
	{
		SDL_Rect result;
		if (SDL_IntersectRect(colliders[i], nextpos, &result) == SDL_TRUE)
		{
			has_crashed = true;
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
	case CM_PATROL:
	{
		if (speed_x < 0)
		{
			aggro.x = collider->x - aggro.w;
			aggro.y = collider->y - 200;
		}
		else
		{
			aggro.x = collider->x + collider->w;
			aggro.y = collider->y - 200;
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
					state = CM_STARTING_CHARGE;
					starting.Reset();
					starting.Start();

					float deltaX = (*it)->object->collider->x + (*it)->object->collider->w / 2 - x - 32;
					float deltaY = (*it)->object->collider->y + (*it)->object->collider->h / 2 - y - 32;

					float angle = atan2(deltaY, -deltaX);


					if((*it)->object->collider->x > collider->x)
					{
						charge_speed_x = -charge_speed_base * cos(angle);
					}
					else
					{
						charge_speed_x = charge_speed_base * cos(angle);
					}
					charge_speed_y = charge_speed_base * sin(angle);
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

		speed_y = 0;


		last_state = CM_PATROL;
	}
		break;
	case CM_STARTING_CHARGE:
	{
		acceleration_y = 0;
		speed_y = 0;

		if (starting.Read() > time_starting)
		{
			state = CM_CHARGE;
			charge_timer.Reset();
		}

		if (last_state == CM_PATROL)
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
		last_state = CM_STARTING_CHARGE;
	}
		break;
	case CM_CHARGE:
	{
		if (last_state == CM_STARTING_CHARGE)
		{
			speed_y = charge_speed_y;
			speed_x = charge_speed_x;
		}

		if (charge_timer.Read() > 5000 || has_crashed)
		{
			state = CM_RECOVER;
			charge_timer.Reset();
		}

		last_state = CM_CHARGE;
	}
		break;
	case CM_RECOVER:
	{
		bool is_in_position = true;
		if (collider->y > initial_y)
		{
			is_in_position = false;
			speed_y = -2;
		}
		else
		{
			speed_y = 0;
		}

		if (std::abs(collider->x - initial_x) > 50)
		{
			is_in_position = false;
			if (collider->x > initial_x)
				speed_x = -patrol_speed;
			else
				speed_x = patrol_speed;
		}
		else
		{
			speed_x = 0;
		}

		if(is_in_position)
		{
			state = CM_PATROL;
			speed_x = patrol_speed;
		}

	}
	default:
		break;
	}

	return true;
}

bool CloudMelee::Render()
{
	if (state == CM_PATROL || state == CM_CHARGE || state == CM_RECOVER)
	{
		if (speed_x < 0)
		{
			App->ren->Blit(App->tex->Get_Texture("cloud_melee"), collider->x, collider->y, facing_left.GetCurrentFrame(), 0);
		}
		else
		{
			App->ren->Blit(App->tex->Get_Texture("cloud_melee"), collider->x, collider->y, facing_right.GetCurrentFrame(), 0);
		}
	}

	if (state == CM_STARTING_CHARGE)
	{
		if (speed_x > 0)
		{
			App->ren->Blit(App->tex->Get_Texture("cloud_melee"), collider->x, collider->y, facing_left.GetCurrentFrame(), 0);
		}
		else
		{
			App->ren->Blit(App->tex->Get_Texture("cloud_melee"), collider->x, collider->y, facing_right.GetCurrentFrame(), 0);
		}
	}

	return true;
}

void CloudMelee::RecieveDamage(int dmg, int direction)
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