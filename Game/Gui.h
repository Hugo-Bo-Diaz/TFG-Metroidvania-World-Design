#ifndef GUI__H
#define GUI__H

#include "PartsDef.h"
#include<vector>
#include "UIelement.h"

#include "SDL/include/SDL.h"
#include "Physics.h"

enum TextBoxColor
{
	RED,
	GREY,
	WHITE
};

enum playerresource;

class UserInterface : public Part
{
public:
	UserInterface();

	bool Init();
	bool Loop(float dt);
	bool CleanUp();
	void Clearelements();

	std::vector<UIelement*> elements;
	std::vector<UIelement*> to_delete;

	void LoadUIScene(const char* scene) {};

	void RemoveElement(UIelement* to_delete);
	bool ElementExists(UIelement* to_check);

	bool canopenmenu = true;

	UIelement* AddMainMenu();
	UIelement* AddPauseMenu();
	UIelement* AddSettingsMenu();
	UIelement* AddSelectMenu();

	UIelement* Add_Image(float x, float y, float w, float h, float depth, const char* texture);
	UIelement* Add_GameUI(physobj* player);
	UIelement* Add_ResourceBar(float x, float y, playerresource res);
	UIelement* AddSpellDisplay(float x, float y);
	UIelement* AddTextBox(const char* author, const char* text, TextBoxColor color, int tiles_x, int tiles_y, int x, int y, float size = 1, float speed = 1);
	UIelement* AddCheckpointIndicator();


	//TEXT WORKS WITH A REFERENCE TO A STRUCT THAT CAN GIVE AN SDL_TEXTURE
};

#endif // !CAMERA__H

