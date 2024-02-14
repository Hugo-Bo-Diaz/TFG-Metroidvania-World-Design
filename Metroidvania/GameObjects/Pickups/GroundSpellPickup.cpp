#include "GroundSpellPickup.h"
#include "Application.h"
#include "Physics.h"
#include "../Player.h"
#include "Particles.h"
#include "Gui.h"
#include "ProgressTracker.h"
#include "../../UIElements/UItextbox.h"
#include "../EntityIDs.h"
#include "../../UIelementFunctions.h"
GroundSpellPickup::GroundSpellPickup()
{
	groundspellbook = { 216,0,52,64 };

	p = App->par->AddParticleEmitter(&App->par->sand_left, collider->x, collider->y);
	q = App->par->AddParticleEmitter(&App->par->sand_right, collider->x, collider->y);

	if (App->trk->BaseSaveSection->GetChild("SpellsUnlock") == nullptr)
	{
		App->trk->BaseSaveSection->AddNewChild("SpellsUnlock");
	}
	if (App->trk->BaseSaveSection->GetChild("LoreLogs") == nullptr)
	{
		App->trk->BaseSaveSection->AddNewChild("LoreLogs");
	}
}

GroundSpellPickup::~GroundSpellPickup()
{
	//App->par->to_delete.push_back(p);
	App->par->RemoveParticleEmitter(p);
	App->par->RemoveParticleEmitter(q);

}

bool GroundSpellPickup::Loop(float dt)
{
	if (App->trk->BaseSaveSection->GetChild("SpellsUnlock")->GetValue("Ground"))
	{
		App->phy->DeleteObject(this);
	}

	p->position_x = collider->x;
	p->position_y = collider->y;
	
	q->position_x = collider->x;
	q->position_y = collider->y;

	std::vector<collision*> collisions;
	App->phy->GetCollisions(collider, collisions);

	for (std::vector<collision*>::iterator it = collisions.begin(); it != collisions.end(); it++)
	{
		if ((*it)->object != this)
		{
			if ((*it)->type == PLAYER_ID)
			{
				((Player*)((*it)->object))->unlock_spell(GROUND);
				App->trk->BaseSaveSection->GetChild("SpellsUnlock")->SetValue("Ground", 1);
				App->phy->DeleteObject(this);
				UItextbox* textbox = new UItextbox("", "congratulations you unlocked ground!", TextBoxColor::GREY, 15, 4, 272, 420, 2, 0.2);
				textbox->AddPanelToTextBox("this castle is still alive, just broken");
				App->gui->AddElement(textbox);
				//App->par->to_delete.push_back(p);
				App->par->AddParticleEmitter(&App->par->sand_left, collider->x, collider->y, 1500);
				App->par->AddParticleEmitter(&App->par->sand_right, collider->x, collider->y, 1500);
				App->trk->BaseSaveSection->GetChild("LoreLogs")->SetValue("16", 1.0f);

			}
		}
	}

	return true;
}

bool GroundSpellPickup::Render()
{
	App->ren->Blit(App->tex->Get_Texture("spell_books"), collider->x, collider->y, &groundspellbook, 10);
	return true;
}