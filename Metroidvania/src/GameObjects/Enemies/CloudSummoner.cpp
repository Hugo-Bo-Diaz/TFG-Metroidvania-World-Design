#include "CloudSummoner.h"
#include "Modules/Particles.h"
#include "Application.h"
#include "Modules/Audio.h"
#include "CloudSummonerProjectile.h"
#include "Utils/Utils.h"
#include "../Player.h"

CloudSummoner::CloudSummoner()
{

}

CloudSummoner::CloudSummoner(float _initial_x, float _initial_y)
{
	initial_y = _initial_y;
	initial_x = _initial_x;

	base_book_offset_x = 64 / 2 - 22 / 2;
	base_book_offset_y = 48 / 2 - 30 / 2;
}

void CloudSummoner::Destroy()
{
	Engine->GetModule<Particles>().AddParticleEmitter(&explosion, collider.x, collider.y, 300);
}

void CloudSummoner::Init()
{
	nextpos = new RXRect();
	nextpos->x = collider.x;
	nextpos->y = collider.y;
	nextpos->w = collider.w;
	nextpos->h = collider.h;

	cloud_summoner = Engine->GetModule<Textures>().Load_Texture("Assets/Sprites/enemies/cloud_summoner.png");
	particles = Engine->GetModule<Textures>().Load_Texture("Assets/Sprites/particles.png");

	mSFXHit = Engine->GetModule<Audio>().LoadSFX("Assets/SFX/enemy_hit.wav");

	//64*48
	book.AddFrame({ 192,0,22,22 });
	//projectile.AddFrame({ 268,0,26,26 });
	facing_right.AddFrame({ 128,0,64,48 });
	facing_left.AddFrame({ 64,0,64,48 });
	facing_front.AddFrame({ 0,0,64,48 });

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

	book.AddFrame({ 192,0,22,22 });

	facing_right.AddFrame({ 128,0,64,48 });
	facing_right.mTexture = cloud_summoner;
	facing_left.AddFrame({ 64,0,64,48 });
	facing_left.mTexture = cloud_summoner;
	facing_front.AddFrame({ 0,0,64,48 });
	facing_front.mTexture = cloud_summoner;

}

bool CloudSummoner::Loop(float dt)
{
	float d_to_origin = std::sqrt(std::pow(collider.x - initial_x, 2) + std::pow(collider.y - initial_y, 2));

	//STEP 1
	collider.x = nextpos->x + oscilated_y;
	collider.y = nextpos->y + oscilated_x;

	nextpos->x += speed_x;
	nextpos->y += speed_y;

	std::vector<RXRect*> colliders;
	Engine->GetModule<ObjectManager>().GetNearbyWalls(collider.x + collider.w / 2, collider.y + collider.h / 2, 100, colliders);

	//IF IT HAS BEEN IN THIS STATE FOR MORE THAN 3 SECS GO BACKWARDS
	for (int i = 0; i < colliders.size(); ++i)
	{
		RXRect result;
		if (RXRectCollision(colliders[i], nextpos, &result) == true)
		{
			if (result.h < result.w)
			{

				if (collider.y < colliders[i]->y)// he goin crash!
				{
					//speed_y = 0;
					nextpos->y -= result.h;
				}

				if (collider.y > colliders[i]->y)// he goin crash!
				{
					//speed_y = 0;
					nextpos->y += result.h;
					speed_y = 0;
				}
			}
			else
			{
				if (collider.x > colliders[i]->x)
				{
					nextpos->x += result.w;
				}

				if (collider.x < colliders[i]->x)
				{
					nextpos->x -= result.w;
				}
			}
		}
	}


	switch (state)
	{
	case CS_PATROL:
	{
		aggro.x = collider.x - aggro.w/2;
		aggro.y = collider.y - aggro.h/2;

		//detect player
		std::vector<collision*> collisions;
		Engine->GetModule<ObjectManager>().GetCollisions(&aggro, collisions);

		for (std::vector<collision*>::iterator it = collisions.begin(); it != collisions.end(); it++)
		{
			if ((*it)->object != this)
			{
				if ((*it)->object->IsSameTypeAs<Player>())
				{
					state = CS_AGGRO;
					following = (*it)->object;
					home = false;
				}
			}
		}

		Engine->GetModule<::Render>().RenderRect(aggro, RXColor{ 0, 255, 0, 50 }, true, RenderQueue::RENDER_DEBUG, 0);

		last_state = CS_PATROL;

		if (d_to_origin > 50 && !home) 
		{
			float deltaX = initial_x - collider.x - 32;
			float deltaY = initial_y - collider.y - 32;

			float angle = atan2(deltaY, -deltaX);

			speed_x = -patrol_speed * cos(angle);
			speed_y = patrol_speed * sin(angle);
		}

		if (d_to_origin < 50)
		{
			home = true;
			speed_x = 0;
			speed_y = 0;
		}

	}
	break;
	case CS_AGGRO:
	{

		float deltaX = following->collider.x + following->collider.w / 2 - collider.x - 32;
		float deltaY = following->collider.y + following->collider.h / 2 - collider.y - 32;

		float angle = atan2(deltaY, -deltaX);

		if (shooting_timer.Read() > shooting_timer_total_cycle)
		{
			shooting_timer.Reset();
			shooting_timer.Start();
			GameObject* obj = Engine->GetModule<ObjectManager>().AddObject(collider.x + collider.w/2 + 11, collider.y+ collider.h / 2 + 11, 22, 22, GetTypeIndex<CloudSummonerProjectile>());

			float proj_speed_x = -cos(angle) * projectile_speed;
			float proj_speed_y = sin(angle) * projectile_speed;

			((CloudSummonerProjectile*)obj)->Fire(proj_speed_x, proj_speed_y);
		}
		else if (shooting_timer.Read() > shooting_timer_total_cycle - shooting_timer_charge)
		{
			speed_x = 0;
			speed_y = 0;
		}
		else
		{
			speed_x = -patrol_speed * cos(angle);
			speed_y = patrol_speed * sin(angle);

			if (d_to_origin > max_distance_from_origin)
			{
				speed_x = 0;
				speed_y = 0;
			}

			float d_to_follow = std::sqrt(std::pow(collider.x - following->collider.x, 2) + std::pow(collider.y - following->collider.y, 2));

			if (d_to_follow > max_distance_to_player)
			{
				state = CS_PATROL;
				shooting_timer.Reset();
				shooting_timer.Start();
				following = nullptr;

			}
			else if (d_to_follow < min_distance_to_player)
			{
				speed_x = 0;
				speed_y = 0;
			}
		}

		last_state = CS_AGGRO;
	}
	break;
	default:
		break;
	}

	if (!(shooting_timer.Read() > shooting_timer_total_cycle - shooting_timer_charge))
	{
		perm+=5;
	}

	oscilated_x = cos(3.14 * perm / 180) * 15;
	oscilated_y = sin(3.14 * perm / 180) * 15;

	book_angle += 4;

	if (book_angle > 360)
		book_angle = 0;

	int mult = 1;
	if (book_angle > 180)
		mult = -1;

	oscilated_book_x = base_book_offset_x + mult * (oscilated_book_strength - cos((3.14 / 180) * book_angle*2) * oscilated_book_strength);
	oscilated_book_y = base_book_offset_y + oscilated_book_strength + sin((3.14 / 180) * book_angle*2) * oscilated_book_strength;

	return true;
}

bool CloudSummoner::Render()
{
	if (speed_x < 0)
	{
		Engine->GetModule<::Render>().RenderAnimation(facing_left, collider.x, collider.y);
	}
	else if (speed_x > 0)
	{
		Engine->GetModule<::Render>().RenderAnimation(facing_right, collider.x, collider.y);
	}
	else if(following != nullptr)
	{
		if (following->collider.x > collider.x)
		{
			Engine->GetModule<::Render>().RenderAnimation(facing_right, collider.x, collider.y);
		}
		else
		{
			Engine->GetModule<::Render>().RenderAnimation(facing_left, collider.x, collider.y);
		}
	}
	else
	{
		Engine->GetModule<::Render>().RenderAnimation(facing_front, collider.x, collider.y);
	}


	Engine->GetModule<::Render>().RenderAnimation(book, collider.x + oscilated_book_x, collider.y+oscilated_book_y);

	return true;
}

void CloudSummoner::RecieveDamage(int dmg, int direction)
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