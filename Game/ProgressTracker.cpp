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

void ProgressTracker::LoadLogs(const char * logs_file)
{
	if (!CanLoadGame(logs_file))
		return;

	pugi::xml_document	lore_file;
	lore_file.load_file(logs_file);
	pugi::xml_node logs_file_node;
	logs_file_node = lore_file.child("lore");

	pugi::xml_node iterator;

	for (iterator = logs_file_node.first_child(); iterator; iterator = iterator.next_sibling())
	{
		LoreLog* log = new LoreLog();

		log->id = iterator.attribute("id").as_int();
		log->title = iterator.attribute("title").as_string();
		log->text = iterator.attribute("content").as_string();

		lore_logs.push_back(log);
		//active_logs.push_back(log);
	}

}

void ProgressTracker::UnlockLog(int id)
{
	if (id < lore_logs.size())
	{
		active_logs.push_back(lore_logs[id]);

		//order lore
		OrderActiveLogs();
	}

}

void ProgressTracker::OrderActiveLogs()
{

	bool swapped = true;
	while (swapped)
	{
		swapped = false;
		int i = 0;
		int size = active_logs.size() - 1;
		for (i = 0; i < size; ++i)
		{
			if (active_logs[i]->id > active_logs[i + 1]->id)
			{
				swapped = true;
				//swap em;

				//save him into the temp
				LoreLog* temp = new LoreLog();
				*temp = *active_logs[i];

				//swap em
				*active_logs[i] = *active_logs[i + 1];

				//normalize everything
				*active_logs[i + 1] = *temp;
				delete temp;

			}
		}
	}
}

bool ProgressTracker::HasLogBeenUnlocked(int id)
{
	bool ret = false;

	for (int i = 0; i < active_logs.size(); ++i)
	{
		if (active_logs[i]->id == id)
			ret = true;
	}

	return ret;
}

bool ProgressTracker::LoadConfig(pugi::xml_node & node)
{
	pugi::xml_node lore_location = node.child("lore_file");

	LoadLogs(lore_location.attribute("path").as_string());

	return true;
}

bool ProgressTracker::CreateConfig(pugi::xml_node & node)
{
	pugi::xml_node lore_location = node.append_child("lore_file");

	lore_location.append_attribute("path") = "lore.xml";
	LoadLogs("lore.xml");

	return true;
}

bool ProgressTracker::Loop(float dt)
{
	if(start_new_game)
	{
		StartNewGame();
		start_new_game = false;
	}

	if (start_load_game)
	{
		//StartLoadGame();
		LoadGame("save_file.xml");
		start_load_game = false;
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

	if (App->cam->GetCoveragePercent() >= 100 && send_player_to_checkpoint)
	{

		pl->speed_x = 0;
		pl->speed_y = 0;

		App->scn->GoToLastCheckPoint();

		send_player_to_checkpoint = false;
	}

	if (send_player_to_load&&App->cam->GetCoveragePercent() >= 100)
	{

		App->scn->GoToLoadedScene();

		send_player_to_load = false;
	}

	if (go_to_main_menu&&App->cam->GetCoveragePercent() >= 100)
	{
		App->scn->GoToMainMenu();
		go_to_main_menu = false;
	}

	if (pl != nullptr && pl->health <= 0)
	{
		SetPlayerToLastCheckPoint();
	}

	return true;
}

bool ProgressTracker::CleanUp()
{
	return true;
}

void ProgressTracker::SaveGame(const char * file)
{
	bool create_file = false;
	pugi::xml_document	config_file;
	pugi::xml_node save_file_node;

	save_file_node = config_file.append_child("save_state");

	pugi::xml_node health_node = save_file_node.append_child("health");
	health_node.append_attribute("current") = player_hp;
	health_node.append_attribute("max") = max_player_hp;

	pugi::xml_node mana_node = save_file_node.append_child("mana");
	mana_node.append_attribute("current") = player_mana;
	mana_node.append_attribute("max") = max_player_mana;

	pugi::xml_node spells = save_file_node.append_child("spells");
	
	for (int i = 0; i < 5; ++i)
	{
		pugi::xml_node spell_node = spells.append_child("spell");
		spell_node.append_attribute("unlocked")=unlocked[i];
	}
	
	pugi::xml_node checkpoints = save_file_node.append_child("checkpoint");
	
	checkpoints.append_attribute("room_id") = last_checkpoint_id;
	checkpoints.append_attribute("x") = last_checkpoint_x;
	checkpoints.append_attribute("y") = last_checkpoint_y;

	pugi::xml_node collectables = save_file_node.append_child("collectables");

	for (int i = 0; i < MAX_EXPANSIONS; ++i)
	{
		pugi::xml_node expansion = collectables.append_child("collectable");
		expansion.append_attribute("picked_up") = itemspickedup[i];
	}

	pugi::xml_node lore_node = save_file_node.append_child("lore");

	for (int i = 0; i < active_logs.size(); ++i)
	{
		pugi::xml_node expansion = collectables.append_child("entry");
		expansion.append_attribute("unlocked") = active_logs[i]->id;
	}

	config_file.save_file(file);
	
}

void ProgressTracker::LoadGame(const char * file)
{
	if (!CanLoadGame(file))
		return;
	
	pugi::xml_document	save_file;
	save_file.load_file(file);
	pugi::xml_node save_file_node;
	save_file_node = save_file.child("save_state");

	pugi::xml_node health_node = save_file_node.child("health");
	float newplayer_hp = health_node.attribute("current").as_float();
	max_player_hp = health_node.attribute("max").as_float();

	pugi::xml_node mana_node = save_file_node.child("mana");
	float newplayer_mana = mana_node.attribute("current").as_float();
	max_player_mana = mana_node.attribute("max").as_float();

	pugi::xml_node spells_node = save_file_node.child("spells");
	pugi::xml_node iterator;

	int i = 0;
	bool has_one_spell = false;
	for (iterator = spells_node.first_child(); iterator; iterator = iterator.next_sibling())
	{
		unlocked[i]= iterator.attribute("unlocked").as_bool(false);
		if(unlocked[i]==true)
		{
			has_one_spell = true;
		}
		++i;
	}
	
	pugi::xml_node checkpoint_node = save_file_node.child("checkpoint");

	last_checkpoint_id = checkpoint_node.attribute("room_id").as_int();
	last_checkpoint_x = checkpoint_node.attribute("x").as_int();
	last_checkpoint_y = checkpoint_node.attribute("y").as_int();

	pugi::xml_node collectables_node = save_file_node.child("collectables");

	i = 0;
	for (iterator = collectables_node.first_child(); iterator; iterator = iterator.next_sibling())
	{
		itemspickedup[i] = iterator.attribute("picked_up").as_bool(true);
		++i;
	}

	pugi::xml_node lore_node = save_file_node.child("lore");

	i = 0;
	for (iterator = lore_node.first_child(); iterator; iterator = iterator.next_sibling())
	{
		UnlockLog(iterator.attribute("unlocked").as_int());
		++i;
	}

	if (!send_player_to_load )
	{
		App->cam->CoverScreen(500, 200, 0, 0, 0);
		send_player_to_load = true;
	}

	player_hp = newplayer_hp;
	player_mana = newplayer_mana;
}

bool ProgressTracker::CanLoadGame(const char * file)
{
	pugi::xml_document	file_to_check;
	pugi::xml_parse_result result = file_to_check.load_file(file);

	if (result == NULL)
	{
		return false;
	}
	else
	{
		return true;
	}

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

void ProgressTracker::SetPlayerToLastCheckPoint()
{
	if (!send_player_to_checkpoint && App->cam->GetCoveragePercent()<100)
	{
		App->cam->CoverScreen(500, 200, 100, 0, 0);
		send_player_to_checkpoint = true;

		App->cam->CameraShake(15, 200);
	}
}

void ProgressTracker::SetCheckPoint(int x, int y, int room_id)
{
	if (!send_player_to_checkpoint)
	{
		last_checkpoint_x = x;
		last_checkpoint_y = y;
		last_checkpoint_id = room_id;
	}
}

void ProgressTracker::StartNewGame()
{
	App->scn->LoadMap("Assets/maps/map0_entrance.tmx");

	App->phy->AddObject(0, 0, 10, 10, FIRST_DIALOGUE);

	pl = (Player*)App->phy->AddObject(576, 300, 64, 64, PLAYER);
	App->gui->Add_GameUI((physobj*)pl);

	//App->phy->AddObject(1000, 300, 52, 64, FIRE_SPELL_PICKUP);

}

void ProgressTracker::StartLoadGame()
{
	LoadGame("save_file.xml");
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

