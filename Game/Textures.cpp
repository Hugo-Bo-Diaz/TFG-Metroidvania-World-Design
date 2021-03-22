#include "Application.h"

#include "Textures.h"
#include "Render.h"

#include "SDL_image/include/SDL_image.h"
#pragma comment( lib, "SDL_image/libx86/SDL2_image.lib" )

#include <algorithm>



Textures::Textures()
{
	name = "Textures";
}

bool Textures::LoadConfig(pugi::xml_node& config_node)
{
	bool ret = true;

	printf("Init Image library\n");
	// load support for the PNG image format
	int flags = IMG_INIT_PNG;
	int init = IMG_Init(flags);

	if ((init & flags) != flags)
	{
		printf("Could not initialize Image lib. IMG_Init: %s \n", IMG_GetError());
		ret = false;
	}
	
	pugi::xml_node node_textures = config_node.child("textures");

	pugi::xml_node texture;
	int i = 0;
	for (texture = node_textures.first_child(); texture; texture = texture.next_sibling())// there are more entities with properties
	{
		Load_Texture(texture.attribute("path").as_string(), texture.attribute("name").as_string());
	}
	return ret;
}

bool Textures::CreateConfig(pugi::xml_node& config_node)
{
	bool ret = true;

	printf("Init Image library\n");
	// load support for the PNG image format
	int flags = IMG_INIT_PNG;
	int init = IMG_Init(flags);

	if ((init & flags) != flags)
	{
		printf("Could not initialize Image lib. IMG_Init: %s \n", IMG_GetError());
		ret = false;
	}

	pugi::xml_node textures_node = config_node.append_child("textures");

	Load_Texture("Assets/Sprites/Player.png", "player", textures_node);
	Load_Texture("Assets/Sprites/spells.png", "spells", textures_node);
	Load_Texture("Assets/UI/books.png", "spell_books", textures_node);
	Load_Texture("Assets/UI/healthandmana.png", "healthbars", textures_node);
	Load_Texture("Assets/UI/spell_location.png", "spell_display_base", textures_node);

	return ret;


}

bool Textures::Init()
{
	bool ret = true;


	return ret;
}

bool Textures::Valid_Texture(const char* texture_to_validate)
{
	/*if (std::find(texture_list.begin(), texture_list.end(), texture_to_validate) != texture_list.end())
	{
		return true;
	}
	else
	{
		printf("texture couldn't be validated\n");
		return false;
	}*/
	return true;
}

void Textures::Load_Texture(const char*path,const char* name, pugi::xml_node& node)
{
	if (!node.empty())
	{
		pugi::xml_node tex_node = node.append_child("texture");
		tex_node.append_attribute("path") = path;
		tex_node.append_attribute("name") = name;
	}

	SDL_Texture* texture = NULL;
	SDL_Surface* surface = IMG_Load(path);

	if (surface == NULL)
	{
		printf("Could not load surface with path: %s. IMG_Load: %s \n", path, IMG_GetError());
	}
	else
	{
		texture = SDL_CreateTextureFromSurface(App->ren->renderer,surface);
		if (texture == NULL)
		{
			printf("couldn't make texture from surface \n");
		}
		SDL_FreeSurface(surface);
		
	Texture* new_tex = new Texture();
	new_tex->texture = texture;
	new_tex->id = number_of_textures;
	++number_of_textures;
	new_tex->name = name;

	texture_list.push_back(new_tex);
	}



	return;
}

SDL_Texture * Textures::Load_Texture_Scene(const char * path)
{
	SDL_Texture* texture = NULL;
	SDL_Surface* surface = IMG_Load(path);

	if (surface == NULL)
	{
		printf("Could not load surface with path: %s. IMG_Load: %s \n", path, IMG_GetError());
	}
	else
	{
		texture = SDL_CreateTextureFromSurface(App->ren->renderer, surface);
		if (texture == NULL)
		{
			printf("couldn't make texture from surface \n");
		}
		SDL_FreeSurface(surface);

		Texture* new_tex = new Texture();
		new_tex->texture = texture;
		new_tex->id = number_of_textures;
		++number_of_textures;
		new_tex->name = name;

		texture_list.push_back(new_tex);
	}



	return texture;
}

SDL_Texture* Textures::Get_Texture(const char* name)
{
	for (std::vector<Texture*>::iterator it = texture_list.begin(); it != texture_list.end(); it++)
	{
		if ((*it)->name==name && Valid_Texture((*it)->name.c_str()))
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
		if ((*it)->name==texture_to_destroy&&Valid_Texture((*it)->name.c_str()))
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
	printf("Freeing textures and Image library \n");
	for (std::vector<Texture*>::iterator it = texture_list.begin(); it != texture_list.end(); it++)
	{
		if (Valid_Texture((*it)->name.c_str()))
		{
			delete(*it);
		}
	}
	texture_list.clear();
	IMG_Quit();
	return true;
}