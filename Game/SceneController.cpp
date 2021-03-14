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

	LoadMap("Assets/maps/testing_grounds.tmx");

	
	Player* pl= (Player*)App->phy->AddObject(576, 322, 64, 64, PLAYER);
	App->gui->Add_GameUI(pl);
	p = App->txt->CreateText("Hello", SDL_Color{ 0,0,0 }, 1000,"",3);
	c = App->txt->CreateText("Hello", SDL_Color{ 0,0,0 }, 1000, "", 5);
	v = App->txt->CreateText("Hello", SDL_Color{ 0,0,0 }, 1000, "", 7);

	song_try = App->aud->LoadMusic("Assets/Music/wtf.ogg","track1");

	return true;//CHANGE PLS
}

bool SceneController::Loop(float dt)
{
	bool ret = true;

	App->aud->PlayMusic(song_try,1000);


	/*App->ren->BlitText(p, 100, 100);
	App->ren->BlitText(c, 100, 175);
	App->ren->BlitText(v, 100, 275);
	*/

	for (std::vector<std::string>::iterator it = backgrounds.begin(); it != backgrounds.end(); it++)
	{
		App->ren->Blit(App->tex->Get_Texture((*it).c_str()), 0, 0, NULL, 50,0);
	}

	return ret;
}

bool SceneController::CleanUp()
{
	bool ret = true;

	return ret;
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

	map_node = map_file.child("map");

	pugi::xml_node layer_node = map_node.first_child();
	pugi::xml_node iterator;
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
	}

	return true;
}

bool SceneController::LoadBackground(pugi::xml_node& imagelayer_node)
{
	pugi::xml_node image_node = imagelayer_node.first_child();
	
	std::string image_location;
	if (imagelayer_node.child("properties"))
	{
		pugi::xml_node property_iterator;
		for (property_iterator = imagelayer_node.child("properties").child("property"); property_iterator; property_iterator = property_iterator.next_sibling())
		{
			std::string tmp = property_iterator.attribute("name").as_string("");
			if(tmp=="directory")
			{
				image_location += property_iterator.attribute("value").as_string();
			}
		}
	}

	image_location += image_node.attribute("source").as_string();

	backgrounds.push_back(imagelayer_node.attribute("name").as_string());
	App->tex->Load_Texture(image_location.c_str(),imagelayer_node.attribute("name").as_string());
	
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