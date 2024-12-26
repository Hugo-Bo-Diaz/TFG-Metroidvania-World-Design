#include "MaxManaPickup.h"
#include "Application.h"
#include "Modules/ProgressTracker.h"
#include "Modules/Gui.h"
#include "../../UIElements/UItextbox.h"
#include "../Player.h"
#include "../../UIelementFunctions.h"
#include "../../MetroidvaniaConstants.h"

MaxManaPickup::MaxManaPickup()
{
}

void MaxManaPickup::Init()
{
	Engine->GetModule<::Render>().LoadTexture("Assets/Sprites/particles.png", particles);
	Engine->GetModule<::Render>().LoadTexture("Assets/Sprites/items.png", items);

	maxmanaplus = { 48,0,48,48 };

	r13magic = { 0,36,12,12 };
	magic.area_in_texture.push_back(&r13magic);
	magic.name = "magic";
	magic.minmax_x_offset = std::make_pair(-5, 53);
	magic.minmax_y_offset = std::make_pair(24, 55);
	magic.minmax_speed_y = std::make_pair(-0.3, -0.5);
	magic.minmax_scale = std::make_pair(0.5, 1);
	magic.minmax_angle_speed = std::make_pair(5, 15);
	magic.minmax_angle = std::make_pair(0, 360);
	magic.minmax_lifespan = std::make_pair(300, 400);
	magic.minmax_frequency = std::make_pair(20, 40);
	magic.texture_name = particles;

	p = Engine->GetModule<::Render>().AddParticleEmitter(&magic, 0, 0);

	if (Engine->GetModule<ProgressTracker>().GetBaseSaveSection()->GetChild("LoreLogs") == nullptr)
	{
		Engine->GetModule<ProgressTracker>().GetBaseSaveSection()->AddNewChild("LoreLogs");
	}
}

MaxManaPickup::MaxManaPickup(std::list<ObjectProperty*>& aProperties)
{
	new (this) MaxManaPickup;

	for (std::list<ObjectProperty*>::iterator it = aProperties.begin(); it != aProperties.end(); ++it)
	{
		if ((*it)->name.compare("text") == 0)
		{
			text = (*it)->str_value;
		}
		else if ((*it)->name.compare("id") == 0)
		{
			pickup_id = (*it)->num_value;
		}
		else if ((*it)->name.compare("lore") == 0)
		{
			lore_unlock = (*it)->num_value;
		}
	}
}

void MaxManaPickup::Destroy()
{
	//Engine->GetModule<::Render>().to_delete.push_back(p);
	Engine->GetModule<::Render>().RemoveParticleEmitter(p);
}

bool MaxManaPickup::Loop(float dt)
{
	Section* lSection = Engine->GetModule<ProgressTracker>().GetBaseSaveSection()->GetChild("pickups");
	if (lSection != nullptr && lSection->GetValue(std::to_string(pickup_id).c_str()) != 0.0f)
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
				Player* pl = (Player*)((*it).object);
				pl->AddMana(1);

				Engine->GetModule<ProgressTracker>().GetBaseSaveSection()->GetChild("pickups")->SetValue(std::to_string(pickup_id).c_str(), 1.0f);

				int charges_for_mana = Engine->GetModule<ProgressTracker>().GetBaseSaveSection()->GetChild("Stats")->GetValue("CurrentFragmentsMana");

				Engine->GetModule<SceneController>().DeleteObject(this);

				std::string s = std::to_string(charges_for_mana + 1) + "/" + std::to_string(ITEM_SHARDS_FOR_UPGRADE) + " to increase mana";

				charges_for_mana += 1;

				if (charges_for_mana >= ITEM_SHARDS_FOR_UPGRADE)
				{
					pl->max_mana += 1;
					pl->mana += 1;
					charges_for_mana = 0;
					Engine->GetModule<ProgressTracker>().GetBaseSaveSection()->GetChild("Stats")->SetValue("MaxMana", pl->max_mana);
				}

				Engine->GetModule<ProgressTracker>().GetBaseSaveSection()->GetChild("Stats")->SetValue("CurrentFragmentsMana", charges_for_mana);

				UItextbox* textbox = new UItextbox("", s.c_str(), TextBoxColor::GREY, 15, 4, 272, 420, 2, 0.2);
				Engine->GetModule<UserInterface>().AddElement(textbox);

				if (text != "")
				{
					textbox->AddPanelToTextBox(text.c_str());
				}
				if (lore_unlock != -1 && Engine->GetModule<ProgressTracker>().GetBaseSaveSection()->GetChild("LoreLogs")->GetValue(std::to_string(lore_unlock).c_str()) == 0.0f)
				{
					std::string lEntry = "Lore" + std::to_string(lore_unlock);
					Engine->GetModule<ProgressTracker>().GetBaseSaveSection()->GetChild("LoreLogs")->SetValue(lEntry.c_str(), 1.0f);
					textbox->AddPanelToTextBox("New lore entry unlocked");
				}
				//Engine->GetModule<::Render>().to_delete.push_back(p);
				Engine->GetModule<::Render>().AddParticleEmitter(&magic, collider.x, collider.y, 1500);
			}
		}
	}


	return true;
}

bool MaxManaPickup::Render()
{
	Engine->GetModule<::Render>().RenderTexture(items, collider.x, collider.y, maxmanaplus, 10);

	return true;
}
