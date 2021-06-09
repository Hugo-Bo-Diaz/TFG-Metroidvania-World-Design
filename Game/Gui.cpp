#include "Gui.h"
#include "UImainmenu.h"
#include "UIPauseMenu.h"
#include "UISettingsMenu.h"
#include "UIimage.h"
#include "UIhealthbar.h"
#include "UIingameUI.h"
#include "UIspelldisplay.h"
#include "UItextbox.h"
#include "UISelectMenu.h"
#include "UICheckpointIndicator.h"

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
	//delete elements
	for (std::vector<UIelement*>::iterator it = to_delete.begin(); it != to_delete.end(); it++)
	{
		delete (*it);
		elements.erase(std::find(elements.begin(), elements.end(), *it));
	}
	to_delete.clear();

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

void UserInterface::Clearelements()
{
	for (std::vector<UIelement*>::iterator it = elements.begin(); it != elements.end(); it++)
	{
				delete (*it);
	}
	to_delete.clear();
	elements.clear();
}

void UserInterface::RemoveElement(UIelement * _to_delete)
{
	bool is_in_array = false;

	for (std::vector<UIelement*>::iterator it = to_delete.begin(); it != to_delete.end(); it++)
	{
		if ((*it) == _to_delete)
			is_in_array = true;
	}

	if(!is_in_array)
		to_delete.push_back(_to_delete);
}

bool UserInterface::ElementExists(UIelement * to_check)
{
	bool is_in_array = false;

	for (std::vector<UIelement*>::iterator it = to_delete.begin(); it != to_delete.end(); it++)
	{
		if ((*it) == to_check)
			is_in_array = true;
	}
	
	return is_in_array;
}

UIelement * UserInterface::AddMainMenu()
{
	UIMainMenu* ret = new UIMainMenu();

	elements.push_back(ret);

	return ret;
}

UIelement * UserInterface::AddPauseMenu()
{
	UIPauseMenu*ret = new UIPauseMenu();

	ret->x = 320;
	ret->y = 64;
	elements.push_back(ret);

	return ret;
}

UIelement * UserInterface::AddSettingsMenu()
{
	UISettingsMenu*ret = new UISettingsMenu();

	ret->x = 320;
	ret->y = 64;
	elements.push_back(ret);

	return ret;
}

UIelement * UserInterface::AddSelectMenu()
{
	UISelectMenu* ret = new UISelectMenu();

	ret->x = 106;
	ret->y = 64;

	elements.push_back(ret);

	return ret;
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

UIelement * UserInterface::AddTextBox(const char * author, const char * text, TextBoxColor color, int tiles_x, int tiles_y, int x, int y, float size, float speed)
{
	UItextbox* ret = new UItextbox(author, text, color,tiles_x,tiles_y, x,y,size,speed);

	elements.push_back(ret);

	return ret;
}

UIelement * UserInterface::AddCheckpointIndicator()
{
	UIcheckpointIndicator* ret = new UIcheckpointIndicator();

	elements.push_back(ret);

	return ret;
}
