#include "CheckPoint.h"
#include "Application.h"
#include "Modules/ProgressTracker.h"
#include "Modules/Gui.h"
#include "Player.h"
#include "../UIElements/UICheckpointIndicator.h"

void CheckPoint::Init()
{
	if (Engine->GetModule<ProgressTracker>().GetBaseSaveSection()->GetChild("Checkpoint") == nullptr)
	{
		Engine->GetModule<ProgressTracker>().GetBaseSaveSection()->AddNewChild("Checkpoint");
	}
}


CheckPoint::~CheckPoint()
{
}

bool CheckPoint::Loop(float dt)
{
	std::vector<collision*> collisions;
	Engine->GetModule<ObjectManager>().GetCollisions(collider, collisions);

	for (std::vector<collision*>::iterator it = collisions.begin(); it != collisions.end(); it++)
	{
		if ((*it)->object != this)
		{
			if ((*it)->object->IsSameTypeAs<Player>())
			{
				if (Engine->GetModule<ProgressTracker>().GetBaseSaveSection()->GetChild("Checkpoint")->GetValue("last_checkpoint_id") != room_id ||
					Engine->GetModule<ProgressTracker>().GetBaseSaveSection()->GetChild("Checkpoint")->GetValue("last_checkpoint_x") != spawn_pos_x ||
					Engine->GetModule<ProgressTracker>().GetBaseSaveSection()->GetChild("Checkpoint")->GetValue("last_checkpoint_y") != spawn_pos_y)
				{
					//show checkpoint indicator
					Engine->GetModule<ProgressTracker>().SaveFile("save_file.xml");
					UIcheckpointIndicator* indicator = new  UIcheckpointIndicator();
					Engine->GetModule<UserInterface>().AddElement(indicator);
					Player* p = ((Player*)((*it)->object));
					while (p->health<p->max_health)
					{
						p->AddHealth(1);
					}
				}

				Engine->GetModule<ProgressTracker>().GetBaseSaveSection()->GetChild("Checkpoint")->SetValue("last_checkpoint_id", room_id);
				Engine->GetModule<ProgressTracker>().GetBaseSaveSection()->GetChild("Checkpoint")->SetValue("last_checkpoint_id", spawn_pos_x);
				Engine->GetModule<ProgressTracker>().GetBaseSaveSection()->GetChild("Checkpoint")->SetValue("last_checkpoint_id", spawn_pos_y);
			}
		}
	}

	return true;
}
