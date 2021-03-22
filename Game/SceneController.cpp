#include "Application.h"

#include "SceneController.h"
#include "Textures.h"
#include "InGameScene.h"
#include "MainMenuScene.h"
#include "Render.h"
#include "Physics.h"
#include "Audio.h"

#include "Player.h"

#include "Gui.h"
#include "Text.h"

SceneController::SceneController()
{
	name = "SceneController";
	active = true;
}

bool SceneController::Init()
{
	//load backgrounds and tilesets

	LoadAssetsMap("Assets/maps/AssetStorage.tmx");

	LoadMapArray("Assets/maps/map_array.xml");

	LoadMap("Assets/maps/testing_grounds.tmx");

	
	Player* pl= (Player*)App->phy->AddObject(576, 322, 64, 64, PLAYER);
	App->gui->Add_GameUI(pl);

	song_try = App->aud->LoadMusic("Assets/Music/wtf.ogg","track1");

	return true;//CHANGE PLS
}

bool SceneController::Loop(float dt)
{
	bool ret = true;

	App->aud->PlayMusic(song_try,1000);

	RenderTiles();
	RenderBackground();

	/*App->ren->BlitText(p, 100, 100);
	App->ren->BlitText(c, 100, 175);
	App->ren->BlitText(v, 100, 275);
	*/

	/*for (std::vector<std::string>::iterator it = backgrounds.begin(); it != backgrounds.end(); it++)
	{
		App->ren->Blit(App->tex->Get_Texture((*it).c_str()), 0, 0, NULL, 50,0);
	}*/

	return ret;
}

bool SceneController::CleanUp()
{
	bool ret = true;

	return ret;
}

bool SceneController::LoadAssetsMap(const char * map)
{

	pugi::xml_document	map_file;
	pugi::xml_node map_node;
	pugi::xml_parse_result result = map_file.load_file(map);

	if (result == NULL)
	{
		printf("couldn't find map %s", map);
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
		printf("couldn't find map %s", filename);
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
		}
		if (iterator_name == "layer")
		{
			LoadTiles(iterator);
		}
	}

	return true;
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
	//pugi::xml_node object_iterator;
	//for (object_iterator = objectgroup_node.child("object"); object_iterator; object_iterator = object_iterator.next_sibling())
	//{
	//	int newpos;
	//}

	return true;
}

bool SceneController::LoadTiles(pugi::xml_node & tile_node)
{
	layer* new_layer = new layer();

	new_layer->width = tile_node.attribute("width").as_int();
	new_layer->height = tile_node.attribute("height").as_int();
	new_layer->size = new_layer->width*new_layer->height;

	//read from properties

	pugi::xml_node properties_node = tile_node.child("properties");
	pugi::xml_node iterator;

	new_layer->depth = 20;
	new_layer->parallax_x = 1;
	new_layer->parallax_y = 1;

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
	}

	new_layer->data = new uint[new_layer->size];
	pugi::xml_node data_node = tile_node.child("data").first_child();

	for (uint i = 0; i<new_layer->size; i++)
	{
		new_layer->data[i] = data_node.attribute("gid").as_uint(0);
		data_node = data_node.next_sibling();
	}

	layers.push_back(new_layer);

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

SDL_Rect SceneController::GetImageRectFromId(int id)
{
	tileset* from = GetTilesetFromId(id);

	int relative_id = id - from->firstgid;

	SDL_Rect rect;
	rect.w = from->tile_width;
	rect.h = from->tile_height;
	rect.x = ((rect.w) * (relative_id % from->columns));
	rect.y = ((rect.h) * (relative_id / from->columns));

	return rect;
}


SDL_Texture * SceneController::GetTextureFromId(int id)
{

	return GetTilesetFromId(id)->texture;
}

void SceneController::RenderTiles()
{
	for (std::vector<layer*>::iterator it = layers.begin(); it != layers.end(); it++)
	{
		float para_x = (*it)->parallax_x;
		float para_y = (*it)->parallax_y;
		int depth = (*it)->depth;

		for (int _y = 0; _y < (*it)->height; ++_y)
		{
			for (int _x = 0; _x < (*it)->width; ++_x)
			{
				int i = _y * (*it)->width + _x;
				tileset* t = GetTilesetFromId((*it)->data[i]);

				float worldcoords_x = _x*t->tile_width;
				float worldcoords_y = _y * t->tile_height;

				App->ren->BlitMapTile(
					t->texture,
					_x,_y,
					GetImageRectFromId((*it)->data[i]),
					depth, para_x, para_y);
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

void SceneController::LoadMapArray(const char * document)
{
	pugi::xml_document	document_file;
	pugi::xml_node completemap;
	pugi::xml_parse_result result = document_file.load_file(document);

	if (result == NULL)
	{
		printf("couldn't find map %s", document);
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

void SceneController::CleanMap()
{
	for (std::vector<layer*>::iterator it = layers.begin(); it != layers.end(); it++)
	{
		delete (*it)->data;
		delete *it;
	}
	layers.clear();

	active_backgrounds.clear();
}
