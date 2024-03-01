#include "HazardSpikes.h"
#include "Application.h"
#include "Render.h"
#include "../Player.h"
#include "ProgressTracker.h"
#include "Textures.h"
#include "../../SceneProcessing.h"

HazardSpikes::HazardSpikes()
{
	hazards = App->tex->Load_Texture("Assets/Sprites/hazards.png");
	spikes = { 96,48,48,48 };
}

bool HazardSpikes::Loop(float dt)
{
	std::vector<collision*> collisions;
	App->phy->GetCollisions(collider, collisions);

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

	return true;
}

bool HazardSpikes::Render()
{
	int tilesx = collider->w / 48;
	int tilesy = collider->h / 48;

	//top
	for (int i = 0; i < tilesx; ++i)
	{
		App->ren->Blit(hazards, collider->x + 48 * i, collider->y, &spikes, -20);
	}

	return true;
}
