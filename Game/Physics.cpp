#include "Physics.h"
//#include "Player.h"
//
//#include "FireBall.h"
//
//#include "WindSlash.h"
//
//#include "IceShard.h"
//#include "IceBlock.h"
//
//#include "Cloud.h"
//#include "Leaf.h"
//#include "Thorns.h"
//
//#include "Rock.h"
//#include "Shockwave.h"
//
//#include "FireSpellPickup.h"
//#include "GroundSpellPickup.h"
//
//#include "MaxHealthPickup.h"
//#include "MaxManaPickup.h"
//
//#include "GroundedElemental.h"
//#include "FlyingElemental.h"
//#include "CoalJumper.h"
//#include "ArmorTrap.h"
//#include "ShieldMonster.h"
//#include "ClingingCreature.h"
//#include "FlyingAxe.h"
//#include "FlyingShield.h"
//#include "CloudMelee.h"
//#include "CloudSummoner.h"
//#include "CloudSummonerProjectile.h"
//
//#include "HazardLava.h"
//#include "HazardLavaWaterfall.h"
//#include "HazardSpikes.h"
//#include "HazardsRockBlock.h"
//#include "CloudTrampoline.h"

//#include "CheckPoint.h"
//#include "FirstDialogue.h"
//#include "TextBoxObject.h"
//#include "DemoEndObject.h"
#include "Logger.h"

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
	Logger::Console_log(LogLevel::LOG_INFO,"Initiated phyisics");

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
//if (App->debug)
	if(true)
	{
		for (int i = 0; i < MAX_WALLS; ++i)
		{
			if (walls[i] !=nullptr)
				App->ren->DrawRect(walls[i],0,0,255,75,true);
		}
	
		for (std::list<GameObject*>::iterator it = objects.begin(); it != objects.end(); it++)
		{
			App->ren->DrawRect((*it)->collider, 0, 255, 0, 75, true);
			//App->ren->DrawRect((*it)->nextpos, 0, 255, 255, 75, true);
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

std::vector<GameObject*>* Physics::GetAllObjectsOfType(objectId type)
{
	std::vector<GameObject*>* ret = new std::vector<GameObject*>();
	for (std::list<GameObject*>::iterator it = objects.begin(); it != objects.end(); it++)
	{
		if ((*it)->type == type)
		{
			ret->push_back(*it);
		}
	}
	return ret;
}

void Physics::GetCollisions(SDL_Rect* obj, std::vector<collision*>& collisions)
{
	//std::vector<collision*>* ret;

	for (std::list<GameObject*>::iterator it = objects.begin(); it != objects.end(); it++)
	{
		if (SDL_HasIntersection((*it)->collider,obj))
		{
			collision* col = new collision();

			col->object = *it;
			col->type = (*it)->type;

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

GameObject* Physics::AddObject(int x, int y, int w_col, int h_col,objectId objType, std::map<std::string, float>* properties)
{
	GameObject* r = CallBack(objType,properties);

	if (r != nullptr)
	{
		r->collider = new SDL_Rect();
		r->nextpos = new SDL_Rect();

		r->collider->x = x;
		r->collider->y = y;
		r->collider->w = w_col;
		r->collider->h = h_col;
		r->nextpos->x = x;
		r->nextpos->y = y;
		r->nextpos->w = w_col;
		r->nextpos->h = h_col;
		r->type = objType;

		objects.push_back(r);
	}

	return r;
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


bool Physics::CleanUp()
{
	bool ret = true;

	Clearphysics();

	active = false;

	return ret;
}

bool Physics::Clearphysics()
{
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
