#ifndef PHYSICS__H
#define PHYSICS__H

#include "PartsDef.h"
#include "Textures.h"
#include "Application.h"
#include "Render.h"

#include <list>
#include <vector>
#include <map>

#include "SDL/include/SDL.h"
#include <functional>
#include <typeindex>

#define MAX_WALLS 500

struct SDL_Texture;


enum texture_type
{
	PLAYER_TEX,
	NUMBER_OF_TEXTURES
};


struct ObjectProperty
{
	std::string name;
	std::string str_value;
	float num_value;
	bool bool_value;
};

class GameObject
{
public:
	SDL_Rect* collider;
	std::type_index mType = std::type_index(typeid(*this));

	virtual void Init() {};
	virtual bool Loop(float dt) { return true; };
	virtual bool Render() { return true; };
	virtual void RenderDebug() {};

	GameObject()
	{
		collider = new SDL_Rect({ 0,0,0,0 });
	}
	~GameObject()
	{
	}
	//visuals
	virtual void Destroy()
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
};

class FactoryBase
{
public:
	FactoryBase() {};
	FactoryBase(const char* nameInMap){};

	virtual GameObject* CreateInstace() { return nullptr; };
	virtual GameObject* CreateInstace(std::list<ObjectProperty*>&) { return nullptr; };

	virtual std::string GetObjectMapName() { return "ERRORTYPE"; };
	virtual std::type_index GetObjectTypeIndex() { return std::type_index(typeid(this)); };
};
template<typename T> 
class Factory : public FactoryBase
{
private:
	std::string mNameInMap;
public:
	Factory() {};
	Factory(const char* nameInMap) :mNameInMap(nameInMap) {};

	GameObject* CreateInstace() { return new T(); };
	GameObject* CreateInstace(std::list<ObjectProperty*>& lProps)
	{
		return new T(lProps); 
	};

	std::type_index GetObjectTypeIndex() { return std::type_index(typeid(T)); };
	std::string GetObjectMapName() { return mNameInMap; };
};

struct collision
{
	GameObject* object;
};


class Physics : public Part
{
private:

	std::list<GameObject*> objects;
	std::list<GameObject*> to_delete;
public:

	Physics();

	bool Init();
	bool Loop(float dt);
	void RenderDebug();
	bool CleanUp();
	bool Clearphysics();
	void DeleteObject(GameObject*);
	void GetNearbyWalls(int x, int y, int pxls_range, std::vector<SDL_Rect*>& colliders_near);

	std::vector<GameObject*>* GetAllObjectsOfType(std::type_index);

	void GetCollisions(SDL_Rect* rect,std::vector<collision*>&collisions);
	void ClearCollisionArray(std::vector<collision*>&collisions);

	int AddWall(SDL_Rect rect);
	void DeleteWall(int id);

	bool is_paused = false;
	void PauseObjects() { is_paused = true; };
	void UnPauseObjects() { is_paused = false; };

	GameObject* AddObject(int x, int y, int w_col, int h_col,std::type_index lType);
	void AddObject(GameObject*);

	int GetTotalObjectNumber() { return objects.size(); };

	SDL_Rect* walls[MAX_WALLS];

	std::list<FactoryBase*> mFactories;

	FactoryBase* GetFactory(const char* aNameInMap);
	FactoryBase* GetFactory(std::type_index& aType);

	bool AddFactory(FactoryBase* lFactory);

};

#endif // !PHYSICS__H
