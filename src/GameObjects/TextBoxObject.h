#ifndef TEXT__BOX__OBJECT__H
#define TEXT__BOX__OBJECT__H

#include "Modules/ObjectManager.h"
#include "Modules/Gui.h"
#include "Modules/Textures.h"

class UItextbox;

class TextBoxObject :
	public GameObject
{
public:
	TextBoxObject();
	TextBoxObject(std::list<ObjectProperty*>&);
	void Init();
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

	TextureID texture;
};

#endif