#ifndef UI__ELEMENT
#define UI__ELEMENT

#include "RXRect.h"
#include <list>

#define DLL_EXPORT __declspec(dllexport)

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

class EngineAPI;
class DLL_EXPORT UIelement
{
public:

	float global_x;
	float global_y;

	float x;
	float y;

	float depth;

	RXRect screenspace;

	virtual void Init();
	virtual void Loop();
	virtual void Render();
	virtual void RenderDebug() {};

	UIelement* parent;

	std::list<UIelement*> children;
	void AddChild(UIelement* element);
	void RemoveChild(UIelement* element);

	std::list<UIeventType> events;

	EngineAPI* Engine = nullptr;

	UIelement() {};
	virtual ~UIelement();
};

#endif // !UI__ELEMENT
