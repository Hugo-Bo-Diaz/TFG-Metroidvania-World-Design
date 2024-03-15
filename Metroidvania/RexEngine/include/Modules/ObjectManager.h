#ifndef OBJECT_MANAGER__H
#define OBJECT_MANAGER__H

#include "PartsDef.h"
#include "Part.h"
#include <list>
#include <vector>
#include <map>
#include <unordered_set>

#include <functional>
#include <typeindex>
#include "../EngineElements/GameObject.h"

#define MAX_WALLS 500

struct ObjectProperty
{
	std::string name;
	std::string str_value;
	float num_value;
	bool bool_value;
};


class DLL_EXPORT FactoryBase
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
class DLL_EXPORT Factory : public FactoryBase
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


class DLL_EXPORT ObjectManager : public Part
{
public:

	ObjectManager(EngineAPI& aAPI);

	bool Clearphysics();
	void DeleteObject(GameObject*);
	void GetNearbyWalls(int x, int y, int pxls_range, std::vector<RXRect*>& colliders_near);

	std::vector<GameObject*>* GetAllObjectsOfType(std::type_index);

	void GetCollisions(RXRect* rect,std::vector<collision*>&collisions);
	void ClearCollisionArray(std::vector<collision*>&collisions);

	int AddWall(RXRect& rect);
	void DeleteWall(int id);

	void PauseObjects();
	void UnPauseObjects();
	bool isPaused();

	GameObject* AddObject(int x, int y, int w_col, int h_col,std::type_index lType);
	void AddObject(GameObject*);

	int GetTotalObjectNumber();

	bool AddFactory(FactoryBase* lFactory);

	class ObjectManagerImpl;
	friend class SceneManager;
};

#endif // !PHYSICS__H
