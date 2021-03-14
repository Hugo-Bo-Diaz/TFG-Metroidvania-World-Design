#ifndef TEXT
#define TEXT

#include "PartsDef.h"
#include "SDL\include\SDL_pixels.h"

#include <vector>
#include <string>

#define DEFAULT_FONT "Assets/font1.png"
#define DEFAULT_FONT_SIZE 33

struct SDL_Texture;
struct _TTF_Font;

struct Font
{
	std::string name;
	SDL_Texture* font_texture;
	int hsize;
	int vsize;
	int char_per_row;
};

struct TextPrint
{
	//PARAMS
	std::string text;
	SDL_Color color;
	Font* font_used;
	int max_width;
	int scale;

	//RESULTS
	uint id;
	int current_letter;
};


class Text : public Part
{
public:
	Text();

	bool LoadConfig(pugi::xml_node&);
	bool CreateConfig(pugi::xml_node&);

	virtual bool Init();
	virtual bool CleanUp();

	Font* LoadFont(const char* path, const char* name, int char_per_row);
	TextPrint* CreateText(const char* text, SDL_Color color, int max_width, const char* font = "",int scale = 1);
	Font* GetFont(const char* name);
	void DeleteText(TextPrint* to_delete);
	void ChangeText(TextPrint* to_change, const char* new_text);

	std::vector<TextPrint*> texts;
	std::vector<Font*> fonts;
	Font* default_font;
};


#endif // !TEXT
