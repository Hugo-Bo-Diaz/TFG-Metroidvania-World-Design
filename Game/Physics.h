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
	//physics

	SDL_Rect* collider;

	objectId type;

	virtual void Init() {};
	virtual bool Loop(float dt) { return true; };
	virtual bool Render() { return true; };

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
	bool IsSameTypeAs(std::type_index& aInfo)
	{
		return (aInfo == GetTypeInfo());
	}
	template<typename T>
	bool IsSameTypeAs() {
		return (dynamic_cast<T*>(this) != NULL);
	}

	template<typename T>
	static std::type_index GetTypeInfo()
	{
		return std::type_index(typeid(T));
	}

	virtual std::type_index GetTypeInfo() = 0;
};

struct collision
{
	objectId type;
	GameObject* object;
};



class Physics : public Part
{
public:

	Physics();

	bool Init();
	bool Loop(float dt);
	bool CleanUp();
	bool Clearphysics();
	void DeleteObject(GameObject*);
	void GetNearbyWalls(int x, int y, int pxls_range, std::vector<SDL_Rect*>& colliders_near);

	std::vector<GameObject*>* GetAllObjectsOfType(std::type_index);
	std::vector<GameObject*>* GetAllObjectsOfType();

	void GetCollisions(SDL_Rect* rect,std::vector<collision*>&collisions);
	void ClearCollisionArray(std::vector<collision*>&collisions);

	int AddWall(SDL_Rect rect);
	void DeleteWall(int id);

	bool is_paused = false;
	void PauseObjects() { is_paused = true; };
	void UnPauseObjects() { is_paused = false; };

	//GameObject* AddObject(int x, int y, int w_col, int h_col, objectId type, std::map<std::string, float>* properties = nullptr);
	GameObject* AddObject(int x, int y, int w_col, int h_col,std::type_index lType);
	void AddObject(GameObject*);

	SDL_Rect* walls[MAX_WALLS];

	std::map<std::type_index, std::function<GameObject * (std::list<ObjectProperty*>&)>> lFactoriesType;
	std::map<std::string, std::function<GameObject * (std::list<ObjectProperty*>&)>> lFactoriesString;
	bool AddFactory(const char* lNameInMap, std::type_index lType, std::function<GameObject * (std::list<ObjectProperty*>&)> lFactory);

private:

	std::list<GameObject*> objects;
	std::list<GameObject*> to_delete;
};

#endif // !PHYSICS__H
