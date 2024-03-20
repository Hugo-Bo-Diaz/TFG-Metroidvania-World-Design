#ifndef GAMEOBJECT__H
#define GAMEOBJECT__H

#include "EngineAPI.h"
#include "RXRect.h"
#include <typeindex>

class ObjectManager;
class SceneController;

/*this is the main class that should be INHERITED to create custom behaviours of objects
IMPORTANT: during the constructor and destructor the object's reference to the engine has not been initialized,
please use the Init and Destroy function to manage engine functionality
*/
class DLL_EXPORT GameObject
{
public:
	//the collider is the main presence of the object inside of the engine
	RXRect* collider;
	std::type_index mType = std::type_index(typeid(*this));

	//function that is called right after the engine creates the gameobject
	virtual void Init() {};
	//function that is called every frame, it gives the Delta Time as an argument
	virtual bool Loop(float dt) { return true; };
	//function that is called every frame, especially for rendering events
	virtual bool Render() { return true; };
	//function that is called every frame when the debug visualization is active
	virtual void RenderDebug() {};
	//function right before the engine reference of the object is destroyed
	virtual void Destroy() {};

	GameObject()
	{
		collider = new RXRect({ 0,0,0,0 });
	}
	//visuals
	~GameObject()
	{
		if (collider != nullptr)
		{
			delete collider;
			collider = nullptr;
		}
	}

	bool IsSameTypeAs(GameObject* lG)
	{
		return (typeid(lG) == typeid(this));
	}

	template<typename T>
	bool IsSameTypeAs() {
		return (dynamic_cast<T*>(this) != NULL);
	}

	friend class ObjectManager;
	friend class SceneController;
protected:
	EngineAPI* Engine = nullptr;
};

#endif