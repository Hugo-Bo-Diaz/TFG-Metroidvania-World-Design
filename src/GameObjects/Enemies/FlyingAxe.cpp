#include "FlyingAxe.h"
#include "Modules/Particles.h"
#include "Application.h"
#include "Modules/Audio.h"
#include "../Player.h"

FlyingAxe::FlyingAxe()
{

}

FlyingAxe::FlyingAxe(float _initial_y)
{
	initial_y = _initial_y;
}


void FlyingAxe::Destroy()
{
	Engine->GetModule<Particles>().AddParticleEmitter(&metal, collider->x, collider->y, 300);
}


void FlyingAxe::Init()
{
	particles = Engine->GetModule<Textures>().Load_Texture("Assets/Sprites/particles.png");
	floating_axe = Engine->GetModule<Textures>().Load_Texture("Assets/Sprites/enemies/floating_axe.png");

	mSFXHit = Engine->GetModule<Audio>().LoadSFX("Assets/SFX/enemy_hit.wav");

	r18metalfirst = { 48,24,12,12 };
	r19metalsecond = { 48,36,12,12 };

	metal.area_in_texture.push_back(&r18metalfirst);
	metal.area_in_texture.push_back(&r19metalsecond);
	metal.name = "metal";
	metal.minmax_speed_y = std::make_pair(-6, -4);
	metal.minmax_speed_x = std::make_pair(-3, 3);
	metal.minmax_lifespan = std::make_pair(75, 150);
	metal.minmax_angle_speed = std::make_pair(5, 15);
	metal.minmax_angle = std::make_pair(0, 360);
	metal.minmax_x_offset = std::make_pair(-15, 15);
	metal.minmax_y_offset = std::make_pair(-15, 15);
	metal.minmax_acc_y = std::make_pair(1, 2);
	metal.minmax_frequency = std::make_pair(8, 15);
	metal.texture_name = particles;

	facing_right = { 0,0,64,64 };

	rotate_right.AddFrame({ 0,0,64,64 });
	rotate_right.AddFrame({ 64,0,64,64 });
	rotate_right.AddFrame({ 128,0,64,64 });
	rotate_right.AddFrame({ 192,0,64,64 });

	facing_left = { 0,64,64,64 };

	rotate_left.AddFrame({ 0,64,64,64 });
	rotate_left.AddFrame({ 64,64,64,64 });
	rotate_left.AddFrame({ 128,64,64,64 });
	rotate_left.AddFrame({ 192,64,64,64 });

}

bool FlyingAxe::Loop(float dt)
{
	//STEP 1
	collider->x = nextpos->x;
	collider->y = nextpos->y;

	//STEP 2

	nextpos->x += speed_x;
	nextpos->y += speed_y;

	std::vector<SDL_Rect*> colliders;
	Engine->GetModule<ObjectManager>().GetNearbyWalls(collider->x + collider->w / 2, collider->y + collider->h / 2, 100, colliders);

	//IF IT HAS BEEN IN THIS STATE FOR MORE THAN 3 SECS GO BACKWARDS
	for (int i = 0; i < colliders.size(); ++i)
	{
		SDL_Rect result;
		if (SDL_IntersectRect(colliders[i], nextpos, &result) == SDL_TRUE)
		{
			if (state = AXE_CHARGE)
			{
				Engine->GetModule<ObjectManager>().DeleteObject(this);

			}
			else
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

	}


	switch (state)
	{
	case AXE_PATROL:
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
		Engine->GetModule<ObjectManager>().GetCollisions(&aggro, collisions);

		for (std::vector<collision*>::iterator it = collisions.begin(); it != collisions.end(); it++)
		{
			if ((*it)->object != this)
			{
				if ((*it)->object->IsSameTypeAs<Player>())
				{
					state = AXE_STARTING_CHARGE;
					starting.Reset();
					starting.Start();

					float deltaX = (*it)->object->collider->x + (*it)->object->collider->w / 2 - x - 32;
					float deltaY = (*it)->object->collider->y + (*it)->object->collider->h / 2 - y - 32;

					float angle = atan2(deltaY, -deltaX);

					charge_speed_x = cos(angle) * charge_speed;
					charge_speed_y = sin(angle) * charge_speed;
				}
			}
		}

		current_distance += speed_x;

		if (abs(current_distance) > patrol_width)
		{
			speed_x = -speed_x;
			current_distance = 0;
		}

		last_state = AXE_PATROL;
	}
		break;
	case AXE_STARTING_CHARGE:
	{
		speed_x = 0;
		speed_y = 0;

		if (starting.Read() > time_starting)
		{
			state = AXE_CHARGE;
		}
		
		animation_frequency = 100;

		last_state = AXE_STARTING_CHARGE;
	}
		break;
	case AXE_CHARGE:
	{
		if (last_state == AXE_STARTING_CHARGE)
		{
			//calculate angle and speeds

			charged = true;
			speed_y = charge_speed_y;
			speed_x = charge_speed_x;
		}


		if(collider->y<initial_y && charged)
		{
			state = AXE_PATROL;
			if(speed_x<0)
				speed_x = patrol_speed;
			else
				speed_x = -patrol_speed;
		}

		last_state = AXE_CHARGE;
	}
		break;
	default:
		break;
	}

	return true;
}

bool FlyingAxe::Render()
{
	if (state == AXE_STARTING_CHARGE || state == AXE_CHARGE)
	{
		if (speed_x < 0)
		{
			Engine->GetModule<::Render>().Blit(floating_axe, collider->x, collider->y, rotate_left.GetCurrentFrame(), 0);
		}
		else
		{
			Engine->GetModule<::Render>().Blit(floating_axe, collider->x, collider->y, rotate_right.GetCurrentFrame(), 0);
		}
	}

	if (state == AXE_PATROL)
	{
		if (speed_x > 0)
		{
			Engine->GetModule<::Render>().Blit(floating_axe, collider->x, collider->y, rotate_left.GetCurrentFrame(), 0);
		}
		else
		{
			Engine->GetModule<::Render>().Blit(floating_axe, collider->x, collider->y, rotate_right.GetCurrentFrame(), 0);
		}
	}

	if (animation_timer.Read() > animation_frequency)
	{
		animation_timer.Reset();
		rotate_left.NextFrame();
		rotate_right.NextFrame();
	}

	return true;
}

void FlyingAxe::RecieveDamage(int dmg, int direction)
{
	Engine->GetModule<Audio>().PlaySFX(mSFXHit);
	health -= dmg;
	if (health <= 0)
	{
		Engine->GetModule<ObjectManager>().DeleteObject(this);
	}

	speed_x = -direction * speed_x;
	speed_y = -10;

}

