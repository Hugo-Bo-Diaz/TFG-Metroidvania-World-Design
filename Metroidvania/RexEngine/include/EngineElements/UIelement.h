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
/*this is the main class that should be INHERITED to create custom behaviours of UI elements
IMPORTANT: during the constructor and destructor the object's reference to the engine has not been initialized,
please use the Init and Destroy function to manage engine functionality
*/
class DLL_EXPORT UIelement
{
public:
	//position on the global scale x
	float global_x;
	//position on the global scale y
	float global_y;

	//position relative to the father object in x
	float x;
	//position relative to the father object in y
	float y;

	//priority when rendering
	float depth;

	//space that it occupies on screen, it is required to get the debug view of the object
	RXRect screenspace;

	//function that is called right after the engine creates the gameobject
	virtual void Init();
	//function that is called every frame, it gives the Delta Time as an argument
	virtual void Loop();
	//function that is called every frame, especially for rendering events
	virtual void Render();
	//function that is called every frame when the debug visualization is active
	virtual void RenderDebug() {};
	//function right before the engine reference of the object is destroyed
	virtual void Destroy() {};

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
