#ifndef TEXT__BOX__OBJECT__H
#define TEXT__BOX__OBJECT__H
#include "Physics.h"
#include "Gui.h"
class UItextbox;

class TextBoxObject :
	public GameObject
{
public:
	TextBoxObject();
	bool Loop(float dt);
	bool Render();
	~TextBoxObject();

	std::string author;

	std::vector<std::string> strings;
	UItextbox* textbox;
	
	void AddText(const char* text);

	TextBoxColor c = GREY;

	bool player_contact = false;
	bool textboxexists = false;

	int lore_unlock = -1;

	SDL_Rect rectanglekeyboard;
	SDL_Rect rectanglecontroller;
};

#endif