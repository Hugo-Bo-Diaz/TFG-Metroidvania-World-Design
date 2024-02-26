#include "Application.h"
#include "Textures.h"
#include "Render.h"
#include "Logger.h"
#include "Utils.h"

#include "SDL_image/include/SDL_image.h"
#pragma comment( lib, "SDL_image/libx86/SDL2_image.lib" )

#include <algorithm>
#include <direct.h>
#include <sstream>

Textures::Textures()
{
	name = "Textures";
}

bool Textures::LoadConfig(pugi::xml_node& config_node)
{
	bool ret = true;

	Logger::Console_log(LogLevel::LOG_INFO, "Init Image library");
	// load support for the PNG image format
	int flags = IMG_INIT_PNG;
	int init = IMG_Init(flags);

	if ((init & flags) != flags)
	{
		std::string lStr = "Could not initialize Image lib. IMG_Init: ";
		lStr += IMG_GetError();
		Logger::Console_log(LogLevel::LOG_ERROR, lStr.c_str());
		ret = false;
	}

	std::list<std::string> lFiles;
	GetAllExtensionPathRecursive("","png", lFiles);
	
	for (std::list<std::string>::iterator it = lFiles.begin(); it != lFiles.end(); it++)
	{
		Load_Texture((*it).c_str());
	}

	return ret;
}

bool Textures::CreateConfig(pugi::xml_node& config_node)
{
	bool ret = true;

	Logger::Console_log(LogLevel::LOG_INFO, "Init Image library");
	// load support for the PNG image format
	int flags = IMG_INIT_PNG;
	int init = IMG_Init(flags);

	if ((init & flags) != flags)
	{
		std::string lStr = "Could not initialize Image lib. IMG_Init: ";
		lStr += IMG_GetError();
		Logger::Console_log(LogLevel::LOG_ERROR, lStr.c_str());
		ret = false;
	}

	pugi::xml_node textures_node = config_node.append_child("textures");



	return ret;
}

bool Textures::Init()
{
	bool ret = true;


	return ret;
}

TextureID Textures::Load_Texture(const char*path)
{
	for (std::vector<Texture*>::iterator it = texture_list.begin(); it != texture_list.end(); it++)
	{
		if (std::strcmp((*it)->name.c_str(), path) == 0)
		{
			return (*it)->id;
		}
	}

	std::stringstream lStr;
	lStr << "Loading texture from: " << path;
	Logger::Console_log(LogLevel::LOG_INFO, lStr.str().c_str());

	SDL_Texture* texture = NULL;
	SDL_Surface* surface = IMG_Load(path);

	if (surface == NULL)
	{
		std::string lStr = "Could not load surface with path: ";
		lStr += path;
		lStr +=" IMG_Init: ";
		lStr += IMG_GetError();
		Logger::Console_log(LogLevel::LOG_ERROR, lStr.c_str());
	}
	else
	{
		texture = SDL_CreateTextureFromSurface(App->ren->renderer,surface);
		if (texture == NULL)
		{
			Logger::Console_log(LogLevel::LOG_ERROR, "couldn't make texture from surface");
		}
		SDL_FreeSurface(surface);
		
		Texture* new_tex = new Texture();
		new_tex->texture = texture;
		++number_of_textures;
		new_tex->id = number_of_textures;
		new_tex->name = path;

		texture_list.push_back(new_tex);
		return new_tex->id;
	}
	return 0;
}

SDL_Texture* Textures::Get_Texture(TextureID id)
{
	for (std::vector<Texture*>::iterator it = texture_list.begin(); it != texture_list.end(); it++)
	{
		if ((*it)->id==id)
		{
			return ((*it)->texture);
		}
	}
	//std::vector<Texture*>::iterator it = std::find(texture_list.begin(), texture_list.end(), name);
	return nullptr;
}

void Textures::Destroy_Texture(const char* texture_to_destroy)
{
	for (std::vector<Texture*>::iterator it = texture_list.begin(); it != texture_list.end(); it++)
	{
		if ((*it)->name==texture_to_destroy)
		{
			delete(*it);
			texture_list.erase(it);
			return;
		}
	}
	//std::vector<Texture*>::iterator it = std::find(texture_list.begin(), texture_list.end(), texture_to_destroy);
	//delete(*it);
	//texture_list.erase(it);
	return;
}

bool Textures::CleanUp()
{
	Logger::Console_log(LogLevel::LOG_ERROR, "Freeing textures and Image library");
	for (std::vector<Texture*>::iterator it = texture_list.begin(); it != texture_list.end(); it++)
	{
		delete(*it);
		
	}
	texture_list.clear();
	IMG_Quit();
	return true;
}