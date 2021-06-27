#include "CheckPoint.h"
#include "Application.h"
#include "ProgressTracker.h"
#include "Gui.h"
#include "Player.h"

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
				if (App->trk->last_checkpoint_id != room_id || 
					App->trk->last_checkpoint_x != spawn_pos_x || 
					App->trk->last_checkpoint_y != spawn_pos_y)
				{
					//show checkpoint indicator
					App->trk->SaveGame("save_file.xml");
					App->gui->AddCheckpointIndicator();
					Player* p = ((Player*)((*it)->object));
					while (p->health<p->max_health)
					{
						p->AddHealth(1);
					}
				}

				App->trk->SetCheckPoint(spawn_pos_x,spawn_pos_y,room_id);
			}
		}
	}

	return true;
}
