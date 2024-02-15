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
	r16sandfirst = { 48,0,12,12 };
	r17sandsecond = { 48,12,12,12 };

	sand_left.area_in_texture.push_back(&r16sandfirst);
	sand_left.area_in_texture.push_back(&r17sandsecond);
	sand_left.name = "sand_left";
	sand_left.minmax_x_offset = std::make_pair(-5, 69);
	sand_left.minmax_y_offset = std::make_pair(50, 70);
	sand_left.minmax_speed_y = std::make_pair(-1.5, -2.5);
	sand_left.minmax_speed_x = std::make_pair(1.5, 3.0);
	sand_left.minmax_scale_speed = std::make_pair(-0.03, -0.04);
	sand_left.minmax_scale = std::make_pair(0.75, 1.25);
	sand_left.minmax_acc_y = std::make_pair(0.04, 0.05);
	sand_left.minmax_lifespan = std::make_pair(500, 600);
	sand_left.minmax_frequency = std::make_pair(5, 20);
	sand_left.texture_name = "particles";

	sand_right.area_in_texture.push_back(&r16sandfirst);
	sand_right.area_in_texture.push_back(&r17sandsecond);
	sand_right.name = "sand_left";
	sand_right.minmax_x_offset = std::make_pair(-5, 69);
	sand_right.minmax_y_offset = std::make_pair(50, 70);
	sand_right.minmax_speed_y = std::make_pair(-1.5, -2.5);
	sand_right.minmax_speed_x = std::make_pair(-1.5, -3.0);
	sand_right.minmax_scale_speed = std::make_pair(-0.03, -0.04);
	sand_right.minmax_scale = std::make_pair(0.75, 1.25);
	sand_right.minmax_acc_y = std::make_pair(0.04, 0.05);
	sand_right.minmax_lifespan = std::make_pair(500, 600);
	sand_right.minmax_frequency = std::make_pair(5, 20);
	sand_right.texture_name = "particles";

	groundspellbook = { 216,0,52,64 };

	p = App->par->AddParticleEmitter(&sand_left, collider->x, collider->y);
	q = App->par->AddParticleEmitter(&sand_right, collider->x, collider->y);

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
				App->par->AddParticleEmitter(&sand_left, collider->x, collider->y, 1500);
				App->par->AddParticleEmitter(&sand_right, collider->x, collider->y, 1500);
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
