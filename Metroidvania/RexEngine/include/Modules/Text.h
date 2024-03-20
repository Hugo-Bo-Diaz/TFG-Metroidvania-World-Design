#ifndef TEXT__H
#define TEXT__H

#include "PartsDef.h"
#include "Part.h"
#include <string>

#include "Textures.h"
#include "RXColor.h"

#define DEFAULT_FONT "Assets/font1.png"
#define DEFAULT_FONT_SIZE 33

#define XMLFONTEXTENSION "xml"
#define TTFFONTEXTENSION "ttf"


typedef int FontID;
//module that manages the fonts and texts
class DLL_EXPORT Text : public Part
{
public:
	Text(EngineAPI& aAPI);

	//list of supported characters in the engine
	const std::string mSupportedChars = ":abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890/.,'";
	
	//loads a font of a supported character format (xml definition or TTF)
	FontID LoadFont(const char* path, const RXColor& aColor, int size = 15);
	//returns the size of a text in that font
	void GetTextSize(FontID aFontID, const char* string, int& w, int& y);

	class TextImpl;
};


#endif // !TEXT
