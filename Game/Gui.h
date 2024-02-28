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

class UserInterface : public Part
{
public:
	UserInterface();

	bool Init();
	bool Loop(float dt);
	void RenderDebug();
	bool CleanUp();
	void Clearelements();

	std::vector<UIelement*> elements;
	std::vector<UIelement*> to_delete;

	void LoadUIScene(const char* scene) {};

	void RemoveElement(UIelement* to_delete);
	bool ElementExists(UIelement* to_check);

	bool AddElement(UIelement* uiElement);


	//TEXT WORKS WITH A REFERENCE TO A STRUCT THAT CAN GIVE AN SDL_TEXTURE
};

#endif // !CAMERA__H

