#ifndef OBJECT_MANAGER_IMPL__H
#define OBJECT_MANAGER_IMPL__H

#include "../include/Modules/ObjectManager.h"
#include "EngineElements/GameObject.h"
#include "PartImpl.h"

class ObjectManager::ObjectManagerImpl : public Part::Part_Impl
{
public:

	ObjectManager::ObjectManagerImpl(ObjectManager* aObjectManager) :mPartInst(aObjectManager) {};

	void SetObjectManagerInst(ObjectManager* aObjectManager)
	{
		mPartInst = aObjectManager;
	}
	FactoryBase* GetFactory(std::type_index& aType);
	FactoryBase* GetFactory(const char* aNameInMap);
	void RenderDebug();

protected:
	bool Init();
	bool Loop(float dt);
	bool CleanUp();

private:

	bool is_paused = false;
	RXRect* walls[MAX_WALLS];

	std::list<FactoryBase*> mFactories;
	std::list<GameObject*> objects;
	std::unordered_set<GameObject*> to_delete;

	friend class ObjectManager;

	ObjectManager* mPartInst;
};


#endif