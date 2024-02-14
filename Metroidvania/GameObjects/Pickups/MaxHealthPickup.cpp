#include "MaxHealthPickup.h"
#include "Particles.h"
#include "Application.h"
#include "ProgressTracker.h"
#include "Gui.h"
#include "../../UIElements/UItextbox.h"
#include "../../UIelementFunctions.h"
#include "../Player.h"
#include "../EntityIDs.h"
MaxHealthPickup::MaxHealthPickup()
{
	maxhealthplus = {0,0,48,48};
	p = App->par->AddParticleEmitter(&App->par->magic, 0, 0);
	if (App->trk->BaseSaveSection->GetChild("pickups") == nullptr)
	{
		App->trk->BaseSaveSection->AddNewChild("pickups");
	}
}

MaxHealthPickup::~MaxHealthPickup()
{
	//App->par->to_delete.push_back(p);
	App->par->RemoveParticleEmitter(p);
	//GameObject::~GameObject();
}

bool MaxHealthPickup::Loop(float dt)
{
	if (App->trk->BaseSaveSection->GetChild("pickups")->GetValue(std::to_string(pickup_id).c_str()) != 0.0f)
	{
		App->phy->DeleteObject(this);
	}

	p->position_x = collider->x;
	p->position_y = collider->y;



	std::vector<collision*> collisions;
	App->phy->GetCollisions(collider, collisions);

	for (std::vector<collision*>::iterator it = collisions.begin(); it != collisions.end(); it++)
	{
		if ((*it)->object != this)
		{
			if ((*it)->type == PLAYER_ID)
			{
				Player* pl = (Player*)((*it)->object);
				pl->AddHealth(1);

				App->trk->BaseSaveSection->GetChild("pickups")->SetValue(std::to_string(pickup_id).c_str(),1.0f);
				
				int charges_for_hp = App->trk->BaseSaveSection->GetChild("pickups")->GetValue("CurrentFragmentsHP");
				int max_charges_for_hp = App->trk->BaseSaveSection->GetChild("pickups")->GetValue("MaxFragmentsHP");

				App->phy->DeleteObject(this);

				std::string s = std::to_string(charges_for_hp + 1) + "/" + std::to_string(max_charges_for_hp)+ " to increase hp";

				charges_for_hp += 1;
				if (charges_for_hp >= max_charges_for_hp)
				{
					pl->max_health += 1;
					pl->health += 1;
					int charges_for_hp = App->trk->BaseSaveSection->GetChild("pickups")->SetValue("CurrentFragmentsHP",0);
					App->trk->BaseSaveSection->GetChild("pickups")->SetValue("MaxHP",pl->max_health);
				}

				UItextbox* textbox= new UItextbox("", s.c_str(), TextBoxColor::GREY, 15, 4, 272, 420, 2, 0.2);
				App->gui->AddElement(textbox);

				if (text != "")
				{
					textbox->AddPanelToTextBox(text.c_str());
				}
				if (lore_unlock != -1 && App->trk->BaseSaveSection->GetChild("LoreLogs")->GetValue(std::to_string(lore_unlock).c_str()) == 0.0f)
				{
					App->trk->BaseSaveSection->GetChild("LoreLogs")->SetValue(std::to_string(lore_unlock).c_str(), 1.0f);
					textbox->AddPanelToTextBox("New lore entry unlocked");
				}
				//App->par->to_delete.push_back(p);
				App->par->AddParticleEmitter(&App->par->magic, collider->x, collider->y, 1500);
			}
		}
	}


	return true;
}

bool MaxHealthPickup::Render()
{
	App->ren->Blit(App->tex->Get_Texture("items"), collider->x, collider->y, &maxhealthplus, 10);

	return true;
}