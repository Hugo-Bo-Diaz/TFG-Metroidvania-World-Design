#ifndef UI_TEXTBOX
#define UI_TEXTBOX

#include "EngineElements/ParticleEmitter.h"
#include "Modules/Textures.h"

#include "Modules/Text.h"
#include "Modules/Gui.h"
#include <string>
#include <list>

class UItextbox : public UIelement
{
public:
	
	int current_text = 0;

	FontID mFont;
	
	std::vector<std::string>texts;
	std::string author;

	int plus_rect_y;

	int tile_width;
	int tile_height;

	int position_y;
	int position_x;

	float text_speed;
	float text_size;

	UItextbox(const char* author,const char* first_text, TextBoxColor color, int tiles_x, int tiles_y, int x, int y, float size = 1, float speed = 1);
	void AddPanelToTextBox(const char* text);

	~UItextbox();

	void Init();
	void Loop();
	void Render();

	float current_letter;

	RXRect up_left =		{ 0, 0, 32,32 };
	RXRect left =			{ 0, 32,32,32 };
	RXRect down_left =	{ 0, 64,32,32 };
	RXRect down =			{ 32,64,32,32 };
	RXRect down_right =	{ 64,64,32,32 };
	RXRect right =		{ 64,32,32,32 };
	RXRect up_right =		{ 64, 0,32,32 };
	RXRect up =			{ 32, 0,32,32 };
	RXRect center =		{ 32,32,32,32 };

	RXRect small_left =	{ 96, 0,32,48 };
	RXRect small_center =	{ 128,0,32,48 };
	RXRect small_right =	{ 160,0,32,48 };

	TextureID TexTextBox;
};

#endif