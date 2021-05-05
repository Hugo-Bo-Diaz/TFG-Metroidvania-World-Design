#include "CoalJumper.h"
#include "Application.h"
#include "Render.h"
#include "Textures.h"
#include "Particles.h"

#include <cmath>

CoalJumper::CoalJumper()
{
	//1 {0,0,64,64}
	//2 {64,0,64,64}
	//3 {128,0,64,64}
	//4 {192,0,64,64}
	//5 {256,0,64,64}
	//6 {320,0,64,64}
	//7 {384,0,64,64}

	animations[COALJUMPER_IDLE].AddFrame({ 0,0,64,64 });//1
	animations[COALJUMPER_IDLE].AddFrame({ 64,0,64,64 });//2
	animations[COALJUMPER_IDLE].AddFrame({ 128,0,64,64 });//3
	animations[COALJUMPER_IDLE].AddFrame({ 64,0,64,64 });//2

	animations[COALJUMPER_CROUCHING].AddFrame({ 64,0,64,64 });//2
	animations[COALJUMPER_CROUCHING].AddFrame({ 192,0,64,64 });//4
	animations[COALJUMPER_CROUCHING].AddFrame({ 256,0,64,64 });//5
	animations[COALJUMPER_CROUCHING].AddFrame({ 320,0,64,64 });//6

	animations[COALJUMPER_JUMPING].AddFrame({ 0,0,64,64 });//1
	animations[COALJUMPER_JUMPING].AddFrame({ 384,0,64,64 });//7

	animations[COALJUMPER_LANDING].AddFrame({ 320,0,64,64 });//6
	animations[COALJUMPER_LANDING].AddFrame({ 256,0,64,64 });//5
	animations[COALJUMPER_LANDING].AddFrame({ 192,0,64,64 });//4
	animations[COALJUMPER_LANDING].AddFrame({ 64,0,64,64 });//2

}

CoalJumper::~CoalJumper()
{
	App->par->AddParticleEmitter(&App->par->fireshield, collider->x, collider->y, 600);
}

bool CoalJumper::Loop(float dt)
{

	collider->x = nextpos->x;
	collider->y = nextpos->y;

	std::vector<SDL_Rect*> colliders;
	App->phy->GetNearbyWalls(collider->x + collider->w / 2, collider->y + collider->h / 2, 100, colliders);

	speed_y += acceleration_y;

	nextpos->x += speed_x * direction;
	nextpos->y += speed_y;

	for (int i = 0; i < colliders.size(); ++i)
	{
		SDL_Rect result;
		if (SDL_IntersectRect(colliders[i], nextpos, &result) == SDL_TRUE && collider->y < colliders[i]->y)// he goin crash!
		{
			if (result.h > result.w)
			{
				direction *= -1;
				jumps_to_side = 0;
				if (colliders[i]->x > nextpos->x)//right collision
				{
					nextpos->x -= result.w;
				}
				else//left collision
				{
					nextpos->x += result.w;
				}
			}
			if (result.h < result.w)
			{
				if (colliders[i]->y > nextpos->y)//down collision
				{
					if (speed_y > 0)
					{
						if (speed_y > acceleration_y)
						{
							state = COALJUMPER_LANDING;
							printf("landing\n");
						}
						nextpos->y -= result.h;
						speed_y = 0;
					}
				}
				else
				{
					speed_y = 0;
					nextpos->y += result.h;
				}
			}
		}
	}

	switch (state)
	{
	case COALJUMPER_IDLE:
	{
		if(last_state != COALJUMPER_IDLE)
		{
			idle_timer.Reset();
		}

		if (animation_timer.Read() > time_between_frames)
		{
			animations[COALJUMPER_IDLE].NextFrame();
			animation_timer.Reset();
		}
		
		if (idle_timer.Read() > idle_time)
		{
			state = COALJUMPER_CROUCHING;
			printf("crouching\n");
		}

		last_state = COALJUMPER_IDLE;
	}
		break;
	case COALJUMPER_CROUCHING:
	{
		if (last_state != COALJUMPER_CROUCHING)
		{
			crouching.Reset();
		}

		float frame = (crouching.Read()/time_crouching)*3;
		printf("frame: %f\n", frame);
		animations[COALJUMPER_CROUCHING].current_frame = frame;

		if (crouching.Read() >= time_crouching)
		{
			state = COALJUMPER_JUMPING;
			printf("jumping\n");
		}

		last_state = COALJUMPER_CROUCHING;
	}
		break;
	case COALJUMPER_JUMPING:
	{
		if (last_state != COALJUMPER_JUMPING)
		{
			speed_y = jump_strength;
			speed_x = speed_while_jumping;
		}

		if (animation_timer.Read() > time_between_frames)
		{
			animations[COALJUMPER_JUMPING].NextFrame();
			animation_timer.Reset();
		}

		last_state = COALJUMPER_JUMPING;
	}
		break;
	case COALJUMPER_LANDING:
	{
		if (last_state != COALJUMPER_LANDING)
		{
			speed_x = 0;
			cooldown_timer.Reset();
			++jumps_to_side;
			if (jumps_to_side >= max_jumps_to_side)
			{
				jumps_to_side = 0;
				direction *= -1;
			}
		}

		float frame = (cooldown_timer.Read() / cooldown) * 3;

		animations[COALJUMPER_LANDING].current_frame = frame;

		if (cooldown_timer.Read() > cooldown)
		{
			state = COALJUMPER_IDLE;
			printf("idling\n");
		}

		last_state = COALJUMPER_LANDING;
	}
		break;
	default:
		break;
	}

	std::vector<collision*> collisions;
	App->phy->GetCollisions(collider, collisions);




	return true;
}

bool CoalJumper::Render()
{
	App->ren->Blit(App->tex->Get_Texture("coaljumper"), collider->x, collider->y, animations[state].GetCurrentFrame(), -1);

	return true;
}

void CoalJumper::RecieveDamage(int dmg, int direction)
{
	health -= dmg;
	if (health <= 0)
	{
		App->phy->DeleteObject(this);
	}

	speed_x = direction * 6;
	speed_y = -10;
	state = COALJUMPER_JUMPING;

}