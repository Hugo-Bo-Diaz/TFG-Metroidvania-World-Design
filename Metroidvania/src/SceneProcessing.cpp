#include <stdlib.h>
#include <stdio.h>
#include <functional>
#include "Modules/Input.h"
#include "Modules/SceneController.h"
#include "Modules/Gui.h"
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

	//return true;
}

void MetroidVaniaSceneProcessor::SceneProcessingInGame()
{
	if (App->mAPI->GetModule<Input>().GetInput(BUTTON_3))
	{
		if (pl->IsSameTypeAs<UIPauseMenu>())
		{
			Logger::Console_log(LogLevel::LOG_ERROR, "WTF");
		}

		if (pl->IsSameTypeAs<GameObject>())
		{
			Logger::Console_log(LogLevel::LOG_WARN, "Inheritance yay");
		}
		if (pl->IsSameTypeAs<Portal>())
		{
			Logger::Console_log(LogLevel::LOG_INFO, "WTF portal lol");
		}
	}

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
}

void MetroidVaniaSceneProcessor::SceneCreationInGame()
{
	//if (pl == nullptr)
	//{
	//	pl= (Player*)Engine->GetModule<ObjectManager>().AddObject(87, 200, 64, 64, GetTypeIndex<Player>());
	//}

	if (inGameUI == nullptr)
	{
		inGameUI = new UIingameui(pl);
	}
	else
	{
		inGameUI->SetPlayer(pl);
	}
	App->mAPI->GetModule<UserInterface>().AddElement(inGameUI);

	portals = *App->mAPI->GetModule<ObjectManager>().GetAllObjectsOfType(GetTypeIndex<Portal>());
	spawnpoints = *App->mAPI->GetModule<ObjectManager>().GetAllObjectsOfType(GetTypeIndex<SpawnPoint>());





	//ON NEW MAP
	int newplayer_x = 87;
	int newplayer_y = 200;

	int spawnpoint_x = 87;
	int spawnpoint_y = 200;

	portals = *App->mAPI->GetModule<ObjectManager>().GetAllObjectsOfType(GetTypeIndex<Portal>());
	spawnpoints = *App->mAPI->GetModule<ObjectManager>().GetAllObjectsOfType(GetTypeIndex<SpawnPoint>());

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

	//set player
	pl = (Player*)App->mAPI->GetModule<ObjectManager>().AddObject(newplayer_x, newplayer_y, 64, 64, GetTypeIndex<Player>());

	inGameUI->SetPlayer(pl);

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

	//Engine->GetModule<Particles>().ClearParticles();

	int point_id = p->id_destination_point;
	spawn_point_id = point_id;
	this->offset = offset;
	bool horizontal = p->horizontal;
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
	////pl = (Player*)Engine->GetModule<ObjectManager>().AddObject(newplayer_x, newplayer_y, 64, 64, GetTypeIndex<Player>());

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

		//pl = (Player*)Engine->GetModule<ObjectManager>().AddObject(0, 0, 64, 64, "Player");
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

		//pl = (Player*)Engine->GetModule<ObjectManager>().AddObject(0, 0, 64, 64, "Player");
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
