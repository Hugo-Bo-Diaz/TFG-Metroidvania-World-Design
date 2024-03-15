#ifndef TEXT_IMPL__H
#define TEXT_IMPL__H

#include "PartImpl.h"
#include "../include/Modules/Text.h"
#include "SDL/include/SDL_rect.h"

struct _TTF_Font;

struct Font
{
	std::string name;
	TextureID font_texture;

	std::map<char, SDL_Rect*> lMapping;

	int size;
	int char_per_row;
	RXColor color;

	Font(const char* aName, TextureID aTexture,const RXColor& aColor, int aSize)
		:name(aName), font_texture(aTexture), color(aColor), size(aSize)
	{}
};

class Text::TextImpl : public Part::Part_Impl
{
public:
	Text::TextImpl(Text* aText) :mPartInst(aText) {};

	void SetTextInst(Text* aText)
	{
		mPartInst = aText;
	}

	Font* GetFont(FontID aFontID);
protected:
	bool LoadConfig(pugi::xml_node&);
	bool CreateConfig(pugi::xml_node&);

	virtual bool Init();
	virtual bool CleanUp();

private:
	FontID LoadFontXML(const char* path, const RXColor& aColor, int size);
	FontID LoadFontTTF(const char*, const RXColor& aColor, int size);

	std::map<FontID, Font*> fonts;
	friend class Text;

	Text* mPartInst;
};
#endif