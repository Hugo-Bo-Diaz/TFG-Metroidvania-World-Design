#include"WaterSpellPickup.h"
#include "Application.h"
#include "../Player.h"
#include "Modules/Gui.h"
#include "Modules/ProgressTracker.h"
#include "../../UIElements/UItextbox.h"

WaterSpellPickup::WaterSpellPickup()
{

}

void WaterSpellPickup::Destroy()
{
	//Engine->GetModule<::Render>().to_delete.push_back(p);
	Engine->GetModule<::Render>().RemoveParticleEmitter(p);
}

void WaterSpellPickup::Init()
{
	Engine->GetModule<::Render>().LoadTexture("Assets/Sprites/particles.png", particles);
	Engine->GetModule<::Render>().LoadTexture("Assets/UI/books.png", spell_books);

	waterspellbook = { 108,0,52,64 };

	if (Engine->GetModule<ProgressTracker>().GetBaseSaveSection()->GetChild("SpellsUnlock") == nullptr)
	{
		Engine->GetModule<ProgressTracker>().GetBaseSaveSection()->AddNewChild("SpellsUnlock");
	}
	if (Engine->GetModule<ProgressTracker>().GetBaseSaveSection()->GetChild("LoreLogs") == nullptr)
	{
		Engine->GetModule<ProgressTracker>().GetBaseSaveSection()->AddNewChild("LoreLogs");
	}


	r18waterfirst = { 0,48,12,12 };
	r19watersecond = { 12,48,12,12 };
	r20waterthird = { 24,48,12,12 };
	water_drip.area_in_texture.push_back(&r18waterfirst);
	water_drip.area_in_texture.push_back(&r19watersecond);
	water_drip.area_in_texture.push_back(&r20waterthird);
	water_drip.name = "water_drip";
	water_drip.minmax_x_offset = std::make_pair(0, 52);
	water_drip.minmax_y_offset = std::make_pair(0, 62);
	water_drip.minmax_speed_x = std::make_pair(-0.5, 0.5);
	water_drip.minmax_speed_y = std::make_pair(-2.0, -4.0);
	water_drip.minmax_scale = std::make_pair(1, 1.1);
	water_drip.minmax_lifespan = std::make_pair(350, 500);
	water_drip.minmax_frequency = std::make_pair(10, 20);
	water_drip.minmax_acc_y = std::make_pair(0.1, 0.2);
	water_drip.texture_name = particles;

	p = Engine->GetModule<::Render>().AddParticleEmitter(&water_drip, collider.x, collider.y);
}

bool WaterSpellPickup::Loop(float dt)
{
	if(Engine->GetModule<ProgressTracker>().GetBaseSaveSection()->GetChild("SpellsUnlock")->GetValue("Water"))
	{
		Engine->GetModule<SceneController>().DeleteObject(this);
	}

	p->position_x = collider.x;
	p->position_y = collider.y;

	std::vector<collision> collisions;
	Engine->GetModule<SceneController>().GetCollisions(&collider, collisions);

	for (std::vector<collision>::iterator it = collisions.begin(); it != collisions.end(); it++)
	{
		if ((*it).object != this)
		{
			if ((*it).object->IsSameTypeAs<Player>())
			{
				((Player*)((*it).object))->unlock_spell(WATER);
				Engine->GetModule<ProgressTracker>().GetBaseSaveSection()->GetChild("SpellsUnlock")->SetValue("Water", 1);
				Engine->GetModule<SceneController>().DeleteObject(this);
				UItextbox* textbox= new UItextbox("","congratulations you unlocked water!",TextBoxColor::RED, 15, 4, 272, 420, 2, 0.2);
				textbox->AddPanelToTextBox("remember your past      we need you");
				Engine->GetModule<UserInterface>().AddElement(textbox);
				//Engine->GetModule<::Render>().to_delete.push_back(p);
				Engine->GetModule<::Render>().AddParticleEmitter(&water_drip, collider.x, collider.y,1500);
				Engine->GetModule<ProgressTracker>().GetBaseSaveSection()->GetChild("LoreLogs")->SetValue("Lore4",4.0f);
			}
		}
	}

	return true;
}

bool WaterSpellPickup::Render()
{
	Engine->GetModule<::Render>().RenderTexture(spell_books, collider.x, collider.y, waterspellbook, 10);
	return true;
}
