#include "Modules/ObjectManager.h"
#include "Utils/Logger.h"
#include "Modules/Debug.h"
#include "Modules/Render.h"
#include "EngineElements/GameObject.h"

#include <sstream>
#include <typeindex>
#include "SDL/include/SDL.h"
#include "ObjectManagerImpl.h"


ObjectManager::ObjectManager(EngineAPI& aAPI) : Part("ObjectManager",aAPI)
{
	mPartFuncts = new ObjectManagerImpl(this);
}

bool ObjectManager::ObjectManagerImpl::Init()
{
	bool ret = true;

	for (int i = 0; i < MAX_WALLS; ++i)
	{
		walls[i] = nullptr;
	}

	return ret;
}

bool ObjectManager::ObjectManagerImpl::Loop(float dt)
{
	bool ret = true;

	if (!is_paused)
	{
		for (std::list<GameObject*>::iterator it = objects.begin(); it != objects.end(); it++)
		{
			if (!(*it)->Loop(dt))
			{
				ret = false;
			}
		}
	}
	
	for (std::list<GameObject*>::iterator it = objects.begin(); it != objects.end(); it++)
	{
		if (!(*it)->Render())
		{
			ret = false;
		}
	}
	//delete the current list
	for (std::unordered_set<GameObject*>::iterator it = to_delete.begin(); it != to_delete.end(); it++)
	{
		(*it)->Destroy();
		delete(*it);
		objects.erase(std::find(objects.begin(), objects.end(), *it));
		
	}
	to_delete.clear();

	return ret;
}

void ObjectManager::ObjectManagerImpl::RenderDebug()
{
	for (int i = 0; i < MAX_WALLS; ++i)
	{
		if (walls[i] != nullptr)
		{
			RXRect lRect = { walls[i]->x,walls[i]->y,walls[i]->w,walls[i]->h };
			mPartInst->mApp.GetModule<Render>().DrawRect(lRect, 0, 0, 255, 75, true, RenderQueue::RENDER_DEBUG, 0);
		}
	}

	for (std::list<GameObject*>::iterator it = objects.begin(); it != objects.end(); it++)
	{
		(*it)->RenderDebug();
		mPartInst->mApp.GetModule<Render>().DrawRect(*(*it)->collider, 0, 255, 0, 75, true, RenderQueue::RENDER_DEBUG, 0);
	}
}

void ObjectManager::GetNearbyWalls(int x, int y, int pxls_range, std::vector<RXRect*>& colliders_near)
{
	ObjectManagerImpl* lImpl = dynamic_cast<ObjectManagerImpl*>(mPartFuncts);
	if (!lImpl)
	{
		Logger::Console_log(LogLevel::LOG_ERROR, "Wrong format on the implementation class");
		return;
	}

	RXRect* toleration_area = new RXRect();
	*toleration_area = {x-pxls_range, y -pxls_range, pxls_range*2, pxls_range*2};

	//App->ren->DrawRect(toleration_area, 255, 0, 0, 100, true);

	for (int i = 0; i < MAX_WALLS; ++i)
	{
		if (RXRectCollision(lImpl->walls[i], toleration_area))
		{
			colliders_near.push_back(lImpl->walls[i]);
		}
	}
}
std::vector<GameObject*>* ObjectManager::GetAllObjectsOfType(std::type_index info)
{
	ObjectManagerImpl* lImpl = dynamic_cast<ObjectManagerImpl*>(mPartFuncts);
	if (!lImpl)
	{
		Logger::Console_log(LogLevel::LOG_ERROR, "Wrong format on the implementation class");
		return nullptr;
	}

	std::vector<GameObject*>* ret = new std::vector<GameObject*>();

	for (std::list<GameObject*>::iterator it = lImpl->objects.begin(); it != lImpl->objects.end(); it++)
	{
		if ((*it)->mType == info)
		{
			ret->push_back(*it);
		}
	}
	return ret;
}

void ObjectManager::GetCollisions(RXRect* obj, std::vector<collision*>& collisions)
{
	ObjectManagerImpl* lImpl = dynamic_cast<ObjectManagerImpl*>(mPartFuncts);
	if (!lImpl)
	{
		Logger::Console_log(LogLevel::LOG_ERROR, "Wrong format on the implementation class");
		return;
	}

	for (std::list<GameObject*>::iterator it = lImpl->objects.begin(); it != lImpl->objects.end(); it++)
	{
		if (RXRectCollision((*it)->collider,obj))
		{
			collision* col = new collision();

			col->object = *it;

			collisions.push_back(col);
		}
	}
}

void ObjectManager::ClearCollisionArray(std::vector<collision*>& collisions)
{

	for (std::vector<collision*>::iterator it = collisions.begin(); it != collisions.end(); it++)
	{
		delete *it;
	}
	collisions.clear();
}

GameObject* ObjectManager::AddObject(int x, int y, int w_col, int h_col,std::type_index lType)
{
	ObjectManagerImpl* lImpl = dynamic_cast<ObjectManagerImpl*>(mPartFuncts);
	if (!lImpl)
	{
		Logger::Console_log(LogLevel::LOG_ERROR, "Wrong format on the implementation class");
		return nullptr;
	}

	auto lID = lImpl->GetFactory(lType);

	std::list<ObjectProperty*> lPropList;

	//GameObject* r = (*lID).second(lPropList);

	GameObject* r = (*lID).CreateInstace();
	if (r != nullptr)
	{
		r->mType = lID->GetObjectTypeIndex();
		r->collider = new RXRect();

		r->Engine = new EngineAPI(mApp);
		r->collider->x = x;
		r->collider->y = y;
		r->collider->w = w_col;
		r->collider->h = h_col;

		r->Init();

		lImpl->objects.push_back(r);
	}
	else
	{
		std::stringstream str;
		str << "Attempted to create: " << lType.name() << " as GameObject, Invalid operation, please make sure that the class inherits from GameObject!";
		Logger::Console_log(LogLevel::LOG_ERROR, str.str().c_str());
	}

	return r;
}

int ObjectManager::GetTotalObjectNumber()
{
	ObjectManagerImpl* lImpl = dynamic_cast<ObjectManagerImpl*>(mPartFuncts);
	if (!lImpl)
	{
		Logger::Console_log(LogLevel::LOG_ERROR, "Wrong format on the implementation class");
		return 0;
	}

	return lImpl->objects.size();
}


void ObjectManager::AddObject(GameObject* lToAdd)
{
	ObjectManagerImpl* lImpl = dynamic_cast<ObjectManagerImpl*>(mPartFuncts);
	if (!lImpl)
	{
		Logger::Console_log(LogLevel::LOG_ERROR, "Wrong format on the implementation class");
		return;
	}

	if(lToAdd != nullptr)
		lImpl->objects.push_back(lToAdd);
}

int ObjectManager::AddWall(RXRect& rect)
{
	ObjectManagerImpl* lImpl = dynamic_cast<ObjectManagerImpl*>(mPartFuncts);
	if (!lImpl)
	{
		Logger::Console_log(LogLevel::LOG_ERROR, "Wrong format on the implementation class");
		return -1;
	}

	int i = 0;
	bool exit = false;

	while (i < MAX_WALLS && !exit)
	{
		if (lImpl->walls[i] == nullptr)
		{
			exit = true;

		}
		else
		{
			++i;
		}
	}

	
	RXRect* wall = new RXRect();
	wall->x = rect.x;
	wall->y = rect.y;
	wall->w = rect.w;
	wall->h = rect.h;

	lImpl->walls[i] = wall;

	return i;
}

void ObjectManager::DeleteWall(int id)
{
	ObjectManagerImpl* lImpl = dynamic_cast<ObjectManagerImpl*>(mPartFuncts);
	if (!lImpl)
	{
		Logger::Console_log(LogLevel::LOG_ERROR, "Wrong format on the implementation class");
		return;
	}

	delete lImpl->walls[id];
	lImpl->walls[id] = nullptr;
}

bool ObjectManager::AddFactory(FactoryBase* aFactory)
{
	ObjectManagerImpl* lImpl = dynamic_cast<ObjectManagerImpl*>(mPartFuncts);
	if (!lImpl)
	{
		Logger::Console_log(LogLevel::LOG_ERROR, "Wrong format on the implementation class");
		return false;
	}

	lImpl->mFactories.push_back(aFactory);
	return true;
}

FactoryBase* ObjectManager::ObjectManagerImpl::GetFactory(const char* aNameInMap)
{
	for (std::list<FactoryBase*>::iterator it = mFactories.begin(); it != mFactories.end(); it++)
	{
		if (strcmp((*it)->GetObjectMapName().c_str(), aNameInMap) == 0)
		{
			return (*it);
		}
	}
	return nullptr;
}
FactoryBase* ObjectManager::ObjectManagerImpl::GetFactory(std::type_index& aType)
{
	for (std::list<FactoryBase*>::iterator it = mFactories.begin(); it != mFactories.end(); it++)
	{
		if ((*it)->GetObjectTypeIndex()== aType)
		{
			return (*it);
		}
	}
	return nullptr;
}

bool ObjectManager::ObjectManagerImpl::CleanUp()
{
	mPartInst->Clearphysics();

	for (std::list<FactoryBase*>::iterator it = mFactories.begin(); it != mFactories.end(); it++)
	{
		delete *it;
	}
	return true;
}

bool ObjectManager::Clearphysics()
{
	ObjectManagerImpl* lImpl = dynamic_cast<ObjectManagerImpl*>(mPartFuncts);
	if (!lImpl)
	{
		Logger::Console_log(LogLevel::LOG_ERROR, "Wrong format on the implementation class");
		return false;
	}

	Logger::Console_log(LogLevel::LOG_INFO, "Clearing UI physics");
	bool ret = true;

	for (int i = 0; i < MAX_WALLS; ++i)
	{
		if (lImpl->walls[i] != nullptr)
		{
			delete lImpl->walls[i];
			lImpl->walls[i] = nullptr;
		}
	}

	for (std::list<GameObject*>::iterator it = lImpl->objects.begin(); it != lImpl->objects.end(); it++)
	{
		(*it)->Destroy();
		delete (*it)->Engine;
		delete *it;
	}
	lImpl->objects.clear();

	return ret;
}

void ObjectManager::DeleteObject(GameObject* _to_delete)
{
	ObjectManagerImpl* lImpl = dynamic_cast<ObjectManagerImpl*>(mPartFuncts);
	if (!lImpl)
	{
		Logger::Console_log(LogLevel::LOG_ERROR, "Wrong format on the implementation class");
		return;
	}
	//std::list<physobj*>::iterator obj_to_del;

	//for (std::list<physobj*>::iterator it = objects.begin(); it != objects.end(); it++)
	//{
	//	if (to_delete == *it)
	//	{
	//		obj_to_del = it;
	//	}
	//}
	//delete to_delete;
	lImpl->to_delete.insert(_to_delete);

}

bool ObjectManager::isPaused()
{
	ObjectManagerImpl* lImpl = dynamic_cast<ObjectManagerImpl*>(mPartFuncts);
	if (!lImpl)
	{
		Logger::Console_log(LogLevel::LOG_ERROR, "Wrong format on the implementation class");
		return true;
	}

	return lImpl->is_paused;
}

void ObjectManager::PauseObjects()
{
	ObjectManagerImpl* lImpl = dynamic_cast<ObjectManagerImpl*>(mPartFuncts);
	if (!lImpl)
	{
		Logger::Console_log(LogLevel::LOG_ERROR, "Wrong format on the implementation class");
		return;
	}

	lImpl->is_paused = true;
}

void ObjectManager::UnPauseObjects()
{
	ObjectManagerImpl* lImpl = dynamic_cast<ObjectManagerImpl*>(mPartFuncts);
	if (!lImpl)
	{
		Logger::Console_log(LogLevel::LOG_ERROR, "Wrong format on the implementation class");
		return;
	}

	lImpl->is_paused = false;
}