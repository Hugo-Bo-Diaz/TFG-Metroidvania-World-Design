#include"FireSpellPickup.h"
#include "Application.h"
#include "Modules/ObjectManager.h"
#include "../Player.h"
#include "Modules/Particles.h"
#include "Modules/Gui.h"
#include "Modules/ProgressTracker.h"
#include "../../UIElements/UItextbox.h"

FireSpellPickup::FireSpellPickup()
{
	particles = Engine->GetModule<Textures>().Load_Texture("Assets/Sprites/particles.png");
	spell_books = Engine->GetModule<Textures>().Load_Texture("Assets/UI/books.png");

	firespellbook = { 0,0,52,64 };

	if (Engine->GetModule<ProgressTracker>().BaseSaveSection->GetChild("SpellsUnlock") == nullptr)
	{
		Engine->GetModule<ProgressTracker>().BaseSaveSection->AddNewChild("SpellsUnlock");
	}
	if (Engine->GetModule<ProgressTracker>().BaseSaveSection->GetChild("LoreLogs") == nullptr)
	{
		Engine->GetModule<ProgressTracker>().BaseSaveSection->AddNewChild("LoreLogs");
	}

	r2shield = { 12,0,12,12 };
	r12shield = { 24,0,12,12 };
	fireshield.area_in_texture.push_back(&r2shield);
	fireshield.area_in_texture.push_back(&r2shield);
	fireshield.area_in_texture.push_back(&r12shield);
	fireshield.name = "fireshield";
	fireshield.minmax_x_offset = std::make_pair(-5, 69);
	fireshield.minmax_y_offset = std::make_pair(0, 70);
	fireshield.minmax_speed_y = std::make_pair(-1.5, -2.5);
	fireshield.minmax_speed_x = std::make_pair(-0.1, 0.1);
	fireshield.minmax_scale_speed = std::make_pair(-0.03, -0.04);
	fireshield.minmax_scale = std::make_pair(1, 1.5);
	fireshield.minmax_acc_y = std::make_pair(0.04, 0.05);
	fireshield.minmax_lifespan = std::make_pair(300, 400);
	fireshield.minmax_frequency = std::make_pair(5, 20);
	fireshield.texture_name = particles;

	p = Engine->GetModule<Particles>().AddParticleEmitter(&fireshield, collider->x, collider->y);
}

FireSpellPickup::~FireSpellPickup()
{
	//Engine->GetModule<Particles>().to_delete.push_back(p);
	Engine->GetModule<Particles>().RemoveParticleEmitter(p);
}

bool FireSpellPickup::Loop(float dt)
{
	if(Engine->GetModule<ProgressTracker>().BaseSaveSection->GetChild("SpellsUnlock")->GetValue("Fire"))
	{
		Engine->GetModule<ObjectManager>().DeleteObject(this);
	}

	p->position_x = collider->x;
	p->position_y = collider->y;

	std::vector<collision*> collisions;
	Engine->GetModule<ObjectManager>().GetCollisions(collider, collisions);

	for (std::vector<collision*>::iterator it = collisions.begin(); it != collisions.end(); it++)
	{
		if ((*it)->object != this)
		{
			if ((*it)->object->IsSameTypeAs<Player>())
			{
				((Player*)((*it)->object))->unlock_spell(FIRE);
				Engine->GetModule<ProgressTracker>().BaseSaveSection->GetChild("SpellsUnlock")->SetValue("Fire", 1);
				Engine->GetModule<ObjectManager>().DeleteObject(this);
				UItextbox* textbox= new UItextbox("","congratulations you unlocked fire!",TextBoxColor::RED, 15, 4, 272, 420, 2, 0.2);
				textbox->AddPanelToTextBox("remember your past      we need you");
				Engine->GetModule<UserInterface>().AddElement(textbox);
				//Engine->GetModule<Particles>().to_delete.push_back(p);
				Engine->GetModule<Particles>().AddParticleEmitter(&fireshield, collider->x, collider->y,1500);
				Engine->GetModule<ProgressTracker>().BaseSaveSection->GetChild("LoreLogs")->SetValue("4",1.0f);
			}
		}
	}

	return true;
}

bool FireSpellPickup::Render()
{
	Engine->GetModule<::Render>().Blit(spell_books, collider->x, collider->y, &firespellbook, 10);
	return true;
}
