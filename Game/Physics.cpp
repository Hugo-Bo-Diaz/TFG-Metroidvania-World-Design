#include "Physics.h"
#include "Player.h"

#include "FireBall.h"

#include "WindSlash.h"

#include "IceShard.h"
#include "IceBlock.h"

#include "Cloud.h"
#include "Leaf.h"
#include "Thorns.h"

#include "Rock.h"
#include "Shockwave.h"

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
	printf("Initiated phyisics\n");

	return ret;
}

bool Physics::Loop(float dt)
{
	bool ret = true;
	
	for (std::list<physobj*>::iterator it = objects.begin(); it != objects.end(); it++)
	{
		if (!(*it)->Loop(dt))
		{
			ret = false;
		}
	}

if (false)
	{
		for (int i = 0; i < MAX_WALLS; ++i)
		{
			if (walls[i] !=nullptr)
				App->ren->DrawRect(walls[i],0,0,255,75,true);
		}
	
		for (std::list<physobj*>::iterator it = objects.begin(); it != objects.end(); it++)
		{
			App->ren->DrawRect((*it)->collider, 0, 255, 0, 75, true);
			//App->ren->DrawRect((*it)->nextpos, 0, 255, 255, 75, true);
		}
	}

	//delete the current list
	for (std::list<physobj*>::iterator it = to_delete.begin(); it != to_delete.end(); it++)
	{
		
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

physobj* Physics::AddObject(int x, int y, int w_col, int h_col, object_type type)
{
	physobj* r;

	switch (type)
	{
	case PLAYER:{
		r = new Player();
		break;}
	case FIRE_BALL: {
		r = new FireBall();
		break; }
	case WIND_SLASH: {
		r = new WindSlash();
		break; }
	case ICE_SHARD: {
		r = new IceShard();
		break; }
	case ICE_BLOCK: {
		r = new IceBlock();
		break; }
	case CLOUD: {
		r = new Cloud();
		break; }
	case LEAF: {
		r = new Leaf();
		break; }
	case THORNS: {
		r = new Thorns();
		break; }
	case ROCK: {
		r = new Rock();
		break; }
	case SHOCKWAVE: {
		r = new Shockwave();
		break; }
	default:
		r = new physobj();
		break;
	}

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
	r->type = type;

	objects.push_back(r);

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
			//delete walls[i];
			walls[i] = nullptr;
		}
	}

	for (std::list<physobj*>::iterator it = objects.begin(); it != objects.end(); it++)
	{
		(*it)->~physobj();
		if ((*it)->type == PLAYER)
		{
			((Player*)(*it))->~Player();
		}
		if ((*it)->type == FIRE_BALL)
		{
			((FireBall*)(*it))->~FireBall();
		}
		delete(*it);
	}
	objects.clear();

	return ret;
}

void Physics::DeleteObject(physobj* _to_delete)
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