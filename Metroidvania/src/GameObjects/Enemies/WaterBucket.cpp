#include "WaterBucket.h"
#include "Application.h"
#include "Modules/Render.h"
#include "Modules/Audio.h"
#include "Modules/Debug.h"
#include "../Player.h"
#include "WaterBucketDrop.h"

WaterBucket::WaterBucket()
{
}

void WaterBucket::Init()
{
	Engine->GetModule<::Render>().LoadTexture("Assets/Sprites/enemies/drop_water.png", waterbucket);
	Engine->GetModule<::Render>().LoadTexture("Assets/Sprites/particles.png", particles);

	r18metalfirst = { 48,24,12,12 };
	r19metalsecond = { 48,36,12,12 };

	r18waterfirst = { 0,48,12,12 };
	r19watersecond = { 12,48,12,12 };
	r20waterthird = { 24,48,12,12 };

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

	left.AddFrame({ 144,0,48,48 });
	left.AddFrame({ 192,0,48,48 });
	left.AddFrame({ 144,0,48,48 });
	left.AddFrame({ 236,0,48,48 });
	left.mTexture = waterbucket;

	right.AddFrame({ 0,0,48,48 });
	right.AddFrame({ 48,0,48,48 });
	right.AddFrame({ 0,0,48,48 });
	right.AddFrame({ 96,0,48,48 });
	right.mTexture = waterbucket;

	dump_left.AddFrame({ 96,48,48,48 });
	dump_left.AddFrame({ 144,48,48,48 });
	dump_left.mTexture = waterbucket;

	dump_right.AddFrame({ 0,48,48,48 });
	dump_right.AddFrame({ 48,48,48,48 });
	dump_right.mTexture = waterbucket;

	aggro.w = 400;
	aggro.h = 400;
}

bool WaterBucket::Loop(float dt)
{
	//STEP 2
	speed_y += acceleration_y;

	if (current_state == WaterBucket_PATROL)
		collider.x += speed_x;

	collider.y += speed_y;

	if(speed_x > 0)
		aggro.x = collider.x + collider.w / 2;
	else
		aggro.x = collider.x + collider.w / 2 - aggro.w;

	aggro.y = collider.y - aggro.h/2;

	std::vector<collision> collisions;
	Engine->GetModule<SceneController>().GetCollisions(&aggro, collisions);

	bool foundplayer = false;
	if (current_state == WaterBucket_PATROL)
	{
		for (std::vector<collision>::iterator it = collisions.begin(); it != collisions.end(); it++)
		{
			if ((*it).object != this)
			{
				if ((*it).object->IsSameTypeAs<Player>())
				{
					foundplayer = true;
					water_drop_timer.Reset();
					current_state = WaterBucket_DROP;
					//drop water
					dump_left.SetCurrentFrame(0);
					dump_right.SetCurrentFrame(0);
				}
			}
		}
	}

	if (current_state == WaterBucket_DROP)
	{
		if (!mHasDroppedWater && water_drop_timer.Read() > drop_time)
		{
			mHasDroppedWater = true;
			WaterBucketDrop* lProjectile = new WaterBucketDrop();
			lProjectile->collider.x = collider.x;
			lProjectile->collider.y = collider.y+12;
			lProjectile->collider.w = 48;
			lProjectile->collider.h = 48;
			if(speed_x > 0)
				lProjectile->mIsRight = true;
			else
				lProjectile->mIsRight = false;

			Engine->GetModule<SceneController>().AddObject(lProjectile);
			dump_left.NextFrame();
			dump_right.NextFrame();
			
		}
		if (water_drop_timer.Read() > total_time)
		{
			current_state = WaterBucket_PATROL;
			mHasDroppedWater = false;
		}
	}



	std::vector<RXRect*> colliders;
	Engine->GetModule<SceneController>().GetNearbyWalls(collider.x + collider.w / 2, collider.y + collider.h / 2, 100, colliders);

	bool change_direction = false;
	bool floor_below = false;
	if (speed_y > acceleration_y)
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

bool WaterBucket::Render()
{
	if (animation_timer.Read() > animation_interval_patrol)
	{
		if (speed_x < 0)
			left.NextFrame();
		else
			right.NextFrame();

		animation_timer.Reset();
	}


	if (current_state == WaterBucket_PATROL)
	{
		if (speed_x < 0)
			Engine->GetModule<::Render>().RenderAnimation(left, collider.x, collider.y);
		else
			Engine->GetModule<::Render>().RenderAnimation(right, collider.x, collider.y);
	}
	else
	{
		if (speed_x < 0)
			Engine->GetModule<::Render>().RenderAnimation(dump_left, collider.x, collider.y);
		else
			Engine->GetModule<::Render>().RenderAnimation(dump_right, collider.x, collider.y);
	}

	return true;
}

void WaterBucket::RenderDebug()
{
	Engine->GetModule<::Render>().RenderRect(aggro, RXColor{ 255, 255, 0, 100 }, true, RenderQueue::RENDER_DEBUG, 0);
}

void WaterBucket::Destroy()
{
	Engine->GetModule<::Render>().AddParticleEmitter(&metal, collider.x, collider.y, 200);
}


bool WaterBucket::RecieveDamage(int dmg, int direction)
{
	health -= dmg;
	if (health <= 0)
	{
		Engine->GetModule<SceneController>().DeleteObject(this);
		return false;
	}
	else
	{
		Engine->GetModule<::Render>().AddParticleEmitter(&metal, collider.x, collider.y, 300);
	}
	
	if (direction != 0)
	{
		speed_x = direction * 6;
		speed_y = -10;
		knocked_up = true;
	}

	return true;
}