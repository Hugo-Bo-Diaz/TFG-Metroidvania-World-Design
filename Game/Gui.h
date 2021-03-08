#ifndef GUI__H
#define GUI__H

#include "PartsDef.h"
#include<vector>
#include "UIelement.h"

#include "SDL/include/SDL.h"

class UserInterface : public Part
{
public:
	UserInterface();

	bool Init();
	bool Loop(float dt);
	bool CleanUp();

	std::vector<UIelement*> elements;

	void LoadUIScene(const char* scene) {};

	void RemoveElement(UIelement* to_delete);

	UIelement* Add_Image(float x, float y, float w, float h, float depth, const char* texture);


	//TEXT WORKS WITH A REFERENCE TO A STRUCT THAT CAN GIVE AN SDL_TEXTURE
};

#endif // !CAMERA__H

