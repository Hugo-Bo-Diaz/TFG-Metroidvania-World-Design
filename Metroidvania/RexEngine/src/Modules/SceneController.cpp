#include "RXpch.h"
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
#include "EngineElements/GameObject.h"

#include "Modules/Gui.h"
#include "Modules/Text.h"

#include "SceneControllerImpl.h"
#include "RenderImpl.h"
#include "ParticlesImpl.h"
#include "ObjectManagerImpl.h"

#include "Utils/Utils.h"

SceneController::SceneController(EngineAPI& aAPI):Part("SceneController",aAPI)
{
	mPartFuncts = new SceneControllerImpl(this);
}

#pragma region IMPLEMENTATION

bool SceneController::SceneControllerImpl::Loop(float dt)
{
	bool ret = true;

	if (SceneFunction != nullptr)
	{
		SceneFunction();
	}

	if (lMapToLoad != "")
	{
		LoadMapExecute(lMapToLoad.c_str());
		lMapToLoad = "";
	}

	for (std::vector<background_texture*>::iterator it = active_backgrounds.begin(); it != active_backgrounds.end(); it++)
	{
		mPartInst->mApp.GetImplementation<Render, Render::RenderImpl>()->RenderMapBackground((*it)->texture, (*it)->depth, (*it)->repeat_y, (*it)->parallax_x, (*it)->parallax_y);
	}

	for (std::vector<layer*>::iterator it = layers.begin(); it != layers.end(); ++it)
	{
		mPartInst->mApp.GetImplementation<Render,Render::RenderImpl>()->RenderMapLayer(*it);
	}

	return ret;
}

bool SceneController::SceneControllerImpl::CleanUp()
{
	bool ret = true;
	mPartInst->CleanMap();
	mPartInst->mApp.GetModule<ObjectManager>().Clearphysics();
	return ret;
}

bool SceneController::SceneControllerImpl::LoadTilesets(pugi::xml_node & node, const char* aMapFolder)
{
	tileset* set = new tileset(
	node.attribute("firstgid").as_int(),
	node.attribute("tilewidth").as_int(),
	node.attribute("tileheight").as_int(),
	node.attribute("columns").as_int(),
	node.attribute("tilecount").as_int());


	pugi::xml_node imagenode = node.child("image");
	std::string base_folder = aMapFolder;
	base_folder += imagenode.attribute("source").as_string();

	//load this texture
	set->texture = mPartInst->mApp.GetModule<Textures>().Load_Texture(base_folder.c_str());
	tilesets.push_back(set);

	return true;
}

bool SceneController::SceneControllerImpl::LoadBackgroundImage(pugi::xml_node& node, const char* aMapFolder)
{
	int depth;
	float parallax_x, parallax_y;
	bool repeat_y;

	pugi::xml_node iterator;
	pugi::xml_node properties_node = node.child("properties");
	for (iterator = properties_node.first_child(); iterator; iterator = iterator.next_sibling())
	{
		std::string temp = iterator.attribute("name").as_string();
		if (temp == "depth")
		{
			depth = iterator.attribute("value").as_int(20);
		}
		else if (temp == "parallax_x")
		{
			parallax_x = iterator.attribute("value").as_float(1);
		}
		else if (temp == "parallax_y")
		{
			parallax_y = iterator.attribute("value").as_float(1);
		}
		else if (temp == "repeat_y")
		{
			repeat_y = iterator.attribute("value").as_bool(true);
		}
	}

	pugi::xml_node imagenode = node.child("image");
	const char* path = imagenode.attribute("source").as_string();

	std::string base_folder = aMapFolder;
	base_folder += path;

	//load this texture
	TextureID texture = mPartInst->mApp.GetModule<Textures>().Load_Texture(base_folder.c_str());

	background_texture* back = new background_texture(texture, parallax_x, parallax_y, depth, path, repeat_y);
	active_backgrounds.push_back(back);

	return true;
}

bool SceneController::SceneControllerImpl::LoadMapExecute(const char* filename)
{
	mPartInst->CleanMap();
	mPartInst->mApp.GetModule<ObjectManager>().Clearphysics();
	mPartInst->mApp.GetImplementation<Particles,Particles::ParticlesImpl>()->ClearParticles();

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

	std::string lMapFolder = GetDirectoryFromPath(filename);

	pugi::xml_node layer_node = map_node.first_child();
	for (iterator = layer_node; iterator; iterator = iterator.next_sibling())
	{
		std::string iterator_name = iterator.name();
		if (iterator_name == "tileset")
		{
			LoadTilesets(iterator, lMapFolder.c_str());
		}
		if(iterator_name == "imagelayer")
		{
			LoadBackgroundImage(iterator, lMapFolder.c_str());
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

void SceneController::SceneControllerImpl::LoadMapProperties(pugi::xml_node & node)
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

bool SceneController::SceneControllerImpl::LoadBackground(pugi::xml_node& imagelayer_node)
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

bool SceneController::SceneControllerImpl::LoadWalls(pugi::xml_node& objectgroup_node)
{
	pugi::xml_node object_iterator;
	for (object_iterator = objectgroup_node.child("object"); object_iterator; object_iterator = object_iterator.next_sibling())
	{
		RXRect newwall;
		newwall.x = object_iterator.attribute("x").as_int();
		newwall.y = object_iterator.attribute("y").as_int();
		newwall.w = object_iterator.attribute("width").as_int();
		newwall.h = object_iterator.attribute("height").as_int();
		mPartInst->mApp.GetModule<ObjectManager>().AddWall(newwall);
	}

	return true;
}

bool SceneController::SceneControllerImpl::LoadObjects(pugi::xml_node& objectgroup_node)
{
	pugi::xml_node object_iterator;
	for (object_iterator = objectgroup_node.child("object"); object_iterator; object_iterator = object_iterator.next_sibling())
	{
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

		auto lID = mPartInst->mApp.GetImplementation<ObjectManager,ObjectManager::ObjectManagerImpl>()->GetFactory(type.c_str());
		
		if (lID != nullptr)
		{
			ret = (*lID).CreateInstace(lProperties);
			ret->mType = lID->GetObjectTypeIndex();
			ret->Engine = new EngineAPI(mPartInst->mApp);
			ret->collider->x = x;
			ret->collider->y = y;
			ret->collider->w = w;
			ret->collider->h = h;

			ret->Init();

			mPartInst->mApp.GetModule<ObjectManager>().AddObject(ret);
		}
	}

	return true;
}

bool SceneController::SceneControllerImpl::LoadTiles(pugi::xml_node & tile_node)
{
	//load the main map properties
	int width = tile_node.attribute("width").as_int();
	int height = tile_node.attribute("height").as_int();
	int size = width * height;

	room_w = width;
	room_h = height;

	//read from properties

	pugi::xml_node properties_node = tile_node.child("properties");
	pugi::xml_node iterator;

	int depth = 20;
	int parallax_x = 1;
	int parallax_y = 1;
	tileset* tileset_of_layer = (*tilesets.begin());

	for (iterator = properties_node.first_child(); iterator; iterator = iterator.next_sibling())
	{
		std::string temp = iterator.attribute("name").as_string();
		if (temp == "depth")
		{
			depth = iterator.attribute("value").as_int(20);
		}
		else if (temp == "parallax_x")
		{
			parallax_x = iterator.attribute("value").as_float(1);
		}
		else if (temp == "parallax_y")
		{
			parallax_y = iterator.attribute("value").as_float(1);
		}
		else if (temp == "tileset")
		{
			int tileset = iterator.attribute("value").as_int(0);
			tileset_of_layer = tilesets[tileset];
		}
	}

	uint* data = new uint[size];
	pugi::xml_node data_node = tile_node.child("data").first_child();

	for (uint i = 0; i<size; i++)
	{
		data[i] = data_node.attribute("gid").as_uint(-1);
		if (data[i] != -1)
		{
			data[i] -= tileset_of_layer->firstgid;
		}

		data_node = data_node.next_sibling();
	}

	layer* new_layer = new layer(tileset_of_layer, data, width, height, parallax_x, parallax_y, depth, size);
	layers.push_back(new_layer);

	return true;
}

#pragma endregion

#pragma region PUBLIC API

void SceneController::LoadMap(const char* filename)
{
	SceneControllerImpl* lImpl = dynamic_cast<SceneControllerImpl*>(mPartFuncts);
	if (!lImpl)
	{
		Logger::Console_log(LogLevel::LOG_ERROR, "Wrong format on the implementation class");
		return;
	}

	std::stringstream lStr;
	lStr << "Change map to: " << filename;
	Logger::Console_log(LogLevel::LOG_INFO, lStr.str().c_str());

	lImpl->lMapToLoad = filename;
}

bool SceneController::AssignGameLoopFunction(std::function<void()> aSceneFunction)
{
	SceneControllerImpl* lImpl = dynamic_cast<SceneControllerImpl*>(mPartFuncts);
	if (!lImpl)
	{
		Logger::Console_log(LogLevel::LOG_ERROR, "Wrong format on the implementation class");
		return false;
	}

	if (aSceneFunction != nullptr)
	{
		lImpl->SceneFunction = aSceneFunction;
		return true;
	}
	return false;
}

bool SceneController::AssignLoadFunction(std::function<void()> aLoadFunction)
{
	SceneControllerImpl* lImpl = dynamic_cast<SceneControllerImpl*>(mPartFuncts);
	if (!lImpl)
	{
		Logger::Console_log(LogLevel::LOG_ERROR, "Wrong format on the implementation class");
		return false;
	}

	if (aLoadFunction != nullptr)
	{
		lImpl->LoadFunction = aLoadFunction;
		return true;
	}
	return false;
}

void SceneController::CleanMap()
{
	SceneControllerImpl* lImpl = dynamic_cast<SceneControllerImpl*>(mPartFuncts);
	if (!lImpl)
	{
		Logger::Console_log(LogLevel::LOG_ERROR, "Wrong format on the implementation class");
		return;
	}

	for (std::vector<layer*>::iterator it = lImpl->layers.begin(); it != lImpl->layers.end(); it++)
	{
		//delete (*it)->data;
		delete *it;
	}
	lImpl->layers.clear();


	for (std::vector<background_texture*>::iterator it = lImpl->active_backgrounds.begin(); it != lImpl->active_backgrounds.end(); it++)
	{
		//delete (*it)->data;
		delete* it;
	}

	lImpl->active_backgrounds.clear();
	
	for (std::vector<tileset*>::iterator it = lImpl->tilesets.begin(); it != lImpl->tilesets.end(); it++)
	{
		//delete (*it)->data;
		delete* it;
	}
	lImpl->tilesets.clear();
}

void SceneController::GetRoomSize(int& x, int& y)
{
	SceneControllerImpl* lImpl = dynamic_cast<SceneControllerImpl*>(mPartFuncts);
	if (!lImpl)
	{
		Logger::Console_log(LogLevel::LOG_ERROR, "Wrong format on the implementation class");
		return;
	}

	x = lImpl->room_w;
	y = lImpl->room_h;
}

#pragma endregion