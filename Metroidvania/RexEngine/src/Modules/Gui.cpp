#include "Modules/Gui.h"
#include "Modules/Input.h"
#include "Modules/SceneController.h"

#include "Utils/Logger.h"

UserInterface::UserInterface(EngineAPI& aAPI):Part("UserInterface",aAPI)
{}

bool UserInterface::Init()
{
	return true;
}

bool UserInterface::Loop(float dt)
{
	//delete elements
	for (std::vector<UIelement*>::iterator it = to_delete.begin(); it != to_delete.end(); it++)
	{
		delete(*it)->Engine;
		delete (*it);
		elements.erase(std::find(elements.begin(), elements.end(), *it));
	}
	to_delete.clear();

	//DO IT WITH 
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

void UserInterface::RenderDebug()
{
	for (std::vector<UIelement*>::iterator it = elements.begin(); it != elements.end(); it++)
	{
		(*it)->RenderDebug();
	}

}

bool UserInterface::CleanUp()
{
	for (std::vector<UIelement*>::iterator it = elements.begin(); it != elements.end(); it++)
	{
		delete(*it)->Engine;
		delete (*it);
	}

	elements.clear();

	return true;
}

void UserInterface::Clearelements()
{
	Logger::Console_log(LogLevel::LOG_INFO, "Clearing UI elements");
	for (std::vector<UIelement*>::iterator it = elements.begin(); it != elements.end(); it++)
	{
		delete(*it)->Engine;
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

	for (std::vector<UIelement*>::iterator it = elements.begin(); it != elements.end(); it++)
	{
		if ((*it) == to_check)
			is_in_array = true;
	}
	
	return is_in_array;
}

bool UserInterface::AddElement(UIelement* uiElement)
{
	if (uiElement == nullptr)
		return false;

	uiElement->Engine = new EngineAPI(mApp);
	uiElement->Init();
	elements.push_back(uiElement);
	return true;
}