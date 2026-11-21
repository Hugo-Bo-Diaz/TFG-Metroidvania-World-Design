#include "WaterBucketDrop.h"
#include "Application.h"
#include "Modules/Input.h"
#include "Modules/Render.h"
#include "Modules/Audio.h"

#include "../Player.h"
#include "Modules/Camera.h"
#include "RXRand.h"

WaterBucketDrop::WaterBucketDrop(float aAmountOfParticles)
{
}

WaterBucketDrop::WaterBucketDrop()
{
}

WaterBucketDrop::WaterBucketDrop(std::list<ObjectProperty*>& aProperties)
{
	new (this) WaterBucketDrop;

	for (std::list<ObjectProperty*>::iterator it = aProperties.begin(); it != aProperties.end(); ++it)
	{
		if ((*it)->name.compare("amount") == 0)
		{
			mAmountOfParticles = (*it)->num_value;
		}
		else if ((*it)->name.compare("direction") == 0)
		{
			mIsRight = (*it)->bool_value;
		}
	}
}

void WaterBucketDrop::Destroy()
{
	for (std::vector<WaterParticle*>::iterator it = mParticles.begin(); it != mParticles.end(); it++)
	{
		delete (*it);
	}
	mParticles.clear();
}

void WaterBucketDrop::Init()
{
	Engine->GetModule<::Render>().LoadTexture("Assets/Sprites/particles.png", particles);

	mRectangles.push_back({ 0,48,12,12 });
	mRectangles.push_back({ 12,48,12,12 });
	mRectangles.push_back({ 24,48,12,12 });

	float part_size_x = sqrt(mAmountOfParticles);
	int particles_per_row = ceil(part_size_x);
	float particle_size = (float)collider.w / (float)particles_per_row;
	for (int i = 0; i < mAmountOfParticles; i++)
	{
		float pos_x = i % particles_per_row;
		int pos_y = i / particles_per_row;
		float coord_x = pos_x * particle_size;
		WaterParticle* lWater = new WaterParticle(collider.x + pos_x * particle_size, collider.y + pos_y * particle_size);
		lWater->lRect = RXGetRandom(0, mRectangles.size() - 1);
		if (mIsRight)
		{
			lWater->v_x = RXGetRandom(3.0f, 5.0f);
		}
		else
		{
			lWater->v_x = RXGetRandom(-1.5f, -2.0f);
		}
		lWater->v_y = RXGetRandom(-1.0f, 0.0f);
		lWater->mLife = RXGetRandom(mMinLife,mMaxLife);

		mParticles.push_back(lWater);
	}
	mLifeTime.Start();
}

bool WaterBucketDrop::Loop(float dt)
{
	bool ret = true;

	for (std::vector<WaterParticle*>::iterator it = mParticles.begin(); it != mParticles.end(); it++)
	{
		if ((*it)->mLife < mLifeTime.Read())
		{
			continue;
		}

		//apply gravity
		(*it)->v_y += 0.08;

		(*it)->x += (*it)->v_x;
		(*it)->y += (*it)->v_y;

		RXRect nextpos = { (*it)->x, (*it)->y, msizeOfParticles, msizeOfParticles};
		//STEP 3 oh boi
		std::vector<RXRect*> colliders;
		Engine->GetModule<SceneController>().GetNearbyWalls(nextpos.x + nextpos.w / 2, nextpos.y + nextpos.h / 2, 100, colliders);

		for (int i = 0; i < colliders.size(); ++i)
		{
			RXRect result;
			if (RXRectCollision(colliders[i], &nextpos, &result))// he goin crash!
			{
				if (result.h < result.w)
				{
					if ((*it)->v_y > 0)//down collision
					{
						nextpos.y -= result.h;
						(*it)->v_y = -(*it)->v_y * 0.2;
					}
				}
				if (result.h > result.w)
				{
					if ((*it)->v_x != 0)
					{
						if ((*it)->v_x > 0)//right collision
						{
							nextpos.x -= result.w;
						}
						else if ((*it)->v_x < 0)//left collision
						{
							nextpos.x += result.w;
						}
						(*it)->v_x = -(*it)->v_x * 0.3;
					}
				}
			}
		}
		(*it)->x = nextpos.x;
		(*it)->y = nextpos.y;

		if (!mHasCollided)
		{
			std::vector<collision> lCollisions;
			RXRect lRect = { (*it)->x,(*it)->y,16, 16 };
			Engine->GetModule<SceneController>().GetCollisions(&lRect, lCollisions);
			for (std::vector<collision>::iterator it = lCollisions.begin(); it != lCollisions.end(); it++)
			{
				if ((*it).object != this)
				{
					if ((*it).object->IsSameTypeAs<Player>())
					{
						Player* lPlayer = (Player*)(*it).object;
						if ((*it).object->collider.x < collider.x)
						{
							lPlayer->AddHealth(-1, -1);
						}
						else
						{
							lPlayer->AddHealth(-1, 1);
						}

						mHasCollided = true;
						Engine->GetModule<Camera>().CameraShake(7, 40);
					}
				}
			}
		}

	}

	if (mLifeTime.Read() > mMaxLife)
	{
		Engine->GetModule<SceneController>().DeleteObject(this);
	}

	return ret;
}

bool WaterBucketDrop::Render()
{
	for (std::vector<WaterParticle*>::iterator it = mParticles.begin(); it != mParticles.end(); it++)
	{
		if ((*it)->mLife < mLifeTime.Read())
		{
			continue;
		}

		Engine->GetModule<::Render>().RenderTexture(particles, (*it)->x, (*it)->y,mRectangles[(*it)->lRect], -2);
	}

	return true;
}
