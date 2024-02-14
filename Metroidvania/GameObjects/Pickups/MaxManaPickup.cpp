#include "MaxManaPickup.h"
#include "Particles.h"
#include "Application.h"
#include "ProgressTracker.h"
#include "Gui.h"
#include "../../UIElements/UItextbox.h"
#include "../Player.h"
#include "../EntityIDs.h"
#include "../../UIelementFunctions.h"
MaxManaPickup::MaxManaPickup()
{
	maxmanaplus = { 48,0,48,48 };
	p = App->par->AddParticleEmitter(&App->par->magic, 0, 0);

	if (App->trk->BaseSaveSection->GetChild("LoreLogs") == nullptr)
	{
		App->trk->BaseSaveSection->AddNewChild("LoreLogs");
	}
}

MaxManaPickup::~MaxManaPickup()
{
	//App->par->to_delete.push_back(p);
	App->par->RemoveParticleEmitter(p);
	GameObject::~GameObject();
}

bool MaxManaPickup::Loop(float dt)
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
				pl->AddMana(1);

				App->trk->BaseSaveSection->GetChild("pickups")->SetValue(std::to_string(pickup_id).c_str(), 1.0f);

				int charges_for_mana = App->trk->BaseSaveSection->GetChild("pickups")->GetValue("CurrentFragmentsMana");
				int max_charges_for_mana = App->trk->BaseSaveSection->GetChild("pickups")->GetValue("MaxFragmentsMana");

				App->phy->DeleteObject(this);

				std::string s = std::to_string(charges_for_mana + 1) + "/" + std::to_string(max_charges_for_mana) + " to increase mana";

				charges_for_mana += 1;

				if (charges_for_mana >= max_charges_for_mana)
				{
					pl->max_mana += 1;
					pl->mana += 1;
					charges_for_mana = 0;
					App->trk->BaseSaveSection->GetChild("pickups")->SetValue("MaxMana", pl->max_mana);
				}

				App->trk->BaseSaveSection->GetChild("pickups")->SetValue("CurrentFragmentsMana", charges_for_mana);

				UItextbox* textbox = new UItextbox("", s.c_str(), TextBoxColor::GREY, 15, 4, 272, 420, 2, 0.2);
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

bool MaxManaPickup::Render()
{
	App->ren->Blit(App->tex->Get_Texture("items"), collider->x, collider->y, &maxmanaplus, 10);

	return true;
}