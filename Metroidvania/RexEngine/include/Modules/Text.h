#ifndef TEXT
#define TEXT

#include "PartsDef.h"
#include "SDL\include\SDL_pixels.h"
#include "Part.h"
#include <vector>
#include <string>
#include <map>

#define DEFAULT_FONT "Assets/font1.png"
#define DEFAULT_FONT_SIZE 33

#define XMLFONTEXTENSION "xml"
#define TTFFONTEXTENSION "ttf"

struct SDL_Texture;
struct _TTF_Font;
struct SDL_Rect;

typedef int FontID;

struct DLL_EXPORT Font
{
	std::string name;
	SDL_Texture* font_texture;

	std::map<char, SDL_Rect*> lMapping;

	int size;
	int char_per_row;
	SDL_Color color;

	Font(const char* aName, SDL_Texture* aTexture, SDL_Color aColor, int aSize )
		:name(aName),font_texture(aTexture),color (aColor),size(aSize) 
	{}
};


class DLL_EXPORT Text : public Part
{
public:
	Text(EngineAPI& aAPI);

	const std::string mSupportedChars = ":abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890/.,'";

	bool LoadConfig(pugi::xml_node&);
	bool CreateConfig(pugi::xml_node&);

	virtual bool Init();
	virtual bool CleanUp();

	FontID LoadFont(const char* path, SDL_Color aColor, int size = 15);

	FontID LoadFontXML(const char* path, SDL_Color aColor, int size);
	FontID LoadFontTTF(const char*, SDL_Color aColor, int size);

	Font* GetFont(FontID aFontID);

	std::map<FontID,Font*> fonts;
};


#endif // !TEXT
