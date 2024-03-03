#include "HazardLava.h"
#include "Application.h"
#include "Modules/Render.h"
#include "../Player.h"
#include "Modules/ProgressTracker.h"
#include "Modules/Textures.h"
#include "../../SceneProcessing.h"

HazardLava::HazardLava()
{
	hazards = Engine->GetModule<Textures>().Load_Texture("Assets/Sprites/hazards.png");
	lava = {0,48,48,48};
	lava_top1 = {0,0,48,48};
	lava_top2 = {48,0,48,48};
}

bool HazardLava::Loop(float dt)
{
	std::vector<collision*> collisions;
	Engine->GetModule<ObjectManager>().GetCollisions(collider, collisions);

	for (std::vector<collision*>::iterator it = collisions.begin(); it != collisions.end(); it++)
	{
		if ((*it)->object != this)
		{
			if ((*it)->object->IsSameTypeAs<Player>())
			{
				((Player*)(*it))->Respawn();
			}
		}
	}

	if (lava_animation.Read() > iteration_time)
	{
		offset_cycle = !offset_cycle;
		lava_animation.Reset();
	}

	return true;
}

bool HazardLava::Render()
{
	int tilesx = collider->w/48;
	int tilesy = collider->h/48;

	//top
	for (int i = 0; i < tilesx; ++i)
	{
		int cycle =std::floor(i / 2);

		cycle += offset_cycle;

		if (cycle % 2 == 0)
		{
			Engine->GetModule<::Render>().Blit(hazards, collider->x + 48 * i, collider->y - 48, &lava_top1, -20);
		}
		else
		{
			Engine->GetModule<::Render>().Blit(hazards, collider->x + 48 * i, collider->y - 48, &lava_top2, -20);
		}
	}

	//body
	for (int i = 0; i < tilesx; ++i)
	{
		for (int j = 0; j < tilesy; ++j)
		{
			Engine->GetModule<::Render>().Blit(hazards, collider->x + 48 * i, collider->y + 48 * j, &lava, -20);
		}
	}

	return true;
}
