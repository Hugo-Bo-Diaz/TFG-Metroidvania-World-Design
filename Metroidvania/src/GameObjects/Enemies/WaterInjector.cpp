#include "WaterInjector.h"
#include "Application.h"
#include "Modules/Render.h"
#include "Modules/Audio.h"
#include "Modules/Debug.h"
#include "../Player.h"

#include "WaterInjectorProjectile.h"

WaterInjector::WaterInjector()
{
}

void WaterInjector::Init()
{
	Engine->GetModule<::Render>().LoadTexture("Assets/Sprites/enemies/Injector.png", injector);
	Engine->GetModule<::Render>().LoadTexture("Assets/Sprites/particles.png", particles);

	mSFXHit = Engine->GetModule<Audio>().LoadSFX("Assets/SFX/enemy_hit.wav");

	r18waterfirst = { 0,48,12,12 };
	r19watersecond = { 12,48,12,12 };
	r20waterthird = { 24,48,12,12 };
	r21waterdrop = { 36,48,12,12 };

	water_death.area_in_texture.push_back(&r18waterfirst);
	water_death.area_in_texture.push_back(&r19watersecond);
	water_death.area_in_texture.push_back(&r20waterthird);
	water_death.name = "water_death";
	water_death.minmax_x_offset = std::make_pair(0, 48);
	water_death.minmax_y_offset = std::make_pair(0, 48);
	water_death.minmax_speed_x = std::make_pair(-0.6, 0.6);
	water_death.minmax_speed_y = std::make_pair(-2, -3);
	water_death.minmax_scale = std::make_pair(1, 1.5);
	water_death.minmax_angle = std::make_pair(0, 360);
	water_death.minmax_lifespan = std::make_pair(200, 500);
	water_death.minmax_frequency = std::make_pair(7, 15);
	water_death.minmax_acc_y = std::make_pair(0.1, 0.3);
	water_death.texture_name = particles;

	water_drip.area_in_texture.push_back(&r21waterdrop);
	water_drip.area_in_texture.push_back(&r19watersecond);
	water_drip.name = "water_drip";
	water_drip.minmax_x_offset = std::make_pair(0, 48);
	water_drip.minmax_y_offset = std::make_pair(0, 5);
	water_drip.minmax_speed_x = std::make_pair(0.0, 0.0);
	water_drip.minmax_speed_y = std::make_pair(0, 0);
	water_drip.minmax_scale = std::make_pair(1, 1);
	water_drip.minmax_lifespan = std::make_pair(200, 500);
	water_drip.minmax_frequency = std::make_pair(40, 75);
	water_drip.minmax_acc_y = std::make_pair(0.1, 0.2);
	water_drip.texture_name = particles;

	left.AddFrame({ 0,0,48,48 });
	left.AddFrame({ 48,0,48,48 });
	left.AddFrame({ 96,0,48,48 });
	left.AddFrame({ 144,0,48,48 });
	left.mTexture = injector;

	right.AddFrame({ 144,0,48,48 });
	right.AddFrame({ 96,0,48,48 });
	right.AddFrame({ 48,0,48,48 });
	right.AddFrame({ 0,0,48,48 });
	right.mTexture = injector;

	injection.AddFrame({ 0,48,48,48 });
	injection.AddFrame({ 48,48,48,48 });
	injection.AddFrame({ 96,48,48,48 });
	injection.mTexture = injector;

	aggro.w = 48;
	aggro.h = 500;
}

bool WaterInjector::Loop(float dt)
{
	//STEP 2
	speed_y += acceleration_y;

	if(current_state == WaterInjector_PATROL)
		collider.x += speed_x;
	
	collider.y += speed_y;

	aggro.x = collider.x + collider.w/2 - aggro.w/2;
	aggro.y = collider.y;

	std::vector<collision> collisions;
	Engine->GetModule<SceneController>().GetCollisions(&aggro, collisions);

	bool foundplayer = false;
	if (current_state == WaterInjector_PATROL)
	{
		for (std::vector<collision>::iterator it = collisions.begin(); it != collisions.end(); it++)
		{
			if ((*it).object != this)
			{
				if ((*it).object->IsSameTypeAs<Player>())
				{
					foundplayer = true;
					target = (*it).object;
					injection_timer.Reset();
					current_state = WaterInjector_INJECT;
					Engine->GetModule<::Render>().AddParticleEmitter(&water_drip, collider.x, collider.y + 96, 1500);
				}
			}
		}
	}

	//switch (current_state)
	//{
	//case WaterInjector_PATROL:
	//	int direction = 0;
	//	int distance = target->collider.x - collider.x;
	//	if (distance > 0)
	//		direction = 1;
	//	else
	//		direction = -1;

	//	speed_x = direction * patrol_speed;

	//	if (!foundplayer)
	//	{
	//		current_state = WaterInjector_INJECT;
	//		speed_x = direction * patrol_speed;
	//		target = nullptr;
	//	}
	//
	//	break;
	//	case WaterInjector_INJECT:
	//		speed_x = 0;
	//		break;
	//default:
	//	break;
	//}

	if (current_state == WaterInjector_INJECT)
	{
		if (injection_timer.Read() > injection_Interval)
		{
			injection_timer.Reset();
			injection_phase++;
			injection.NextFrame();
			if (injection_phase >= 3)
			{
				WaterInjectorProjectile* lProjectile = new WaterInjectorProjectile();
				lProjectile->collider.x = collider.x;
				lProjectile->collider.y = collider.y + 96;
				lProjectile->collider.w = 48;
				lProjectile->collider.h = 48;
				Engine->GetModule<SceneController>().AddObject(lProjectile);
				lProjectile->Fire(0,0);
				injection_phase = 0;
				current_state = WaterInjector_PATROL;
			}
		}
	}



	std::vector<RXRect*> colliders;
	Engine->GetModule<SceneController>().GetNearbyWalls(collider.x + collider.w / 2, collider.y + collider.h / 2, 100, colliders);

	bool change_direction = false;
	bool floor_below = false;
	if (speed_y >acceleration_y)
	{
		floor_below = true;
	}

	for (int i = 0; i < colliders.size(); ++i)
	{
		RXPoint p1;//left(should not collide)
		RXPoint p2;//down(should collide)
		if (speed_x < 0)//left
		{
			p1.x = collider.x - abs(speed_x);
			p1.y = collider.y + collider.h / 2;

			p2.x = collider.x - abs(speed_x);
			p2.y = collider.y + collider.h + 10;

		}
		else if (speed_x > 0)
		{
			p1.x = collider.x + collider.w + abs(speed_x);
			p1.y = collider.y + collider.h / 2;

			p2.x = collider.x + collider.w + abs(speed_x);
			p2.y = collider.y + collider.h + 10;
		}

		RXRect result;
		if (RXRectCollision(colliders[i], &collider, &result) == true && collider.y < colliders[i]->y)// he goin crash!
		{
			speed_y = 0;
			collider.y -= result.h;

			if (knocked_up)
			{
				speed_x = patrol_speed;
				knocked_up = false;
			}
		}

		if (RXPointInRect(&p2, colliders[i]) == true)
		{
			floor_below = true;
		}

		if (RXPointInRect(&p1, colliders[i]) == true)
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

bool WaterInjector::Render()
{
	if (animation_timer.Read() > animation_interval_patrol )
	{
		if (speed_x < 0)
			left.NextFrame();
		else
			right.NextFrame();

		animation_timer.Reset();
	}

	if (current_state == WaterInjector_INJECT)
	{
		Engine->GetModule<::Render>().RenderAnimation(injection, collider.x, collider.y);
	}
	else
	{
		if (speed_x < 0)
			Engine->GetModule<::Render>().RenderAnimation(left, collider.x, collider.y);
		else
			Engine->GetModule<::Render>().RenderAnimation(right, collider.x, collider.y);
	}


	return true;
}

void WaterInjector::RenderDebug()
{
	Engine->GetModule<::Render>().RenderRect(aggro, RXColor{ 255, 255, 0, 100 }, true, RenderQueue::RENDER_DEBUG, 0);
}

void WaterInjector::Destroy()
{
	Engine->GetModule<::Render>().AddParticleEmitter(&water_death, collider.x, collider.y, 200);
}


bool WaterInjector::RecieveDamage(int dmg, int direction)
{
	Engine->GetModule<Audio>().PlaySFX(mSFXHit);

	health -= dmg;
	if (health <= 0)
	{
		Engine->GetModule<SceneController>().DeleteObject(this);
		return false;
	}
	else
	{
		Engine->GetModule<::Render>().AddParticleEmitter(&water_death, collider.x, collider.y, 300);
	}
	
	if (direction != 0)
	{
		speed_x = direction * 6;
		speed_y = -10;
		knocked_up = true;
	}

	return true;
}