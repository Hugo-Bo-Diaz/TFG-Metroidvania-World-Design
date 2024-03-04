#include "FlyingShield.h"
#include "Modules/Particles.h"
#include "Application.h"
#include "Modules/Audio.h"

FlyingShield::FlyingShield()
{
}

FlyingShield::FlyingShield(float _initial_y)
{
	initial_y = _initial_y;
}


FlyingShield::~FlyingShield()
{
	Engine->GetModule<Particles>().AddParticleEmitter(&explosion, collider->x, collider->y, 300);
}

void FlyingShield::Init()
{
	initial_y = collider->x;
	nextpos = new SDL_Rect();
	nextpos->x = collider->x;
	nextpos->y = collider->y;
	nextpos->w = collider->w;
	nextpos->h = collider->h;

	floating_shield = Engine->GetModule<Textures>().Load_Texture("Assets/Sprites/enemies/floating_shield.png");
	particles = Engine->GetModule<Textures>().Load_Texture("Assets/Sprites/particles.png");

	flying_left.AddFrame({ 0,0,48,48 });

	flying_right.AddFrame({ 48,0,48,48 });

	r1exp = { 0,0,12,12 };
	explosion.area_in_texture.push_back(&r1exp);
	explosion.name = "explosion";
	explosion.minmax_x_offset = std::make_pair(-20, 20);
	explosion.minmax_y_offset = std::make_pair(-20, 20);
	explosion.minmax_speed_y = std::make_pair(-2, -3);
	explosion.minmax_speed_x = std::make_pair(-0.6, 0.6);
	explosion.minmax_scale_speed = std::make_pair(0.01, 0.02);
	explosion.minmax_lifespan = std::make_pair(200, 500);
	explosion.minmax_frequency = std::make_pair(10, 50);
	explosion.minmax_acc_y = std::make_pair(0.05, 0.2);
	explosion.texture_name = particles;
}



bool FlyingShield::Loop(float dt)
{

	//STEP 1
	collider->x = nextpos->x;
	collider->y = nextpos->y;

	//STEP 2

	nextpos->x += speed_x;

	std::vector<SDL_Rect*> colliders;
	Engine->GetModule<ObjectManager>().GetNearbyWalls(collider->x + collider->w / 2, collider->y + collider->h / 2, 100, colliders);

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

	//Engine->GetModule<::Render>().DrawRect(&aggro, 0, 255, 0, 50,true);

	current_distance += speed_x;
	if (abs(current_distance) > patrol_width)
	{
		speed_x = -speed_x;
		current_distance = 0;
	}


	return true;
}

bool FlyingShield::Render()
{

	if (speed_x < 0)
	{
		Engine->GetModule<::Render>().Blit(floating_shield, collider->x, collider->y, flying_left.GetCurrentFrame(), 0);
	}
	else
	{
		Engine->GetModule<::Render>().Blit(floating_shield, collider->x, collider->y, flying_right.GetCurrentFrame(), 0);
	}
	return true;
}

void FlyingShield::RecieveDamage(int dmg, int direction)
{
	if (direction == speed_x / abs(speed_x))
	{
		Engine->GetModule<Audio>().PlaySFX(SFX_ENEMY_HIT);
		health -= dmg;
		if (health <= 0)
		{
			Engine->GetModule<ObjectManager>().DeleteObject(this);
		}
	}
	else
	{
		Engine->GetModule<Audio>().PlaySFX(SFX_ENEMY_PING);
	}
}
