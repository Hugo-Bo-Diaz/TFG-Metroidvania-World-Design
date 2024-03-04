#include "EngineElements/UIelement.h"
#include "EngineAPI.h"

void UIelement::Init()
{
	for (std::list<UIelement*>::iterator it = children.begin(); it != children.end(); it++)
	{
		(*it)->Engine = new EngineAPI(*Engine);
		(*it)->Init();
	}
}

void UIelement::Loop()
{
	for (std::list<UIelement*>::iterator it = children.begin(); it != children.end(); it++)
	{
		(*it)->Loop();
	}
}

void UIelement::Render()
{
	for (std::list<UIelement*>::iterator it = children.begin(); it != children.end(); it++)
	{
		(*it)->Render();
	}
}

void UIelement::AddChild(UIelement * element)
{
	element->x = x - element->x;
	element->y = y - element->y;
	element->parent = this;
	
	children.push_back(element);
}

void UIelement::RemoveChild(UIelement * element)
{

	//x and y are global now
}

UIelement::~UIelement()
{
	for (std::list<UIelement*>::iterator it = children.begin(); it != children.end(); it++)
	{
		delete *it;
	}
}
