#include "FlyingElemental.h"



FlyingElemental::FlyingElemental()
{
}

FlyingElemental::FlyingElemental(float _initial_y)
{
	initial_y = _initial_y;
}


FlyingElemental::~FlyingElemental()
{
}
#include "Application.h"
#include "Render.h"
#include "Textures.h"


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

	bool change_direction = false;
	/*
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
		}

		if (SDL_PointInRect(&p1, colliders[i]) == SDL_TRUE)
		{
			change_direction = true;
		}

	}
	if (change_direction)
	{
		speed_x = -speed_x;
	}
	*/
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
				if ((*it)->type == PLAYER)
				{
					state = FE_STARTING_CHARGE;
					starting.Reset();
					starting.Start();

				}
			}
		}

		App->ren->DrawRect(&aggro, 0, 255, 0, 50,true);

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

			if (speed_x > 0)
				speed_x = -charge_speed_x;
			else
				speed_x = charge_speed_x;
		}
		acceleration_y = charge_accel_y;

		for (int i = 0; i < colliders.size(); ++i)
		{
			SDL_Rect result;
			if (SDL_IntersectRect(colliders[i], nextpos, &result) == SDL_TRUE && collider->y < colliders[i]->y)// he goin crash!
			{
				speed_y = 0;
				nextpos->y -= result.h;
			}
		}
		if (speed_y < 0)
		{
			charged = true;
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
