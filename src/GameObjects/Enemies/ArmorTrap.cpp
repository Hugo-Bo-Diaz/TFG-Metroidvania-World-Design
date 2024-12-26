#include "ArmorTrap.h"
#include "Application.h"
#include "Modules/Render.h"
#include "Modules/Textures.h"
#include "Modules/Particles.h"
#include "Modules/Audio.h"
#include "Modules/Debug.h"
#include "../Player.h"

ArmorTrap::ArmorTrap()
{
}

void ArmorTrap::Init()
{
	armortrap = Engine->GetModule<Textures>().Load_Texture("Assets/Sprites/enemies/armortrap.png");
	particles = Engine->GetModule<Textures>().Load_Texture("Assets/Sprites/particles.png");

	mSFXHit = Engine->GetModule<Audio>().LoadSFX("Assets/SFX/enemy_hit.wav");

	r14firegedeath = { 24,36,12,12 };
	r15firegedeath = { 36,36,12,12 };
	r18metalfirst = { 48,24,12,12 };
	r19metalsecond = { 48,36,12,12 };

	fire_ge_death.area_in_texture.push_back(&r14firegedeath);
	fire_ge_death.area_in_texture.push_back(&r15firegedeath);
	fire_ge_death.name = "fire grounded death";
	fire_ge_death.minmax_x_offset = std::make_pair(0, 48);
	fire_ge_death.minmax_y_offset = std::make_pair(0, 48);
	fire_ge_death.minmax_speed_x = std::make_pair(-0.6, 0.6);
	fire_ge_death.minmax_speed_y = std::make_pair(-2, -3);
	fire_ge_death.minmax_scale = std::make_pair(1, 1.5);
	fire_ge_death.minmax_angle = std::make_pair(0, 360);
	fire_ge_death.minmax_lifespan = std::make_pair(200, 500);
	fire_ge_death.minmax_frequency = std::make_pair(10, 20);
	fire_ge_death.minmax_acc_y = std::make_pair(0.1, 0.3);
	fire_ge_death.texture_name = particles;


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

	idle = {0,0,48,72};

	left.AddFrame({ 48,0,48,72 });
	left.AddFrame({ 96,0,48,72 });

	right.AddFrame({ 144,0,48,72 });
	right.AddFrame({ 192,0,48,72 });

	aggro.w = 300;
	aggro.h = 144;
}

bool ArmorTrap::Loop(float dt)
{
	//STEP 2
	speed_y += acceleration_y;

	collider->x += speed_x;
	collider->y += speed_y;

	aggro.x = collider->x + collider->w/2 - aggro.w/2;
	aggro.y = collider->y + collider->h - aggro.h;

	std::vector<collision*> collisions;
	Engine->GetModule<ObjectManager>().GetCollisions(&aggro, collisions);

	isplayernearby = false;
	for (std::vector<collision*>::iterator it = collisions.begin(); it != collisions.end(); it++)
	{
		if ((*it)->object != this)
		{
			if ((*it)->object->IsSameTypeAs<Player>())
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
	Engine->GetModule<ObjectManager>().GetNearbyWalls(collider->x + collider->w / 2, collider->y + collider->h / 2, 100, colliders);

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
		if (SDL_IntersectRect(colliders[i], collider, &result) == SDL_TRUE && collider->y < colliders[i]->y)// he goin crash!
		{
			speed_y = 0;
			collider->y -= result.h;

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
		Engine->GetModule<::Render>().Blit(armortrap, collider->x, collider->y, &idle, 0);
	}
	else
	{
		if (speed_x < 0)
			Engine->GetModule<::Render>().Blit(armortrap, collider->x, collider->y, left.GetCurrentFrame(), 0);
		else
			Engine->GetModule<::Render>().Blit(armortrap, collider->x, collider->y, right.GetCurrentFrame(), 0);
	}


	return true;
}

void ArmorTrap::RenderDebug()
{
	Engine->GetModule<::Render>().DrawRect(aggro, 255, 255, 0, 100, true, RenderQueue::RENDER_DEBUG, 0);
}

void ArmorTrap::Destroy()
{
	Engine->GetModule<Particles>().AddParticleEmitter(&fire_ge_death, collider->x, collider->y, 200);
}


void ArmorTrap::RecieveDamage(int dmg, int direction)
{
	Engine->GetModule<Audio>().PlaySFX(mSFXHit);

	health -= dmg;
	if (health <= 0)
	{
		Engine->GetModule<ObjectManager>().DeleteObject(this);
	}
	else
	{
		Engine->GetModule<Particles>().AddParticleEmitter(&metal, collider->x, collider->y, 300);
	}

	speed_x = direction * 6;
	speed_y = -10;
	knocked_up = true;
}