#ifndef UI__ELEMENT
#define UI__ELEMENT

#include "SDL/include/SDL.h"
#include <list>

enum UItype
{
	TEXT_EL,
	WINDOW_EL,
	IMAGE_EL,
};


enum UIeventType
{
	ONCLICKDOWN,
	ONACTIVATION,
	ONMOUSEOVER,
	ONMOUSELEAVE
};

class UIelement 
{
public:

	float global_x;
	float global_y;

	float x;
	float y;

	float depth;

	SDL_Rect screenspace;

	virtual void Loop();
	virtual void Render();
	virtual void RenderDebug() {};

	UIelement* parent;

	std::list<UIelement*> children;
	void AddChild(UIelement* element);
	void RemoveChild(UIelement* element);

	std::list<UIeventType> events;


	UIelement() {};
	virtual ~UIelement();
};

#endif // !UI__ELEMENT
