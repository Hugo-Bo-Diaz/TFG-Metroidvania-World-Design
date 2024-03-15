#include "Modules/Text.h"
#include "Modules/Textures.h"
#include "Application.h"
#include "Modules/Render.h"
#include "Utils/Logger.h"
#include "Utils/Utils.h"
#include "EngineAPI.h"

#include "TexturesImpl.h"
#include <sstream>
#include "TextImpl.h"

#include "SDL_image/include/SDL_image.h"
#include "SDL\include\SDL.h"
#include "SDL_ttf\include\SDL_ttf.h"
#pragma comment( lib, "SDL_ttf/libx86/SDL2_ttf.lib" )

Text::Text(EngineAPI& aAPI) :Part("Text",aAPI)
{
	mPartFuncts = new TextImpl(this);
	if (TTF_Init() == -1)
	{
		printf("SDL_ttf could not initialize! SDL_ttf Error: %s", TTF_GetError());
	}
}

#pragma region IMPLEMENTATION

bool Text::TextImpl::CleanUp()
{
	TTF_Quit();
	return true;
}

FontID Text::TextImpl::LoadFontXML(const char* path, const RXColor& aColor, int size)
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

	texture = SDL_CreateTextureFromSurface(mPartInst->mApp.GetModule<Render>().GetSDL_Renderer(), surface);
	if (texture == NULL)
	{
		Logger::Console_log(LogLevel::LOG_ERROR, "couldn't make texture from surface");
	}
	SDL_FreeSurface(surface);

	pugi::xml_node properties_node = font_node.child("rectangles");
	pugi::xml_node iterator;
	TextureID lTex = mPartInst->mApp.GetImplementation<Textures, Textures::TexturesImpl>()->AddTexture(texture, path);

	Font* lfont = new Font(path, lTex, aColor, size);

	for (iterator = properties_node.first_child(); iterator; iterator = iterator.next_sibling())
	{
		char c = iterator.child_value("char")[0];
		SDL_Rect* lRect = new SDL_Rect();

		pugi::xml_node lRectangle = iterator.child("rectangle");

		lRect->x = std::stoi(lRectangle.child_value("x"));
		lRect->y = std::stoi(lRectangle.child_value("y"));
		lRect->w = std::stoi(lRectangle.child_value("w"));
		lRect->h = std::stoi(lRectangle.child_value("h"));

		lfont->lMapping.insert(std::make_pair(c, lRect));
	}

	FontID lID = fonts.size();
	fonts.insert(std::make_pair(lID, lfont));

	return lID;
}

FontID Text::TextImpl::LoadFontTTF(const char* aPath, const RXColor& aColor, int size)
{
	TTF_Font* lFont = TTF_OpenFont(aPath, size);

	if (lFont == nullptr)
	{
		std::stringstream lStream;
		lStream << "Could not load the font: " << aPath;
		Logger::Console_log(LogLevel::LOG_ERROR, lStream.str().c_str());
		return -1;
	}

	SDL_Surface* totalSurface = SDL_CreateRGBSurface(0, 512, 512, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
	Font* lResult = new Font(aPath, NULL, aColor, size);

	int cursor_x = 0, cursor_y = 0;
	for (int i = 0; i < mPartInst->mSupportedChars.size(); i++)
	{
		int size_char_x, size_char_y;
		std::string lChar;
		lChar += mPartInst->mSupportedChars[i];
		TTF_SizeText(lFont, lChar.c_str(), &size_char_x, &size_char_y);

		SDL_Rect* lCharMapping = new SDL_Rect();
		lCharMapping->x = cursor_x;
		lCharMapping->y = cursor_y;
		lCharMapping->w = size_char_x;
		lCharMapping->h = size_char_y;

		SDL_Surface* lCharSurf = TTF_RenderText_Solid(lFont, lChar.c_str(), { aColor.r,aColor.g,aColor.b,aColor.a });
		cursor_x += size_char_x;

		if (cursor_x + size > 512)
		{
			cursor_y += 2 * size;
			cursor_x = 0;
		}

		if (lCharSurf == NULL)
		{
			std::stringstream lStream;
			lStream << "Could not create the font character for font: " << aPath << " character: " << mPartInst->mSupportedChars[i];
			Logger::Console_log(LogLevel::LOG_ERROR, lStream.str().c_str());
		}

		lResult->lMapping.insert(std::make_pair(mPartInst->mSupportedChars[i], lCharMapping));

		SDL_Rect forBlit = { lCharMapping->x,lCharMapping->y,lCharMapping->w,lCharMapping->h };
		SDL_UpperBlit(lCharSurf, new SDL_Rect{ 0,0,size_char_x,size_char_y }, totalSurface, &forBlit);
		SDL_FreeSurface(lCharSurf);
	}

	SDL_Texture* lSDLTexture = SDL_CreateTextureFromSurface(mPartInst->mApp.GetModule<Render>().GetSDL_Renderer(), totalSurface);
	lResult->font_texture = mPartInst->mApp.GetImplementation<Textures, Textures::TexturesImpl>()->AddTexture(lSDLTexture, aPath);

	if (lResult->font_texture == NULL)
	{
		Logger::Console_log(LogLevel::LOG_ERROR, "Could not render the font to a texture");
	}

	FontID lID = fonts.size();
	fonts.insert(std::make_pair(lID, lResult));

	return lID;
}

Font* Text::TextImpl::GetFont(FontID aFontID)
{
	return fonts[aFontID];
}

#pragma endregion

#pragma region PUBLIC API

FontID Text::LoadFont(const char * path, const RXColor& aColor, int size)
{
	TextImpl* lImpl = dynamic_cast<TextImpl*>(mPartFuncts);
	if (!lImpl)
	{
		Logger::Console_log(LogLevel::LOG_ERROR, "Wrong format on the implementation class");
		return 0;
	}

	for (std::map<FontID,Font*>::iterator it = lImpl->fonts.begin(); it != lImpl->fonts.end(); it++)
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
		lResult = lImpl->LoadFontXML(path,aColor,size);
	}
	else if (strcmp(lExtension.c_str(), TTFFONTEXTENSION) == 0) {
		lResult = lImpl->LoadFontTTF(path,aColor,size);
	}
	else
	{
		std::stringstream lStream;
		lStream << "Could not load font from :" << path << " - incorrect format";
		Logger::Console_log(LogLevel::LOG_ERROR, lStream.str().c_str());
	}

	return lResult;
}

void Text::GetTextSize(FontID aFontID, const char* string, int& w, int& y)
{
	TextImpl* lImpl = dynamic_cast<TextImpl*>(mPartFuncts);
	if (!lImpl)
	{
		Logger::Console_log(LogLevel::LOG_ERROR, "Wrong format on the implementation class");
		return;
	}

	Font* lFont = lImpl->GetFont(aFontID);

	if (lFont == nullptr)
		return;

	w = 0;
	y = 0;
	for (size_t i = 0;string[i] != '\0'; i++)
	{
		SDL_Rect* lMap = lFont->lMapping[string[i]];
		if (lMap == nullptr)
		{
			continue;
		}

		y = max(lMap->h, y);
		w += lMap->w;
	}
}

#pragma endregion