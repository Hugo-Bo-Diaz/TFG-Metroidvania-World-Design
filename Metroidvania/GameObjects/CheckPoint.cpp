#include "CheckPoint.h"
#include "Application.h"
#include "ProgressTracker.h"
#include "Gui.h"
#include "Player.h"
#include "../UIElements/UICheckpointIndicator.h"

CheckPoint::CheckPoint()
{
	if (App->trk->BaseSaveSection->GetChild("Checkpoint") == nullptr)
	{
		App->trk->BaseSaveSection->AddNewChild("Checkpoint");
	}
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
			if ((*it)->object->IsSameTypeAs<Player>())
			{
				if (App->trk->BaseSaveSection->GetChild("Checkpoint")->GetValue("last_checkpoint_id") != room_id ||
					App->trk->BaseSaveSection->GetChild("Checkpoint")->GetValue("last_checkpoint_x") != spawn_pos_x ||
					App->trk->BaseSaveSection->GetChild("Checkpoint")->GetValue("last_checkpoint_y") != spawn_pos_y)
				{
					//show checkpoint indicator
					App->trk->SaveGame("save_file.xml");
					UIcheckpointIndicator* indicator = new  UIcheckpointIndicator();
					App->gui->AddElement(indicator);
					Player* p = ((Player*)((*it)->object));
					while (p->health<p->max_health)
					{
						p->AddHealth(1);
					}
				}

				App->trk->BaseSaveSection->GetChild("Checkpoint")->SetValue("last_checkpoint_id", room_id);
				App->trk->BaseSaveSection->GetChild("Checkpoint")->SetValue("last_checkpoint_id", spawn_pos_x);
				App->trk->BaseSaveSection->GetChild("Checkpoint")->SetValue("last_checkpoint_id", spawn_pos_y);
			}
		}
	}

	return true;
}
