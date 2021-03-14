#include "Gui.h"
#include "UIimage.h"
#include "UIhealthbar.h"
#include "UIingameUI.h"
#include "UIspelldisplay.h"

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

UIelement * UserInterface::Add_GameUI(physobj * player)
{
	UIingameui* ret = new UIingameui((Player*)player);

	elements.push_back(ret);

	return ret;
}

UIelement * UserInterface::Add_ResourceBar(float x, float y, playerresource res)
{
	UIhealthbar* ret = new UIhealthbar(res);

	ret->x = x;
	ret->y = y;

	elements.push_back(ret);

	return ret;
}

UIelement * UserInterface::AddSpellDisplay(float x, float y)
{
	UIspelldisplay* ret = new UIspelldisplay();

	ret->x = x;
	ret->y = y;

	elements.push_back(ret);

	return ret;
}
