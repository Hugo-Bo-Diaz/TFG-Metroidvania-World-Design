#include "CheckPoint.h"
#include "Application.h"
#include "ProgressTracker.h"


CheckPoint::CheckPoint()
{
}


CheckPoint::~CheckPoint()
{
}

bool CheckPoint::Loop(float dt)
{
	std::vector<collision*> collisions;
	App->phy->GetCollisions(collider, collisions);

	for (std::vector<collision*>::iterator it = collisions.begin(); it != collisions.end(); it++)
	{
		if ((*it)->object != this)
		{
			if ((*it)->type == PLAYER)
			{
				App->trk->SetCheckPoint(spawn_pos_x,spawn_pos_y,room_id);
			}
		}
	}

	return true;
}
