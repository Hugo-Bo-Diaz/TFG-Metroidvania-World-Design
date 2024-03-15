#ifndef GAMEOBJECT__H
#define GAMEOBJECT__H

#include "EngineAPI.h"
#include "RXRect.h"
#include <typeindex>

class ObjectManager;
class SceneController;
class DLL_EXPORT GameObject
{
public:
	RXRect* collider;
	std::type_index mType = std::type_index(typeid(*this));

	virtual void Init() {};
	virtual bool Loop(float dt) { return true; };
	virtual bool Render() { return true; };
	virtual void RenderDebug() {};
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