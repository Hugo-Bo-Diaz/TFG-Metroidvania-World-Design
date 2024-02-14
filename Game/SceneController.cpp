#include "Application.h"

#include "SceneController.h"
#include "Textures.h"
#include "InGameScene.h"
#include "MainMenuScene.h"
#include "Render.h"
#include "Physics.h"
#include "Audio.h"
#include "Camera.h"
#include "ProgressTracker.h"
#include "Particles.h"
#include "Window.h"
#include "Audio.h"
#include "Logger.h"

//#include "Player.h"
//#include "MaxHealthPickup.h"
//#include "MaxManaPickup.h"
//#include "GroundedElemental.h"
//#include "FlyingElemental.h"
//#include "ArmorTrap.h"
//#include "ClingingCreature.h"
//
//#include "CheckPoint.h"
//#include "TextBoxObject.h"
//#include "DemoEndObject.h"
////#include "HazardLava.h"
//#include "HazardsRockBlock.h"

#include "Gui.h"
#include "Text.h"

SceneController::SceneController()
{
	name = "SceneController";
	active = true;
}

bool SceneController::Init()
{
	LoadAssetsMap("Assets/maps/AssetStorage.tmx");

	return true;
}

bool SceneController::Start()
{
	//load backgrounds and tilesets

	//App->gui->AddMainMenu();
	//App->gui->AddSettingsMenu();
	//App->gui->AddPauseMenu();
	
	//testing code starts
	//LoadMap("Assets/maps/testing_grounds.tmx");
	LoadMap("Assets/maps/map0_entrance.tmx");

	//Player* pl= (Player*)App->phy->AddObject(87, 1000, 64, 64, "Player");
	//App->gui->Add_GameUI((GameObject*)pl);

	//pl->collider->w -= pl->separation * 2;
	//pl->nextpos->w -= pl->separation * 2;
	//testing code ends

	//App->trk->pl = pl;
	//song_try = App->aud->LoadMusic("Assets/Music/wtf.ogg","track1");

	return true;//CHANGE PLS
}

bool SceneController::Loop(float dt)
{
	bool ret = true;

	//App->aud->PlayMusic(song_try,1000);

//	RenderTiles();
	RenderBackground();

	if (SceneFunction != nullptr)
	{
		SceneFunction();
	}

	return ret;
}

bool SceneController::CleanUp()
{
	bool ret = true;
	CleanMap();
	App->phy->Clearphysics();
	//App->gui->Clearelements();
	return ret;
}

bool SceneController::LoadAssetsMap(const char * map)
{

	pugi::xml_document	map_file;
	pugi::xml_node map_node;
	pugi::xml_parse_result result = map_file.load_file(map);

	if (result == NULL)
	{
		std::string errstr = "couldn't find map ";
		errstr += map;
		Logger::Console_log(LogLevel::LOG_ERROR,errstr.c_str());
		return false;
	}

	//start!
	map_node = map_file.child("map");
	pugi::xml_node iterator;
	for (iterator = map_node.first_child(); iterator; iterator = iterator.next_sibling())
	{
		std::string iterator_name = iterator.name();
		if(iterator_name == "tileset")
		{
			LoadTilesets(iterator);
		}
		else if (iterator_name == "imagelayer")
		{
			LoadBackgroundImage(iterator);
		}
	}

	return true;
}

bool SceneController::LoadTilesets(pugi::xml_node & node)
{
	tileset* set = new tileset();

	set->firstgid = node.attribute("firstgid").as_int();
	set->tile_height = node.attribute("tileheight").as_int();
	set->tile_width = node.attribute("tilewidth").as_int();
	set->columns = node.attribute("columns").as_int();
	set->total_tiles = node.attribute("tilecount").as_int();

	pugi::xml_node imagenode = node.child("image");
	std::string base_folder = "Assets/maps/";
	base_folder += imagenode.attribute("source").as_string();

	//load this texture
	set->texture = App->tex->Load_Texture_Scene(base_folder.c_str());

	tilesets.push_back(set);

	return true;
}

bool SceneController::LoadBackgroundImage(pugi::xml_node & node)
{
	background_texture* back = new background_texture();

	pugi::xml_node iterator;
	pugi::xml_node properties_node = node.child("properties");
	for (iterator = properties_node.first_child(); iterator; iterator = iterator.next_sibling())
	{
		std::string temp = iterator.attribute("name").as_string();
		if (temp == "depth")
		{
			back->depth = iterator.attribute("value").as_int(20);
		}
		else if (temp == "parallax_x")
		{
			back->parallax_x = iterator.attribute("value").as_float(1);
		}
		else if (temp == "parallax_y")
		{
			back->parallax_y = iterator.attribute("value").as_float(1);
		}
		else if (temp == "repeat_y")
		{
			back->repeat_y = iterator.attribute("value").as_bool(true);
		}
	}

	/*
	back->parallax_x = node.child("properties").child("property").attribute("parallax_x").as_float(1);
	back->parallax_y = node.child("properties").child("property").attribute("parallax_y").as_float(1);
	back->depth = node.child("properties").child("property").attribute("depth").as_int(20);
	back->repeat_y = node.child("properties").child("property").attribute("repeat_y").as_bool(true);
	*/
	pugi::xml_node imagenode = node.child("image");
	back->path = imagenode.attribute("source").as_string();

	std::string base_folder = "Assets/maps/";
	base_folder += back->path;

	//load this texture
	back->texture = App->tex->Load_Texture_Scene(base_folder.c_str());

	backgrounds.push_back(back);

	return true;
}

bool SceneController::LoadMap(const char* filename)
{
	pugi::xml_document	map_file;
	pugi::xml_node map_node;
	pugi::xml_parse_result result = map_file.load_file(filename);

	if (result == NULL)
	{
		std::string errstr = "couldn't find map ";
		errstr += filename;
		Logger::Console_log(LogLevel::LOG_ERROR, errstr.c_str());
		return false;
	}

	//start!
	map_node = map_file.child("map");
	pugi::xml_node iterator;

	pugi::xml_node layer_node = map_node.first_child();
	for (iterator = layer_node; iterator; iterator = iterator.next_sibling())
	{
		std::string iterator_name = iterator.name();
		if(iterator_name == "imagelayer")
		{
			LoadBackground(iterator);
		}
		if (iterator_name == "objectgroup")
		{
			std::string tmp = iterator.attribute("name").as_string();
			if(tmp == "walls")
			{
				LoadWalls(iterator);
			}
			if (tmp == "objects")
			{
				LoadObjects(iterator);
			}
			if (tmp == "doors")
			{
				LoadObjects(iterator);

				//LoadPortals(iterator);
			}
			if (tmp == "spawnpoints")
			{
				LoadObjects(iterator);

				//LoadSpawnPoints(iterator);
			}
			if (tmp == "checkpoints")
			{

				//LoadCheckPoints(iterator);
			}
		}
		if (iterator_name == "layer")
		{
			LoadTiles(iterator);
		}
		if (iterator_name == "properties")
		{
			LoadMapProperties(iterator);
		}
	}

	if (LoadFunction != nullptr)
	{
		LoadFunction();
	}

	return true;
}

void SceneController::LoadMapProperties(pugi::xml_node & node)
{
	pugi::xml_node iterator;
	pugi::xml_node property_node = node.first_child();
	for (iterator = property_node; iterator; iterator = iterator.next_sibling())
	{
		std::string name = iterator.attribute("name").as_string();
		if (name == "music")
		{
			//App->aud->PlayMusic(iterator.attribute("value").as_int(1),500);
		}
	}
}


bool SceneController::LoadBackground(pugi::xml_node& imagelayer_node)
{
	pugi::xml_node image_node = imagelayer_node.first_child();
	
	//find in map properties the active backgrounds id
	std::string temp = image_node.attribute("source").as_string();

	for (std::vector<background_texture*>::iterator it = backgrounds.begin(); it != backgrounds.end(); it++)
	{
		if (temp == (*it)->path)
		{
			active_backgrounds.push_back(*it);
		}
	}
	//backgrounds.push_back(imagelayer_node.attribute("name").as_string());
	//App->tex->Load_Texture(image_location.c_str(),imagelayer_node.attribute("name").as_string());
	
	return true;
}

bool SceneController::LoadWalls(pugi::xml_node& objectgroup_node)
{
	pugi::xml_node object_iterator;
	for (object_iterator = objectgroup_node.child("object"); object_iterator; object_iterator = object_iterator.next_sibling())
	{
		SDL_Rect newwall;
		newwall.x = object_iterator.attribute("x").as_int();
		newwall.y = object_iterator.attribute("y").as_int();
		newwall.w = object_iterator.attribute("width").as_int();
		newwall.h = object_iterator.attribute("height").as_int();
		App->phy->AddWall(newwall);
	}

	return true;
}

bool SceneController::LoadObjects(pugi::xml_node& objectgroup_node)
{
	pugi::xml_node object_iterator;
	for (object_iterator = objectgroup_node.child("object"); object_iterator; object_iterator = object_iterator.next_sibling())
	{
	//	int newpos;
		int x = object_iterator.attribute("x").as_int();
		int y = object_iterator.attribute("y").as_int();// -object_iterator.attribute("height").as_int();//tile height inside tiled
		int w = 0;
		int h = 0;
		std::string type = object_iterator.attribute("type").as_string();
		objectId lType = App->phy->typeCallBack(type.c_str());
		//if(type== "SpellUnlockFire")
		//{
		//	w = 52;
		//	h = 64;
		//}
		//if (type == "SpellUnlockGround")
		//{
		//	w = 52;
		//	h = 64;
		//}
		//else if (type == "HealthChargePickup")
		//{
		//	w = 48;
		//	h = 48;
		//}
		//else if (type == "ManaChargePickup")
		//{
		//	w = 48;
		//	h = 48;
		//}
		//else if (type == "HazardLava")
		//{
		//	w = object_iterator.attribute("width").as_int();
		//	h = object_iterator.attribute("height").as_int();
		//}
		//else if (type == "HazardLavaWaterfall")
		//{
		//	w = object_iterator.attribute("width").as_int();
		//	h = object_iterator.attribute("height").as_int();
		//}
		//else if (type == "HazardSpikes")
		//{
		//	w = object_iterator.attribute("width").as_int();
		//	h = object_iterator.attribute("height").as_int();
		//}
		//else if (type == "HazardRockBlock")
		//{
		//	w = object_iterator.attribute("width").as_int();
		//	h = object_iterator.attribute("height").as_int();
		//}
		//else if (type == "HazardsCloudTrampoline")
		//{
		//	w = object_iterator.attribute("width").as_int();
		//	h = object_iterator.attribute("height").as_int();
		//}
		//else if (type == "EnemyGroundedElemental")
		//{
		//	w = 48;
		//	h = 48;
		//}
		//else if (type == "EnemyFlyingElemental")
		//{
		//	w = 56;
		//	h = 56;
		//}
		//else if (type == "EnemyCoalJumper")
		//{
		//	w = 64;
		//	h = 64;
		//}
		//else if (type == "EnemyArmorTrap" )
		//{
		//	w = 48;
		//	h = 72;
		//}
		//else if (type == "EnemyShieldMonster")
		//{
		//	w = 112;
		//	h = 140;
		//}
		//else if (type == "EnemyClingCreature")
		//{
		//	w = 48;
		//	h = 48;
		//	//y += 48;
		//}
		//else if (type == "EnemyFlyingAxe")
		//{
		//	w = 64;
		//	h = 64;
		//}
		//else if (type == "EnemyFlyingShield")
		//{
		//	w = 48;
		//	h = 48;
		//}
		//else if (type == "EnemyCloudMelee")
		//{
		//	w = 64;
		//	h = 48;
		//}
		//else if (type == "EnemyCloudSummoner")
		//{
		//	w = 64;
		//	h = 48;
		//}
		//else if (type == "TextBoxObject")
		//{
		//	w = object_iterator.attribute("width").as_int();
		//	h = object_iterator.attribute("height").as_int();
		//	y+=object_iterator.attribute("height").as_int();

		//}
		//else if (type == "DemoEndObject")
		//{
		//	w = object_iterator.attribute("width").as_int();
		//	h = object_iterator.attribute("height").as_int();
		//	y += object_iterator.attribute("height").as_int();

		//}
		GameObject*ret = nullptr;

		w = object_iterator.attribute("width").as_int();
		h = object_iterator.attribute("height").as_int();

		std::map<std::string, float> lProperties;

		pugi::xml_node properties_node = object_iterator.child("properties");
		pugi::xml_node iterator;

		for (iterator = properties_node.first_child(); iterator; iterator = iterator.next_sibling())
		{
			std::string name = iterator.attribute("name").as_string();
			std::string type = iterator.attribute("type").as_string();
			float value = 0;
			if (strcmp(type.c_str(), "bool") == 0)
			{
				value = iterator.attribute("value").as_bool();
			}
			else if (strcmp(type.c_str(), "float") == 0)
			{
				value = iterator.attribute("value").as_float();
			}
			else if (strcmp(type.c_str(), "int") == 0)
			{
				value = iterator.attribute("value").as_int();
			}

			lProperties.insert(std::make_pair(name, value));
		}

		ret = App->phy->AddObject(x, y, w, h, lType,&lProperties);

		//if (ret != nullptr)
		//{
		//	if (t == MAX_HEALTH_PICKUP)
		//	{

		//		pugi::xml_node properties_node = object_iterator.child("properties");
		//		pugi::xml_node iterator;

		//		for (iterator = properties_node.first_child(); iterator; iterator = iterator.next_sibling())
		//		{
		//			std::string temp = iterator.attribute("name").as_string();
		//			if (temp == "id")
		//			{
		//				((MaxHealthPickup*)ret)->pickup_id = iterator.attribute("value").as_int();
		//			}
		//			if (temp == "text")
		//			{
		//				((MaxHealthPickup*)ret)->text = iterator.attribute("value").as_string("");
		//			}
		//			if (temp == "lore")
		//			{
		//				((MaxHealthPickup*)ret)->lore_unlock = iterator.attribute("value").as_int(-1);
		//			}
		//		}
		//	}
		//	else if (t == MAX_MANA_PICKUP)
		//	{

		//		pugi::xml_node properties_node = object_iterator.child("properties");
		//		pugi::xml_node iterator;

		//		for (iterator = properties_node.first_child(); iterator; iterator = iterator.next_sibling())
		//		{
		//			std::string temp = iterator.attribute("name").as_string();
		//			if (temp == "id")
		//			{
		//				((MaxManaPickup*)ret)->pickup_id = iterator.attribute("value").as_int();
		//			}
		//			if (temp == "text")
		//			{
		//				((MaxManaPickup*)ret)->text = iterator.attribute("value").as_string("");
		//			}
		//			if (temp == "lore")
		//			{
		//				((MaxHealthPickup*)ret)->lore_unlock = iterator.attribute("value").as_int(-1);
		//			}
		//		}
		//	}
			//else if (t == GROUNDED_ELEMENTAL)
			//{

			//	pugi::xml_node properties_node = object_iterator.child("properties");
			//	pugi::xml_node iterator;

			//	for (iterator = properties_node.first_child(); iterator; iterator = iterator.next_sibling())
			//	{
			//		std::string temp = iterator.attribute("name").as_string();
			//		if (temp == "color")
			//		{
			//			((GroundedElemental*)ret)->SetAnimations((GroundedElementalColor)iterator.attribute("value").as_int());
			//		}
			//	}
			//}

			//else if (t == FLYING_ELEMENTAL)
			//{

			//	pugi::xml_node properties_node = object_iterator.child("properties");
			//	pugi::xml_node iterator;

			//	for (iterator = properties_node.first_child(); iterator; iterator = iterator.next_sibling())
			//	{
			//		std::string temp = iterator.attribute("name").as_string();
			//		if (temp == "color")
			//		{
			//			((FlyingElemental*)ret)->SetAnimations((FlyingElementalColor)iterator.attribute("value").as_int());
			//		}
			//	}
			//}

			//else if (t == TEXTBOXOBJECT)
			//{

			//	pugi::xml_node properties_node = object_iterator.child("properties");
			//	pugi::xml_node iterator;

			//	for (iterator = properties_node.first_child(); iterator; iterator = iterator.next_sibling())
			//	{
			//		std::string temp = iterator.attribute("name").as_string();
			//		if (temp == "text")
			//		{
			//			((TextBoxObject*)ret)->AddText(iterator.attribute("value").as_string());
			//		}
			//		if (temp == "author")
			//		{
			//			((TextBoxObject*)ret)->author = iterator.attribute("value").as_string();
			//		}
			//		if (temp == "lore")
			//		{
			//			((TextBoxObject*)ret)->lore_unlock = iterator.attribute("value").as_int(-1);
			//		}
			//	}
			//}
			//else if (t == HAZARDS_ROCK_BLOCK)
			//{

			//	((HazardRockBlock*)ret)->Init();
			//}
			//else if (t == CLING_CREATURE)
			//{
			//	pugi::xml_node properties_node = object_iterator.child("properties");
			//	pugi::xml_node iterator;

			//	for (iterator = properties_node.first_child(); iterator; iterator = iterator.next_sibling())
			//	{
			//		std::string temp = iterator.attribute("name").as_string();
			//		if (temp == "direction")
			//		{
			//			((ClingCreature*)ret)->curr_dir = (ClingCreatureDirection)iterator.attribute("value").as_int(0);
			//		}
			//	}
			//}
		//}

	}

	return true;
}

bool SceneController::LoadTiles(pugi::xml_node & tile_node)
{
	layer* new_layer = new layer();

	new_layer->width = tile_node.attribute("width").as_int();
	new_layer->height = tile_node.attribute("height").as_int();
	new_layer->size = new_layer->width*new_layer->height;

	room_w = new_layer->width;
	room_h = new_layer->height;

	//read from properties

	pugi::xml_node properties_node = tile_node.child("properties");
	pugi::xml_node iterator;

	new_layer->depth = 20;
	new_layer->parallax_x = 1;
	new_layer->parallax_y = 1;
	new_layer->tileset_of_layer = (*tilesets.begin());

	for (iterator = properties_node.first_child(); iterator; iterator = iterator.next_sibling())
	{
		std::string temp = iterator.attribute("name").as_string();
		if (temp == "depth")
		{
			new_layer->depth = iterator.attribute("value").as_int(20);
		}
		else if (temp == "parallax_x")
		{
			new_layer->parallax_x = iterator.attribute("value").as_float(1);
		}
		else if (temp == "parallax_y")
		{
			new_layer->parallax_y = iterator.attribute("value").as_float(1);
		}
		else if (temp == "tileset")
		{
			int tileset = iterator.attribute("value").as_int(0);
			new_layer->tileset_of_layer = tilesets[tileset];
		}
	}

	new_layer->data = new uint[new_layer->size];
	pugi::xml_node data_node = tile_node.child("data").first_child();

	for (uint i = 0; i<new_layer->size; i++)
	{
		new_layer->data[i] = data_node.attribute("gid").as_uint(-1);
		if (new_layer->data[i] != -1)
		{
			new_layer->data[i] -= new_layer->tileset_of_layer->firstgid;
		}

		data_node = data_node.next_sibling();
	}

	layers.push_back(new_layer);

	return true;
}

//bool SceneController::LoadPortals(pugi::xml_node & objectgroup_node)
//{
//	pugi::xml_node object_iterator;
//	for (object_iterator = objectgroup_node.child("object"); object_iterator; object_iterator = object_iterator.next_sibling())
//	{
//		Portal* new_portal = new Portal();
//
//		new_portal->area.x = object_iterator.attribute("x").as_int();
//		new_portal->area.y = object_iterator.attribute("y").as_int();
//		new_portal->area.w = object_iterator.attribute("width").as_int();
//		new_portal->area.h = object_iterator.attribute("height").as_int();
//
//
//		pugi::xml_node properties_node = object_iterator.child("properties");
//		pugi::xml_node iterator;
//
//		new_portal->id_destination_room = 0;
//		new_portal->id_destination_point = 0;
//
//		for (iterator = properties_node.first_child(); iterator; iterator = iterator.next_sibling())
//		{
//			std::string temp = iterator.attribute("name").as_string();
//			if (temp == "id_room")
//			{
//				new_portal->id_destination_room = iterator.attribute("value").as_int(0);
//			}
//			else if (temp == "id_point")
//			{
//				new_portal->id_destination_point = iterator.attribute("value").as_int(0);
//			}
//			else if (temp == "horizontal")
//			{
//				new_portal->horizontal = iterator.attribute("value").as_bool(false);
//			}
//		}
//		AddPortal(new_portal);
//
//	}
//
//
//	return true;
//}

//bool SceneController::LoadSpawnPoints(pugi::xml_node & objectgroup_node)
//{
//	pugi::xml_node object_iterator;
//	for (object_iterator = objectgroup_node.child("object"); object_iterator; object_iterator = object_iterator.next_sibling())
//	{
//		SpawnPoint* new_point = new SpawnPoint();
//
//		new_point->x = object_iterator.attribute("x").as_int();
//		new_point->y = object_iterator.attribute("y").as_int();
//
//		pugi::xml_node properties_node = object_iterator.child("properties");
//		pugi::xml_node iterator;
//
//		new_point->id = 0;
//
//		for (iterator = properties_node.first_child(); iterator; iterator = iterator.next_sibling())
//		{
//			std::string temp = iterator.attribute("name").as_string();
//			if (temp == "id")
//			{
//				new_point->id = iterator.attribute("value").as_int(0);
//			}
//		}
//
//		spawnpoints.push_back(new_point);
//	}
//	return true;
//}

bool SceneController::LoadCheckPoints(pugi::xml_node &objectgroup_node)
{
	pugi::xml_node object_iterator;
	for (object_iterator = objectgroup_node.child("object"); object_iterator; object_iterator = object_iterator.next_sibling())
	{
		int x = object_iterator.attribute("x").as_int();
		int y= object_iterator.attribute("y").as_int();
		int w= object_iterator.attribute("width").as_int();
		int h= object_iterator.attribute("height").as_int();

		//CheckPoint* new_point = (CheckPoint*)App->phy->AddObject(x,y,w,h,"Checkpoint");

		//pugi::xml_node properties_node = object_iterator.child("properties");
		//pugi::xml_node iterator;

		//for (iterator = properties_node.first_child(); iterator; iterator = iterator.next_sibling())
		//{
		//	std::string temp = iterator.attribute("name").as_string();
		//	if (temp == "x")
		//	{
		//		new_point->spawn_pos_x = iterator.attribute("value").as_int(0) + x;
		//	}
		//	if (temp == "y")
		//	{
		//		new_point->spawn_pos_y = iterator.attribute("value").as_int(0) + y;
		//	}
		//	if (temp == "room_id")
		//	{
		//		new_point->room_id = iterator.attribute("value").as_int(0);
		//	}
		//}
	}
	return true;
}

tileset* SceneController::GetTilesetFromId(int id)
{
	tileset* ret = nullptr;
	int id_iter = id;

	for (std::vector<tileset*>::iterator it = tilesets.begin(); it != tilesets.end(); it++)
	{
		if ((*it)->total_tiles > id)
		{
			ret = *it;
		}
		else
		{
			id_iter -= (*it)->total_tiles;
		}
	}
	return ret;
}

SDL_Rect SceneController::GetImageRectFromId(tileset* t, int id)
{

	SDL_Rect rect;
	rect.w = t->tile_width;
	rect.h = t->tile_height;
	rect.x = ((rect.w) * (id % t->columns));
	rect.y = ((rect.h) * (id / t->columns));

	return rect;
}


SDL_Texture * SceneController::GetTextureFromId(int id)
{

	return GetTilesetFromId(id)->texture;
}

int SceneController::RoundToNearestTileset(int id)
{
	
	tileset* ret = nullptr;
	int id_iter = id;

	for (std::vector<tileset*>::iterator it = tilesets.begin(); it != tilesets.end(); it++)
	{
		if ((*it)->total_tiles > id)
		{
			ret = *it;
		}
		else
		{
			id_iter -= (*it)->total_tiles;
		}
	}
	return 0;
}

void SceneController::RenderTiles()
{
	for (std::vector<layer*>::iterator it = layers.begin(); it != layers.end(); it++)
	{
		float scale = App->win->GetScale();

		float para_x = (*it)->parallax_x;
		float para_y = (*it)->parallax_y;
		int depth = (*it)->depth;

		for (int _y = 0; _y < (*it)->height; ++_y)
		{
			for (int _x = 0; _x < (*it)->width; ++_x)
			{
				int i = _y * (*it)->width + _x;
				//tileset* t = GetTilesetFromId((*it)->data[i]);
				tileset* t = (*it)->tileset_of_layer;//(*tilesets.begin());
				float worldcoords_x = scale *_x * t->tile_width + App->cam->GetCameraXoffset();
				float worldcoords_y = scale *_y * t->tile_height + App->cam->GetCameraYoffset();

				//App->ren->BlitMapTile(t->texture, _x, _y, GetImageRectFromId((*it)->data[i]), depth, para_x, para_y);

				SDL_Rect on_scn;
				on_scn.x = worldcoords_x;
				on_scn.y = worldcoords_y;
				on_scn.w = 48*scale;
				on_scn.h = 48*scale;

				if (SDL_RenderCopyEx(App->ren->renderer, t->texture, &GetImageRectFromId((*it)->tileset_of_layer,(*it)->data[i]), &on_scn, 0, NULL, SDL_FLIP_NONE) != 0)
				{
					std::string errstr = "Cannot blit to screen. SDL_RenderCopy error: ";
					errstr += SDL_GetError();
					Logger::Console_log(LogLevel::LOG_ERROR, errstr.c_str());
				}
			}
		}

	}
}

void SceneController::RenderBackground()
{
	for (std::vector<background_texture*>::iterator it = active_backgrounds.begin(); it != active_backgrounds.end(); it++)
	{
		App->ren->BlitMapBackground((*it)->texture,(*it)->depth,(*it)->repeat_y,(*it)->parallax_x, (*it)->parallax_y);
	}
}




bool SceneController::AssignGameLoopFunction(std::function<void()> aSceneFunction)
{
	if (aSceneFunction != nullptr)
	{
		SceneFunction = aSceneFunction;
		return true;
	}
	return false;
}

bool SceneController::AssignLoadFunction(std::function<void()> aLoadFunction)
{
	if (aLoadFunction != nullptr)
	{
		LoadFunction = aLoadFunction;
		return true;
	}
	return false;
}


//void SceneController::AddPortal(SDL_Rect area, int destination_r, int destination_i, bool horizontal)
//{
//	Portal* p = new Portal();
//
//	p->area = area;
//	p->id_destination_room = destination_r;
//	p->id_destination_point = destination_i;
//	p->horizontal = horizontal;
//
//	portals.push_back(p);
//}
//
//void SceneController::AddPortal(Portal * p)
//{
//	portals.push_back(p);
//}
//
//void SceneController::DeletePortals()
//{
//	for (std::list<Portal*>::iterator it = portals.begin(); it != portals.end(); it++)
//	{
//		delete *it;
//	}
//	portals.clear();
//}



void SceneController::CleanMap()
{
	for (std::vector<layer*>::iterator it = layers.begin(); it != layers.end(); it++)
	{
		//delete (*it)->data;
		delete *it;
	}
	layers.clear();

	//for (std::vector<SpawnPoint*>::iterator it = spawnpoints.begin(); it != spawnpoints.end(); it++)
	//{
	//	delete *it;
	//}
	//spawnpoints.clear();

	//DeletePortals();

	active_backgrounds.clear();
}

void SceneController::ChangeMap(const char * filename)
{

	CleanMap();
	App->phy->Clearphysics();
	//App->gui->Clearelements();
	App->par->ClearParticles();

	LoadMap(filename);
}
