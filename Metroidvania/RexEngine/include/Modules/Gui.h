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
//module that manages the user interface
class DLL_EXPORT UserInterface : public Part
{
public:
	UserInterface(EngineAPI&);

	//Loads a scene meant to hold UI
	void LoadUIScene(const char* scene) {};

	//Removes an element from the engine
	void RemoveElement(UIelement* to_delete);
	//returns true if element exists
	bool ElementExists(UIelement* to_check);

	//adds UI element to the engine
	bool AddElement(UIelement* uiElement);

	class GuiImpl;
};

#endif // !CAMERA__H

