#include "HazardLavaWaterfall.h"

#include "../Player.h"
#include "Modules/Render.h"
#include "Application.h"
#include "../../Spells/Fire.h"

HazardLavaWaterfall::HazardLavaWaterfall()
{
	anim.AddFrame({ 0,96,48,48 });
	anim.AddFrame({ 0,108,48,48 });
	anim.AddFrame({ 0,120,48,48 });
	anim.AddFrame({ 0,132,48,48 });
}

void HazardLavaWaterfall::Init()
{
	hazards = Engine->GetModule<Textures>().Load_Texture("Assets/Sprites/hazards.png");
	anim.mTexture = hazards;
}

bool HazardLavaWaterfall::Loop(float dt)
{
	std::vector<collision*> collisions;
	Engine->GetModule<ObjectManager>().GetCollisions(collider, collisions);

	for (std::vector<collision*>::iterator it = collisions.begin(); it != collisions.end(); it++)
	{
		if ((*it)->object != this)
		{
			if ((*it)->object->IsSameTypeAs<Player>() && !((Fire*)((Player*)(*it)->object)->spells[FIRE])->is_fireshield_up)
			{
				if ((*it)->object->collider->x > collider->x)
				{
					((Player*)(*it)->object)->AddHealth(-1, 1);
				}
				else
				{
					((Player*)(*it)->object)->AddHealth(-1, -1);
				}
			}
		}
	}

	if (lava_animation.Read() > iteration_time)
	{
		anim.NextFrame();
		lava_animation.Reset();
	}
	return true;
}

bool HazardLavaWaterfall::Render()
{
	int tilesx = collider->w / 48;
	int tilesy = collider->h / 48;

	for (int i = 0; i < tilesx; ++i)
	{
		for (int j = 0; j < tilesy; ++j)
		{
			Engine->GetModule<::Render>().RenderAnimation(anim, collider->x + 48 * i, collider->y + 48 * j, 50);
		}
	}
	return true;
}