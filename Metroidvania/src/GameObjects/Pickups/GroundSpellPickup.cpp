#include "GroundSpellPickup.h"
#include "Application.h"
#include "Modules/ObjectManager.h"
#include "../Player.h"
#include "Modules/Particles.h"
#include "Modules/Gui.h"
#include "Modules/ProgressTracker.h"
#include "../../UIElements/UItextbox.h"
#include "../../UIelementFunctions.h"
GroundSpellPickup::GroundSpellPickup()
{

}

void GroundSpellPickup::Destroy()
{
	//Engine->GetModule<Particles>().to_delete.push_back(p);
	Engine->GetModule<Particles>().RemoveParticleEmitter(p);
	Engine->GetModule<Particles>().RemoveParticleEmitter(q);

}

void GroundSpellPickup::Init()
{
	particles = Engine->GetModule<Textures>().Load_Texture("Assets/Sprites/particles.png");
	spell_books = Engine->GetModule<Textures>().Load_Texture("Assets/UI/books.png");

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
	sand_left.texture_name = particles;

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
	sand_right.texture_name = particles;

	groundspellbook = { 216,0,52,64 };

	p = Engine->GetModule<Particles>().AddParticleEmitter(&sand_left, collider->x, collider->y);
	q = Engine->GetModule<Particles>().AddParticleEmitter(&sand_right, collider->x, collider->y);

	if (Engine->GetModule<ProgressTracker>().BaseSaveSection->GetChild("SpellsUnlock") == nullptr)
	{
		Engine->GetModule<ProgressTracker>().BaseSaveSection->AddNewChild("SpellsUnlock");
	}
	if (Engine->GetModule<ProgressTracker>().BaseSaveSection->GetChild("LoreLogs") == nullptr)
	{
		Engine->GetModule<ProgressTracker>().BaseSaveSection->AddNewChild("LoreLogs");
	}
}

bool GroundSpellPickup::Loop(float dt)
{
	if (Engine->GetModule<ProgressTracker>().BaseSaveSection->GetChild("SpellsUnlock")->GetValue("Ground"))
	{
		Engine->GetModule<ObjectManager>().DeleteObject(this);
	}

	p->position_x = collider->x;
	p->position_y = collider->y;
	
	q->position_x = collider->x;
	q->position_y = collider->y;

	std::vector<collision*> collisions;
	Engine->GetModule<ObjectManager>().GetCollisions(collider, collisions);

	for (std::vector<collision*>::iterator it = collisions.begin(); it != collisions.end(); it++)
	{
		if ((*it)->object != this)
		{
			if ((*it)->object->IsSameTypeAs<Player>())
			{
				((Player*)((*it)->object))->unlock_spell(GROUND);
				Engine->GetModule<ProgressTracker>().BaseSaveSection->GetChild("SpellsUnlock")->SetValue("Ground", 1);
				Engine->GetModule<ObjectManager>().DeleteObject(this);
				UItextbox* textbox = new UItextbox("", "congratulations you unlocked ground!", TextBoxColor::GREY, 15, 4, 272, 420, 2, 0.2);
				textbox->AddPanelToTextBox("this castle is still alive, just broken");
				Engine->GetModule<UserInterface>().AddElement(textbox);
				//Engine->GetModule<Particles>().to_delete.push_back(p);
				Engine->GetModule<Particles>().AddParticleEmitter(&sand_left, collider->x, collider->y, 1500);
				Engine->GetModule<Particles>().AddParticleEmitter(&sand_right, collider->x, collider->y, 1500);
				Engine->GetModule<ProgressTracker>().BaseSaveSection->GetChild("LoreLogs")->SetValue("16", 1.0f);

			}
		}
	}

	return true;
}

bool GroundSpellPickup::Render()
{
	Engine->GetModule<::Render>().Blit(spell_books, collider->x, collider->y, groundspellbook, 10);
	return true;
}
