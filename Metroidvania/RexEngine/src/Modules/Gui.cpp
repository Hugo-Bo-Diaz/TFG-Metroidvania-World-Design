#include "Modules/Gui.h"
#include "Modules/Input.h"
#include "Modules/SceneController.h"

#include "Utils/Logger.h"
#include "GuiImpl.h"

UserInterface::UserInterface(EngineAPI& aAPI):Part("UserInterface",aAPI)
{
	mPartFuncts = new GuiImpl(this);
}

#pragma region IMPLEMENTATION

bool UserInterface::GuiImpl::Loop(float dt)
{
	//delete elements
	for (std::unordered_set<UIelement*>::iterator it = to_delete.begin(); it != to_delete.end(); it++)
	{
		(*it)->Destroy();
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
	Clearelements();
	return true;
}

void UserInterface::GuiImpl::Clearelements()
{
	Logger::Console_log(LogLevel::LOG_INFO, "Clearing UI elements");
	for (std::vector<UIelement*>::iterator it = elements.begin(); it != elements.end(); it++)
	{
		(*it)->Destroy();
		delete(*it)->Engine;
		delete (*it);
	}
	to_delete.clear();
	elements.clear();
}

#pragma endregion

#pragma region PUBLIC API

void UserInterface::RemoveElement(UIelement * _to_delete)
{
	GuiImpl* lImpl = dynamic_cast<GuiImpl*>(mPartFuncts);
	if (!lImpl)
	{
		Logger::Console_log(LogLevel::LOG_ERROR, "Wrong format on the implementation class");
		return;
	}

	lImpl->to_delete.insert(_to_delete);
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
	return (std::find(lImpl->elements.begin(), lImpl->elements.end(), to_check) != lImpl->elements.end());
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

	if (!ElementExists(uiElement))
	{
		lImpl->elements.push_back(uiElement);
		return true;
	}
	else
	{
		return false;
	}
}

#pragma endregion