#include "HazardLavaWaterfall.h"

#include "../Player.h"
#include "Render.h"
#include "Application.h"
#include "Textures.h"
#include "../../Spells/Fire.h"
#include "../EntityIDs.h"

HazardLavaWaterfall::HazardLavaWaterfall()
{
	anim.AddFrame({ 0,96,48,48 });
	anim.AddFrame({ 0,108,48,48 });
	anim.AddFrame({ 0,120,48,48 });
	anim.AddFrame({ 0,132,48,48 });

}

bool HazardLavaWaterfall::Loop(float dt)
{
	std::vector<collision*> collisions;
	App->phy->GetCollisions(collider, collisions);

	for (std::vector<collision*>::iterator it = collisions.begin(); it != collisions.end(); it++)
	{
		if ((*it)->object != this)
		{
			if ((*it)->type == PLAYER_ID && !((Fire*)((Player*)(*it)->object)->spells[FIRE])->is_fireshield_up)
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
			App->ren->Blit(App->tex->Get_Texture("hazards"), collider->x + 48 * i, collider->y + 48 * j, anim.GetCurrentFrame(), 50);
		}
	}
	return true;
}