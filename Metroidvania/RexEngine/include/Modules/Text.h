#ifndef TEXT__H
#define TEXT__H

#include "PartsDef.h"
#include "Part.h"
#include <vector>
#include <string>
#include <map>

#include "Textures.h"
#include "RXColor.h"

#define DEFAULT_FONT "Assets/font1.png"
#define DEFAULT_FONT_SIZE 33

#define XMLFONTEXTENSION "xml"
#define TTFFONTEXTENSION "ttf"


typedef int FontID;

class DLL_EXPORT Text : public Part
{
public:
	Text(EngineAPI& aAPI);

	const std::string mSupportedChars = ":abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890/.,'";
	FontID LoadFont(const char* path, const RXColor& aColor, int size = 15);
	void GetTextSize(FontID aFontID, const char* string, int& w, int& y);

	class TextImpl;
};


#endif // !TEXT
