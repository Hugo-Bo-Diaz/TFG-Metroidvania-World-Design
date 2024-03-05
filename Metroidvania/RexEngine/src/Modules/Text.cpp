#include "Modules/Text.h"
#include "Modules/Textures.h"
#include "Application.h"
#include "Modules/Render.h"
#include "Utils/Logger.h"
#include "Utils/Utils.h"

#include <sstream>

#include "SDL\include\SDL.h"
#include "SDL_ttf\include\SDL_ttf.h"
#pragma comment( lib, "SDL_ttf/libx86/SDL2_ttf.lib" )

Text::Text(EngineAPI& aAPI) :Part("Text",aAPI)
{
}

bool Text::LoadConfig(pugi::xml_node & config_node)
{
	if (TTF_Init() == -1)
	{
		printf("SDL_ttf could not initialize! SDL_ttf Error: %s", TTF_GetError());
	}
	else
	{


		//LOAD THE REST OF THE FONTS IN THE CONFIG
	}
	/*		const char* path = config_node.child("default_font").attribute("file").as_string(DEFAULT_FONT);
		default_font = LoadFont(path, "default",10);*/
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

FontID Text::LoadFont(const char * path, SDL_Color aColor, int size)
{
	for (std::map<FontID,Font*>::iterator it = fonts.begin(); it != fonts.end(); it++)
	{
		if (std::strcmp((*it).second->name.c_str(), path) == 0 && (*it).second->size == size)
		{
			return (*it).first;
		}
	}

	std::string lFullPath = path;
	std::string lExtension = lFullPath.substr(lFullPath.find_last_of(".") + 1).c_str();

	FontID lResult = -1;

	if (strcmp(lExtension.c_str(), XMLFONTEXTENSION) == 0) {
		lResult = LoadFontXML(path,aColor,size);
	}
	else if (strcmp(lExtension.c_str(), TTFFONTEXTENSION) == 0) {
		lResult = LoadFontTTF(path,aColor,size);
	}
	else
	{
		std::stringstream lStream;
		lStream << "Could not load font from :" << path << " - incorrect format";
		Logger::Console_log(LogLevel::LOG_ERROR, lStream.str().c_str());
	}

	return lResult;
}

FontID Text::LoadFontXML(const char* path, SDL_Color aColor, int size)
{
	pugi::xml_document	font_file;
	pugi::xml_node font_node;
	pugi::xml_parse_result result = font_file.load_file(path);

	if (result.status != pugi::xml_parse_status::status_ok)
	{
		return -1;
	}

	font_node = font_file.child("font");

	const char* lImagePath = font_node.child_value("image_path");

	SDL_Surface* surface = NULL;
	if (FileExists(lImagePath))
	{
		surface = IMG_Load(lImagePath);
	}
	else
	{
		std::string lDir = GetDirectoryFromPath(path);
		std::string lCompletePath = lDir + lImagePath;
		surface = IMG_Load(lCompletePath.c_str());
	}

	SDL_Texture* texture = NULL;

	if (surface == NULL)
	{
		std::string lStr = "Could not load surface with path:";
		lStr += path;
		lStr += " IMG_Load: ";
		lStr += IMG_GetError();
		Logger::Console_log(LogLevel::LOG_ERROR, lStr.c_str());
		return -1;
	}

	texture = SDL_CreateTextureFromSurface(mApp.GetModule<Render>().renderer, surface);
	if (texture == NULL)
	{
		Logger::Console_log(LogLevel::LOG_ERROR, "couldn't make texture from surface");
	}
	SDL_FreeSurface(surface);

	pugi::xml_node properties_node = font_node.child("rectangles");

	pugi::xml_node iterator;
	Font* lfont = new Font(path,texture,aColor,size);

	for (iterator = properties_node.first_child(); iterator; iterator = iterator.next_sibling())
	{
		char c = iterator.child_value("char")[0];
		SDL_Rect* lRect = new SDL_Rect();

		pugi::xml_node lRectangle = iterator.child("rectangle");

		lRect->x = std::stoi(lRectangle.child_value("x"));
		lRect->y = std::stoi(lRectangle.child_value("y"));
		lRect->w = std::stoi(lRectangle.child_value("w"));
		lRect->h = std::stoi(lRectangle.child_value("h"));

		lfont->lMapping.insert(std::make_pair(c,lRect));
	}

	FontID lID = fonts.size();

	fonts.insert(std::make_pair(lID, lfont));

	return lID;
}

FontID Text::LoadFontTTF(const char* aPath, SDL_Color aColor, int size)
{
	TTF_Font* lFont = TTF_OpenFont(aPath,size);

	if (lFont == nullptr)
	{
		std::stringstream lStream;
		lStream << "Could not load the font: " << aPath;
		Logger::Console_log(LogLevel::LOG_ERROR, lStream.str().c_str());
		return -1;
	}

	SDL_Surface* totalSurface = SDL_CreateRGBSurface(0, 512, 512, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);

	Font* lResult = new Font(aPath, nullptr, aColor, size);

	int cursor_x = 0,cursor_y = 0;
	for (int i = 0; i < mSupportedChars.size(); i++)
	{
		int size_char_x,size_char_y;
		std::string lChar;
		lChar += mSupportedChars[i];
		TTF_SizeText(lFont, lChar.c_str(), &size_char_x, &size_char_y);

		SDL_Rect* lCharMapping = new SDL_Rect();
		lCharMapping->x = cursor_x;
		lCharMapping->y = cursor_y;
		lCharMapping->w = size_char_x;
		lCharMapping->h = size_char_y;

		SDL_Surface* lCharSurf = TTF_RenderText_Solid(lFont,lChar.c_str(),aColor);

		cursor_x += size_char_x;

		if (cursor_x + size > 512)
		{
			cursor_y += 2 * size;
			cursor_x = 0;
		}

		if(lCharSurf == NULL)
		{
			std::stringstream lStream;
			lStream << "Could not create the font character for font: " << aPath << " character: " << mSupportedChars[i];
			Logger::Console_log(LogLevel::LOG_ERROR, lStream.str().c_str());
		}

		lResult->lMapping.insert(std::make_pair(mSupportedChars[i], lCharMapping));
		
		SDL_Rect forBlit = { lCharMapping->x,lCharMapping->y,lCharMapping->w,lCharMapping->h };
		SDL_UpperBlit(lCharSurf, new SDL_Rect{0,0,size_char_x,size_char_y},totalSurface,&forBlit);
		SDL_FreeSurface(lCharSurf);
	}

	lResult->font_texture = SDL_CreateTextureFromSurface(mApp.GetModule<Render>().renderer, totalSurface);

	if (lResult->font_texture == NULL)
	{
		Logger::Console_log(LogLevel::LOG_ERROR, "Could not render the font to a texture");
	}

	FontID lID = fonts.size();

	fonts.insert(std::make_pair(lID, lResult));

	return lID;
}

Font* Text::GetFont(FontID aFontID)
{
	return fonts[aFontID];
}