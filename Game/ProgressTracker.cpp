#include "ProgressTracker.h"
#include "Application.h"
#include "SceneController.h"
#include "Physics.h"
#include "Gui.h"
#include "Player.h"

ProgressTracker::ProgressTracker()
{
	name = "ProgressTracker";
}

bool ProgressTracker::Init()
{
	unlocked.push_back(false);
	unlocked.push_back(false);
	unlocked.push_back(false);
	unlocked.push_back(false);
	unlocked.push_back(false);

	return true;
}

bool ProgressTracker::Loop(float dt)
{
	if(start_new_game)
	{
		StartNewGame();
		start_new_game = false;
	}

	return true;
}

bool ProgressTracker::CleanUp()
{
	return true;
}

void ProgressTracker::SaveGame(const char * file)
{
}

void ProgressTracker::LoadGame(const char * file)
{
}

void ProgressTracker::SetPlayer(Player * p)
{
	pl = p;

	for (int i = 0; i < 5;++i)
	{
		if (unlocked[i] == true)
			pl->unlock_spell((spell_type)i);
	}

	//ADD SAME HP MANA AS WELL!!!!!!!
}

void ProgressTracker::AddPlayer()
{
}

void ProgressTracker::RespawnPlayer()
{
	//ADD BLACKSCREEN

}

void ProgressTracker::StartNewGame()
{
	App->scn->LoadMap("Assets/maps/map0_entrance.tmx");

	pl = (Player*)App->phy->AddObject(576, 300, 64, 64, PLAYER);
	App->gui->Add_GameUI((physobj*)pl);

	//App->phy->AddObject(1000, 300, 52, 64, FIRE_SPELL_PICKUP);

}
