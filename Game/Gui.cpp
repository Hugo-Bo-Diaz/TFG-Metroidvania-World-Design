#include "Gui.h"
#include "UIimage.h"

UserInterface::UserInterface()
{
	name = "UserInterface";
}

bool UserInterface::Init()
{
	return true;
}

bool UserInterface::Loop(float dt)
{
	for (std::vector<UIelement*>::iterator it = elements.begin(); it != elements.end(); it++)
	{
		(*it)->Loop();
	}

	for (std::vector<UIelement*>::iterator it = elements.begin(); it != elements.end(); it++)
	{
		(*it)->Render();
	}

	return true;
}

bool UserInterface::CleanUp()
{
	for (std::vector<UIelement*>::iterator it = elements.begin(); it != elements.end(); it++)
	{
		delete (*it);
	}

	elements.clear();

	return true;
}

void UserInterface::RemoveElement(UIelement * to_delete)
{
}

UIelement * UserInterface::Add_Image(float x, float y, float w, float h, float depth, const char * texture)
{
	UIimage* ret = new UIimage(x, y, w, h, depth, texture);

	elements.push_back(ret);

	return ret;
}
