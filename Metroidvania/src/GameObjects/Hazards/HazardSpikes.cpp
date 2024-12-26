#include "HazardSpikes.h"
#include "Application.h"
#include "Modules/Render.h"
#include "../Player.h"
#include "Modules/ProgressTracker.h"
#include "../../SceneProcessing.h"

HazardSpikes::HazardSpikes()
{
	spikes = { 96,48,48,48 };
}

void HazardSpikes::Init()
{
	Engine->GetModule<::Render>().LoadTexture("Assets/Sprites/hazards.png", hazards);
}

bool HazardSpikes::Loop(float dt)
{
	std::vector<collision> collisions;
	Engine->GetModule<SceneController>().GetCollisions(&collider, collisions);

	for (std::vector<collision>::iterator it = collisions.begin(); it != collisions.end(); it++)
	{
		if ((*it).object != this)
		{
			if ((*it).object->IsSameTypeAs<Player>())
			{
				if (!((Player*)(*it).object)->movement_is_locked && !((Player*)(*it).object)->respawn_player)
				{
					((Player*)(*it).object)->LockMovement(900);
					((Player*)(*it).object)->Respawn();
				}
			}
		}
	}

	return true;
}

bool HazardSpikes::Render()
{
	int tilesx = collider.w / 48;
	int tilesy = collider.h / 48;

	//top
	for (int i = 0; i < tilesx; ++i)
	{
		Engine->GetModule<::Render>().RenderTexture(hazards, collider.x + 48 * i, collider.y, spikes, -20);
	}

	return true;
}
