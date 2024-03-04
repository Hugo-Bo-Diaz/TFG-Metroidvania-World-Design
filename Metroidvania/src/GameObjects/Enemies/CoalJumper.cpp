#include "CoalJumper.h"
#include "Application.h"
#include "Modules/Render.h"
#include "Modules/Textures.h"
#include "Modules/Particles.h"
#include "Modules/Camera.h"
#include "Modules/Audio.h"

#include <cmath>
#include <time.h>

CoalJumper::CoalJumper()
{
	//1 {0,0,64,64}
	//2 {64,0,64,64}
	//3 {128,0,64,64}
	//4 {192,0,64,64}
	//5 {256,0,64,64}
	//6 {320,0,64,64}
	//7 {384,0,64,64}


	//srand(time(NULL));
	idle_time += rand() % max_variation + 1;
}

CoalJumper::~CoalJumper()
{
	Engine->GetModule<Particles>().AddParticleEmitter(&fireshield, collider->x, collider->y, 600);
}

void CoalJumper::Init()
{
	nextpos = new SDL_Rect();
	nextpos->x = collider->x;
	nextpos->y = collider->y;
	nextpos->w = collider->w;
	nextpos->h = collider->h;

	particles = Engine->GetModule<Textures>().Load_Texture("Assets/Sprites/particles.png");
	coaljumper = Engine->GetModule<Textures>().Load_Texture("Assets/Sprites/enemies/coaljumper.png");

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

	animations[COALJUMPER_HIT].AddFrame({ 0,0,64,64 });//1
	animations[COALJUMPER_HIT].AddFrame({ 384,0,64,64 });//7


	r2shield = { 12,0,12,12 };
	r12shield = { 24,0,12,12 };
	r7buff = { 24,12,12,12 };

	fireshield.area_in_texture.push_back(&r2shield);
	fireshield.area_in_texture.push_back(&r2shield);
	fireshield.area_in_texture.push_back(&r12shield);
	fireshield.name = "fireshield";
	fireshield.minmax_x_offset = std::make_pair(-5, 69);
	fireshield.minmax_y_offset = std::make_pair(0, 70);
	fireshield.minmax_speed_y = std::make_pair(-1.5, -2.5);
	fireshield.minmax_speed_x = std::make_pair(-0.1, 0.1);
	fireshield.minmax_scale_speed = std::make_pair(-0.03, -0.04);
	fireshield.minmax_scale = std::make_pair(1, 1.5);
	fireshield.minmax_acc_y = std::make_pair(0.04, 0.05);
	fireshield.minmax_lifespan = std::make_pair(300, 400);
	fireshield.minmax_frequency = std::make_pair(5, 20);
	fireshield.texture_name = particles;

	smoke.area_in_texture.push_back(&r7buff);
	smoke.area_in_texture.push_back(&r12shield);
	smoke.area_in_texture.push_back(&r12shield);
	smoke.name = "smoke";
	smoke.minmax_x_offset = std::make_pair(-5, 69);
	smoke.minmax_y_offset = std::make_pair(0, 70);
	smoke.minmax_speed_y = std::make_pair(-1.5, -2.5);
	smoke.minmax_speed_x = std::make_pair(-0.1, 0.1);
	smoke.minmax_scale_speed = std::make_pair(-0.03, -0.04);
	smoke.minmax_scale = std::make_pair(1.2, 1.7);
	smoke.minmax_acc_y = std::make_pair(0.04, 0.05);
	smoke.minmax_lifespan = std::make_pair(300, 400);
	smoke.minmax_frequency = std::make_pair(5, 20);
	smoke.texture_name = particles;
}

bool CoalJumper::Loop(float dt)
{
	if (!Engine->GetModule<Camera>().isOnScreen(*collider) && !idle_timer.paused)
	{
		idle_timer.Pause();
	}
	else if (idle_timer.paused)
	{
		idle_timer.Resume();
	}

	collider->x = nextpos->x;
	collider->y = nextpos->y;

	std::vector<SDL_Rect*> colliders;
	Engine->GetModule<ObjectManager>().GetNearbyWalls(collider->x + collider->w / 2, collider->y + collider->h / 2, 100, colliders);

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
		animations[COALJUMPER_CROUCHING].current_frame = frame;

		if (crouching.Read() >= time_crouching)
		{
			state = COALJUMPER_JUMPING;
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
	case COALJUMPER_HIT:
	{
		if (last_state != COALJUMPER_HIT)
		{
			speed_y = speed_y_knockback;
			speed_x = speed_x_knockback;
		}

		if (animation_timer.Read() > time_between_frames)
		{
			animations[COALJUMPER_JUMPING].NextFrame();
			animation_timer.Reset();
		}

		last_state = COALJUMPER_HIT;
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
		}

		last_state = COALJUMPER_LANDING;
	}
		break;
	default:
		break;
	}

	std::vector<collision*> collisions;
	Engine->GetModule<ObjectManager>().GetCollisions(collider, collisions);

	return true;
}

bool CoalJumper::Render()
{
	Engine->GetModule<::Render>().Blit(coaljumper, collider->x, collider->y, animations[state].GetCurrentFrame(), -1);

	return true;
}

void CoalJumper::RecieveDamage(int dmg, int _direction)
{
	Engine->GetModule<Audio>().PlaySFX(SFX_ENEMY_HIT);
	health -= dmg;
	if (health <= 0)
	{
		Engine->GetModule<ObjectManager>().DeleteObject(this);
	}
	else
	{
		Engine->GetModule<Particles>().AddParticleEmitter(&smoke, collider->x, collider->y, 200);
	}

	//speed_x = direction * 6;
	direction = _direction;
	speed_x_knockback = 6;
	speed_y = -5;
	state = COALJUMPER_HIT;

}