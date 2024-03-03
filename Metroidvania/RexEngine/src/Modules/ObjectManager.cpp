#include "Modules/ObjectManager.h"
#include "Utils/Logger.h"
#include "Modules/Debug.h"

#include <sstream>
#include <typeindex>

ObjectManager::ObjectManager(EngineAPI& aAPI) : Part("ObjectManager",aAPI)
{
	for (int i = 0; i < MAX_WALLS; ++i)
	{
		walls[i] = nullptr;
	}
}

bool ObjectManager::Init()
{
	bool ret = true;


	return ret;
}

bool ObjectManager::Loop(float dt)
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
	for (std::list<GameObject*>::iterator it = to_delete.begin(); it != to_delete.end(); it++)
	{
		(*it)->Destroy();
		delete(*it);
		objects.erase(std::find(objects.begin(), objects.end(), *it));
		
	}
	to_delete.clear();

	return ret;
}

void ObjectManager::RenderDebug()
{
	for (int i = 0; i < MAX_WALLS; ++i)
	{
		if (walls[i] != nullptr)
			mApp.GetModule<Render>().DrawRect(*walls[i], 0, 0, 255, 75, true, RenderQueue::RENDER_DEBUG, 0);
	}

	for (std::list<GameObject*>::iterator it = objects.begin(); it != objects.end(); it++)
	{
		(*it)->RenderDebug();
		mApp.GetModule<Render>().DrawRect(*(*it)->collider, 0, 255, 0, 75, true, RenderQueue::RENDER_DEBUG, 0);
	}
}

void ObjectManager::GetNearbyWalls(int x, int y, int pxls_range, std::vector<SDL_Rect*>& colliders_near)
{
	SDL_Rect* toleration_area = new SDL_Rect();
	*toleration_area = {x-pxls_range, y -pxls_range, pxls_range*2, pxls_range*2};

	//App->ren->DrawRect(toleration_area, 255, 0, 0, 100, true);

	for (int i = 0; i < MAX_WALLS; ++i)
	{
		if (SDL_HasIntersection(walls[i], toleration_area))
		{
			colliders_near.push_back(walls[i]);
		}
	}
}
std::vector<GameObject*>* ObjectManager::GetAllObjectsOfType(std::type_index info)
{
	std::vector<GameObject*>* ret = new std::vector<GameObject*>();

	for (std::list<GameObject*>::iterator it = objects.begin(); it != objects.end(); it++)
	{
		if ((*it)->mType == info)
		{
			ret->push_back(*it);
		}
	}
	return ret;
}

void ObjectManager::GetCollisions(SDL_Rect* obj, std::vector<collision*>& collisions)
{
	for (std::list<GameObject*>::iterator it = objects.begin(); it != objects.end(); it++)
	{
		if (SDL_HasIntersection((*it)->collider,obj))
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
	auto lID = GetFactory(lType);

	std::list<ObjectProperty*> lPropList;

	//GameObject* r = (*lID).second(lPropList);

	GameObject* r = (*lID).CreateInstace();
	if (r != nullptr)
	{
		r->mType = lID->GetObjectTypeIndex();
		r->collider = new SDL_Rect();

		r->Engine = new EngineAPI(mApp);
		r->collider->x = x;
		r->collider->y = y;
		r->collider->w = w_col;
		r->collider->h = h_col;

		r->Init();

		objects.push_back(r);
	}
	else
	{
		std::stringstream str;
		str << "Attempted to create: " << lType.name() << " as GameObject, Invalid operation, please make sure that the class inherits from GameObject!";
		Logger::Console_log(LogLevel::LOG_ERROR, str.str().c_str());
	}

	return r;
}

void ObjectManager::AddObject(GameObject* lToAdd)
{
	if(lToAdd != nullptr)
		objects.push_back(lToAdd);
}

int ObjectManager::AddWall(SDL_Rect rect)
{
	int i = 0;
	bool exit = false;

	while (i < MAX_WALLS && !exit)
	{
		if (walls[i] == nullptr)
		{
			exit = true;

		}
		else
		{
			++i;
		}
	}

	
	SDL_Rect* wall = new SDL_Rect();
	wall->x = rect.x;
	wall->y = rect.y;
	wall->w = rect.w;
	wall->h = rect.h;



	walls[i] = wall;

	return i;
}

void ObjectManager::DeleteWall(int id)
{
	delete walls[id];
	walls[id] = nullptr;

	
}

bool ObjectManager::AddFactory(FactoryBase* aFactory)
{
	mFactories.push_back(aFactory);
	return true;
}

FactoryBase* ObjectManager::GetFactory(const char* aNameInMap)
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
FactoryBase* ObjectManager::GetFactory(std::type_index& aType)
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

bool ObjectManager::CleanUp()
{
	bool ret = true;

	Clearphysics();

	for (std::list<FactoryBase*>::iterator it = mFactories.begin(); it != mFactories.end(); it++)
	{
		delete *it;
	}
	return ret;
}

bool ObjectManager::Clearphysics()
{
	Logger::Console_log(LogLevel::LOG_INFO, "Clearing UI physics");
	bool ret = true;

	for (int i = 0; i < MAX_WALLS; ++i)
	{
		if (walls[i] != nullptr)
		{
			delete walls[i];
			walls[i] = nullptr;
		}
	}

	for (std::list<GameObject*>::iterator it = objects.begin(); it != objects.end(); it++)
	{
		(*it)->Destroy();
		delete *it;
	}
	objects.clear();

	return ret;
}

void ObjectManager::DeleteObject(GameObject* _to_delete)
{

	//std::list<physobj*>::iterator obj_to_del;

	//for (std::list<physobj*>::iterator it = objects.begin(); it != objects.end(); it++)
	//{
	//	if (to_delete == *it)
	//	{
	//		obj_to_del = it;
	//	}
	//}
	//delete to_delete;
	if(!(std::find(to_delete.begin(), to_delete.end(), _to_delete)!=to_delete.end()))
		to_delete.push_back(_to_delete);

}
