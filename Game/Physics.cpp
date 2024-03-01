#include "Physics.h"
#include "Logger.h"
#include "Debug.h"

#include <sstream>
#include <typeindex>

Physics::Physics()
{
	name = "Physics";
	for (int i = 0; i < MAX_WALLS; ++i)
	{
		walls[i] = nullptr;
	}
}

bool Physics::Init()
{
	bool ret = true;


	return ret;
}

bool Physics::Loop(float dt)
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

void Physics::RenderDebug()
{
	for (int i = 0; i < MAX_WALLS; ++i)
	{
		if (walls[i] != nullptr)
			App->ren->DrawRect(*walls[i], 0, 0, 255, 75, true, RenderQueue::RENDER_DEBUG, 0);
	}

	for (std::list<GameObject*>::iterator it = objects.begin(); it != objects.end(); it++)
	{
		(*it)->RenderDebug();
		App->ren->DrawRect(*(*it)->collider, 0, 255, 0, 75, true, RenderQueue::RENDER_DEBUG, 0);
	}
}

void Physics::GetNearbyWalls(int x, int y, int pxls_range, std::vector<SDL_Rect*>& colliders_near)
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
std::vector<GameObject*>* Physics::GetAllObjectsOfType(std::type_index info)
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

void Physics::GetCollisions(SDL_Rect* obj, std::vector<collision*>& collisions)
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

void Physics::ClearCollisionArray(std::vector<collision*>& collisions)
{

	for (std::vector<collision*>::iterator it = collisions.begin(); it != collisions.end(); it++)
	{
		delete *it;
	}
	collisions.clear();
}

GameObject* Physics::AddObject(int x, int y, int w_col, int h_col,std::type_index lType)
{
	auto lID = GetFactory(lType);

	std::list<ObjectProperty*> lPropList;

	//GameObject* r = (*lID).second(lPropList);

	GameObject* r = (*lID).CreateInstace();
	if (r != nullptr)
	{
		r->mType = lID->GetObjectTypeIndex();
		r->collider = new SDL_Rect();

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

void Physics::AddObject(GameObject* lToAdd)
{
	if(lToAdd != nullptr)
		objects.push_back(lToAdd);
}

int Physics::AddWall(SDL_Rect rect)
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

void Physics::DeleteWall(int id)
{
	delete walls[id];
	walls[id] = nullptr;

	
}

bool Physics::AddFactory(FactoryBase* aFactory)
{
	mFactories.push_back(aFactory);
	return true;
}

FactoryBase* Physics::GetFactory(const char* aNameInMap)
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
FactoryBase* Physics::GetFactory(std::type_index& aType)
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

bool Physics::CleanUp()
{
	bool ret = true;

	Clearphysics();

	for (std::list<FactoryBase*>::iterator it = mFactories.begin(); it != mFactories.end(); it++)
	{
		delete *it;
	}


	active = false;

	return ret;
}

bool Physics::Clearphysics()
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

void Physics::DeleteObject(GameObject* _to_delete)
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
