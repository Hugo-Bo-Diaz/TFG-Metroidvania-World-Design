#include <stdlib.h>
#include <stdio.h>
#include <functional>
#include "Modules/Input.h"
#include "Modules/SceneController.h"
#include "Modules/Gui.h"
#include "Modules/Camera.h"
#include "SceneProcessing.h"
#include "UIelementFunctions.h"
#include "Application.h"
#include "UIElements/UISettingsMenu.h"
#include "UIElements/UIPauseMenu.h"
#include "UIElements/UISelectMenu.h"
#include "GameObjects/Portal.h"
#include "Modules/ProgressTracker.h"
#include "Utils/Logger.h"
#include "Utils/Utils.h"
#include "RXRect.h"

MetroidVaniaSceneProcessor* Instance = nullptr;


void MetroidVaniaSceneProcessor::SceneProcessingMainMenu()
{
	if (should_add_settings && settings == nullptr)
	{
		settings = new UISettingsMenu();
		//settings->parent = this;
		settings->x = 320;
		settings->y = 64;
		settings->x += 76;
		settings->y += 92;
		App->mAPI->GetModule<UserInterface>().AddElement(settings);
		settings->parent_type = SETTINGS_PARENT_MAIN_MENU;
	}

	if (should_start_game)
	{
		App->mAPI->GetModule<SceneController>().AssignGameLoopFunction(std::bind(&MetroidVaniaSceneProcessor::SceneProcessingInGame, &MetroidVaniaSceneProcessor::GetInstance()));
		App->mAPI->GetModule<SceneController>().AssignLoadFunction(std::bind(&MetroidVaniaSceneProcessor::SceneCreationInGame, &MetroidVaniaSceneProcessor::GetInstance()));
		App->mAPI->GetModule<SceneController>().LoadMap("Assets/maps/map0_entrance.tmx");

		Section* lSection = App->mAPI->GetModule<ProgressTracker>().GetBaseSaveSection()->GetChild("Stats");
		if (lSection == nullptr)
		{
			lSection = App->mAPI->GetModule<ProgressTracker>().GetBaseSaveSection()->AddNewChild("Stats");
		}

		lSection->SetValue("MaxHP",4);
		lSection->SetValue("CurrentHP",4);
		lSection->SetValue("CurrentFragmentsHP",0);
		lSection->SetValue("MaxMana",3);
		lSection->SetValue("CurrentMana",3);
		lSection->SetValue("CurrentFragmentsMana",0);
	}
	if (start_load_game)
	{
		if(App->mAPI->GetModule<ProgressTracker>().CanLoadGame("save_file.xml"))
			App->mAPI->GetModule<ProgressTracker>().LoadFile("save_file.xml");

		int lRoom = App->mAPI->GetModule<ProgressTracker>().GetBaseSaveSection()->GetChild("Checkpoint")->GetValue("last_checkpoint_id");

		//ON OLD MAP
		std::string map_to_change;

		for (std::vector<room*>::iterator it = rooms.begin(); it != rooms.end(); it++)
		{
			if ((*it)->id == lRoom)
			{
				map_to_change = (*it)->path;
				current_room_id = lRoom;
			}
		}
		//CHANGES ALL MAP
		App->mAPI->GetModule<SceneController>().LoadMap(map_to_change.c_str());

		App->mAPI->GetModule<SceneController>().AssignGameLoopFunction(std::bind(&MetroidVaniaSceneProcessor::SceneProcessingInGame, &MetroidVaniaSceneProcessor::GetInstance()));
		App->mAPI->GetModule<SceneController>().AssignLoadFunction(std::bind(&MetroidVaniaSceneProcessor::SceneCreationInGame, &MetroidVaniaSceneProcessor::GetInstance()));
	}

	//return true;
}

void MetroidVaniaSceneProcessor::SceneProcessingInGame()
{

	//if (App->inp->GetInput(BUTTON_4))
	//{
	//	App->trk->LoadGame("save.xml");
	//}

	if (App->mAPI->GetModule<Input>().GetInput(START) == BUTTON_DOWN && canopenmenu && !is_pause_menu_up && !is_select_menu_up)
	{
		UIPauseMenu* ret = new UIPauseMenu();

		ret->x = 320;
		ret->y = 64;
		
		App->mAPI->GetModule<UserInterface>().AddElement(ret);
		is_pause_menu_up = true;
	}


	if (App->mAPI->GetModule<Input>().GetInput(SELECT) == BUTTON_DOWN && canopenmenu && !is_select_menu_up && !is_pause_menu_up)
	{
		UISelectMenu* ret = new UISelectMenu();
		
		ret->x = 106;
		ret->y = 64;
		
		App->mAPI->GetModule<UserInterface>().AddElement(ret);
		is_pause_menu_up = true;

		//Engine->GetModule<SceneController>().AssignGameLoopFunction(std::bind(&MetroidVaniaSceneProcessor::SceneProcessingMainMenu, &MetroidVaniaSceneProcessor::GetInstance()));

	}

	if (App->mAPI->GetModule<Camera>().GetCoveragePercent() >= 100 && pl->respawn_player)
	{
		pl->nextpos->x = spawnpoint_x;
		pl->nextpos->y = spawnpoint_y;

		pl->speed_x = 0;
		pl->speed_y = 0;

		pl->respawn_player = false;
		pl->AddHealth(-1, 0);
	}


	if (should_add_settings && settings == nullptr)
	{
		settings = new UISettingsMenu();
		//settings->parent = this;
		settings->x = 320;
		settings->y = 64;
		settings->x += 76;
		settings->y += 92;
		settings->parent_type = SETTINGS_PARENT_PAUSE_MENU;
	}


	//CHECK IF PLAYER COMES IN CONTACT WITH PORTALS

	Portal* to_use = nullptr;
	int intersection_d;
	for (std::vector<GameObject*>::iterator it = portals.begin(); it != portals.end(); it++)
	{
		if(pl != nullptr && RXRectCollision(pl->nextpos,&(*it)->collider))
		{
			to_use = (Portal*)*it;
			if (to_use->horizontal)
			{
				intersection_d = pl->x - to_use->collider.x;
			}
			else
			{
				intersection_d = pl->y - to_use->collider.y;
			}
		}
	}
	if (to_use != nullptr)
	{
		UsePortal(to_use, intersection_d);
	}



	//return true;
}

void MetroidVaniaSceneProcessor::SceneCreationMainMenu()
{
	//gets called on loading save file from main menu
	if (inGameUI == nullptr)
	{
		inGameUI = new UIingameui(pl);
	}
	else
	{
		inGameUI->SetPlayer(pl);
	}
	//App->mAPI->GetModule<UserInterface>().AddElement(inGameUI);
	//set player
	int lposX = App->mAPI->GetModule<ProgressTracker>().GetBaseSaveSection()->GetChild("Checkpoint")->GetValue("last_checkpoint_x");
	int lposY = App->mAPI->GetModule<ProgressTracker>().GetBaseSaveSection()->GetChild("Checkpoint")->GetValue("last_checkpoint_y");

	int lMaxHP = App->mAPI->GetModule<ProgressTracker>().GetBaseSaveSection()->GetChild("Stats")->GetValue("MaxHP");
	int lCurrHP = App->mAPI->GetModule<ProgressTracker>().GetBaseSaveSection()->GetChild("Stats")->GetValue("CurrentHP");
	int lMaxMana = App->mAPI->GetModule<ProgressTracker>().GetBaseSaveSection()->GetChild("Stats")->GetValue("MaxMana");
	int lCurrMana = App->mAPI->GetModule<ProgressTracker>().GetBaseSaveSection()->GetChild("Stats")->GetValue("CurrentMana");

	pl = (Player*)App->mAPI->GetModule<SceneController>().AddObject(lposX, lposY, 64, 64, GetTypeIndex<Player>());

	pl->max_health = lMaxHP;
	pl->max_mana = lMaxMana;
	pl->health = lCurrHP;
	pl->mana = lCurrMana;

	inGameUI->SetPlayer(pl);

	//inGameUI->player = pl;
	//App->trk->SetPlayer(pl);
	pl->collider.w -= pl->separation * 2;
	pl->nextpos->w -= pl->separation * 2;
}

void MetroidVaniaSceneProcessor::SceneCreationInGame()
{
	//if (pl == nullptr)
	//{
	//	pl= (Player*)Engine->GetModule<SceneController>().AddObject(87, 200, 64, 64, GetTypeIndex<Player>());
	//}
	bool lFirstLoad = false;
	if (inGameUI == nullptr)
	{
		inGameUI = new UIingameui(pl);
		App->mAPI->GetModule<UserInterface>().AddElement(inGameUI);
		lFirstLoad = true;
	}
	else
	{
		inGameUI->SetPlayer(pl);
	}

	portals = *App->mAPI->GetModule<SceneController>().GetAllObjectsOfType(GetTypeIndex<Portal>());
	spawnpoints = *App->mAPI->GetModule<SceneController>().GetAllObjectsOfType(GetTypeIndex<SpawnPoint>());

	//ON NEW MAP
	newplayer_x = 87;
	newplayer_y = 200;

	spawnpoint_x = 87;
	spawnpoint_y = 200;

	portals = *App->mAPI->GetModule<SceneController>().GetAllObjectsOfType(GetTypeIndex<Portal>());
	spawnpoints = *App->mAPI->GetModule<SceneController>().GetAllObjectsOfType(GetTypeIndex<SpawnPoint>());

	for (std::vector<GameObject*>::iterator it = spawnpoints.begin(); it != spawnpoints.end(); it++)
	{
		SpawnPoint* s = (SpawnPoint*)(*it);
		if (s->id == spawn_point_id)
		{
			spawnpoint_x = s->collider.x;
			spawnpoint_y = s->collider.y;
		}
	}
	newplayer_x = spawnpoint_x;
	newplayer_y = spawnpoint_y;
	if (horizontal)
	{
		newplayer_x = spawnpoint_x + offset;
	}
	else
	{
		newplayer_y = spawnpoint_y + offset;
	}

	if (lFirstLoad && App->mAPI->GetModule<ProgressTracker>().GetBaseSaveSection()->GetChild("Checkpoint") != nullptr)
	{
		newplayer_x = App->mAPI->GetModule<ProgressTracker>().GetBaseSaveSection()->GetChild("Checkpoint")->GetValue("last_checkpoint_x");
		spawnpoint_x = App->mAPI->GetModule<ProgressTracker>().GetBaseSaveSection()->GetChild("Checkpoint")->GetValue("last_checkpoint_x");
		newplayer_y = App->mAPI->GetModule<ProgressTracker>().GetBaseSaveSection()->GetChild("Checkpoint")->GetValue("last_checkpoint_y");
		spawnpoint_y = App->mAPI->GetModule<ProgressTracker>().GetBaseSaveSection()->GetChild("Checkpoint")->GetValue("last_checkpoint_y");
	}

	int lMaxHP = App->mAPI->GetModule<ProgressTracker>().GetBaseSaveSection()->GetChild("Stats")->GetValue("MaxHP");
	int lCurrHP = App->mAPI->GetModule<ProgressTracker>().GetBaseSaveSection()->GetChild("Stats")->GetValue("CurrentHP");
	int lMaxMana = App->mAPI->GetModule<ProgressTracker>().GetBaseSaveSection()->GetChild("Stats")->GetValue("MaxMana");
	int lCurrMana = App->mAPI->GetModule<ProgressTracker>().GetBaseSaveSection()->GetChild("Stats")->GetValue("CurrentMana");

	//set player
	pl = (Player*)App->mAPI->GetModule<SceneController>().AddObject(newplayer_x, newplayer_y, 64, 64, GetTypeIndex<Player>());

	pl->current_spell = last_spell;
	inGameUI->SetPlayer(pl);

	pl->max_health = lMaxHP;
	pl->max_mana = lMaxMana;
	pl->health = lCurrHP;
	pl->mana = lCurrMana;

	//inGameUI->player = pl;
	//App->trk->SetPlayer(pl);

	//pl->current_spell = (spell_type)current_spell;
	pl->speed_y = prev_speed_y;
	pl->speed_x = prev_speed_x;
	pl->collider.w -= pl->separation * 2;
	pl->nextpos->w -= pl->separation * 2;
}

void MetroidVaniaSceneProcessor::LoadMapArray(const char* document)
{
	pugi::xml_document	document_file;
	pugi::xml_node completemap;
	pugi::xml_parse_result result = document_file.load_file(document);

	if (result == NULL)
	{
		std::string strerr = "couldn't find map ";
		strerr += document;
		Logger::Console_log(LogLevel::LOG_ERROR ,strerr.c_str());
		return;
	}

	//start!
	completemap = document_file.child("completemap");
	pugi::xml_node iterator;

	for (iterator = completemap.first_child(); iterator; iterator = iterator.next_sibling())
	{
		room* new_room = new room();
		new_room->path = iterator.attribute("path").as_string();
		new_room->id = iterator.attribute("id").as_int();

		rooms.push_back(new_room);
		/*
		pugi::xml_node tile_iterator;
		for (tile_iterator = iterator.first_child().first_child(); tile_iterator; tile_iterator = tile_iterator.next_sibling())
		{
			int x = tile_iterator.attribute("x").as_int();
			int y = tile_iterator.attribute("y").as_int();

			map_rooms[x][y] = new_room->id;
		}
		*/
	}

}

void MetroidVaniaSceneProcessor::UsePortal(Portal* p, int offset)
{
	//ON OLD MAP
	std::string map_to_change;

	for (std::vector<room*>::iterator it = rooms.begin(); it != rooms.end(); it++)
	{
		if ((*it)->id == p->id_destination_room)
		{
			map_to_change = (*it)->path;
			current_room_id = p->id_destination_room;
		}
	}

	//int current_spell = App->trk->pl->current_spell;
	prev_speed_x = pl->speed_x;
	prev_speed_y = pl->speed_y;
	last_spell = pl->current_spell;

	//Engine->GetModule<::Render>().ClearParticles();

	int point_id = p->id_destination_point;
	spawn_point_id = point_id;
	this->offset = offset;
	horizontal = p->horizontal;
	//CHANGES ALL MAP
	inGameUI->SetPlayer(nullptr);

	App->mAPI->GetModule<SceneController>().LoadMap(map_to_change.c_str());

	//ON NEW MAP
	//int newplayer_x = 0;
	//int newplayer_y = 0;

	//int spawnpoint_x = 0;
	//int spawnpoint_y = 0;

	//portals = *App->phy->GetAllObjectsOfType(GetTypeIndex<Portal>());
	//spawnpoints = *App->phy->GetAllObjectsOfType(GetTypeIndex<SpawnPoint>());

	//for (std::vector<GameObject*>::iterator it = spawnpoints.begin(); it != spawnpoints.end(); it++)
	//{
	//	SpawnPoint* s = (SpawnPoint*)(*it);
	//	if (s->id == point_id)
	//	{
	//		spawnpoint_x = s->collider.x;
	//		spawnpoint_y = s->collider.y;
	//	}
	//}
	//newplayer_x = spawnpoint_x;
	//newplayer_y = spawnpoint_y;
	//if (horizontal)
	//{
	//	newplayer_x = spawnpoint_x + offset;
	//}
	//else
	//{
	//	newplayer_y = spawnpoint_y + offset;
	//}

	////set player
	////pl = (Player*)Engine->GetModule<SceneController>().AddObject(newplayer_x, newplayer_y, 64, 64, GetTypeIndex<Player>());

	//inGameUI->SetPlayer(pl);

	////inGameUI->player = pl;
	////App->trk->SetPlayer(pl);

	////pl->current_spell = (spell_type)current_spell;
	////pl->speed_y = speed_y;
	////pl->speed_x = speed_x;
	//pl->collider.w -= pl->separation * 2;
	//pl->nextpos->w -= pl->separation * 2;


	//TODO
}

void MetroidVaniaSceneProcessor::GoToLastCheckPoint()
{
	//Player* pl = App->trk->pl;

	//App->trk->player_hp = App->trk->max_player_hp;
	//App->trk->player_mana = App->trk->max_player_mana;

	//std::string map_to_change;

	////if (current_room_id != App->trk->last_checkpoint_id)
	//{
	//	//int current_spell = pl->current_spell;

	//	for (std::vector<room*>::iterator it = rooms.begin(); it != rooms.end(); it++)
	//	{
	//		if ((*it)->id == App->trk->last_checkpoint_id)
	//		{
	//			map_to_change = (*it)->path;
	//			current_room_id = App->trk->last_checkpoint_id;
	//		}
	//	}
	//	ChangeMap(map_to_change.c_str());

		//pl = (Player*)Engine->GetModule<SceneController>().AddObject(0, 0, 64, 64, "Player");
		//App->gui->Add_GameUI((GameObject*)pl);

		//pl->current_spell = (spell_type)current_spell;

	}
	//pl->speed_x = 0;
	//pl->speed_y = 0;
	//pl->nextpos->x = App->trk->last_checkpoint_x;
	//pl->nextpos->y = App->trk->last_checkpoint_y;

	//pl->collider.w -= pl->separation * 2;
	//pl->nextpos->w -= pl->separation * 2;
	//App->trk->SetPlayer(pl);


void MetroidVaniaSceneProcessor::GoToLoadedScene()
{
	std::string map_to_change;

	//if (current_room_id != App->trk->last_checkpoint_id)
	{
		//int current_spell = NONE_UNLOCKED;

		//for (std::vector<room*>::iterator it = rooms.begin(); it != rooms.end(); it++)
		//{
		//	if ((*it)->id == App->trk->last_checkpoint_id)
		//	{
		//		map_to_change = (*it)->path;
		//		current_room_id = App->trk->last_checkpoint_id;
		//	}
		//}
		//ChangeMap(map_to_change.c_str());


		//for (int i = 0; i < 5; ++i)
		//{
		//	if (App->trk->unlocked[i] == true)
		//	{
		//		//current_spell = i;
		//		break;
		//	}
		//}

		//pl = (Player*)Engine->GetModule<SceneController>().AddObject(0, 0, 64, 64, "Player");
		//App->gui->Add_GameUI((GameObject*)pl);

		//pl->current_spell = (spell_type)current_spell;

	}
	//pl->speed_x = 0;
	//pl->speed_y = 0;
	//pl->nextpos->x = App->trk->last_checkpoint_x;
	//pl->nextpos->y = App->trk->last_checkpoint_y;
	//App->trk->SetPlayer(pl);
}



MetroidVaniaSceneProcessor& MetroidVaniaSceneProcessor::GetInstance()
{
	if (Instance == nullptr)
		Instance = new MetroidVaniaSceneProcessor();

	return *Instance;
}

void MetroidVaniaSceneProcessor::EngineInitialization()
{
	LoadMapArray("Assets/maps/map_array.xml");


	pugi::xml_document	lore_file;
	lore_file.load_file("lore.xml");
	pugi::xml_node logs_file_node;
	logs_file_node = lore_file.child("lore");

	pugi::xml_node iterator;

	for (iterator = logs_file_node.first_child(); iterator; iterator = iterator.next_sibling())
	{
		LogEntry* log = new LogEntry();

		log->title = iterator.attribute("title").as_string();
		log->text = iterator.attribute("content").as_string();

		logs.push_back(log);
		//active_logs.push_back(log);
	}

}
