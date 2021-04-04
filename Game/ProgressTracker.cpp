#include "ProgressTracker.h"
#include "Application.h"
#include "SceneController.h"
#include "Physics.h"
#include "Gui.h"
#include "Player.h"
#include "Camera.h"

ProgressTracker::ProgressTracker()
{
	name = "ProgressTracker";

	for (int i = 0; i < MAX_EXPANSIONS; ++i)
	{
		itemspickedup[i] = false;
	}
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

	if (charges_hp >= charges_per_hp)
	{
		player_hp += 1;
		max_player_hp += 1;
		
		pl->max_health += 1;
		pl->health += 1;
		
		charges_hp = 0;
	}

	if (charges_mana >= charges_per_mana)
	{
		player_mana += 1;
		max_player_mana += 1;
		
		pl->max_mana += 1;
		pl->mana += 1;

		charges_mana = 0;
	}

	if (App->cam->GetCoveragePercent()>=100 && respawn_player)
	{
		pl->nextpos->x = App->scn->spawnpoint_x;
		pl->nextpos->y = App->scn->spawnpoint_y;

		pl->speed_x = 0;
		pl->speed_y = 0;

		respawn_player = false;
		pl->AddHealth(-1,0);

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

	pl->health = player_hp;
	pl->max_health = max_player_hp;

	pl->mana = player_mana;
	pl->max_mana = max_player_mana;
}

void ProgressTracker::AddPlayer()
{
}

void ProgressTracker::RespawnPlayer()
{
	//ADD BLACKSCREEN
	if (!respawn_player && App->cam->GetCoveragePercent()<100)
	{
		App->cam->CoverScreen(500, 200, 0, 0, 0);
		respawn_player = true;
		App->cam->CameraShake(15,200);
	}

}

void ProgressTracker::StartNewGame()
{
	App->scn->LoadMap("Assets/maps/map0_entrance.tmx");

	pl = (Player*)App->phy->AddObject(576, 300, 64, 64, PLAYER);
	App->gui->Add_GameUI((physobj*)pl);

	//App->phy->AddObject(1000, 300, 52, 64, FIRE_SPELL_PICKUP);

}

void ProgressTracker::AddPickupToList(int id)
{
	if (id < MAX_EXPANSIONS)
	{
		itemspickedup[id] = true;
	}
}

bool ProgressTracker::HasBeenFound(int id)
{
	if (id < MAX_EXPANSIONS)
	{
		return itemspickedup[id];
	}

	return true;
}
