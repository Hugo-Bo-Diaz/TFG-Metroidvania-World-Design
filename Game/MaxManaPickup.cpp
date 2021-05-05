#include "MaxManaPickup.h"
#include "Particles.h"
#include "Application.h"
#include "ProgressTracker.h"
#include "Gui.h"
#include "UItextbox.h"

MaxManaPickup::MaxManaPickup()
{
	maxmanaplus = { 48,0,48,48 };
	p = App->par->AddParticleEmitter(&App->par->magic, 0, 0);
}

MaxManaPickup::~MaxManaPickup()
{
	//App->par->to_delete.push_back(p);
	App->par->RemoveParticleEmitter(p);
	physobj::~physobj();
}

bool MaxManaPickup::Loop(float dt)
{
	if (App->trk->HasBeenFound(pickup_id))
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
			if ((*it)->type == PLAYER)
			{

				App->trk->AddPickupToList(pickup_id);
				App->trk->charges_mana++;
				App->phy->DeleteObject(this);

				std::string s = std::to_string(App->trk->charges_mana) + "/" + std::to_string(App->trk->charges_per_mana) + " to increase mana";

				UItextbox* t = (UItextbox*)App->gui->AddTextBox("", s.c_str(), TextBoxColor::GREY, 15, 4, 272, 420, 2, 0.2);
				if (text != "")
				{
					t->AddPanelToTextBox(text.c_str());
				}
				if (lore_unlock != -1 && !App->trk->HasLogBeenUnlocked(lore_unlock))
				{
					App->trk->UnlockLog(lore_unlock);
					t->AddPanelToTextBox("New lore entry unlocked");
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
