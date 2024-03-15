#include "Modules/Gui.h"
#include "Modules/Input.h"
#include "Modules/SceneController.h"

#include "Utils/Logger.h"
#include "GuiImpl.h"

UserInterface::UserInterface(EngineAPI& aAPI):Part("UserInterface",aAPI)
{
	mPartFuncts = new GuiImpl(this);
}

bool UserInterface::GuiImpl::Init()
{
	return true;
}

bool UserInterface::GuiImpl::Loop(float dt)
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

void UserInterface::GuiImpl::RenderDebug()
{
	for (std::vector<UIelement*>::iterator it = elements.begin(); it != elements.end(); it++)
	{
		(*it)->RenderDebug();
	}

}

bool UserInterface::GuiImpl::CleanUp()
{
	for (std::vector<UIelement*>::iterator it = elements.begin(); it != elements.end(); it++)
	{
		//delete(*it)->Engine;
		delete (*it);
	}

	elements.clear();

	return true;
}

void UserInterface::GuiImpl::Clearelements()
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
	GuiImpl* lImpl = dynamic_cast<GuiImpl*>(mPartFuncts);
	if (!lImpl)
	{
		Logger::Console_log(LogLevel::LOG_ERROR, "Wrong format on the implementation class");
		return;
	}

	bool is_in_array = false;

	for (std::vector<UIelement*>::iterator it = lImpl->to_delete.begin(); it != lImpl->to_delete.end(); it++)
	{
		if ((*it) == _to_delete)
			is_in_array = true;
	}

	if(!is_in_array)
		lImpl->to_delete.push_back(_to_delete);
}

bool UserInterface::ElementExists(UIelement * to_check)
{
	GuiImpl* lImpl = dynamic_cast<GuiImpl*>(mPartFuncts);
	if (!lImpl)
	{
		Logger::Console_log(LogLevel::LOG_ERROR, "Wrong format on the implementation class");
		return false;
	}

	bool is_in_array = false;

	for (std::vector<UIelement*>::iterator it = lImpl->elements.begin(); it != lImpl->elements.end(); it++)
	{
		if ((*it) == to_check)
			return true;
	}
	
	return false;
}

bool UserInterface::AddElement(UIelement* uiElement)
{
	if (uiElement == nullptr)
		return false;

	GuiImpl* lImpl = dynamic_cast<GuiImpl*>(mPartFuncts);
	if (!lImpl)
	{
		Logger::Console_log(LogLevel::LOG_ERROR, "Wrong format on the implementation class");
		return false;
	}

	uiElement->Engine = new EngineAPI(mApp);
	uiElement->Init();
	lImpl->elements.push_back(uiElement);
	return true;
}