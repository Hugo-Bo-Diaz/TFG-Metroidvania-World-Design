#include "Application.h"

#include "Modules/SceneController.h"
#include "Modules/Textures.h"
#include "Modules/Render.h"
#include "Modules/ObjectManager.h"
#include "Modules/Audio.h"
#include "Modules/Camera.h"
#include "Modules/ProgressTracker.h"
#include "Modules/Particles.h"
#include "Modules/Window.h"
#include "Modules/Audio.h"
#include "Utils/Logger.h"

#include "Modules/Gui.h"
#include "Modules/Text.h"
#include <sstream>
#include <map>

SceneController::SceneController(EngineAPI& aAPI):Part("SceneController",aAPI)
{}

bool SceneController::Init()
{
	LoadMap("Assets/maps/map0_entrance.tmx");
	return true;
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

	if (lMapToLoad != "")
	{
		LoadMapExecute(lMapToLoad.c_str());
		lMapToLoad = "";
	}

	for (std::vector<layer*>::iterator it = layers.begin(); it != layers.end(); ++it)
	{
		mApp.GetModule<Render>().BlitMapLayer(*it);
	}


	return ret;
}

bool SceneController::CleanUp()
{
	bool ret = true;
	CleanMap();
	mApp.GetModule<ObjectManager>().Clearphysics();
	//App->gui->Clearelements();
	return ret;
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
	set->texture = mApp.GetModule<Textures>().Load_Texture(base_folder.c_str());

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
	back->texture = mApp.GetModule<Textures>().Load_Texture(base_folder.c_str());


	active_backgrounds.push_back(back);

	//backgrounds.push_back(back);

	return true;
}

void SceneController::LoadMap(const char* filename)
{
	lMapToLoad = filename;
}

bool SceneController::LoadMapExecute(const char* filename)
{
	CleanMap();
	mApp.GetModule<ObjectManager>().Clearphysics();
	mApp.GetModule<Particles>().ClearParticles();

	std::stringstream lStr;
	lStr << "Loading map from: " << filename;
	Logger::Console_log(LogLevel::LOG_INFO, lStr.str().c_str());

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
		if (iterator_name == "tileset")
		{
			LoadTilesets(iterator);
		}
		if(iterator_name == "imagelayer")
		{
			LoadBackgroundImage(iterator);
		}
		if (iterator_name == "objectgroup")
		{
			bool isWallLayer = false;
			pugi::xml_node lProperties = iterator.child("properties");

			pugi::xml_node iterator_prop;
			for (iterator_prop = lProperties.first_child(); iterator_prop; iterator_prop = iterator_prop.next_sibling())
			{
				pugi::xml_attribute lName = iterator_prop.attribute("name");
				if (std::strcmp(lName.value(), "isWallLayer") == 0)
				{
					isWallLayer = iterator_prop.attribute("value").as_bool(false);
				}
			}
			if(isWallLayer)
			{
				LoadWalls(iterator);
			}
			else
			{
				LoadObjects(iterator);
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
		mApp.GetModule<ObjectManager>().AddWall(newwall);
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

		GameObject*ret = nullptr;

		w = object_iterator.attribute("width").as_int();
		h = object_iterator.attribute("height").as_int();

		//std::map<std::string, float> lProperties;

		std::list<ObjectProperty*> lProperties;

		pugi::xml_node properties_node = object_iterator.child("properties");
		pugi::xml_node iterator;

		for (iterator = properties_node.first_child(); iterator; iterator = iterator.next_sibling())
		{
			ObjectProperty* lObjProp = new ObjectProperty();

			lObjProp->name = iterator.attribute("name").as_string();
			std::string type = iterator.attribute("type").as_string();
			float value = 0;
			if (strcmp(type.c_str(), "bool") == 0)
			{
				lObjProp->bool_value = iterator.attribute("value").as_bool();
			}
			else if (strcmp(type.c_str(), "float") == 0)
			{
				lObjProp->num_value = iterator.attribute("value").as_float();
			}
			else if (strcmp(type.c_str(), "int") == 0)
			{
				lObjProp->num_value = iterator.attribute("value").as_int();
			}
			else if (strcmp(iterator.attribute("value").as_string(""), "") != 0)
			{
				lObjProp->str_value = iterator.attribute("value").as_string("");
			}

			lProperties.push_back(lObjProp);
		}

		auto lID = mApp.GetModule<ObjectManager>().GetFactory(type.c_str());
		
		if (lID != nullptr)
		{
			ret = (*lID).CreateInstace(lProperties);
			ret->mType = lID->GetObjectTypeIndex();
			ret->Engine = new EngineAPI(mApp);
			ret->collider->x = x;
			ret->collider->y = y;
			ret->collider->w = w;
			ret->collider->h = h;

			ret->Init();

			mApp.GetModule<ObjectManager>().AddObject(ret);
		}
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


SDL_Texture * SceneController::GetTextureFromId(int id)
{

	return mApp.GetModule<Textures>().Get_Texture(GetTilesetFromId(id)->texture);
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
	//for (std::vector<layer*>::iterator it = layers.begin(); it != layers.end(); it++)
	//{
	//	float scale = App->win->GetScale();

	//	float para_x = (*it)->parallax_x;
	//	float para_y = (*it)->parallax_y;
	//	int depth = (*it)->depth;
	//	
	//	SDL_Texture* lTex = App->tex->Get_Texture((*it)->tileset_of_layer->texture);

	//	for (int _y = 0; _y < (*it)->height; ++_y)
	//	{
	//		for (int _x = 0; _x < (*it)->width; ++_x)
	//		{
	//			int i = _y * (*it)->width + _x;
	//			//tileset* t = GetTilesetFromId((*it)->data[i]);
	//			tileset* t = (*it)->tileset_of_layer;//(*tilesets.begin());
	//			float worldcoords_x = scale *_x * t->tile_width + App->cam->GetCameraXoffset();
	//			float worldcoords_y = scale *_y * t->tile_height + App->cam->GetCameraYoffset();

	//			//App->ren->BlitMapTile(t->texture, _x, _y, GetImageRectFromId((*it)->data[i]), depth, para_x, para_y);

	//			SDL_Rect on_scn;
	//			on_scn.x = worldcoords_x;
	//			on_scn.y = worldcoords_y;
	//			on_scn.w = 48*scale;
	//			on_scn.h = 48*scale;

	//			if (SDL_RenderCopyEx(App->ren->renderer, lTex, &GetImageRectFromId((*it)->tileset_of_layer,(*it)->data[i]), &on_scn, 0, NULL, SDL_FLIP_NONE) != 0)
	//			{
	//				std::string errstr = "Cannot blit to screen. SDL_RenderCopy error: ";
	//				errstr += SDL_GetError();
	//				Logger::Console_log(LogLevel::LOG_ERROR, errstr.c_str());
	//			}
	//		}
	//	}

	//}
}

void SceneController::RenderBackground()
{
	for (std::vector<background_texture*>::iterator it = active_backgrounds.begin(); it != active_backgrounds.end(); it++)
	{
		mApp.GetModule<Render>().BlitMapBackground((*it)->texture,(*it)->depth,(*it)->repeat_y,(*it)->parallax_x, (*it)->parallax_y);
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



void SceneController::CleanMap()
{
	for (std::vector<layer*>::iterator it = layers.begin(); it != layers.end(); it++)
	{
		//delete (*it)->data;
		delete *it;
	}
	layers.clear();


	for (std::vector<background_texture*>::iterator it = active_backgrounds.begin(); it != active_backgrounds.end(); it++)
	{
		//delete (*it)->data;
		delete* it;
	}

	active_backgrounds.clear();
	
	for (std::vector<tileset*>::iterator it = tilesets.begin(); it != tilesets.end(); it++)
	{
		//delete (*it)->data;
		delete* it;
	}
	tilesets.clear();
}

void SceneController::ChangeMap(const char * filename)
{
	std::stringstream lStr;
	lStr << "Change map to: " << filename;
	Logger::Console_log(LogLevel::LOG_INFO, lStr.str().c_str());

	LoadMap(filename);
}
