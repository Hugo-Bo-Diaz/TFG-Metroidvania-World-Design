#include "Application.h"
#include "Modules/Textures.h"
#include "Modules/Render.h"
#include "Utils/Logger.h"
#include "Utils/Utils.h"
#include "TexturesImpl.h"
#include "EngineAPI.h"

#include "SDL_image/include/SDL_image.h"
#pragma comment( lib, "SDL_image/libx86/SDL2_image.lib" )

#include <algorithm>
#include <direct.h>
#include <sstream>

Textures::Textures(EngineAPI& aAPI) :Part("Textures", aAPI)
{
	mPartFuncts = new TexturesImpl(this);
}

bool Textures::TexturesImpl::LoadConfig(pugi::xml_node& config_node)
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
		mPartInst->Load_Texture((*it).c_str());
	}

	return ret;
}

bool Textures::TexturesImpl::CreateConfig(pugi::xml_node& config_node)
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

bool Textures::TexturesImpl::Init()
{
	bool ret = true;


	return ret;
}

TextureID Textures::Load_Texture(const char*path)
{
	TexturesImpl* lImpl = dynamic_cast<TexturesImpl*>(mPartFuncts);
	if (!lImpl)
	{
		Logger::Console_log(LogLevel::LOG_ERROR, "Wrong format on the implementation class");
		return 0;
	}

	for (std::vector<Texture*>::iterator it = lImpl->texture_list.begin(); it != lImpl->texture_list.end(); it++)
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

	TextureID lResult = 0;

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
		texture = SDL_CreateTextureFromSurface(mApp.GetModule<Render>().GetSDL_Renderer(),surface);
		if (texture == NULL)
		{
			Logger::Console_log(LogLevel::LOG_ERROR, "couldn't make texture from surface");
		}
		SDL_FreeSurface(surface);
		
		lResult = lImpl->AddTexture(texture, path);
	}
	return lResult;
}

SDL_Texture* Textures::TexturesImpl::Get_Texture(TextureID id)
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

TextureID Textures::TexturesImpl::AddTexture(SDL_Texture* aTextureToAdd, const char* aTextureName)
{
	Texture* new_tex = new Texture();
	new_tex->texture = aTextureToAdd;
	++number_of_textures;
	new_tex->id = number_of_textures;
	new_tex->name = aTextureName;

	texture_list.push_back(new_tex);
	return new_tex->id;
}

void Textures::Destroy_Texture(const char* texture_to_destroy)
{
	TexturesImpl* lImpl = dynamic_cast<TexturesImpl*>(mPartFuncts);
	if (!lImpl)
	{
		Logger::Console_log(LogLevel::LOG_ERROR, "Wrong format on the implementation class");
		return;
	}

	for (std::vector<Texture*>::iterator it = lImpl->texture_list.begin(); it != lImpl->texture_list.end(); it++)
	{
		if ((*it)->name==texture_to_destroy)
		{
			delete(*it);
			lImpl->texture_list.erase(it);
			return;
		}
	}
	//std::vector<Texture*>::iterator it = std::find(texture_list.begin(), texture_list.end(), texture_to_destroy);
	//delete(*it);
	//texture_list.erase(it);
	return;
}

bool Textures::TexturesImpl::CleanUp()
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