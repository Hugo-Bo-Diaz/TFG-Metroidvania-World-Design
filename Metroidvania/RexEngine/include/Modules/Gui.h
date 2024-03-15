#ifndef GUI__H
#define GUI__H

#include "PartsDef.h"
#include<vector>
#include "EngineElements/UIelement.h"
#include "Part.h"
#include "ObjectManager.h"

enum TextBoxColor
{
	RED,
	GREY,
	WHITE
};

class DLL_EXPORT UserInterface : public Part
{
public:
	UserInterface(EngineAPI&);

	void LoadUIScene(const char* scene) {};

	void RemoveElement(UIelement* to_delete);
	bool ElementExists(UIelement* to_check);

	bool AddElement(UIelement* uiElement);


	//TEXT WORKS WITH A REFERENCE TO A STRUCT THAT CAN GIVE AN SDL_TEXTURE
	class GuiImpl;
};

#endif // !CAMERA__H

