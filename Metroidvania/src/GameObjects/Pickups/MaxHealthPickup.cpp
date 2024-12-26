#include "MaxHealthPickup.h"
#include "Application.h"
#include "Modules/ProgressTracker.h"
#include "Modules/Gui.h"
#include "../../UIElements/UItextbox.h"
#include "../../UIelementFunctions.h"
#include "../Player.h"
#include "../../MetroidvaniaConstants.h"

MaxHealthPickup::MaxHealthPickup()
{

}

MaxHealthPickup::MaxHealthPickup(std::list<ObjectProperty*>& aProperties)
{
	new (this) MaxHealthPickup;

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

void MaxHealthPickup::Destroy()
{
	//Engine->GetModule<::Render>().to_delete.push_back(p);
	Engine->GetModule<::Render>().RemoveParticleEmitter(p);
	//GameObject::~GameObject();
}

void MaxHealthPickup::Init()
{
	Engine->GetModule<::Render>().LoadTexture("Assets/Sprites/particles.png", particles);
	Engine->GetModule<::Render>().LoadTexture("Assets/Sprites/items.png", items);

	maxhealthplus = { 0,0,48,48 };

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
	if (Engine->GetModule<ProgressTracker>().GetBaseSaveSection()->GetChild("pickups") == nullptr)
	{
		Engine->GetModule<ProgressTracker>().GetBaseSaveSection()->AddNewChild("pickups");
	}
	if (Engine->GetModule<ProgressTracker>().GetBaseSaveSection()->GetChild("LoreLogs") == nullptr)
	{
		Engine->GetModule<ProgressTracker>().GetBaseSaveSection()->AddNewChild("LoreLogs");
	}
}

bool MaxHealthPickup::Loop(float dt)
{
	std::string lPickup = "Pickup" + std::to_string(pickup_id);
	if (Engine->GetModule<ProgressTracker>().GetBaseSaveSection()->GetChild("pickups")->GetValue(lPickup.c_str()) != 0.0f)
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
				pl->AddHealth(1);

				Engine->GetModule<ProgressTracker>().GetBaseSaveSection()->GetChild("pickups")->SetValue(lPickup.c_str(),1.0f);
				
				int charges_for_hp = Engine->GetModule<ProgressTracker>().GetBaseSaveSection()->GetChild("Stats")->GetValue("CurrentFragmentsHP");

				Engine->GetModule<SceneController>().DeleteObject(this);

				std::string s = std::to_string(charges_for_hp + 1) + "/" + std::to_string(ITEM_SHARDS_FOR_UPGRADE)+ " to increase hp";

				charges_for_hp += 1;
				if (charges_for_hp >= ITEM_SHARDS_FOR_UPGRADE)
				{
					pl->max_health += 1;
					pl->health += 1;
					charges_for_hp = 0;
					Engine->GetModule<ProgressTracker>().GetBaseSaveSection()->GetChild("Stats")->SetValue("MaxHP",pl->max_health);
				}
				Engine->GetModule<ProgressTracker>().GetBaseSaveSection()->GetChild("Stats")->SetValue("CurrentFragmentsHP",charges_for_hp);

				UItextbox* textbox= new UItextbox("", s.c_str(), TextBoxColor::GREY, 15, 4, 272, 420, 2, 0.2);
				Engine->GetModule<UserInterface>().AddElement(textbox);

				if (text != "")
				{
					textbox->AddPanelToTextBox(text.c_str());
				}
				
				std::string lEntry = "Lore" + std::to_string(lore_unlock);
				if (lore_unlock != -1 && Engine->GetModule<ProgressTracker>().GetBaseSaveSection()->GetChild("LoreLogs")->GetValue(lEntry.c_str()) == 0.0f)
				{
					Engine->GetModule<ProgressTracker>().GetBaseSaveSection()->GetChild("LoreLogs")->SetValue(lEntry.c_str(), lore_unlock);
					textbox->AddPanelToTextBox("New lore entry unlocked");
				}
				Engine->GetModule<::Render>().AddParticleEmitter(&magic, collider.x, collider.y, 1500);

				Engine->GetModule<ProgressTracker>().SaveFile("save_file.xml");
			}
		}
	}


	return true;
}

bool MaxHealthPickup::Render()
{
	Engine->GetModule<::Render>().RenderTexture(items, collider.x, collider.y, maxhealthplus, 10);

	return true;
}