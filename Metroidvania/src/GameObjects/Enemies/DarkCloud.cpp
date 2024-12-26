#include "DarkCloud.h"
#include "Modules/Render.h"
#include "Application.h"
#include "Modules/Audio.h"
#include "../Player.h"
#include "RXRand.h"

DarkCloud::DarkCloud()
{
}


void DarkCloud::Destroy()
{
	Engine->GetModule<::Render>().AddParticleEmitter(&explosion, collider.x, collider.y, 300);
}



void DarkCloud::Init()
{
	nextpos = new RXRect();
	nextpos->x = collider.x;
	nextpos->y = collider.y;
	nextpos->w = collider.w;
	nextpos->h = collider.h;

	Engine->GetModule<::Render>().LoadTexture("Assets/Sprites/particles.png", particles);
	Engine->GetModule<::Render>().LoadTexture("Assets/Sprites/enemies/dark_cloud.png", flyingelemental);

	flying.AddFrame({ 0,0,48,48 });
	flying.mTexture = flyingelemental;

	mSFXHit = Engine->GetModule<Audio>().LoadSFX("Assets/SFX/enemy_hit.wav");

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

	ResetAngle(RXGetRandom(0,3));
}

bool DarkCloud::Loop(float dt)
{
	//STEP 1
	collider.x = nextpos->x;
	collider.y = nextpos->y;

	//STEP 2
	nextpos->x += speed_x;
	nextpos->y += speed_y;

	std::vector<RXRect*> colliders;
	Engine->GetModule<SceneController>().GetNearbyWalls(collider.x + collider.w / 2, collider.y + collider.h / 2, 100, colliders);

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
					ResetAngle(3);
				}

				if (collider.y > colliders[i]->y)// he goin crash!
				{
					ResetAngle(1);
				}
			}
			else
			{
				if (collider.x > colliders[i]->x)
				{
					ResetAngle(0);
				}

				if (collider.x < colliders[i]->x)
				{
					ResetAngle(2);
				}
			}
		}
	}
	return true;
}

void DarkCloud::ResetAngle(int dir)
{
	float lNewAngleOffset = RXGetRandom(0.0f, 180.0f);

	float lStart = (dir * 90) - 90;

	float lTotalAngle = lStart + lNewAngleOffset;

	lCurrentAngle = lTotalAngle;
	lCurrentAngle = fmod(lCurrentAngle, 360.0);
	if (lCurrentAngle < 0)
		lCurrentAngle += 360.0;

	speed_x = cos(lCurrentAngle * 3.14 / 180)* 3;
	speed_y = sin(lCurrentAngle * 3.14 / 180)* 3;
}

bool DarkCloud::Render()
{
	Engine->GetModule<::Render>().RenderAnimation(flying, collider.x, collider.y, -1);

	return true;
}

bool DarkCloud::RecieveDamage(int dmg, int direction)
{
	Engine->GetModule<Audio>().PlaySFX(mSFXHit);
	health -= dmg;
	if (health <= 0)
	{
		Engine->GetModule<SceneController>().DeleteObject(this);
		return false;
	}
	
	if (direction != 0)
	{
		speed_x = 3 * speed_x;
		speed_y = 3 * speed_y;
	}

	return true;
}