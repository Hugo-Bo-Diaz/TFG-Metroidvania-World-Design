#include "Text.h"
#include "Textures.h"
#include "Application.h"
#include "Render.h"
/*
#include "SDL\include\SDL.h"
#include "SDL_ttf\include\SDL_ttf.h"
#pragma comment( lib, "SDL_ttf/libx86/SDL2_ttf.lib" )
*/

Text::Text()
{
	name = "text";
}

bool Text::LoadConfig(pugi::xml_node & config_node)
{
	/*if (TTF_Init() == -1)
	{
		printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{


		//LOAD THE REST OF THE FONTS IN THE CONFIG
	}*/		const char* path = config_node.child("default_font").attribute("file").as_string(DEFAULT_FONT);
		default_font = LoadFont(path, "default",10);
	return true;
}

bool Text::CreateConfig(pugi::xml_node & config_node)
{


	return true;
}

bool Text::Init()
{
	return true;
}

bool Text::CleanUp()
{


	return true;
}

Font * Text::LoadFont(const char * path, const char * name, int char_per_row)
{
	Font* ret = new Font();

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
	}

	ret->font_texture = texture;
	ret->name = name;
	fonts.push_back(ret);
	ret->hsize = 9;
	ret->vsize = 9;
	ret->char_per_row = char_per_row;

	return ret;
}

TextPrint * Text::CreateText(const char * text, SDL_Color color, int max_width, const char * font, float scale)
{
	TextPrint* ret = new TextPrint();

	ret->color = color;
	ret->font_used = GetFont(font);
	ret->max_width = max_width;
	ret->text = text;

	int number_of_words =  std::count(ret->text.begin(), ret->text.end(), ' ') + 1;

	int iterator = 0;

	for (int j = 0; j < number_of_words; ++j)
	{

		std::string new_string;

		for (; iterator < ret->text.size(); ++iterator)
		{
			if (ret->text[iterator] == 32)
			{
				iterator++;
				break;
			}
			new_string += ret->text[iterator];
		}

		ret->words.push_back(new_string);

	}

	ret->current_letter = ret->text.length();

	ret->scale = scale;

	texts.push_back(ret);
	
	return ret;
}

Font* Text::GetFont(const char * name)
{
	if (name == "")
	{
		return default_font;
	}

	for (std::vector<Font*>::iterator it = fonts.begin(); it != fonts.end(); it++)
	{
		if ((*it)->name == name)
		{
			return (*it);
		}
	}

	return nullptr;
}

void Text::DeleteText(TextPrint * to_delete)
{

}

void Text::ChangeText(TextPrint * to_change, const char * new_text)
{

}

void TextPrint::ChangeText(const char * newtext)
{
	words.clear();

	text = newtext;

	int number_of_words = std::count(text.begin(), text.end(), ' ') + 1;

	int iterator = 0;

	for (int j = 0; j < number_of_words; ++j)
	{

		std::string new_string;

		for (; iterator < text.size(); ++iterator)
		{
			new_string += text[iterator];

			if (text[iterator] == 32)
			{
				iterator++;
				break;
			}
		}

		words.push_back(new_string);

	}

	current_letter = text.length();

}
