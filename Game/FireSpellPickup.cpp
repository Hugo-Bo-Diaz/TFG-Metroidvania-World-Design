#include"FireSpellPickup.h"
#include "Application.h"
#include "Physics.h"
#include "Player.h"
#include "Particles.h"
#include "Gui.h"
#include "ProgressTracker.h"
#include "UItextbox.h"

FireSpellPickup::FireSpellPickup()
{
	firespellbook = { 0,0,52,64 };

	p = App->par->AddParticleEmitter(&App->par->fireshield, collider->x, collider->y);
}

FireSpellPickup::~FireSpellPickup()
{
	//App->par->to_delete.push_back(p);
	App->par->RemoveParticleEmitter(p);
}

bool FireSpellPickup::Loop(float dt)
{
	if(App->trk->unlocked[FIRE])
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
				((Player*)((*it)->object))->unlock_spell(FIRE);
				App->phy->DeleteObject(this);
				UItextbox* t= (UItextbox*)App->gui->AddTextBox("","congratulations you unlocked fire!",TextBoxColor::RED, 15, 4, 272, 420, 2, 0.2);
				t->AddPanelToTextBox("remember your past      we need you");
				//App->par->to_delete.push_back(p);
				App->par->AddParticleEmitter(&App->par->fireshield, collider->x, collider->y,1500);
				App->trk->UnlockLog(4);
			}
		}
	}

	return true;
}

bool FireSpellPickup::Render()
{
	App->ren->Blit(App->tex->Get_Texture("spell_books"), collider->x, collider->y, &firespellbook, 10);
	return true;
}
